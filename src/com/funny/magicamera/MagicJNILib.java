package com.funny.magicamera;

import android.graphics.Bitmap;
import android.util.Log;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;

public class MagicJNILib {
    static {
        System.loadLibrary("magic");
    }

    public static final int IMAGE_FORMAT_PACKET = 0x00000100;
    public static final int IMAGE_FORMAT_RGB565 = 0x00000004;
    public static final int IMAGE_FORMAT_NV21 = 0x00000011;
//    public static final int IMAGE_FORMAT_RGBA8888   = 4;

    /**
     * @param width  the current view width
     * @param height the current view height
     */
    public static native void init(int width, int height);

    public static native void step(float delta);

    public static native void setPreviewDataInfo(int width, int height, int format);

    public static native void uploadPreviewData(byte[] buffer, long len);

    public static native boolean onTouchDown(float x, float y);

    public static native boolean onTouchDrag(float x, float y);

    public static native boolean onTouchUp(float x, float y);

    public static native void setResPath(byte[] path);

    public static void playSound(int soundId) {
        Log.i("lib", String.format("playSound callback: %d", soundId));
    }

    public static void playMusic(int MusicId) {
        Log.i("lib", String.format("playMusic callback: %d", MusicId));
    }

    public static boolean saveImage(byte[] buffer, int w, int h, int format) {
        Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(buffer));
        try {
            FileOutputStream fos = new FileOutputStream("/sdcard/test.jpg");
            bitmap.compress(Bitmap.CompressFormat.JPEG, 90, fos);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        }
        return true;
    }

}
