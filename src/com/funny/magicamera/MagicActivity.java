package com.funny.magicamera;

import android.app.Activity;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;
import java.util.List;

public class MagicActivity extends Activity implements Camera.PreviewCallback, View.OnClickListener,
            MSurfaceView.InitCompleteListener, MSurfaceView.CameraBufferReleaseListener {
    MSurfaceView m_SurfaceView;
    public static String TAG = "MagicEngine";

    //    int m_CameraId; //use above 2.3
    Camera m_Camera = null;
    final static int BufferCount = 2;

    int m_previewHeight = 480;
    int m_previewWidth = 640;

    public String PicPath = null;
    private final static int DIALOG_SELECT_ENGINE = 0;
    private final static int DIALOG_SELECT_COVER = 1;
    private final static int DIALOG_SELECT_FRAME = 2;
    private final static int DIALOG_SELECT_EFFECT = 3;

    private String[] m_frames;
    private String[] m_covers;
    private String[] m_effects;

    enum CameraType {
        FACING_BACK, FACING_FRONT
    }

    /**
     * Called when the activity is first created.
     */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        if (!detectOpenGLES20()) {
            //TODO 不支持gles 2.0
            Toast.makeText(this, "不支持OpenglEs 2.0,程序将退出！", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }
        setContentView(R.layout.magic);


        findViewById(R.id.btn_back).setOnClickListener(this);
        findViewById(R.id.btn_set_cover).setOnClickListener(this);
        findViewById(R.id.btn_set_frame).setOnClickListener(this);
        findViewById(R.id.btn_set_effect).setOnClickListener(this);
        findViewById(R.id.btn_engine).setOnClickListener(this);
        findViewById(R.id.btn_restore).setOnClickListener(this);
        findViewById(R.id.btn_take).setOnClickListener(this);
        findViewById(R.id.btn_focus).setOnClickListener(this);

        m_SurfaceView = (MSurfaceView) findViewById(R.id.surfaceview);
        Log.w(MagicActivity.TAG, "MagicActivity onCreate");
//        if (Build.VERSION.SDK_INT >= 8) {
//            m_SurfaceView.setEGLContextClientVersion(2);
//        } else {
//            m_SurfaceView.setEGLContextFactory(new ContextFactory20());
//        }
//        //设置EGL环境为32位真彩色，不过Android系统貌似只能显示16位色
//        m_SurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0);
//        //如果色深设置成8888，必须设置Holder的format，否则系统会崩溃
//        m_SurfaceView.getHolder().setFormat(PixelFormat.RGBA_8888);
        Intent intent = getIntent();
        String picPath = intent.getStringExtra("PicPath");

        try {
            AssetManager am = getResources().getAssets();
            m_covers = am.list("covers");
            m_frames = am.list("frames");
        } catch (IOException e) {
            e.printStackTrace();
        }
        String effectList = "--None--,"+MagicJNILib.getEffectList();
        m_effects = effectList.split(",");
        m_SurfaceView.setOnInitComplete(this);
        if (Build.VERSION.SDK_INT >= 8)
            m_SurfaceView.setOnCameraBufferRelease(this);

        if (picPath != null && new File(picPath).exists()) {
            PicPath = picPath;
        }
        MagicJNILib.onTake = new MagicJNILib.TakePictureListener(){

            @Override
            public void onTakePicture(String picPath) {
                //Toast.makeText(MagicActivity.this, "图片已保存！", Toast.LENGTH_SHORT);
            }
        };

    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_back) {
            finish();
        } else if (view.getId() == R.id.btn_engine) {
            showDialog(DIALOG_SELECT_ENGINE);
        } else if (view.getId() == R.id.btn_set_cover) {
            showDialog(DIALOG_SELECT_COVER);
        } else if (view.getId() == R.id.btn_set_frame) {
            showDialog(DIALOG_SELECT_FRAME);
        } else if (view.getId() == R.id.btn_set_effect) {
            showDialog(DIALOG_SELECT_EFFECT);
        } else if (view.getId() == R.id.btn_restore) {
            MagicJNILib.restoreMesh();
        } else if (view.getId() == R.id.btn_take) {
            m_SurfaceView.queueEvent(new TakePicture());
        } else if (view.getId() == R.id.btn_focus){
            m_Camera.autoFocus(null);
        }
    }

    private void switchEngine(int type){
        findViewById(R.id.btn_restore).setVisibility(type == MagicJNILib.ENGINE_TYPE_MESH ? View.VISIBLE : View.GONE);

        findViewById(R.id.btn_set_cover).setVisibility(type == MagicJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        findViewById(R.id.btn_set_effect).setVisibility(type == MagicJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        findViewById(R.id.btn_set_frame).setVisibility(type == MagicJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        MagicJNILib.switchEngine(type);
    }

    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
            case DIALOG_SELECT_ENGINE:
                return new AlertDialog.Builder(this)
                        .setItems(R.array.engine_types, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                if (which == 0) {
                                    switchEngine(MagicJNILib.ENGINE_TYPE_MESH);
                                } else {
                                    switchEngine(MagicJNILib.ENGINE_TYPE_EFFECT);
                                }
                            }
                        })
                        .create();
            case DIALOG_SELECT_COVER:
                return new AlertDialog.Builder(this)
                        .setItems(m_covers, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                m_SurfaceView.queueEvent(new SetCover("res://covers/"+m_covers[which]));
                            }
                        })
                        .create();
            case DIALOG_SELECT_FRAME:
                return new AlertDialog.Builder(this)
                        .setItems(m_frames, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                m_SurfaceView.queueEvent(new SetFrame("res://frames/"+m_frames[which]));
                            }
                        })
                        .create();
            case DIALOG_SELECT_EFFECT:
                return new AlertDialog.Builder(this)
                        .setItems(m_effects, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                m_SurfaceView.queueEvent(new SetEffect(m_effects[which]));
                            }
                        })
                        .create();

        }
        return null;
    }

    private class SetCover implements Runnable {
        String m_path;
        private SetCover(String path) {
            this.m_path = path;
        }
        @Override
        public void run() {
            MagicJNILib.setCover(m_path);
        }
    }
    private class SetFrame implements Runnable {
        String m_path;
        private SetFrame(String path) {
            this.m_path = path;
        }
        @Override
        public void run() {
            MagicJNILib.setFrame(m_path);
        }
    }
    private class SetEffect implements Runnable {
        String m_path;
        private SetEffect(String path) {
            this.m_path = path;
        }
        @Override
        public void run() {
            MagicJNILib.setEffect(m_path);
        }
    }



    private class TakePicture implements Runnable {
        @Override
        public void run() {
            MagicJNILib.takePicture();
        }
    }

    private class SetImage implements Runnable {
        String img_path;

        private SetImage(String img_path) {
            this.img_path = img_path;
        }

        @Override
        public void run() {
            MagicJNILib.setInputImage(img_path);
        }
    }

    private class SetPreviewInfo implements Runnable {
        int width;
        int height;
        int format;

        private SetPreviewInfo(int width, int height, int format) {
            this.width = width;
            this.height = height;
            this.format = format;
        }

        @Override
        public void run() {
            MagicJNILib.setInputDataInfo(width, height, format);
            MagicJNILib.rotate90(true);
        }
    }

    @Override
    public void onEngineInitCompleted() {
        if (PicPath != null) {
            m_SurfaceView.queueEvent(new SetImage(PicPath));
        } else {
            //TODO 异步执行
            startCamera(CameraType.FACING_FRONT);
        }
    }


    @Override
    protected void onPause() {
        stopCamera();
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


    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        m_SurfaceView.addCameraBuffer(bytes);
        //    	m_Camera.addCallbackBuffer(bytes);
    }

    @Override
    public void onCameraBufferRelease(byte[] buffer) {
        m_Camera.addCallbackBuffer(buffer);
    }


    public void startCamera(CameraType cameraType) {
        m_Camera = null;
        if (Build.VERSION.SDK_INT >= 9) {
            int numberOfCameras = Camera.getNumberOfCameras();
            if (numberOfCameras > 1){
                int cameraId = 0;
                int facing = Camera.CameraInfo.CAMERA_FACING_BACK;
                if (cameraType == CameraType.FACING_FRONT) {
                    facing = Camera.CameraInfo.CAMERA_FACING_FRONT;
                }
                Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
                for (int i = 0; i < numberOfCameras; i++) {
                    Camera.getCameraInfo(i, cameraInfo);
                    if (cameraInfo.facing == facing) {
                        cameraId = i;
                    }
                }
                m_Camera = Camera.open(cameraId);
            }
        } 
        if (m_Camera == null){
            m_Camera = Camera.open();
        }

        // Now that the size is known, set up the camera parameters and begin
        // the preview.
        Camera.Parameters parameters = m_Camera.getParameters();

        List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
        //        List<Size> psizes = parameters.getSupportedPictureSizes();
        List<Integer> formats = parameters.getSupportedPreviewFormats();

        //formats  = parameters.getSupportedPictureFormats();
        Camera.Size optimalSize = getOptimalPreviewSize(sizes, m_previewWidth, m_previewHeight);
        parameters.setPreviewSize(optimalSize.width, optimalSize.height);
        if (formats.contains(MagicJNILib.IMAGE_FORMAT_RGB565)) {
            parameters.setPreviewFormat(MagicJNILib.IMAGE_FORMAT_RGB565);
        }

        List<String> FocusModes = parameters.getSupportedFocusModes();
        if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO))
            parameters.setFlashMode(Camera.Parameters.FOCUS_MODE_AUTO);
        m_Camera.setParameters(parameters);

        parameters = m_Camera.getParameters();
        Camera.Size previewSize = parameters.getPreviewSize();
        m_previewWidth = previewSize.width;
        m_previewHeight = previewSize.height;
        int previewFormat = parameters.getPreviewFormat();
        int szBuffer = previewSize.width * previewSize.height * ImageFormat.getBitsPerPixel(previewFormat) / 8;
        //2.2以上版本才能使用addCallbackBuffer，这个效率比不是用callbackbuffer高30%
        if (Build.VERSION.SDK_INT >= 8) {
            m_Camera.setPreviewCallbackWithBuffer(this);
            for (int i = 0; i < BufferCount; i++) {
                m_Camera.addCallbackBuffer(new byte[szBuffer]);
            }
        } else {
            m_Camera.setPreviewCallback(this);
        }
        m_SurfaceView.queueEvent(new SetPreviewInfo(m_previewWidth, m_previewHeight, parameters.getPreviewFormat()));
        //MagicJNILib.rotate90(true);
        m_Camera.startPreview();
    }

    public void stopCamera() {
        if (m_Camera == null) return;
        m_Camera.stopPreview();
        if (Build.VERSION.SDK_INT >= 8) {
            m_Camera.setPreviewCallbackWithBuffer(null);
        } else {
            m_Camera.setPreviewCallback(null);
        }

        m_Camera.release();
        m_Camera = null;
    }

    public void switchCamera(CameraType cameraType) {
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

//    @Override
//    public boolean onTouchEvent(MotionEvent event) {
//        if (m_SurfaceView.onTouchEvent(event))
//            return true;
//        return super.onTouchEvent(event);
//    }

}