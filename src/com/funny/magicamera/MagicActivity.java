package com.funny.magicamera;

import android.app.ActivityGroup;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.Gravity;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.*;
import com.funny.magicamera.popupviews.AutoFocusView;
import com.funny.magicamera.popupviews.CameraSetting;
import com.funny.magicamera.popupviews.FilterSelect;
import com.funny.magicamera.utils.SoundEngine;

import java.io.File;
import java.util.List;
import java.util.Timer;
import java.util.TimerTask;

import static com.funny.magicamera.MSurfaceView.*;

public class MagicActivity extends ActivityGroup implements Camera.PreviewCallback,
            View.OnClickListener,
            CameraBufferReleaseListener,
            FilterSelect.FilterSelected {
    public static String TAG = "MagicEngine";

    private Camera mCamera = null;
    final static int BufferCount = 2;

    int mPreviewHeight = 360;
    int mPreviewWidth = 480;

    int mOutputHeight = 768;
    int mOutputWidth = 1024;

    public String mPicPath = null;
    private final static int DIALOG_SELECT_ENGINE = 0;

    private SoundEngine mSoundEngine;

    private FilterSelect mFilterSelect;
    private CameraSetting mCameraSetting = null;
    private AutoFocusView mAutoFocusView;

    MSurfaceView m_SurfaceView;
    private TextSwitcher ts_delay;

    enum CameraFacing {
        BACK, FRONT
    }

    public CameraFacing mCameraFacing = CameraFacing.BACK;

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
        setContentView(R.layout.magic_camera);

        findViewById(R.id.btn_back).setOnClickListener(this);
        findViewById(R.id.btn_mode).setOnClickListener(this);

        //设置倒计时view
        ts_delay = (TextSwitcher)findViewById(R.id.ts_delay);
        ts_delay.setFactory(new ViewSwitcher.ViewFactory() {
            @Override
            public View makeView() {
                return View.inflate(MagicActivity.this, R.layout.delay_text,null);
            }
        });
        Animation in = AnimationUtils.loadAnimation(this,
                android.R.anim.fade_in);
        Animation out = AnimationUtils.loadAnimation(this,
                android.R.anim.fade_out);
        ts_delay.setInAnimation(in);
        ts_delay.setOutAnimation(out);


        Button btn_take = (Button)findViewById(R.id.btn_take);
        btn_take.setOnClickListener(this);
        btn_take.setLongClickable(true);
        //长按自动对焦
        btn_take.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                if (mCamera != null){
                    mCamera.autoFocus(new Camera.AutoFocusCallback() {
                        @Override
                        public void onAutoFocus(boolean b, Camera camera) {
                            mSoundEngine.playEffect(MagicActivity.this, R.raw.auto_fouse);
                            mAutoFocusView.autoFocusCompleted();
                        }
                    });
                    beginAutoFocus();
                }
                return true;
            }
        });
        findViewById(R.id.btn_camera_cfg).setOnClickListener(this);
        findViewById(R.id.btn_effect).setOnClickListener(this);


        m_SurfaceView = (MSurfaceView) findViewById(R.id.surfaceview);
        Log.w(MagicActivity.TAG, "MagicActivity onCreate");

        Intent intent = getIntent();
        String picPath = intent.getStringExtra("mPicPath");

        m_SurfaceView.setOnInitComplete(new InitCompleteListener(){
            @Override
            public void onEngineInitCompleted() {
                mEventHandler.sendMessage(Msg_Engine_Inited, null);
            }
        });
        if (Build.VERSION.SDK_INT >= 8)
            m_SurfaceView.setOnCameraBufferRelease(this);

        if (picPath != null && new File(picPath).exists()) {
            mPicPath = picPath;
        }
        CoreJNILib.onTake = new CoreJNILib.TakePictureListener(){
            @Override
            public void onPictureSaved(String picPath) {
                mEventHandler.sendMessage(Msg_On_Picture_Saved, picPath);
            }
        };

        mFilterSelect = new FilterSelect(this, this);
        mCameraSetting = new CameraSetting(this);
        mAutoFocusView = new AutoFocusView(this);

        mSoundEngine = new SoundEngine();
        mSoundEngine.preloadEffect(this, R.raw.auto_fouse);
        mSoundEngine.preloadEffect(this, R.raw.camera_shoot);
        mSoundEngine.preloadEffect(this, R.raw.delay_timer_beep);
        mSoundEngine.setEffectsVolume(1.0f);
    }


    /**
     * 设置滤镜类型
     * @param type
     * @param name
     */
    @Override
    public void OnFilterSelected(String type, String name) {
        if (type.equals(FilterSelect.Type_Filter)){
            m_SurfaceView.queueEvent(new SetEffect(name));
        }else if (type.equals(FilterSelect.Type_Cover)){
            m_SurfaceView.queueEvent(new SetOverlay(name));
        }else if (type.equals(FilterSelect.Type_Frame)){
            m_SurfaceView.queueEvent(new SetFrame(name));
        }
    }

    /**
     * View.OnClickLister回调
     * @param view
     */
    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_back) {
            finish();
        } else if (view.getId() == R.id.btn_mode) {
            showDialog(DIALOG_SELECT_ENGINE);
        } else if (view.getId() == R.id.btn_take) {
            takePicture();

        } else if (view.getId() == R.id.btn_camera_cfg){
            int[] location = new int[2];
            view.getLocationInWindow(location);
            int y = location[1] - mCameraSetting.getMeasuredHeight();
            mCameraSetting.showAtLocation(view, Gravity.NO_GRAVITY, 0, y);
        } else if (view.getId() == R.id.btn_effect){
            mFilterSelect.showAtLocation(findViewById(R.id.tool_bar), Gravity.BOTTOM, 0, 0);
        }
    }

    /**
     * 界面上现实对焦框
     */
    private void beginAutoFocus(){
        View v = findViewById(R.id.surfaceview);
//        int[] location = new int[2];
//        v.getLocationInWindow(location);
//        v.getMeasuredHeight();
//        int x = location[0] + (v.getMeasuredWidth() - mAutoFocusView.getMeasuredWidth())/2;
//        int y = location[1] + (v.getMeasuredHeight() - mAutoFocusView.getMeasuredHeight())/2;
        mAutoFocusView.beginAutoFocus();
        mAutoFocusView.showAtLocation(v, Gravity.CENTER, 0, 0);
    }

    /**
     * 拍照
     */
    void takePicture() {
        if (mPicPath == null){
            if (mDelayTime > 0){
                ts_delay.setCurrentText(String.valueOf(mDelayTime));
                ts_delay.setVisibility(View.VISIBLE);
                final Timer timer = new Timer();
                timer.scheduleAtFixedRate (new TimerTask() {
                    int mRemainTime = mDelayTime;
                    @Override
                    public void run() {
                        //TODO 播放声音
                        mRemainTime--;
                        mEventHandler.sendMessage(Msg_On_Delay_Shutter_Timer, mRemainTime);
                        if (mRemainTime == 0){
                            timer.cancel();
                        }
                    }
                }, 1000, 1000); 
            }else{
                cameraShutter();    
            }


        }else{
            m_SurfaceView.queueEvent(new TakePicture(null));
        }
    }

    /**
     * 延时拍摄每秒回调
     * @param remainTime 剩余时间
     */
    private void onDelayShutterTimer(Integer remainTime) {
        ts_delay.setText(String.valueOf(remainTime));
        if ((remainTime == 0)){
            ts_delay.setVisibility(View.GONE);
            cameraShutter();
        }else{
            mSoundEngine.playEffect(this, R.raw.delay_timer_beep);
        }
    }

    /**
     * 相机拍照
     */
    private void cameraShutter(){
        mCamera.autoFocus(new Camera.AutoFocusCallback() {
            @Override
            public void onAutoFocus(boolean b, Camera camera) {
                if (b){
                    mSoundEngine.playEffect(MagicActivity.this, R.raw.auto_fouse);
                }
                mAutoFocusView.autoFocusCompleted();
                camera.takePicture(
                        new Camera.ShutterCallback() {

                            @Override
                            public void onShutter() {
                                mSoundEngine.playEffect(MagicActivity.this, R.raw.camera_shoot);
                            }
                        },
                        null,//raw
                        null,//postview
                        new Camera.PictureCallback() {
                            @Override
                            public void onPictureTaken(byte[] bytes, Camera camera) {
                                m_SurfaceView.queueEvent(new TakePicture(bytes));
                            }
                        }
                );
            }
        });
        beginAutoFocus();
    }

    /**
     * 切换引擎 哈哈镜引擎与特效引擎
     * @param type CoreJNILib.ENGINE_TYPE_EFFECT or CoreJNILib.ENGINE_TYPE_MESH
     */
    private void switchEngine(int type){
        //findViewById(R.id.btn_restore).setVisibility(type == CoreJNILib.ENGINE_TYPE_MESH ? View.VISIBLE : View.GONE);

        //findViewById(R.id.btn_set_cover).setVisibility(type == CoreJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        //findViewById(R.id.btn_set_effect).setVisibility(type == CoreJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        //findViewById(R.id.btn_set_frame).setVisibility(type == CoreJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        CoreJNILib.switchEngine(type);
        int resId = R.string.str_haha_mode;
        if (type == CoreJNILib.ENGINE_TYPE_EFFECT)
            resId = R.string.str_effect_mode;
        ((TextView)findViewById(R.id.tv_title)).setText(resId);
    }

    @Override
    protected Dialog onCreateDialog(int id) {
        switch (id) {
            case DIALOG_SELECT_ENGINE:
                Dialog dlg = new AlertDialog.Builder(this)
                        .setItems(R.array.engine_types, new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int which) {
                                if (which == 0) {
                                    switchEngine(CoreJNILib.ENGINE_TYPE_MESH);
                                } else {
                                    switchEngine(CoreJNILib.ENGINE_TYPE_EFFECT);
                                }
                            }
                        })
                        .create();
                dlg.setCanceledOnTouchOutside(true);
                dlg.getWindow().setWindowAnimations(R.style.UpDownAnimation);
                return dlg;

        }
        return null;
    }

    /**
     * 引擎初始化完成后回调
     */
    public void onEngineInitCompleted() {
        //switchEngine(CoreJNILib.ENGINE_TYPE_EFFECT);
        if (mPicPath != null) {
            m_SurfaceView.queueEvent(new SetImage(mPicPath));
        } else {
            startCamera(mCameraFacing);
        }
    }

    /**
     * 图片保存后回调
     * @param picPath 图片地址
     */
    public void onPictureSaved(String picPath){
        Toast.makeText(this, "图片已保存！\n"+picPath, Toast.LENGTH_LONG).show();
        mCamera.startPreview();
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

    /**
     * 打开静音
     */
    public void mute() {
        mSoundEngine.mute();
    }

    /**
     * 关闭静音
     */
    public void unMute() {
        mSoundEngine.unmute();
    }

    /**
     * 是否静音状态
     * @return
     */
    public boolean isMute() {
        return mSoundEngine.isMute();
    }

    /*
     * 是否支持GLES 2.0
     * @return
     */
    private boolean detectOpenGLES20() {
        ActivityManager am =
                (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x20000);
    }

    /**
     * 相机预览帧数据回调
     * @param bytes
     * @param camera
     */
    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        m_SurfaceView.addCameraBuffer(bytes);
        //    	mCamera.addCallbackBuffer(bytes);
    }

    /**
     * 当Surface使用完帧数据后的回调
     * @param buffer
     */
    @Override
    public void onCameraBufferRelease(byte[] buffer) {
        mCamera.addCallbackBuffer(buffer);
    }

    /**
     * 打开相机的线程Runnable
     */
    private class OpenCamera implements Runnable {
        private CameraFacing mFacing;
        public OpenCamera(CameraFacing facing) {
            mFacing = facing;
        }

        @Override
        public void run() {
            Camera camera = null;
            boolean bRightFacing = false;
            if (Build.VERSION.SDK_INT >= 9) {
                int numberOfCameras = Camera.getNumberOfCameras();
                if (numberOfCameras > 1){
                    int cameraId = 0;
                    int facing = Camera.CameraInfo.CAMERA_FACING_BACK;
                    if (mFacing == CameraFacing.FRONT) {
                        facing = Camera.CameraInfo.CAMERA_FACING_FRONT;
                    }
                    Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
                    for (int i = 0; i < numberOfCameras; i++) {
                        Camera.getCameraInfo(i, cameraInfo);
                        if (cameraInfo.facing == facing) {
                            cameraId = i;
                            bRightFacing = true;
                        }
                    }
                    try{
                        camera = Camera.open(cameraId);
                    }catch (Exception e){
                        camera = null;
                        bRightFacing = false;
                    }

                }
            }
            if (camera == null){
                bRightFacing = false;
                try{
                    camera = Camera.open();
                }catch (Exception e){
                    camera = null;
                }
            }
            if (bRightFacing){
                mCameraFacing = mFacing;
            }
            mEventHandler.sendMessage(Msg_Camera_Opened, camera);
        }
    }

    /**
     * 启动相机，异步模式
     * @param facing 前/后摄像机
     */
    public void startCamera(CameraFacing facing) {
        //TODO 显示等待画面
//        Toast.makeText(this, "正在打开相机...", Toast.LENGTH_LONG).show();
        Log.d(TAG, "正在打开相机...");
        stopCamera();
        //异步执行打开相机操作
        new Thread(new OpenCamera(facing)).run();
    }

    /**
     * 切换前后相机
     */
    public void switchCamera(){
        if(!canSwitchCamera() && mCamera != null){
            return;
        }
        CameraFacing facing = mCameraFacing == CameraFacing.BACK ? CameraFacing.FRONT : CameraFacing.BACK;
        startCamera(facing);
    }

    public static boolean canSwitchCamera(){
        return Build.VERSION.SDK_INT >= 9 && Camera.getNumberOfCameras() > 1;
    }

    public Camera getCamera(){
        return mCamera;
    }
    

    /**
     * 异步执行打开相机操作，相机打开后会执行此函数
     * @param camera 打开后的相机，打开失败则为null
     */
    public void onCameraOpened(Camera camera){

        if (camera == null){
            this.finish();
            Toast.makeText(this, R.string.msg_camera_open_failed, Toast.LENGTH_LONG);
            return;
        }
        mCamera = camera;
        configCamera();
        mCamera.startPreview();
    }

    private String mCameraFlashMode = null;

    /**
     * 获取当前相机的闪光灯状态
     * @return "on", "off", "auto", null（不支持闪光灯）
     */
    public String getCameraFlashMode(){
        return mCameraFlashMode;
    }

    /**
     * 设置下一个闪光灯状态，顺序为“auto”， “on”，“off”
     * @return 返回设置后的闪光灯状态
     */
    public String setNextFlashMode(){
        if(mCamera == null || mCameraFlashMode == null) return null;
        Camera.Parameters parameters = mCamera.getParameters();
        //List<String> flashModes = parameters.getSupportedFlashModes();
        if(mCameraFlashMode.equals(Camera.Parameters.FLASH_MODE_AUTO)){
            parameters.setFlashMode(Camera.Parameters.FLASH_MODE_ON);
        }else if(mCameraFlashMode.equals(Camera.Parameters.FLASH_MODE_ON)){
            parameters.setFlashMode(Camera.Parameters.FLASH_MODE_OFF);
        }else if(mCameraFlashMode.equals(Camera.Parameters.FLASH_MODE_OFF)){
            parameters.setFlashMode(Camera.Parameters.FLASH_MODE_AUTO);
        }
        mCameraFlashMode = parameters.getFlashMode();
        mCamera.setParameters(parameters);
        return mCameraFlashMode;
    }

    //延时拍摄时间
    private int mDelayTime = 0;
    /**
     * 获取延时拍摄时间，如果返回0则表示直接拍摄
     * @return 延时时间（0，3，5，10），单位为秒
     */
    public int getShootDelayTime(){
        return mDelayTime;   
    }

    /**
     * 设置延时时间，顺序为 0，3，5，10
     * @return 设置后的延时时间
     */
    public int setNextDelayTime(){
        if (mDelayTime == 0){
            mDelayTime = 3;
        }else if (mDelayTime == 3){
            mDelayTime = 5;
        }else if (mDelayTime == 5){
            mDelayTime = 10;
        }else if (mDelayTime == 10){
            mDelayTime = 0;
        }
        return mDelayTime;
    }
    
    /**
     * 配置当前摄像头
     */
    private void configCamera(){
        Camera.Parameters parameters = mCamera.getParameters();

        List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
        List<Camera.Size> pSizes = parameters.getSupportedPictureSizes();
        List<Integer> formats = parameters.getSupportedPreviewFormats();

        Camera.Size optJpgSize = getOptimalPreviewSize(pSizes, mOutputWidth, mOutputHeight);
        parameters.setPictureSize(optJpgSize.width, optJpgSize.height);
        Camera.Size optSize = getOptimalPreviewSize(sizes, mPreviewWidth, mPreviewHeight);
        parameters.setPreviewSize(optSize.width, optSize.height);
        if (formats.contains(CoreJNILib.IMAGE_FORMAT_RGB565)) {
            parameters.setPreviewFormat(CoreJNILib.IMAGE_FORMAT_RGB565);
        }else if (formats.contains(CoreJNILib.IMAGE_FORMAT_NV21)){
            parameters.setPreviewFormat(CoreJNILib.IMAGE_FORMAT_NV21);
        }

        List<String> FocusModes = parameters.getSupportedFocusModes();
        if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO))
            parameters.setFlashMode(Camera.Parameters.FOCUS_MODE_AUTO);
        mCamera.setParameters(parameters);
        
        List<String> flashModes = parameters.getSupportedFlashModes();
        if (flashModes == null || flashModes.size() <= 1){
            mCameraFlashMode = null;
        }else{
            mCameraFlashMode = parameters.getFlashMode();
        }

        parameters = mCamera.getParameters();
        Camera.Size previewSize = parameters.getPreviewSize();
        mPreviewWidth = previewSize.width;
        mPreviewHeight = previewSize.height;
        int previewFormat = parameters.getPreviewFormat();
        int szBuffer = previewSize.width * previewSize.height * ImageFormat.getBitsPerPixel(previewFormat) / 8;
        //2.2以上版本才能使用addCallbackBuffer，这个效率比不用callbackbuffer高30%
        if (Build.VERSION.SDK_INT >= 8) {
            mCamera.setPreviewCallbackWithBuffer(this);
            for (int i = 0; i < BufferCount; i++) {
                mCamera.addCallbackBuffer(new byte[szBuffer]);
            }
        } else {
            mCamera.setPreviewCallback(this);
        }
        m_SurfaceView.queueEvent(new SetPreviewInfo(mPreviewWidth, mPreviewHeight, parameters.getPreviewFormat(), mCameraFacing));

    }

    /**
     * 关闭相机
     */
    public void stopCamera() {
        if (mCamera == null) return;
        mCamera.stopPreview();
        if (Build.VERSION.SDK_INT >= 8) {
            mCamera.setPreviewCallbackWithBuffer(null);
        } else {
            mCamera.setPreviewCallback(null);
        }

        mCamera.release();
        mCamera = null;
    }

    /**
     * 获取最接近的大小
     * @param sizes size列表
     * @param w 宽
     * @param h 高
     * @return
     */
    public static Camera.Size getOptimalPreviewSize(List<Camera.Size> sizes, int w, int h) {
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

    private EventHandler mEventHandler = new EventHandler();
    private static final int Msg_Engine_Inited = 1;
    private static final int Msg_Camera_Opened = 2;
    private static final int Msg_On_Picture_Saved = 3;
    private static final int Msg_On_Delay_Shutter_Timer = 4;

    /**
     * 多线程环境主线程同步Handler
     */
    private class EventHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case Msg_Engine_Inited:
                    onEngineInitCompleted();
                    break;
                case Msg_Camera_Opened:
                    onCameraOpened((Camera)msg.obj);
                    break;
                case Msg_On_Picture_Saved:
                    onPictureSaved((String)msg.obj);
                    break;
                case Msg_On_Delay_Shutter_Timer:
                    onDelayShutterTimer((Integer) msg.obj);
                    break;
                default:
                    break;
            }
        }
        public void sendMessage(int what, Object obj) {
            sendMessage(obtainMessage(what, obj));
        }

        public void sendMessage(int what, Object obj, int arg1) {
            sendMessage(obtainMessage(what, arg1, 0, obj));
        }

        public void sendMessage(int what, int arg1, int arg2) {
            sendMessage(obtainMessage(what, arg1, arg2));
        }
    }


}