package com.intel.precompiledshaders;

import android.content.res.AssetManager;

public class PrecompiledLib 
{

    // This is static so it will be called at startup (without creating an instance of this class)
     static 
     {
         System.loadLibrary( "precompiledapp" );
     }

    // Define native functions that can be called from Java code
     public static native void init( int width, int height );
     public static native void step( float fElapsedTime );
     public static native void createAssetManager( AssetManager assetManager );
}
