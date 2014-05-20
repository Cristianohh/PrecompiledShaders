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
#include <unistd.h>
#include <ctime>

// Android includes
#include <sys/types.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#include "system.h"

const char* GLErrMsg[] = {
        "GL_INVALID_ENUM",
        "GL_INVALID_VALUE",
        "GL_INVALID_OPERATION",
        "GL_OUT_OF_MEMORY",
        "unknown",
};

char cCurrentPath[FILENAME_MAX];

AAssetManager* g_pManager = NULL;

void SetAssetManager( AAssetManager* pManager )
{
    g_pManager = pManager;
}

// Read the contents of the give file return the content and the file size.
// The calling function is responsible for free the memory allocated for Content.
void read_file( const char* FileName, char** Content, unsigned int* Size )
{  
    assert( g_pManager );
    
    // Open file
    AAsset* pFile = AAssetManager_open( g_pManager, FileName, AASSET_MODE_UNKNOWN );

    if( pFile != NULL )
    {
        // Determine file size
        off_t FileSize = AAsset_getLength( pFile );
        
        // Read data
        char* pData = (char*)malloc( FileSize );
        AAsset_read( pFile, pData, FileSize );
    
        // Allocate space for the file content
        *Content = (char*)malloc( FileSize );
    
        // Copy the content
        memcpy( *Content, pData, FileSize );
        *Size = FileSize;
        
        // Close the file
        AAsset_close( pFile );

        free( pData );
    }
}

float rand_f ( )
{
    return (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) / 3.0f;
}
