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
}
