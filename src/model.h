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

#ifndef __DRR_MODEL_H__
#define __DRR_MODEL_H__

#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "fonts.h"

typedef struct _vertex_t
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec4 color;
} vertex_t;

typedef struct _mesh_data_t
{
    int vertex_size;
    int vertex_count;
    int index_size;
    int index_count;
} mesh_data_t;

// Model data structure
typedef struct _model_t
{
    // interleaved vertex data
    GLuint vertex_buffer;
    
    // index buffer object
    GLuint index_buffer;
    
    // mesh data
	vertex_t* vertices;
    unsigned int* indices;
    
    unsigned int  num_vertices;
    unsigned int  num_indices;

    // shader attribute handles
    GLuint shader_program;
    GLint  position_attrib;
    GLint  normal_attrib;
    GLint  color_attrib;
    
    int shader_prog;

    _model_t(): shader_prog(0){}

} model_t;

model_t* create_cube_model( const char* shader );
model_t* create_cube_model_precompiled( const char* BINARY_FILENAME, const char* BINARY_FORMAT_FILENAME );
static model_t* complete_cube( model_t* _cube_model );
void draw_cube( model_t* model, glm::mat4* view_projection_matrix, glm::mat4* world_matrix, GLenum type );

ui* create_interface( const char* shader, unsigned int width, unsigned int height );
void draw_interface( ui* user_interface );


void init_shader( model_t* model, const char* shader );
void init_precompiled_shader( model_t* model, const char* BINARY_FILENAME, const char* BINARY_FORMAT_FILENAME );
static void bind_attributes( model_t* model );

#endif // __DRR_MODEL_H__
