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

#ifndef __FONT_PARSER_H__
#define __FONT_PARSER_H__

#include <vector>
#include <string>
#include "system.h"
#include "opengl.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"
#define BMP_CHAR_SZ 256
#define MAX_STRINGS 128

using namespace std;

struct kerning
{
	int second;
	int amount;
};

struct bitmap_char
{
	int id;
	int x, y, width, height;
	float x_offset, y_offset;
	float x_advance;
	int page;
	int channel;
	vector<kerning> kerning_vec;

	bitmap_char() : id( 0 )
		{}
};

struct font
{
	GLuint	  texture;
	GLuint 	  char_vertices[BMP_CHAR_SZ];
	GLuint	  char_indices;
};

class FontLoader
{

public:
	FontLoader()
	{
		bitmap_char* temp = new bitmap_char[BMP_CHAR_SZ];
		char_set.bmp_char = temp;
	}

	struct bitmap_char_set
	{
		int line_height, base;
		int scale_w, scale_h;
		int pages, packed;
		int alpha_chan, red_chan, green_chan, blue_chan;
		int char_count;
		int kerning_count;
		bitmap_char * bmp_char;

		bitmap_char_set() : line_height( 0 ), base( 0 ), scale_w( 0 ),
			scale_h( 0 ), pages( 0 ), packed( 0 ), alpha_chan( 0 ),
			red_chan( 0 ), green_chan( 0 ), blue_chan( 0 ), char_count( 0 ),
			kerning_count( 0 )
			{}
	} char_set;

	bool load_fontfile( const char* filename );
};

struct ui
{
	int width;
	int height;
	font font_description;

    unsigned int  num_vertices;
    unsigned int  num_indices;
    
    GLuint program;
	GLuint world_attrib;
	GLuint color_attrib;
	GLuint texture_attrib;

	FontLoader font_loader;

	struct
	{
		float x;
		float y;
		float scale;
		char string[256];
	} strings[MAX_STRINGS];
    int num_strings;

    ui(): program(0), num_strings(0) {}
};

void add_string(ui* user_interface, float x, float y, float scale, const char* string);
void font_debug_log(ui* user_interface);

#endif // __FONT_PARSER_H__
