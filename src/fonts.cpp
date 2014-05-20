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

#include <iostream>
#include <fstream>
#include <sstream>
#include "fonts.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////
// Read .fnt file into memory
bool FontLoader::load_fontfile( const char* filename )
{
	//ifstream file( filename, std::ios::in );
	char* file_data = NULL;
    unsigned int file_size = 0;
	read_file( filename, &file_data, &file_size );

	string file_data_s(file_data);
	istringstream file(file_data_s);

	if ( !file )
	{
		LOGE( "Error: could not open font file\n" );
		return false;
	}

	string line;
	getline( file, line);		// skip first
	getline( file, line, '=' ) >> char_set.line_height;
	getline( file, line, '=' ) >> char_set.base;
	getline( file, line, '=' ) >> char_set.scale_w;
	getline( file, line, '=' ) >> char_set.scale_h;
	getline( file, line, '=' ) >> char_set.pages;
	getline( file, line, '=' ) >> char_set.packed;
	getline( file, line, '=' ) >> char_set.alpha_chan;
	getline( file, line, '=' ) >> char_set.red_chan;
	getline( file, line, '=' ) >> char_set.green_chan;
	getline( file, line, '=' ) >> char_set.blue_chan;
	getline( file, line );		// skip to next
	getline( file, line );		// skip to next
	getline( file, line, '=' ) >> char_set.char_count;
	getline( file, line );		// skip to next


	for ( int i = 0; i < char_set.char_count; ++i )
	{
		int id = -1;
		getline( file, line, '=') >> id;
		if ( id != -1 )
		{
			char_set.bmp_char[id].id = id;
			getline( file, line, '=' ) >> char_set.bmp_char[id].x;
			getline( file, line, '=' ) >> char_set.bmp_char[id].y;
			getline( file, line, '=' ) >> char_set.bmp_char[id].width;
			getline( file, line, '=' ) >> char_set.bmp_char[id].height;
			getline( file, line, '=' ) >> char_set.bmp_char[id].x_offset;
			getline( file, line, '=' ) >> char_set.bmp_char[id].y_offset;
			getline( file, line, '=' ) >> char_set.bmp_char[id].x_advance;
			getline( file, line, '=' ) >> char_set.bmp_char[id].page;
			getline( file, line, '=' ) >> char_set.bmp_char[id].channel;
		}
		else LOGE( "Error reading into character structure\n");
	}
	getline( file, line );
	getline( file, line, '=' ) >> char_set.kerning_count;

	for ( int i = 0; i < char_set.kerning_count; ++i )
	{
		int id = -1;
		int second = -1;
		int amount = -1;

		getline( file, line, '=') >> id;
		getline( file, line, '=') >> second;
		getline( file, line, '=') >> amount;
		if ( id != - 1 )
		{	
			kerning insert;
			insert.second = second;
			insert.amount = amount;
			char_set.bmp_char[id].kerning_vec.push_back(insert);
		}
		else LOGE( "Error reading into kerning structure\n" );
	}

	return true;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Print structure for debugging purposes
void font_debug_log(ui* user_interface)
{
	LOGI("line height : %d", user_interface->font_loader.char_set.line_height);
	LOGI("base : %d", user_interface->font_loader.char_set.base);
	LOGI("scale_w : %d", user_interface->font_loader.char_set.scale_w);
	LOGI("scale_h : %d", user_interface->font_loader.char_set.scale_h);
	LOGI("pages : %d", user_interface->font_loader.char_set.pages);
	LOGI("packed : %d", user_interface->font_loader.char_set.packed);
	LOGI("alpha_chan %d: ", user_interface->font_loader.char_set.alpha_chan);
	LOGI("red_chan : %d", user_interface->font_loader.char_set.red_chan);
	LOGI("green_chan : %d", user_interface->font_loader.char_set.green_chan);
	LOGI("blue_chan : %d", user_interface->font_loader.char_set.blue_chan);
	LOGI("char_count : %d", user_interface->font_loader.char_set.char_count);

	for ( int i = 0; i < BMP_CHAR_SZ; ++i )
	{
		LOGI("bmp_char[%d].id : %d", i, user_interface->font_loader.char_set.bmp_char[i].id);
		LOGI("bmp_char[%d].x : %d", i, user_interface->font_loader.char_set.bmp_char[i].x);
		LOGI("bmp_char[%d].y : %d", i, user_interface->font_loader.char_set.bmp_char[i].y);
		LOGI("bmp_char[%d].width : %d", i, user_interface->font_loader.char_set.bmp_char[i].width);
		LOGI("bmp_char[%d].height : %d", i, user_interface->font_loader.char_set.bmp_char[i].height);
		LOGI("bmp_char[%d].x_offset : %f", i, user_interface->font_loader.char_set.bmp_char[i].x_offset);
		LOGI("bmp_char[%d].y_offset : %f", i, user_interface->font_loader.char_set.bmp_char[i].y_offset);
		LOGI("bmp_char[%d].x_advance : %f", i, user_interface->font_loader.char_set.bmp_char[i].x_advance);
		LOGI("bmp_char[%d].page : %d", i, user_interface->font_loader.char_set.bmp_char[i].page);
		LOGI("bmp_char[%d].channel : %d", i, user_interface->font_loader.char_set.bmp_char[i].channel);
	}
	LOGI("Kerning count = %d\n", user_interface->font_loader.char_set.kerning_count);
	for ( int i = 0; i < BMP_CHAR_SZ; ++i )
	{
		if ( user_interface->font_loader.char_set.bmp_char[i].id > 0 )
		{
			for( vector<kerning>::iterator it = user_interface->font_loader.char_set.bmp_char[i].kerning_vec.begin(); 
				it != user_interface->font_loader.char_set.bmp_char[i].kerning_vec.end(); ++it )
			{
				kerning temp;
				temp = *(it);
				LOGI( "kerning[%d]: second = %d, amount = %d\n", user_interface->font_loader.char_set.bmp_char[i].id,
					temp.second, temp.amount );
			}
		}
	}

	LOGI( "===== String Info Struct =====\n");
    for( int i = 0; i < user_interface->num_strings; ++i )
    {
		LOGI( "String[%d].x == %f\n", i, user_interface->strings[i].x );
		LOGI( "String[%d].y == %f\n", i, user_interface->strings[i].y );
		LOGI( "String[%d].scale == %f\n", i, user_interface->strings[i].scale );
		LOGI( "String[%d].string == %s\n", i, user_interface->strings[i].string );
    }
	LOGI( "ui.width = %d\nui.height = %d", user_interface->width, user_interface->height );
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Add a string to be printed
void add_string(ui* user_interface, float x, float y, float scale, const char* string)
{
    int index = user_interface->num_strings;
    if( index < MAX_STRINGS )
    {
	    user_interface->strings[index].x = x;
	    user_interface->strings[index].y = y;
	    user_interface->strings[index].scale = scale;
	    strlcpy(user_interface->strings[index].string, string,
	    	    sizeof(user_interface->strings[index].string));
	    ++index;
    	user_interface->num_strings = index;
    }
}