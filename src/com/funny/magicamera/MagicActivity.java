package com.funny.magicamera;

import android.app.ActivityGroup;
import android.app.ActivityManager;
import android.app.AlertDialog;
import android.app.Dialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.pm.ConfigurationInfo;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.animation.Animation;
import android.view.animation.AnimationUtils;
import android.widget.*;

import java.io.File;
import java.io.IOException;
import java.util.*;

import static com.funny.magicamera.MSurfaceView.*;

public class MagicActivity extends ActivityGroup implements Camera.PreviewCallback,
            View.OnClickListener,
            CameraBufferReleaseListener,
            AdapterView.OnItemSelectedListener {
    MSurfaceView m_SurfaceView;
    public static String TAG = "MagicEngine";

    //    int m_CameraId; //use above 2.3
    Camera mCamera = null;
    final static int BufferCount = 2;

    int mPreviewHeight = 360;
    int mPreviewWidth = 480;

    int mOutputHeight = 768;
    int mOutputWidth = 1024;

    public String mPicPath = null;
    private final static int DIALOG_SELECT_ENGINE = 0;

    private ArrayList<String> m_frames;
    private ArrayList<String> m_overlay;
    private ArrayList<String> m_filters;

    TabHost mTabHost;



    enum CameraType {
        FACING_BACK, FACING_FRONT
    }

    private CameraType mCameraType = CameraType.FACING_BACK;

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
        findViewById(R.id.btn_mode).setOnClickListener(this);

        Button btn_take = (Button)findViewById(R.id.btn_take);
        btn_take.setOnClickListener(this);
        btn_take.setLongClickable(true);
        //长按自动对焦
        btn_take.setOnLongClickListener(new View.OnLongClickListener() {
            @Override
            public boolean onLongClick(View view) {
                if (mCamera != null)
                    mCamera.autoFocus(null);
                return true;
            }
        });
        findViewById(R.id.btn_camera_cfg).setOnClickListener(this);
        findViewById(R.id.btn_effect).setOnClickListener(this);


        m_SurfaceView = (MSurfaceView) findViewById(R.id.surfaceview);
        Log.w(MagicActivity.TAG, "MagicActivity onCreate");

        Intent intent = getIntent();
        String picPath = intent.getStringExtra("mPicPath");
        String sl[];
        try {
            AssetManager am = getResources().getAssets();
            sl = am.list("covers");
            m_overlay = new ArrayList<String>(sl.length+1);
            m_overlay.add("none");
            m_overlay.addAll(Arrays.asList(sl));
            sl = am.list("frames");
            m_frames = new ArrayList<String>(sl.length+1);
            m_frames.add("none");
            m_frames.addAll(Arrays.asList(sl));
        } catch (IOException e) {
            e.printStackTrace();
        }
        String effectList = MagicJNILib.getEffectList();
        sl = effectList.split(",");
        m_filters = new ArrayList<String>(sl.length+1);
        m_filters.add("none");
        m_filters.addAll(Arrays.asList(sl));

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
        MagicJNILib.onTake = new MagicJNILib.TakePictureListener(){
            @Override
            public void onTakePicture(String picPath) {
                mEventHandler.sendMessage(Msg_On_Take_Picture, picPath);
                //Toast.makeText(MagicActivity.this, "图片已保存！", Toast.LENGTH_SHORT);
            }
        };
        setupTabHost();
    }

    final static int tabTitleId[] = {
        R.string.str_effect_filter,
        R.string.str_effect_overlay,
        R.string.str_effect_frame
    };
    final static int tabContentId[] = {
        R.id.gallery_filter,
        R.id.gallery_overlay,
        R.id.gallery_frame,
    };

    private final String[] data_from ={
        "preview", "title",
    };
    private final int[] set_to = {
        R.id.im_preview, R.id.tv_title
    };
    public void setupTabHost(){
        mTabHost = (TabHost) findViewById(R.id.tabhost);
        mTabHost.setup(getLocalActivityManager());
        LinearLayout ll = (LinearLayout)mTabHost.getChildAt(0);
        TabWidget tw = (TabWidget)ll.getChildAt(0);
        LinearLayout tabIndicator;
        TextView tvTab;
        View v = LayoutInflater.from(this).inflate(R.layout.effect_tab_content,
                        mTabHost.getTabContentView(),
                        true);
        ArrayList<String> EffectType = null;
        String EffectName[] = {"filter_", "overlay_", "frame_"};
        Resources res = getResources();
        int resId;
        String packageName = getPackageName();
        for(int i = 0; i < tabTitleId.length; i++){
            tabIndicator = (LinearLayout) LayoutInflater.from(this).inflate(R.layout.effect_tab_indicator, tw, false);
            tvTab = (TextView)tabIndicator.findViewById(R.id.title );
            tvTab.setText(getString(tabTitleId[i]));
            mTabHost.addTab(mTabHost.newTabSpec(String.format("tab_%d", i))
                    .setIndicator(tabIndicator)
                    .setContent(tabContentId[i]));

            //设置各个Gallery项

            Gallery g = (Gallery)v.findViewById(tabContentId[i]);
            g.setCallbackDuringFling(false);
            g.setOnItemSelectedListener(this);
            ArrayList<Map<String, ?>> effects = new ArrayList<Map<String, ?>>();
            Map<String, Object> item;
            switch (i){
                case 0:
                    EffectType = m_filters;
                    break;
                case 1:
                    EffectType = m_overlay;
                    break;
                case 2:
                    EffectType = m_frames;
                    break;
            }
            for (String str : EffectType){
                item = new HashMap<String, Object>();
                str = str.replace(".pk", "").toLowerCase();
                //设置标题
                resId = res.getIdentifier(EffectName[i]+str, "string", packageName);
                if (resId != 0){
                    item.put("title", res.getString(resId));
                }else{
                    item.put("title", str);   
                }
                resId = res.getIdentifier(EffectName[i]+str, "drawable", packageName);
                if (resId == 0)
                    resId = R.drawable.noeffect;
                item.put("preview", resId);
                effects.add(item);
            }
            g.setAdapter(new SimpleAdapter(this, effects,
                    R.layout.gallery_item, data_from, set_to));
        } 
    }

    //gallery 按钮回调
    @Override
    public void onItemSelected(AdapterView<?> adapterView, View view, int pos, long id) {
        if (adapterView.getId() == R.id.gallery_filter){
            m_SurfaceView.queueEvent(new SetEffect(m_filters.get(pos)));
        }else if (adapterView.getId() == R.id.gallery_overlay){
            m_SurfaceView.queueEvent(new SetOverlay("res://covers/"+m_overlay.get(pos)));
        }else if (adapterView.getId() == R.id.gallery_frame){
            m_SurfaceView.queueEvent(new SetFrame("res://frames/"+m_frames.get(pos)));
        }
    }

    @Override
    public void onNothingSelected(AdapterView<?> adapterView) {

    }
    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_back) {
            finish();
        } else if (view.getId() == R.id.btn_mode) {
            showDialog(DIALOG_SELECT_ENGINE);
        } else if (view.getId() == R.id.btn_restore) {
            MagicJNILib.restoreMesh();
        } else if (view.getId() == R.id.btn_take) {
            takePicture();

        } else if (view.getId() == R.id.btn_camera_cfg){

        } else if (view.getId() == R.id.btn_effect){
            ToggleButton tbtn = (ToggleButton)view;
            Animation anim;
            if (tbtn.isChecked()){
                anim = AnimationUtils.loadAnimation(this, R.anim.push_up_in);
            }else{
                anim = AnimationUtils.loadAnimation(this, R.anim.push_down_out);
            }
            anim.setAnimationListener(new Animation.AnimationListener() {
                @Override
                public void onAnimationStart(Animation animation) {
                    mTabHost.setVisibility(View.VISIBLE);
                }

                @Override
                public void onAnimationEnd(Animation animation) {
                    ToggleButton tbtn = (ToggleButton)findViewById(R.id.btn_effect);
                    mTabHost.setVisibility(tbtn.isChecked()?View.VISIBLE:View.GONE);
                }

                @Override
                public void onAnimationRepeat(Animation animation) {

                }
            });
            mTabHost.startAnimation(anim);
        }
    }

    /***
     * 拍照
     */
    void takePicture() {
        if (mPicPath == null){
            mCamera.autoFocus(new Camera.AutoFocusCallback() {

                @Override
                public void onAutoFocus(boolean b, Camera camera) {
                    camera.takePicture(
                            new Camera.ShutterCallback() {

                                @Override
                                public void onShutter() {

                                }
                            },
                            null,
                            new Camera.PictureCallback() {
                                @Override
                                public void onPictureTaken(byte[] bytes, Camera camera) {
                                    m_SurfaceView.queueEvent(new TakePicture(bytes));
                                    camera.startPreview();

                                }
                            }
                    );
                }
            });
        }else{
            m_SurfaceView.queueEvent(new TakePicture(null));
        }


    }


    private void switchEngine(int type){
        //findViewById(R.id.btn_restore).setVisibility(type == MagicJNILib.ENGINE_TYPE_MESH ? View.VISIBLE : View.GONE);

        //findViewById(R.id.btn_set_cover).setVisibility(type == MagicJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        //findViewById(R.id.btn_set_effect).setVisibility(type == MagicJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        //findViewById(R.id.btn_set_frame).setVisibility(type == MagicJNILib.ENGINE_TYPE_EFFECT ? View.VISIBLE : View.GONE);
        MagicJNILib.switchEngine(type);
        int resId = R.string.str_haha_mode;
        if (type == MagicJNILib.ENGINE_TYPE_EFFECT)
            resId = R.string.str_effect_mode;
        ((TextView)findViewById(R.id.tv_title)).setText(resId);
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

        }
        return null;
    }

    /**
     * 引擎初始化完成后回调
     */
    public void onEngineInitCompleted() {
        //switchEngine(MagicJNILib.ENGINE_TYPE_EFFECT);
        if (mPicPath != null) {
            m_SurfaceView.queueEvent(new SetImage(mPicPath));
        } else {
            startCamera(mCameraType);
        }
    }

    /***
     * 图片保存后回调
     * @param picPath 图片地址
     */
    public void onPictureSaved(String picPath){
        Toast.makeText(this, "图片已保存！\n"+picPath, Toast.LENGTH_LONG).show();
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

    /***
     * 是否支持GLES 2.0
     * @return
     */
    private boolean detectOpenGLES20() {
        ActivityManager am =
                (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo info = am.getDeviceConfigurationInfo();
        return (info.reqGlEsVersion >= 0x20000);
    }

    /***
     * 相机预览帧数据回调
     * @param bytes
     * @param camera
     */
    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        m_SurfaceView.addCameraBuffer(bytes);
        //    	mCamera.addCallbackBuffer(bytes);
    }

    /***
     * 当Surface使用完帧数据后的回调
     * @param buffer
     */
    @Override
    public void onCameraBufferRelease(byte[] buffer) {
        mCamera.addCallbackBuffer(buffer);
    }

    /***
     * 打开相机的线程Runnable
     */
    private class OpenCamera implements Runnable {
        private CameraType mCameraType;

        public OpenCamera(CameraType type) {
            mCameraType = type;
        }

        @Override
        public void run() {
            Camera camera = null;
            if (Build.VERSION.SDK_INT >= 9) {
                int numberOfCameras = Camera.getNumberOfCameras();
                if (numberOfCameras > 1){
                    int cameraId = 0;
                    int facing = Camera.CameraInfo.CAMERA_FACING_BACK;
                    if (mCameraType == CameraType.FACING_FRONT) {
                        facing = Camera.CameraInfo.CAMERA_FACING_FRONT;
                    }
                    Camera.CameraInfo cameraInfo = new Camera.CameraInfo();
                    for (int i = 0; i < numberOfCameras; i++) {
                        Camera.getCameraInfo(i, cameraInfo);
                        if (cameraInfo.facing == facing) {
                            cameraId = i;
                        }
                    }
                    try{
                        camera = Camera.open(cameraId);
                    }catch (Exception e){
                        camera = null;
                    }

                }
            }
            if (camera == null){
                try{
                    camera = Camera.open();
                }catch (Exception e){
                    camera = null;
                }
            }
            mEventHandler.sendMessage(Msg_Camera_Opened, camera);
        }
    }

    /***
     * 启动相机，异步模式
     * @param cameraType 前/后摄像机
     */
    public void startCamera(CameraType cameraType) {
        //TODO 显示等待画面
        stopCamera();
        //异步执行打开相机操作
        new Thread(new OpenCamera(cameraType)).run();
    }

    /***
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
        //MagicJNILib.rotate90(true);
        mCamera.startPreview();
    }

    /***
     * 设置相机参数
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
        if (formats.contains(MagicJNILib.IMAGE_FORMAT_RGB565)) {
            parameters.setPreviewFormat(MagicJNILib.IMAGE_FORMAT_RGB565);
        }

        List<String> FocusModes = parameters.getSupportedFocusModes();
        if (FocusModes.contains(Camera.Parameters.FOCUS_MODE_AUTO))
            parameters.setFlashMode(Camera.Parameters.FOCUS_MODE_AUTO);
        mCamera.setParameters(parameters);

        parameters = mCamera.getParameters();
        Camera.Size previewSize = parameters.getPreviewSize();
        mPreviewWidth = previewSize.width;
        mPreviewHeight = previewSize.height;
        int previewFormat = parameters.getPreviewFormat();
        int szBuffer = previewSize.width * previewSize.height * ImageFormat.getBitsPerPixel(previewFormat) / 8;
        //2.2以上版本才能使用addCallbackBuffer，这个效率比不是用callbackbuffer高30%
        if (Build.VERSION.SDK_INT >= 8) {
            mCamera.setPreviewCallbackWithBuffer(this);
            for (int i = 0; i < BufferCount; i++) {
                mCamera.addCallbackBuffer(new byte[szBuffer]);
            }
        } else {
            mCamera.setPreviewCallback(this);
        }
        m_SurfaceView.queueEvent(new SetPreviewInfo(mPreviewWidth, mPreviewHeight, parameters.getPreviewFormat()));

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

    /***
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
    private static final int Msg_On_Take_Picture = 3;

    /***
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
                case Msg_On_Take_Picture:
                    onPictureSaved((String)msg.obj);
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