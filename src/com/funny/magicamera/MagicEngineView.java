package com.funny.magicamera;

import android.content.Context;
import android.graphics.Bitmap;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.List;
import java.util.concurrent.locks.ReentrantLock;

/**
 * User: tNextDay
 * Description:
 */
public class MagicEngineView extends GL20SurfaceView
        implements GLSurfaceView.Renderer, Camera.PreviewCallback{

    boolean m_bUseCamera = false;
//    int m_CameraId; //use above 2.3
    Camera m_Camera = null;
    byte[] m_frameBuffer = null;
    boolean m_frameChanged = false;
    final ReentrantLock m_lock = new ReentrantLock();
    int     m_previewHeight = 480;
    int     m_previewWidth = 640;

    public MagicEngineView(Context context) {
        super(context);
//        after api level 8 can do as that
//        this.setEGLContextClientVersion(2);
        setRenderer(this);
    }
    public void onDrawFrame(GL10 gl) {
        checkFrameBuffer();
        MagicJNILib.step();
    }


    public void checkFrameBuffer(){
        m_lock.lock();
        if (m_frameChanged){
            MagicJNILib.uploadPreviewData(m_frameBuffer, m_frameBuffer.length);
            m_frameChanged = false;
        }
        m_lock.unlock();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
         MagicJNILib.init(width, height);
        if (m_bUseCamera){
            startCamera();
        }else{
            setLocalTexture("/sdcard/test/tex.jpg");
        }
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {

    }


    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        if (m_bUseCamera)
            stopCamera();
        super.surfaceDestroyed(holder);
    }

    public void setLocalTexture(String path){
        Bitmap bitmap = null;
//        bitmap = BitmapFactory.decodeFile(path);
//        m_previewHeight = bitmap.getHeight();
//        m_previewWidth = bitmap.getWidth();
//        bitmap.recycle();
        MagicJNILib.setPreviewDataInfo(m_previewWidth, m_previewHeight, MagicJNILib.IMAGE_FORMAT_PACKET);
        InputStream inputStream = null;

        byte[] buffer = null;
        int szRead = 0;
        try {
//            inputStream = new FileInputStream(path);
            inputStream = getResources().getAssets().open("test2.jpg");
            buffer = new byte[inputStream.available()];
            szRead = inputStream.read(buffer);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
        } catch (IOException e) {
            e.printStackTrace(); 
        }
        if (szRead > 0){
            MagicJNILib.uploadPreviewData(buffer, buffer.length);
        }
        buffer = null;
    }


    @Override
    public boolean onTouchEvent(MotionEvent e) {
        float x = e.getX();
        float y = e.getY();
        switch (e.getAction()) {
            case MotionEvent.ACTION_MOVE:
                return MagicJNILib.onTouchDrag(x, y);
            case MotionEvent.ACTION_DOWN:
                return MagicJNILib.onTouchDown(x, y);
            case MotionEvent.ACTION_UP:
                return MagicJNILib.onTouchUp(x, y);
        }
        return true;
//        return super.onTouchEvent(e);
    }

    public void startCamera(){
        m_Camera = Camera.open();
        m_Camera.setPreviewCallback(this);
        // Now that the size is known, set up the camera parameters and begin
        // the preview.
        Camera.Parameters parameters = m_Camera.getParameters();

        List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
        List<Size> psizes = parameters.getSupportedPictureSizes();
        List<Integer> formats = parameters.getSupportedPreviewFormats();

        //formats  = parameters.getSupportedPictureFormats();
        Camera.Size optimalSize = getOptimalPreviewSize(sizes, 640, 480);
        m_previewHeight = optimalSize.height;
        m_previewWidth = optimalSize.width;
        parameters.setPreviewSize(m_previewWidth, m_previewHeight);
        int szBuffer;
        if (formats.contains(MagicJNILib.IMAGE_FORMAT_RGB565)){
            parameters.setPreviewFormat(MagicJNILib.IMAGE_FORMAT_RGB565);
            szBuffer = m_previewWidth*m_previewHeight*2;
        }else {
            szBuffer = m_previewWidth*m_previewHeight*12/8;
        }
        m_frameBuffer = new byte[szBuffer];
        MagicJNILib.setPreviewDataInfo(m_previewWidth, m_previewHeight, parameters.getPreviewFormat());
        m_Camera.setParameters(parameters);
        m_Camera.startPreview();
    }

    public void stopCamera(){
        if (m_Camera == null) return;
        m_Camera.stopPreview();
        m_Camera.setPreviewCallback(null);
        m_Camera.release();
        m_Camera = null;
    }


    private Size getOptimalPreviewSize(List<Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.05;
        double targetRatio = (double) w / h;
        if (sizes == null) return null;

        Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        // Try to find an size match aspect ratio and size
        for (Size size : sizes) {
            double ratio = (double) size.width / size.height;
            if (Math.abs(ratio - targetRatio) > ASPECT_TOLERANCE) continue;
            if (Math.abs(size.height - targetHeight) < minDiff) {
                optimalSize = size;
                minDiff = Math.abs(size.height - targetHeight);
            }
        }

        // Cannot find the one match the aspect ratio, ignore the requirement
        if (optimalSize == null) {
            minDiff = Double.MAX_VALUE;
            for (Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
    }

        @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        m_lock.lock();
        System.arraycopy(bytes, 0, m_frameBuffer, 0, bytes.length);
        m_frameChanged = true;
        m_lock.unlock();
    }
}