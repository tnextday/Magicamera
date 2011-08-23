package com.funny.magicamera;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.graphics.ImageFormat;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.widget.Toast;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

public class MagicActivity extends Activity implements Camera.PreviewCallback, EngineRender.InitCompleteListener, EngineRender.CameraBufferReleaseListener {
    GLSurfaceView m_SurfaceView;
    EngineRender  m_engine;
    public static int SDK_Version = Build.VERSION.SDK_INT;
    public static String TAG = "MagicEngine";

    //    int m_CameraId; //use above 2.3
    Camera m_Camera = null;
    final static int BufferCount = 2;

    int m_previewHeight = 640;
    int m_previewWidth = 480;

    public String PicPath = null;



    enum CameraType {
        FACING_BACK, FACING_FRONT
    }
    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!detectOpenGLES20()){
            //TODO 不支持gles 2.0
            Toast.makeText(this, "不支持OpenglEs 2.0,程序将退出！", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        setContentView(R.layout.magic);
        m_SurfaceView = (GLSurfaceView)findViewById(R.id.glsurfaceview);
        if (SDK_Version >= 8) {
            m_SurfaceView.setEGLContextClientVersion(2);
        } else {
            m_SurfaceView.setEGLContextFactory(new ContextFactory20());
        }
        //设置EGL环境为32位真彩色，不过Android系统貌似只能显示16位色
        m_SurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
        //如果色深设置成8888，必须设置Holder的format，否则系统会崩溃
        m_SurfaceView.getHolder().setFormat(PixelFormat.RGBA_8888);

        m_engine = new EngineRender();
        m_engine.setOnInitComplete(this);
        if (SDK_Version >= 8)
            m_engine.setOnCameraBufferRelease(this);
        m_SurfaceView.setRenderer(m_engine);

        Intent intent = getIntent();
        String picPath =  intent.getStringExtra("PicturePath");
        if (picPath != null && new File(picPath).exists()){
            PicPath = picPath;
        }
    }

    @Override
    public void onEngineInitCompleted(EngineRender engine) {
        if (PicPath != null){
            engine.setLocalTexture(PicPath);
        }else{
            //TODO 异步执行
            startCamera(CameraType.FACING_FRONT);
        }
    }


    @Override
    protected void onPause() {
        super.onPause();
        Log.d(TAG, "OnPause");
    }

    @Override
    protected void onResume() {
        super.onResume();
        Log.d(TAG, "onResume");
    }

    private boolean detectOpenGLES20() {
        ActivityManager am =
                (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x20000);
    }


    int frameCount = 0;

    public void Save2File(byte[] bytes) {
        File texFile = new File(String.format("/sdcard/nv21/%03d.nv21", frameCount));
        frameCount++;
        try {
            texFile.createNewFile();
            FileOutputStream fos = new FileOutputStream(texFile);
            fos.write(bytes);
            fos.close();
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }

    }

    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        m_engine.addCameraBuffer(bytes);
    //    	m_Camera.addCallbackBuffer(bytes);
    }

    @Override
    public void onCameraBufferRelease(byte[] buffer) {
        m_Camera.addCallbackBuffer(buffer);
    }


    public void startCamera(CameraType cameraType) {
        int cameraId = 0;
        if (SDK_Version >= 9) {
            int facing = Camera.CameraInfo.CAMERA_FACING_BACK;
            if (cameraType == CameraType.FACING_FRONT){
                facing = Camera.CameraInfo.CAMERA_FACING_FRONT;
            }
            int numberOfCameras = Camera.getNumberOfCameras();
            Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
            for (int i = 0; i < numberOfCameras; i++) {
                Camera.getCameraInfo(i, cameraInfo);
                if (cameraInfo.facing == facing) {
                    cameraId = i;
                }
            }
        }
        if (SDK_Version >= 9) {
                m_Camera = Camera.open(cameraId);
            } else {
                m_Camera = Camera.open();
            }

            // Now that the size is known, set up the camera parameters and begin
            // the preview.
            Camera.Parameters parameters = m_Camera.getParameters();

            List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
            //        List<Size> psizes = parameters.getSupportedPictureSizes();
            List<Integer> formats = parameters.getSupportedPreviewFormats();

            //formats  = parameters.getSupportedPictureFormats();
            Camera.Size optimalSize = getOptimalPreviewSize(sizes, 640, 480);
            parameters.setPreviewSize(optimalSize.width, optimalSize.height);
            if (formats.contains(MagicJNILib.IMAGE_FORMAT_RGB565)) {
                parameters.setPreviewFormat(MagicJNILib.IMAGE_FORMAT_RGB565);
            }
            m_Camera.setParameters(parameters);

            parameters = m_Camera.getParameters();
            Camera.Size previewSize = parameters.getPreviewSize();
            m_previewWidth = previewSize.width;
            m_previewHeight = previewSize.height;
            int previewFormat = parameters.getPreviewFormat();
            int szBuffer = previewSize.width * previewSize.height * ImageFormat.getBitsPerPixel(previewFormat) / 8;
            //2.2以上版本才能使用addCallbackBuffer，这个效率比不是用callbackbuffer高30%
            if (SDK_Version >= 8) {
                m_Camera.setPreviewCallbackWithBuffer(this);
                for (int i = 0; i < BufferCount; i++) {
                    m_Camera.addCallbackBuffer(new byte[szBuffer]);
                }
            } else {
                m_Camera.setPreviewCallback(this);
            }
            m_engine.setPreviewInfo(m_previewWidth, m_previewHeight, parameters.getPreviewFormat());
            m_Camera.startPreview();
    }

    public void stopCamera() {
        if (m_Camera == null) return;
        m_Camera.stopPreview();
        if (SDK_Version >= 8) {
            m_Camera.setPreviewCallbackWithBuffer(null);
        } else {
            m_Camera.setPreviewCallback(null);
        }

        m_Camera.release();
        m_Camera = null;
    }

    public void switchCamera(CameraType cameraType){
        stopCamera();
        switchCamera(cameraType);
    }


    private Camera.Size getOptimalPreviewSize(List<Camera.Size> sizes, int w, int h) {
        final double ASPECT_TOLERANCE = 0.05;
        double targetRatio = (double) w / h;
        if (sizes == null) return null;

        Camera.Size optimalSize = null;
        double minDiff = Double.MAX_VALUE;

        int targetHeight = h;

        // Try to find an size match aspect ratio and size
        for (Camera.Size size : sizes) {
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
            for (Camera.Size size : sizes) {
                if (Math.abs(size.height - targetHeight) < minDiff) {
                    optimalSize = size;
                    minDiff = Math.abs(size.height - targetHeight);
                }
            }
        }
        return optimalSize;
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (m_engine.onTouchEvent(event))
           return true;
        return super.onTouchEvent(event);
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
}