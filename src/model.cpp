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
#include <ctime>
#include <string>
#include <vector>

#include "opengl.h"
#include "system.h"
#include "shader.h"
#include "model.h"
#include "texture.h"

using namespace std;


///////////////////////////////////////////////////////////////////////////////////////////////////
// Creates a cube using normal runtime compilation method
model_t* create_cube_model( const char* shader )
{
	model_t* _cube_model = NULL;
	_cube_model = new model_t;

	// initialize shader
	init_shader( _cube_model, shader );

	return complete_cube( _cube_model );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Creates a cube using a precompiled shader
model_t* create_cube_model_precompiled( const char* BINARY_FILENAME, const char* BINARY_FORMAT_FILENAME )
{
	model_t* _cube_model = NULL;
	_cube_model = new model_t;

	// initialize shader
	init_precompiled_shader( _cube_model, BINARY_FILENAME, BINARY_FORMAT_FILENAME );

	return complete_cube( _cube_model );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Set vertex and index information for the cube
static model_t* complete_cube( model_t* _cube_model )
{
	// cube vertices
	//
	//               9---------8
	//              /         /
	//             /         /
	//            10-------11
	//
	//    13         20-------21         4       front: {  0,  1,  2 }, {  0,  2,  3 }
	//    /|         |         |        /|       right: {  4,  5,  6 }, {  4,  6,  7 }
	//   / |         |         |       / |         top: {  8,  9, 10 }, {  8, 10, 11 }
	//  12 |      1---------0  |      5  |        left: { 12, 13, 14 }, { 12, 14, 15 }
	//  |  |      |  |      |  |      |  |      bottom: { 16, 17, 18 }, { 16, 18, 19 }
	//  | 14      |  23-----|-22      |  7        back: { 20, 21, 22 }, { 20, 22, 23 }
	//  | /       |         |         | /
	//  |/        |         |         |/
	//  15        2---------3         6
	//
	//               18-------19
	//              /         /
	//             /         /
	//            17-------16
	//
	const vertex_t _cube_vertices[] =
	{
			// FRONT
			{ glm::vec3(  5.0f,  5.0f,  5.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 0
			{ glm::vec3( -5.0f,  5.0f,  5.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 1
			{ glm::vec3( -5.0f, -5.0f,  5.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 2
			{ glm::vec3(  5.0f, -5.0f,  5.0f ), glm::vec3( 0.0f, 0.0f, 1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 3

			// RIGHT
			{ glm::vec3(  5.0f,  5.0f, -5.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 4
			{ glm::vec3(  5.0f,  5.0f,  5.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 5
			{ glm::vec3(  5.0f, -5.0f,  5.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 6
			{ glm::vec3(  5.0f, -5.0f, -5.0f ), glm::vec3( 1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 7

			// TOP
			{ glm::vec3(  5.0f,  5.0f, -5.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 8
			{ glm::vec3( -5.0f,  5.0f, -5.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 9
			{ glm::vec3( -5.0f,  5.0f,  5.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 10
			{ glm::vec3(  5.0f,  5.0f,  5.0f ), glm::vec3( 0.0f, 1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 11

			// LEFT
			{ glm::vec3( -5.0f,  5.0f,  5.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 12
			{ glm::vec3( -5.0f,  5.0f, -5.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 13
			{ glm::vec3( -5.0f, -5.0f, -5.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 14
			{ glm::vec3( -5.0f, -5.0f,  5.0f ), glm::vec3( -1.0f, 0.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 15

			// BOTTOM
			{ glm::vec3(  5.0f, -5.0f,  5.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 16
			{ glm::vec3( -5.0f, -5.0f,  5.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 17
			{ glm::vec3( -5.0f, -5.0f, -5.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 18
			{ glm::vec3(  5.0f, -5.0f, -5.0f ), glm::vec3( 0.0f, -1.0f, 0.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 19

			// BACK
			{ glm::vec3( -5.0f,  5.0f, -5.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 20
			{ glm::vec3(  5.0f,  5.0f, -5.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 21
			{ glm::vec3(  5.0f, -5.0f, -5.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 22
			{ glm::vec3( -5.0f, -5.0f, -5.0f ), glm::vec3( 0.0f, 0.0f, -1.0f ), glm::vec4( rand_f(), rand_f(), rand_f(), 1.0f ) },  // 23
	};

	const unsigned int _cube_indices[] =
	{
			0,  1,  2,   0,  2,  3,  // front
			4,  5,  6,   4,  6,  7,  // right
			8,  9, 10,   8, 10, 11,  // top
			12, 13, 14,  12, 14, 15,  // left
			16, 17, 18,  16, 18, 19,  // bottom
			20, 21, 22,  20, 22, 23,  // back
	};

	_cube_model->num_vertices = sizeof( _cube_vertices) / sizeof( vertex_t );
	_cube_model->num_indices = sizeof( _cube_indices ) / sizeof( GLuint );

	unsigned int _size_of_vertices = sizeof( vertex_t ) * _cube_model->num_vertices;
	_cube_model->vertices = ( vertex_t* )malloc( _size_of_vertices );
	memcpy( _cube_model->vertices, _cube_vertices, _size_of_vertices );

	unsigned int _size_of_indices = sizeof( GLuint ) * _cube_model->num_indices;
	_cube_model->indices = ( GLuint* )malloc( _size_of_indices );
	memcpy( _cube_model->indices, _cube_indices, _size_of_indices );

	// generate VBOs
	glGenBuffers( 1, &_cube_model->vertex_buffer );
	glBindBuffer( GL_ARRAY_BUFFER, _cube_model->vertex_buffer );
	glBufferData( GL_ARRAY_BUFFER, _size_of_vertices, _cube_model->vertices, GL_STATIC_DRAW );

	glGenBuffers( 1, &_cube_model->index_buffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, _cube_model->index_buffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, _size_of_indices, _cube_model->indices, GL_STATIC_DRAW );

	// reset default bindings
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	return _cube_model;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the cube
void draw_cube( model_t* model, glm::mat4* view_projection_matrix, glm::mat4* world_matrix, GLenum type )
{
	// Set up shader model program
	glUseProgram( model->shader_prog );

	// Bind VBO buffers and attributes
	glBindBuffer( GL_ARRAY_BUFFER, model->vertex_buffer );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, model->index_buffer );

	GLuint stride = sizeof( vertex_t );

	GLuint offset = 0;
	glEnableVertexAttribArray( model->position_attrib );
	glVertexAttribPointer( model->position_attrib, 3, GL_FLOAT, GL_FALSE, stride, ( const void* )offset );

	offset += 3 * sizeof( GLfloat );
	glEnableVertexAttribArray( model->normal_attrib );
	glVertexAttribPointer( model->normal_attrib, 3, GL_FLOAT, GL_FALSE, stride, ( const void* )offset );

	offset += 3 * sizeof( GLfloat );
	glEnableVertexAttribArray( model->color_attrib );
	glVertexAttribPointer( model->color_attrib, 2, GL_FLOAT, GL_FALSE, stride, ( const void* )offset );

	// Set the MVP matrix
	GLint view_proj_matrix_loc = glGetUniformLocation( model->shader_prog, "u_view_proj_matrix" );
	glUniformMatrix4fv( view_proj_matrix_loc, 1, GL_FALSE, glm::value_ptr( *view_projection_matrix ) );

	// Set the model view transpose
	GLint world_matrix_loc = glGetUniformLocation( model->shader_prog, "u_world_matrix" );
	glUniformMatrix4fv( world_matrix_loc, 1, GL_FALSE, glm::value_ptr( *world_matrix ) );

	// Draw model
	glDrawElements( type, model->num_indices, GL_UNSIGNED_INT, 0 );

	// Unbind buffer before we return
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize shader using normal runtime compilation
void ///////////////////////////////////////////////////////////////////////////////////////////////////
init_shader( model_t* model, const char* shader )
{
	// Load the program if it's not ready
	if( 0 == model->shader_prog )
	{
		model->shader_prog = load_shader( shader );

		bind_attributes( model );
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Initialize using a precompiled shader file
void init_precompiled_shader( model_t* model, const char* BINARY_FILENAME, const char* BINARY_FORMAT_FILENAME )
{
	string line;
	int format_code;
	ifstream myfile (BINARY_FORMAT_FILENAME);
	if (myfile.is_open())
    {
    	while ( getline (myfile,line) )
    	{
      		format_code = atoi(line.c_str());
    	}
    	myfile.close();
 	}
  	else LOGI("Unable to open file");

	// Load the program if it's not ready
	if( 0 == model->shader_prog )
	{
		model->shader_prog = loadProgramBinary( BINARY_FILENAME, (GLenum)format_code );
		bind_attributes(model);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Bind attribute vars to shader vars
static void bind_attributes( model_t* model )
{
		model->position_attrib = glGetAttribLocation( model->shader_prog, "a_position" );
		model->normal_attrib   = glGetAttribLocation( model->shader_prog, "a_normal" );
		model->color_attrib = glGetAttribLocation( model->shader_prog, "a_color" );

		if ( -1 == model->position_attrib )
		{
			LOGI( "Error with setting a_position in shader\n" );
		}
		if ( -1 == model->normal_attrib )
		{
			LOGI( "Error with setting a_normal in shader\n" );
		}
		if ( -1 == model->color_attrib )
		{
			LOGI( "Error with setting a_texCoord in shader\n" );
		}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Create and fill ui struct
ui* create_interface( const char* shader, unsigned int width, unsigned int height )
{
    int i = 0;
    ui* user_interface = NULL;

	const uint16_t kQuadIndices[] =
	{
	    0, 1, 2,
	    2, 3, 0
	};
	const char* image = "fonts/inconsolata.png";
	const char* font_file = "fonts/inconsolata.fnt";

	user_interface = new ui;

    FontLoader f;
	user_interface->font_loader = f;
	user_interface->font_loader.load_fontfile( font_file );

    user_interface->width = width;
    user_interface->height = height;

    // Load textures
    user_interface->font_description.texture = load_texture( image );

    // Create character index buffer
    glGenBuffers( 1, &user_interface->font_description.char_indices );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, user_interface->font_description.char_indices );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( kQuadIndices ), kQuadIndices, GL_STATIC_DRAW );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

    // Create character meshes
    for( i=0; i< BMP_CHAR_SZ; ++i ) {
        glm::vec3 pos_transform;
        glm::vec3 pos_scale;
        glm::vec2 tex_scale;
        int j;

        bitmap_char c = user_interface->font_loader.char_set.bmp_char[i];

        struct {
            glm::vec3    pos;
            glm::vec2    tex;
        } quad_vertices[] =
        {
            glm::vec3( 0.0f,    c.height, 0.0f ), glm::vec2( c.x, c.y ), // TL
            glm::vec3( c.width, c.height, 0.0f ), glm::vec2( c.x + c.width, c.y ), // TR
            glm::vec3( c.width, 0.0f,     0.0f ), glm::vec2( c.x + c.width, c.y + c.height ), // BR
            glm::vec3( 0.0f,    0.0f,     0.0f ), glm::vec2( c.x,           c.y + c.height ), // BL
        };
        if( c.id == 0 )
            continue;

        // Pre-transform the vertices
        pos_transform = glm::vec3( c.x_offset, -(c.height+c.y_offset) + user_interface->font_loader.char_set.line_height,
        	0.0f );
        for( j = 0; j < 4; ++j )
        {
            quad_vertices[j].pos.x = quad_vertices[j].pos.x + pos_transform.x;
            quad_vertices[j].pos.y = quad_vertices[j].pos.y + pos_transform.y;
            quad_vertices[j].pos.z = quad_vertices[j].pos.z + pos_transform.z;
        }

        // Scale the characters
        pos_scale = glm::vec3( user_interface->font_loader.char_set.line_height,
        	user_interface->font_loader.char_set.line_height, 1.0f );
        tex_scale = glm::vec2( user_interface->font_loader.char_set.scale_w,
        	user_interface->font_loader.char_set.scale_h );
        for( j = 0; j < 4; ++j )
        {
            quad_vertices[j].pos.x = quad_vertices[j].pos.x / pos_scale.x;
            quad_vertices[j].pos.y = quad_vertices[j].pos.y / pos_scale.y;
            quad_vertices[j].pos.z = quad_vertices[j].pos.z / pos_scale.z;

            quad_vertices[j].tex.x = quad_vertices[j].tex.x / tex_scale.x;
            quad_vertices[j].tex.y = quad_vertices[j].tex.y / tex_scale.y;
        }
        glGenBuffers( 1, &user_interface->font_description.char_vertices[i] );
        glBindBuffer( GL_ARRAY_BUFFER, user_interface->font_description.char_vertices[i] );
        glBufferData( GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW );
        glBindBuffer( GL_ARRAY_BUFFER, 0 );
    }

    // Create shader
    user_interface->program = create_ui_program("shaders/ui.vertex.glsl",
                                "shaders/ui.fragment.glsl");

    user_interface->world_attrib = glGetUniformLocation( user_interface->program, "u_world" );
    user_interface->color_attrib = glGetUniformLocation( user_interface->program, "u_color" );
    user_interface->texture_attrib = glGetUniformLocation( user_interface->program, "s_texture" );

    return user_interface;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Draw a string to the screen
static void draw_string(ui* user_interface, float x, float y, float scale, char* string)
{
    glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glUniform4fv(user_interface->color_attrib, 1, (float*)&color);
    while(string && *string) {
        char c = *string;
        bitmap_char glyph = user_interface->font_loader.char_set.bmp_char[c];
        if(c != ' ')
        {
            float* ptr = 0;
            glm::mat4 transform_matrix = glm::translate(glm::mat4(1.0f),glm::vec3( x, y, 0));
            glm::mat4 scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, 1.0));
            glm::mat4 world = transform_matrix * scale_matrix;

            glUniformMatrix4fv(user_interface->world_attrib, 1, GL_FALSE, (float*)&world);
            glUniform1i(user_interface->texture_attrib, 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, user_interface->font_description.texture);
            glBindBuffer(GL_ARRAY_BUFFER, user_interface->font_description.char_vertices[c]);

			GLuint offset = 0;
			GLuint stride = sizeof( glm::vec3 ) + sizeof( glm::vec2 );
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, ( const void* )offset);

			offset += 3 * sizeof( GLfloat );
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, ( const void* )offset);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
        }
        x += (glyph.x_advance/(float)user_interface->font_loader.char_set.line_height) *scale;
        ++string;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Draw the user interface
void draw_interface( ui* user_interface )
{
	int i;
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_ALWAYS);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    glUseProgram(user_interface->program);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, user_interface->font_description.char_indices);
    glActiveTexture(GL_TEXTURE0);
    for (i=0; i<user_interface->num_strings; ++i) {
        draw_string(user_interface, user_interface->strings[i].x, user_interface->strings[i].y, user_interface->strings[i].scale, user_interface->strings[i].string);
    }
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LESS);
    glDisable(GL_BLEND);
    checkOGLError();
}
