package com.funny.magicamera;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Build;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGL10;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.egl.EGLContext;
import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.opengles.GL10;
import java.io.*;
import java.util.LinkedList;
import java.util.List;


/**
 * User: tNextDay
 * Description:
 */
public class MagicEngineView extends GLSurfaceView
        implements GLSurfaceView.Renderer, Camera.PreviewCallback, InputEvent.InputProcessor{
    private static String TAG = "MagicEngineView";
    public static int SDK_Version = Build.VERSION.SDK_INT;
	private long lastFrameTime = System.nanoTime();
	private float deltaTime = 0f;
    boolean m_bUseCamera = true ;
//    int m_CameraId; //use above 2.3
    Camera m_Camera = null;
    final static int BufferCount = 2;
    //2.2及以上版本才用
    LinkedList<byte[]> m_buffers = null;
    int     m_previewHeight = 480;
    int     m_previewWidth = 640;

    private InputEvent inputEvent = new InputEvent();


    public MagicEngineView(Context context) {
        super(context);
        //this.setEGLConfigChooser(8, 8, 8, 0, 16, 0);
        if(SDK_Version >= 8 ){
            this.setEGLContextClientVersion(2);
            m_buffers = new LinkedList<byte[]>();
        }else{
            setEGLContextFactory(new ContextFactory20());
        }

        setRenderer(this);
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


    public void checkFrameBuffer(){
        byte[] bytes;
        synchronized (m_buffers){
            bytes = m_buffers.poll();
        }
        if (bytes != null){
            MagicJNILib.uploadPreviewData(bytes, bytes.length);
            if (SDK_Version >= 8){
                m_Camera.addCallbackBuffer(bytes);
            }
        }

    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
        MagicJNILib.init(width, height);
        MagicJNILib.setSaveImagePath("/sdcard".getBytes());
        if (m_bUseCamera){
            startCamera();
        }else{
            setLocalTexture("/sdcard/test/tex.jpg");
        }
        this.lastFrameTime = System.nanoTime();
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
            inputStream = getResources().getAssets().open("test.jpg");
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
        return super.onTouchEvent(e);
    }

    public void startCamera(){
        //TODO 设置使用前置摄像头
        new OpenCameraTask().execute(0);
    }

    private class OpenCameraTask extends AsyncTask<Integer, Void, Void>{


        @Override
        protected void onPreExecute() {
            //显示等待画面
        }

        protected Void doInBackground(Integer... ints) {
            if(SDK_Version >= 9){
                int cameraId = ints[0];
                m_Camera.open(cameraId);
            }else{
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
            if (formats.contains(MagicJNILib.IMAGE_FORMAT_RGB565)){
                parameters.setPreviewFormat(MagicJNILib.IMAGE_FORMAT_RGB565);
            }
            m_Camera.setParameters(parameters);

            parameters = m_Camera.getParameters();
            Camera.Size  previewSize = parameters.getPreviewSize();
            m_previewWidth = previewSize.width;
            m_previewHeight = previewSize.height;
            int previewFormat = parameters.getPreviewFormat();
            int szBuffer = previewSize.width*previewSize.height*ImageFormat.getBitsPerPixel(previewFormat)/8;
            //2.2以上版本才能使用addCallbackBuffer，这个效率比不是用callbackbuffer高30%
            if(SDK_Version >= 8){
                m_Camera.setPreviewCallbackWithBuffer(MagicEngineView.this);
                for (int i = 0; i < BufferCount; i ++){
                    m_Camera.addCallbackBuffer(new byte[szBuffer]);
                }
            }else{
                m_Camera.setPreviewCallback(MagicEngineView.this);
            }

            MagicJNILib.setPreviewDataInfo(m_previewWidth, m_previewHeight, parameters.getPreviewFormat());

            m_Camera.startPreview();
            return null;
        }

        protected void onPostExecute(long result) {
            //运行结束
        }
    }

    public void stopCamera(){
        if (m_Camera == null) return;
        m_Camera.stopPreview();
        if (SDK_Version >= 8){
            m_Camera.setPreviewCallbackWithBuffer(null);
        }else{
            m_Camera.setPreviewCallback(null);
        }

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

    int frameCount = 0;
    
    public void Save2File(byte[] bytes){
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
        synchronized (m_buffers){
            m_buffers.add(bytes);
        }
        //Log.d("FPSLogger", "onPreviewFrame");
//    	Save2File(bytes);
//    	m_Camera.addCallbackBuffer(bytes);
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
    		if(System.nanoTime()-startTime > 1000000000) {
    			Log.i("FPSLogger", "fps: " + frames);
    			frames = 0;
    			startTime = System.nanoTime();
    		}
    	}
    }

    private static class ContextFactory20 implements GLSurfaceView.EGLContextFactory {
        private static int EGL_CONTEXT_CLIENT_VERSION = 0x3098;
        public EGLContext createContext(EGL10 egl, EGLDisplay display, EGLConfig eglConfig) {
            Log.w(TAG, "creating OpenGL ES 2.0 context");
            checkEglError("Before eglCreateContext", egl);
            int[] attrib_list = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL10.EGL_NONE };
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
            Log.e(TAG, String.format("%s: EGL error: 0x%x", prompt, error));
        }
    }
}