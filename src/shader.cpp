/* Copyright (c) 2014, Intel Corporation
*
* Redistribution and use in source and binary forms, with or without 
* modification, are permitted provided that the following conditions are met:
*
* - Redistributions of source code must retain the above copyright notice, 
*   this list of conditions and the following disclaimer.
* - Redistributions in binary form must reproduce the above copyright notice, 
*   this list of conditions and the following disclaimer in the documentation 
*   and/or other materials provided with the distribution.
* - Neither the name of Intel Corporation nor the names of its contributors 
*   may be used to endorse or promote products derived from this software 
*   without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE 
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
* POSSIBILITY OF SUCH DAMAGE.
*
*/

#include <assert.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "opengl.h"
#include "system.h"
#include "shader.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Compiles the given shader type (vertex or fragment) and returns a handle
GLuint compile_shader( GLenum type, const char* shader_data, int shader_size )
{
    // Create the shader 
    GLuint shader_handle = glCreateShader( type );

    if( shader_handle == 0 )
    {
        return 0;
    }

    // Set up string for #define based on Type 
    // Must keep the same length for the hard coded 17 for string size (see ShaderStringLengths)
    char* define_vertex_shader   = (char*)"#define VERTEX  \n";
    char* define_fragment_shader = (char*)"#define FRAGMENT\n";
    char* shader_define = ( type == GL_VERTEX_SHADER ) ? define_vertex_shader : define_fragment_shader;

    // Set up string table (first string is the #define for this Type and then the shader program)
    const char* shader_strings[2] = { shader_define, shader_data };
    GLint shader_strings_length[2] = { 17, shader_size };

    // Load the shader source
    glShaderSource( shader_handle, 2, shader_strings, shader_strings_length );

    // Compile the shader
    glCompileShader( shader_handle );

    // Check the compiler status
    GLint compile_status;
    glGetShaderiv( shader_handle, GL_COMPILE_STATUS, &compile_status );

    if( !compile_status )
    {
        GLint info_length = 0;

        glGetShaderiv( shader_handle, GL_INFO_LOG_LENGTH, &info_length );
      
        if( info_length > 1 )
        {
            char* info_log = (char*)malloc( sizeof(char) * info_length );

            glGetShaderInfoLog( shader_handle, info_length, NULL, info_log );
            
            LOGE("Error compiling shader:\n%s\n", info_log );
         
            free( info_log );
        }

        glDeleteShader( shader_handle );

        // Failed to compile the shadedrs
        assert( 0 );
        return 0;
    }

    return shader_handle;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
// Links the shaders and creates the final program
GLuint link_shader( GLuint vertex_shader_handle, GLuint fragment_shader_handle )
{ 
    char error_string[1024];
    GLuint shader_program = glCreateProgram();
    if( shader_program == 0 )
    {
        // Failed to create a program handle
        assert( 0 );
        return 0;
    }

    // Associate the shaders with the program
    glAttachShader( shader_program, vertex_shader_handle );
    glAttachShader( shader_program, fragment_shader_handle );

    // Necessary to retrieve shader program bindary
    glProgramParameteri(shader_program, GL_PROGRAM_BINARY_RETRIEVABLE_HINT, GL_TRUE);

    // Link the program
    glLinkProgram( shader_program );

    // Check the link status
    GLint  linker_status;
    glGetProgramiv( shader_program, GL_LINK_STATUS, &linker_status );

    if( !linker_status )
    {
        // Failed to link, get the error string
        GLint info_length = 0;

        glGetProgramiv( shader_program, GL_INFO_LOG_LENGTH, &info_length );
      
        if( info_length > 1 )
        {
            char* info_log = (char*)malloc( sizeof(char) * info_length );

            glGetProgramInfoLog ( shader_program, info_length, NULL, info_log );
            

            LOGE("Error linking program:\n%s\n", info_log );
         
            free( info_log );
        }

        glDeleteProgram( shader_program );

        // Failed to link the shadedrs
        assert( 0 );
        return 0;
    }

    //LOGI("Shaders linked sucessfully\n" );

    return shader_program;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loads, compiles, and links the shader for the given file and returns the programs id
GLuint load_shader( const char* shader_file_name )
{   
    GLuint shader_program = -1;

    // Open files
    char* file_data = NULL;
    unsigned int file_size = 0;
    
    read_file( shader_file_name, &file_data, &file_size );

    // Load and compile the vertex shader
    GLuint vertex_shader = compile_shader( GL_VERTEX_SHADER, file_data, file_size );
    if( vertex_shader == 0 )
    {
        // Couldn't load the shader for the give file
        assert( 0 ); 
        return 0;
    }

    // Load and compile the fragment shader
    GLuint fragment_shader = compile_shader( GL_FRAGMENT_SHADER, file_data, file_size );
    if( fragment_shader == 0 )
    {
        glDeleteShader( vertex_shader );

        // Couldn't load the shader for the give file
        assert( 0 );
        return 0;
    }

    // Create the program handle
    shader_program = link_shader( vertex_shader, fragment_shader );
    if( shader_program == 0 )
    {
        // Failed to link the program
        assert( 0 );
        return 0;
    }

    // Free the shader resources
    glDeleteShader( vertex_shader );
    glDeleteShader( fragment_shader );

    return shader_program;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loads, compiles, and links the shader and then saves the binary code to a file
void precompile_shader( const char* shader_file_name, const char* BINARY_FILENAME, const char* BINARY_FORMAT_FILENAME )
{
    // load the program
    GLuint shader_program = load_shader( shader_file_name );

    void* buffer = NULL;
    GLint bin_size = -1;
    GLenum bin_format = -1;

    // get the length of the binary code to be retrieved
    glGetProgramiv(shader_program, GL_PROGRAM_BINARY_LENGTH, &bin_size);

    // retrieve binary code & code format
    buffer = malloc(bin_size);
    GLsizei bytesWritten;
    glGetProgramBinary (shader_program, bin_size, &bytesWritten, &bin_format, buffer);

    // write binary code to file
    ofstream outfile (BINARY_FILENAME, ofstream::binary);
    outfile.write ((char*)buffer, bin_size);
    free(buffer);

    // write binary format to file
    ostringstream s1;
    s1 << bin_format;
    string s = s1.str();

    ofstream format_outfile ( BINARY_FORMAT_FILENAME );
    format_outfile.write (s.c_str(), s.length());
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Loads the precompiled binary file into a program object
GLuint loadProgramBinary(const char* BINARY_FILENAME, GLenum binary_format )
{
    GLuint prog_obj = -1;
    GLint success = - 1;
    streampos size = -1;
    char * mem_block = NULL;

    // read binary file into memory
    ifstream file( BINARY_FILENAME, ios::in|ios::binary|ios::ate );

    if (file.is_open())
    {
        size = file.tellg();
        mem_block = ( char* )malloc( sizeof(char) * size );
        file.seekg ( 0, ios::beg );
        file.read ( mem_block, size );
        if (!file)
        {
            LOGE( "Error reading program binary:\n\t%s\n", BINARY_FILENAME );
        }
    }
    else LOGE( "Unable to open file:\n\t%s\n", BINARY_FILENAME );

    // create program and set binary code
    prog_obj = ( GLuint )glCreateProgram();
    glProgramBinary( ( GLuint )prog_obj, binary_format, (const void*)mem_block, size );
    glGetProgramiv( ( GLuint )prog_obj, GL_LINK_STATUS, &success );

    free( mem_block );

    // revert to normal runtime compilation method if fails
    if (!success)
    {
        LOGI( "WARNING: Loading binary file failed, reverted to runtime compilation" );
        return load_shader( "shaders/cube.shader" );
    }
    else
        return prog_obj;
}

static GLuint load_ui_shader(const char* filename, GLenum type)
{
    char*   data = NULL;
    size_t  data_size = 0;
    GLuint  shader = 0;
    GLint   compile_status = 0;
    GLint   shader_size = 0;
    GLint   info_length = 0;

    read_file( filename, &data, &data_size );

    shader_size = (GLint)data_size;

    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const char**)&data, &shader_size);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);

    if(compile_status == GL_FALSE) {
        char message[1024] = {0};
        glGetShaderInfoLog(shader, sizeof(message), 0, message);
        LOGI("Error compiling %s: %s", filename, message);
        assert(compile_status != GL_FALSE);
        return 0;
    }

    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_length);
    if(info_length > 1) {
        LOGI("info_length = %d", info_length);
        char info_log[1024] = {0};
        glGetShaderInfoLog(shader, sizeof(info_log), NULL, info_log);
        LOGI("Info compiling %s: %s", filename, info_log);
    }

    return shader;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Create the program for the ui shader
GLuint create_ui_program(const char* vertex_shader_filename,
                       const char* fragment_shader_filename)
{
    GLuint  vertex_shader;
    GLuint  fragment_shader;
    GLuint  program;
    GLint   link_status;


    /* Compile shaders */
    vertex_shader = load_ui_shader(vertex_shader_filename, GL_VERTEX_SHADER);
    fragment_shader = load_ui_shader(fragment_shader_filename, GL_FRAGMENT_SHADER);


    /* Create program */
    program = glCreateProgram();

    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glBindAttribLocation(program, 0, "a_position");
    glBindAttribLocation(program, 1, "a_tex_coord");

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &link_status);

    if(link_status == GL_FALSE) {
        char message[1024];
        glGetProgramInfoLog(program, sizeof(message), 0, message);
        LOGI("Creating program: %s--%s failed: %s\n", vertex_shader_filename, fragment_shader_filename, message);
        glDetachShader(program, fragment_shader);
        glDetachShader(program, vertex_shader);
        glDeleteShader(fragment_shader);
        glDeleteShader(vertex_shader);
        glDeleteProgram(program);
        return 0;
    }
    glDetachShader(program, fragment_shader);
    glDetachShader(program, vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteShader(vertex_shader);

    return program;
}
