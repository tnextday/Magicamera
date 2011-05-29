package com.funny.magicamera;

public class MagicJNILib {
    static {
        System.loadLibrary("magicjni");
    }

   /**
    * @param width the current view width
    * @param height the current view height
    */
    public static native void init(int width, int height);
    public static native void step();
    public static native void setPreviewInfo(int width, int height, int format);
    public static native void uploadPreviewData(byte[] buffer);
    public static native boolean onTouchDown(float x, float y);
    public static native boolean onTouchDrag(float x, float y);
    public static native boolean onTouchUp(float x, float y);

}
