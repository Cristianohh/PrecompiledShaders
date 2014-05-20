package com.intel.precompiledshaders;

import android.app.Activity;
import android.content.res.AssetManager;
import android.os.Bundle;
import android.util.Log;
import android.view.View;


public class PrecompiledMain extends Activity
{
    static AssetManager sAssetManager;
    PrecompiledView mView;

    // On applications creation
    @Override protected void onCreate( Bundle savedInstanceState ) 
    {
        super.onCreate( savedInstanceState );

        // Pass the asset manager to the native code
        sAssetManager = getAssets();
        PrecompiledLib.createAssetManager( sAssetManager );

        // Set the layout
        setContentView( R.layout.main );

        // Create our view for OpenGL rendering
        mView = ( PrecompiledView )findViewById( R.id.precompiled_view );
    }

    @Override protected void onPause() 
    {
        super.onPause();
        mView.onPause();
    }

    @Override protected void onResume() 
    {
        super.onResume();
        mView.onResume();
    }
}