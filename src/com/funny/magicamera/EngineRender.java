package com.funny.magicamera;

import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.util.LinkedList;


/**
 * User: tNextDay
 * Description:
 */
public class EngineRender implements GLSurfaceView.Renderer, InputEvent.InputProcessor {

    private long lastFrameTime = System.nanoTime();
    private float deltaTime = 0.0f;

    LinkedList<byte[]> m_buffers = new LinkedList<byte[]>();
    int m_previewHeight;
    int m_previewWidth;

    String m_imagePath = "";

    private InputEvent inputEvent = new InputEvent();

    private InitCompleteListener m_onInitComplete = null;
    private CameraBufferReleaseListener m_onCameraBufferRelease = null;


    public EngineRender() {
        super();
        inputEvent.setInputProcessor(this);
    }

    static final FPSLogger fps = new FPSLogger();

    public void onDrawFrame(GL10 gl) {
        long time = System.nanoTime();
        deltaTime = (time - lastFrameTime) / 1000000000.0f;
        lastFrameTime = time;
        inputEvent.processEvents();
        checkFrameBuffer();
        MagicJNILib.step(deltaTime);
        fps.log();
    }


    private void checkFrameBuffer() {
        byte[] bytes;
        synchronized (m_buffers) {
            bytes = m_buffers.poll();
        }
        if (bytes != null) {
            MagicJNILib.uploadPreviewData(bytes);
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
        MagicJNILib.init(width, height);

        if (m_onInitComplete != null){
            m_onInitComplete.onEngineInitCompleted(this);
        }
        if (!"".equals(m_imagePath)){
            MagicJNILib.setPreviewImage(m_imagePath);
        }
        this.lastFrameTime = System.nanoTime();
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        Log.d(MagicActivity.TAG, "onSurfaceCreated");
    }

    public void setPreviewInfo(int width, int height, int format){
        m_previewWidth = width;
        m_previewHeight =height;
        MagicJNILib.setPreviewDataInfo(m_previewWidth, m_previewHeight, format);
    }

    public void setDestImage(String path) {
        m_imagePath = path;
    }



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
        return MagicJNILib.onTouchDown(x, y);
    }


    @Override
    public boolean touchUp(int x, int y, int pointer) {
        return MagicJNILib.onTouchUp(x, y);
    }


    @Override
    public boolean touchDragged(int x, int y, int pointer) {
        return MagicJNILib.onTouchDrag(x, y);
    }


    public void setOnInitComplete(InitCompleteListener m_onInitComplete) {
        this.m_onInitComplete = m_onInitComplete;
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
                //Log.i("Magic FPS", "fps: " + frames);
                frames = 0;
                startTime = System.nanoTime();
            }
        }
    }

    public interface InitCompleteListener {
        void onEngineInitCompleted(EngineRender engine);
    }

    public interface CameraBufferReleaseListener{
        void onCameraBufferRelease(byte[] buffer);
    }
}