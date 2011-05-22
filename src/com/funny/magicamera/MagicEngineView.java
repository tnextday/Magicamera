package com.funny.magicamera;

import android.content.Context;
import android.graphics.ImageFormat;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.opengl.GLSurfaceView;
import android.view.SurfaceHolder;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.util.List;

/**
 * User: tNextDay
 * Description:
 */
public class MagicEngineView extends GL20SurfaceView
        implements GLSurfaceView.Renderer, Camera.PreviewCallback{

    boolean m_bUseCamera = false;
//    int m_CameraId; //use above 2.3
    Camera m_Camera;
    public MagicEngineView(Context context) {
        super(context);
        //after api level 8 can do as that
//        this.setEGLContextClientVersion(2);
        setRenderer(this);
    }
    public void onDrawFrame(GL10 gl) {
//            GL2JNILib.step();
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

    public void onSurfaceChanged(GL10 gl, int width, int height) {
//            GL2JNILib.init(width, height);
        // Now that the size is known, set up the camera parameters and begin
        // the preview.
        Camera.Parameters parameters = m_Camera.getParameters();

        List<Camera.Size> sizes = parameters.getSupportedPreviewSizes();
//        List<Size> psizes = parameters.getSupportedPictureSizes();
//        List<Integer> formats = parameters.getSupportedPreviewFormats();
//        formats  = parameters.getSupportedPictureFormats();
        Camera.Size optimalSize = getOptimalPreviewSize(sizes, width, height);
        parameters.setPreviewSize(optimalSize.width, optimalSize.height);
//        parameters.setPreviewSize(320, 240);
        parameters.setPreviewFormat(ImageFormat.RGB_565);
        m_Camera.setParameters(parameters);
        m_Camera.startPreview();
    }

    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        m_Camera = Camera.open();
        m_Camera.setPreviewCallback(this);
    }

    @Override
    public void onPreviewFrame(byte[] bytes, Camera camera) {
        //TODO deal camera data
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
}