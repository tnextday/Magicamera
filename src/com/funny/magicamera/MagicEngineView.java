package com.funny.magicamera;

import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
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
    Camera m_Camera;
    byte[] m_frameBuffer;
    boolean m_frameChanged = false;
    final ReentrantLock m_lock = new ReentrantLock();
    int     m_previewHeight;
    int     m_previewWidth;

    public MagicEngineView(Context context) {
        super(context);
        //after api level 8 can do as that
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
            MagicJNILib.uploadPreviewData(m_frameBuffer);
            m_frameChanged = false;
        }
        m_lock.unlock();
    }

    public void onSurfaceChanged(GL10 gl, int width, int height) {
         MagicJNILib.init(width, height);
        // Now that the size is known, set up the camera parameters and begin
        // the preview.
        Camera.Parameters parameters = m_Camera.getParameters();

        List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
//        List<Size> psizes = parameters.getSupportedPictureSizes();
        List<Integer> formats = parameters.getSupportedPreviewFormats();
        formats  = parameters.getSupportedPictureFormats();
        Camera.Size optimalSize = getOptimalPreviewSize(sizes, width, height);
        m_previewHeight = optimalSize.height;
        m_previewWidth = optimalSize.width;
        parameters.setPreviewSize(optimalSize.width, optimalSize.height);
        
//        parameters.setPreviewSize(320, 240);
        parameters.setPreviewFormat(ImageFormat.RGB_565);
        m_Camera.setParameters(parameters);
        MagicJNILib.setPreviewInfo(optimalSize.width, optimalSize.height, GL10.GL_RGBA);
        m_frameBuffer = new byte[optimalSize.width*optimalSize.height*4];
        m_Camera.startPreview();
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        m_Camera = Camera.open();
        m_Camera.setPreviewCallback(this);
    }

    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        m_lock.lock();
        Camera.Parameters parameters = m_Camera.getParameters();
        int imageFormat = parameters.getPreviewFormat();
        int bits = ImageFormat.getBitsPerPixel(imageFormat);
        System.arraycopy(bytes, 0, m_frameBuffer, 0, bytes.length);
        decodeYUV420SP(m_frameBuffer, bytes, m_previewWidth, m_previewHeight);
        m_frameChanged = true;
        m_lock.unlock();
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        stopCamera();
        super.surfaceDestroyed(holder);
    }

    public void stopCamera(){
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

    static public void decodeYUV420SP(byte[] rgb, byte[] yuv420sp, int width,
			int height) {
		final int frameSize = width * height;

		for (int j = 0, yp = 0; j < height; j++) {
			int uvp = frameSize + (j >> 1) * width, u = 0, v = 0;
			for (int i = 0; i < width; i++, yp++) {
				int y = (0xff & ((int) yuv420sp[yp])) - 16;
				if (y < 0)
					y = 0;
				if ((i & 1) == 0) {
					v = (0xff & yuv420sp[uvp++]) - 128;
					u = (0xff & yuv420sp[uvp++]) - 128;
				}

				int y1192 = 1192 * y;
				int r = (y1192 + 1634 * v);
				int g = (y1192 - 833 * v - 400 * u);
				int b = (y1192 + 2066 * u);

				if (r < 0)
					r = 0;
				else if (r > 262143)
					r = 262143;
				if (g < 0)
					g = 0;
				else if (g > 262143)
					g = 262143;
				if (b < 0)
					b = 0;
				else if (b > 262143)
					b = 262143;

				rgb[yp*4] = (byte)0xff;
                rgb[yp*4 + 1] = (byte)(r >> 10);
                rgb[yp*4 + 2] = (byte)(g >> 10);
                rgb[yp*4 + 3] = (byte)(b >> 10);
			}
		}
	}
}