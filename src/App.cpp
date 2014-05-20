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

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>

#include "opengl.h"
#include "system.h"
#include "shader.h"
#include "model.h"
#include "App.h"
#include "timer.h"
#include "fonts.h"

#define CUBE_ROW_NUM 5
#define CUBE_COL_NUM 10

static model_t* cube_model[CUBE_ROW_NUM][CUBE_COL_NUM] = { NULL };
static model_t* cube_model_precompiled[CUBE_ROW_NUM][CUBE_COL_NUM] = { NULL };
static ui* interface = NULL;

static const char* BINARY_FILENAME = "/sdcard/myShaderBinary.txt";
static const char* BINARY_FORMAT_FILENAME = "/sdcard/myShaderBinaryFormat.txt";

unsigned int screen_width;
unsigned int screen_height;
float aspect_ratio;

//--------------------------------------------------
// Initialization
//--------------------------------------------------
void app_init( unsigned int width, unsigned int height )
{
    float scale = 0.06f;
    float x = -0.95f;
    float y = -0.9f;;
    char ui_text[256] = {0};
    char ui_text2[256] = {0};
    srand (static_cast <unsigned> (time(0)));
    screen_width = width;
    screen_height = height;

    aspect_ratio = ( float )screen_width / ( float )screen_height;

    interface = create_interface( "shaders/ui.shader", screen_width, screen_height );

    glEnable( GL_DEPTH_TEST );

	glViewport(0, 0, screen_width, screen_height );

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClearDepthf(1.0f);

    // Precompile shaders
    precompile_shader( "shaders/precompiled_cube.shader", BINARY_FILENAME, BINARY_FORMAT_FILENAME );

    Timer* timer;
    timer = create_timer();
    LOGI("\n\n\n\t============= Starting =============\n");
    LOGI("** Creating %d cubes without precompiled shaders **", (CUBE_ROW_NUM * CUBE_COL_NUM));


    /* Create cubes using runtime compiled shader */
    for( int i = 0; i < CUBE_ROW_NUM; ++i )
    {
        for( int j = 0; j < CUBE_COL_NUM; ++j )
        {
            cube_model[i][j] =  create_cube_model("shaders/runtime_cube.shader");
        }
    }
    
    float time_diff = (float)get_delta_time(timer);
    sprintf( ui_text, "Runtime Compilation = %f seconds", time_diff );
    add_string( interface, x, y, scale, ui_text );

    LOGI("\t=== Finished in %f seconds ===\n\n", time_diff);
    LOGI("** Creating %d cubes with precompiled shaders **", (CUBE_ROW_NUM * CUBE_COL_NUM));

    /* Create cubes using precompiled shaders */
    for( int i = 0; i < CUBE_ROW_NUM; ++i )
    {
        for( int j = 0; j < CUBE_COL_NUM; ++j )
        {
            cube_model_precompiled[i][j] =  create_cube_model_precompiled( BINARY_FILENAME,
                BINARY_FORMAT_FILENAME );
        }
    }
    time_diff = (float)get_delta_time(timer);
    LOGI("\t=== Finished in %f seconds ===\n", time_diff);
    LOGI("\t=============== Done ===============\n");

    sprintf( ui_text2, "Precompiled Compilation = %f seconds", time_diff );
    add_string( interface, x, y - 0.1, scale, ui_text2 );
}

static float total_time = 0;

//--------------------------------------------------
// Update the frame
//--------------------------------------------------
void app_update( float fElapsedTime )
{
    total_time += fElapsedTime;
}

//--------------------------------------------------
// Render the frame
//--------------------------------------------------
void app_render( void )
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 world_transform(1.0f);
    glm::mat4 projection_transform = glm::perspective(60.0f, aspect_ratio, 0.25f, 1000.0f);
    float tempTime = total_time * 10.0f;

    for( int i = 0; i < CUBE_ROW_NUM; ++i )
    {
        for ( int j = 0; j < CUBE_COL_NUM; ++j )
        {
            float x_val = -145.0f + (15 * i);
            glm::mat4 view_transform = glm::translate(glm::mat4(1.0f),glm::vec3( x_val, 65.0f - (15.0f * j), -160.0f - (15.0f * i)));
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), tempTime, glm::vec3(0.0f, 1.0f, 0.0f));
            glm::mat4 view_projection_transform  = projection_transform*view_transform;

            draw_cube( cube_model_precompiled[i][j], &view_projection_transform, &rotation, GL_TRIANGLES);

            view_transform = glm::translate(glm::mat4(1.0f),glm::vec3(-x_val, 65.0f - (15.0f * j), -160.0f - (15.0f * i)));
            rotation = glm::rotate(glm::mat4(1.0f), tempTime, glm::vec3(0.0f, -1.0f, 0.0f));
            view_projection_transform  = projection_transform*view_transform;

            draw_cube( cube_model[i][j], &view_projection_transform, &rotation, GL_TRIANGLES);
        }
    }
    draw_interface( interface );
    glFinish();
}