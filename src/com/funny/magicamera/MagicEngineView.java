package com.funny.magicamera;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.opengl.GLSurfaceView;
import android.util.Log;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
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

	private long lastFrameTime = System.nanoTime();
	private float deltaTime = 0;
    boolean m_bUseCamera = false;
//    int m_CameraId; //use above 2.3
    Camera m_Camera = null;
    final static int BufferCount = 2;
    LinkedList<byte[]> m_buffers = new LinkedList<byte[]>();
    int     m_previewHeight = 480;
    int     m_previewWidth = 640;

    private InputEvent inputEvent = new InputEvent();


    public MagicEngineView(Context context) {
        super(context);
        this.setEGLContextClientVersion(2);
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
            m_Camera.addCallbackBuffer(bytes);
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
        buffer = null;
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
        m_Camera = Camera.open();
        
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
        m_Camera.setPreviewCallbackWithBuffer(this);
        for (int i = 0; i < BufferCount; i ++){
        	m_Camera.addCallbackBuffer(new byte[szBuffer]);
        }
        MagicJNILib.setPreviewDataInfo(m_previewWidth, m_previewHeight, parameters.getPreviewFormat());
        
        m_Camera.startPreview();
    }

    public void stopCamera(){
        if (m_Camera == null) return;
        m_Camera.stopPreview();
        m_Camera.setPreviewCallbackWithBuffer(null);
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


}