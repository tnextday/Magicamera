package com.funny.magicamera;

import android.content.Context;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;
import java.util.LinkedList;

import static com.funny.magicamera.MagicActivity.CameraFacing;

/**
 * User: tNextDay
 * Description:
 */
public class MSurfaceView extends GLSurfaceView implements GLSurfaceView.Renderer, InputEvent.InputProcessor{
    private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
    private long lastFrameTime = System.nanoTime();
    private float deltaTime = 0.0f;

    LinkedList<byte[]> m_buffers = new LinkedList<byte[]>();

    private InputEvent inputEvent = new InputEvent();

    private CameraBufferReleaseListener m_onCameraBufferRelease = null;

    private InitCompleteListener m_onInitComplete = null;
    
    public MSurfaceView(Context context) {
        super(context);
        Log.w(MagicActivity.TAG, "creating MSurfaceView1");
        init();
    }

    public MSurfaceView(Context context, AttributeSet attrs) {
        super(context, attrs);
        Log.w(MagicActivity.TAG, "creating MSurfaceView2");
        init();

    }

    public void init(){
        if (Build.VERSION.SDK_INT >= 8) {
            setEGLContextClientVersion(2);
        } else {
            setEGLContextFactory(new ContextFactory20());
        }

        //设置EGL环境为32位真彩色，不过Android系统貌似只能显示16位色
        setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        //setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        //如果色深设置成8888，必须设置Holder的format，否则系统会崩溃
        getHolder().setFormat(PixelFormat.TRANSLUCENT);
        //getHolder().setFormat(PixelFormat.RGB_888);

        //setZOrderOnTop(true);
        setRenderer(this);

        inputEvent.setInputProcessor(this);
    }


    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        Log.w(MagicActivity.TAG, "MSurfaceView surfaceDestroyed");
        super.surfaceDestroyed(holder);
        CoreJNILib.destory();
    }

    @Override
    public void onPause() {
        Log.w(MagicActivity.TAG, "MSurfaceView onPause");
        super.onPause();
    }

    @Override
    public void onResume() {
        Log.w(MagicActivity.TAG, "MSurfaceView onResume");
        super.onResume();
    }

    static final FPSLogger fps = new FPSLogger();

    public void onDrawFrame(GL10 gl) {
        long time = System.nanoTime();
        deltaTime = (time - lastFrameTime) / 1000000000.0f;
        lastFrameTime = time;
        inputEvent.processEvents();
        CoreJNILib.step(deltaTime);
        checkFrameBuffer();
        //fps.log();
//        long render_time = System.nanoTime() - time;
//        if (render_time < fps_time){
//            try {
//                Thread.sleep((fps_time-render_time)/1000000);
//            } catch (InterruptedException e) {
//                e.printStackTrace();
//            }
//        }
    }


    private void checkFrameBuffer() {
        byte[] bytes;
        synchronized (m_buffers) {
            bytes = m_buffers.poll();
        }
        if (bytes != null) {
            CoreJNILib.updateInputData(bytes);
            if (m_onCameraBufferRelease != null){
                m_onCameraBufferRelease.onCameraBufferRelease(bytes);
            }
        }
    }

    public void addCameraBuffer(byte[] buffer){
        synchronized (m_buffers) {
            m_buffers.add(buffer);
        }
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        Log.d(MagicActivity.TAG, String.format("onSurfaceChanged: %d,%d", width, height));
        CoreJNILib.resize(width, height);
        this.lastFrameTime = System.nanoTime();
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d(MagicActivity.TAG, "onSurfaceCreated");
        CoreJNILib.create();
        if (m_onInitComplete != null){
            m_onInitComplete.onEngineInitCompleted();
        }
    }
    
    @Override
    public boolean onTouchEvent(MotionEvent e) {
        int x = (int) e.getX();
        int y = (int) e.getY();
        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:
                inputEvent.postTouchEvent(InputEvent.TouchEvent.TOUCH_DRAGGED, x, y, 0);
                return true;
            case MotionEvent.ACTION_DOWN:
                inputEvent.postTouchEvent(InputEvent.TouchEvent.TOUCH_DOWN, x, y, 0);
                return true;
            case MotionEvent.ACTION_UP:
                inputEvent.postTouchEvent(InputEvent.TouchEvent.TOUCH_UP, x, y, 0);
                return true;
        }
        return false;
    }


    @Override
    public boolean touchDown(int x, int y, int pointer) {
        return CoreJNILib.onTouchDown(x, y);
    }


    @Override
    public boolean touchUp(int x, int y, int pointer) {
        return CoreJNILib.onTouchUp(x, y);
    }


    @Override
    public boolean touchDragged(int x, int y, int pointer) {
        return CoreJNILib.onTouchDrag(x, y);
    }


    public void setOnCameraBufferRelease(CameraBufferReleaseListener m_onCameraBufferRelease) {
        this.m_onCameraBufferRelease = m_onCameraBufferRelease;
    }

    public static class FPSLogger {
        long startTime;
        int frames;

        public FPSLogger() {
            startTime = System.nanoTime();
            frames = 0;
        }

        /**
         * Logs the current frames per second to the console.
         */
        public void log() {
            frames++;
            if (System.nanoTime() - startTime > 1000000000) {
                Log.i("Magic FPS", "fps: " + frames);
                frames = 0;
                startTime = System.nanoTime();
            }
        }
    }

    public void setOnInitComplete(InitCompleteListener m_onInitComplete) {
        this.m_onInitComplete = m_onInitComplete;
    }

    public interface InitCompleteListener {
        void onEngineInitCompleted();
    }

    public interface CameraBufferReleaseListener{
        void onCameraBufferRelease(byte[] buffer);
    }

    private static class ContextFactory20 implements GLSurfaceView.EGLContextFactory {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;

        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            Log.w(MagicActivity.TAG, "creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE};
            EGLContext context = egl.eglCreateContext(display, eglConfig, EGL10.EGL_NO_CONTEXT, attrib_list);
            checkEglError("After eglCreateContext", egl);
            return context;
        }

        public void destroyContext(EGL10 egl, EGLDisplay display, EGLContext context) {
            egl.eglDestroyContext(display, context);
        }
    }
    
    private static void checkEglError(String prompt, EGL10 egl) {
        int error;
        while ((error = egl.eglGetError()) != EGL10.EGL_SUCCESS) {
            Log.e(MagicActivity.TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }

    static public class SetOverlay implements Runnable {
        String m_path;
        public SetOverlay(String path) {
            this.m_path = path;
        }
        @Override
        public void run() {
            CoreJNILib.setCover(m_path);
        }
    }
    static public class SetFrame implements Runnable {
        String m_path;
        public SetFrame(String path) {
            this.m_path = path;
        }
        @Override
        public void run() {
            CoreJNILib.setFrame(m_path);
        }
    }
    static public class SetEffect implements Runnable {
        String m_path;
        public SetEffect(String path) {
            this.m_path = path;
        }
        @Override
        public void run() {
            CoreJNILib.setEffect(m_path);
        }
    }



    static public class TakePicture implements Runnable {
        byte[] mBuffer = null;

        public TakePicture(byte[] mBuffer) {
            this.mBuffer = mBuffer;
        }

        @Override
        public void run() {
            if (mBuffer == null)
                CoreJNILib.takePicture();
            else
                CoreJNILib.takePictureWithBuffer(mBuffer);
        }
    }

    static public class SetImage implements Runnable {
        String img_path;

        public SetImage(String img_path) {
            this.img_path = img_path;
        }

        @Override
        public void run() {
            CoreJNILib.setInputImage(img_path);
        }
    }

    static public class SetPreviewInfo implements Runnable {
        int width;
        int height;
        int format;
        CameraFacing mFacing;

        public SetPreviewInfo(int width, int height, int format, CameraFacing facing) {
            this.width = width;
            this.height = height;
            this.format = format;
            mFacing = facing;
        }

        @Override
        public void run() {
            CoreJNILib.resetRotation();
            CoreJNILib.setInputDataInfo(width, height, format);
            if (mFacing == CameraFacing.FRONT){
                CoreJNILib.rotate90(false);
                //因为先旋转了以后要所以才要flipY吗？
                CoreJNILib.flip(false, true);
            } else{
                CoreJNILib.rotate90(true);
            }
        }
    }
}
