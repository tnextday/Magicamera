package com.funny.magicamera;

public class MagicJNILib {
    static {
        System.loadLibrary("magicjni");
    }

    public static final int IMAGE_FORMAT_JPEG       = 0x00000100;
	public static final int IMAGE_FORMAT_RGB565     = 0x00000004;
	public static final int IMAGE_FORMAT_NV21       = 0x00000011;
//    public static final int IMAGE_FORMAT_RGBA8888   = 4;

   /**
    * @param width the current view width
    * @param height the current view height
    */
    public static native void init(int width, int height);
    public static native void step();
    public static native void setPreviewDataInfo(int width, int height, int format);
    public static native void uploadPreviewData(byte[] buffer);
    public static native boolean onTouchDown(float x, float y);
    public static native boolean onTouchDrag(float x, float y);
    public static native boolean onTouchUp(float x, float y);

}
