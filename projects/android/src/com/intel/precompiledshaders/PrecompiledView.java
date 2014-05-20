package com.intel.precompiledshaders;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.util.AttributeSet;
import android.view.View;
import android.view.MotionEvent;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import android.os.SystemClock;

class PrecompiledView extends GLSurfaceView 
{
    private PrecompiledRenderer m_pRenderer;

    public PrecompiledView( Context context ) 
    {
        super( context );
        init();
    }
    
    public PrecompiledView( Context context, AttributeSet attrs )
    {
        super( context, attrs );
        init();
    }

    public void init()
    {
        // Create an OpenGL ES 2.0 context
        setEGLContextClientVersion( 3 );

        // Set the renderer associated with this view
        m_pRenderer = new PrecompiledRenderer();
        setRenderer( m_pRenderer );
    }

    private static class PrecompiledRenderer implements GLSurfaceView.Renderer 
    {
        private long m_nLastTime;

        public void onDrawFrame( GL10 gl )
        {
            // calculate elapsed time
            if( m_nLastTime == 0 )
                m_nLastTime = SystemClock.elapsedRealtime();

            long nCurrentTime = SystemClock.elapsedRealtime();
            long nElapsedTime = nCurrentTime - m_nLastTime;
            float fElapsedTime = nElapsedTime / 1000.0f;
            m_nLastTime = nCurrentTime;

            PrecompiledLib.step( fElapsedTime );
        }

        public void onSurfaceChanged( GL10 gl, int width, int height )
        {
            PrecompiledLib.init( width, height );
        }

        public void onSurfaceCreated( GL10 gl, EGLConfig config ) {}
    }
}

