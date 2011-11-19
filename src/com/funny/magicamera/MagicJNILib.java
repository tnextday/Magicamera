package com.funny.magicamera;

import android.graphics.Bitmap;
import android.os.Environment;
import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.nio.ByteBuffer;
import java.text.SimpleDateFormat;
import java.util.Date;

public class MagicJNILib {
    static {
        System.loadLibrary("magic");
    }

    public static final int IMAGE_FORMAT_PACKET = 0x00000100;
    public static final int IMAGE_FORMAT_RGB565 = 0x00000004;
    public static final int IMAGE_FORMAT_NV21 = 0x00000011;
//    public static final int IMAGE_FORMAT_RGBA8888   = 4;

    public static final int ENGINE_TYPE_NONE = 0;
    public static final int ENGINE_TYPE_MESH = 1;
    public static final int ENGINE_TYPE_EFFECT = 2;
    //public static final int ENGINE_TYPE_Kaleidoscope = 3;


    public static native void create();
    public static native void destory();
    public static native void resize(int width, int height);
    public static native void step(float delta);
    public static native void setInputDataInfo(int width, int height, int format);
    public static native void updateInputData(byte[] buffer);
    public static native boolean onTouchDown(float x, float y);
    public static native boolean onTouchDrag(float x, float y);
    public static native boolean onTouchUp(float x, float y);
    public static native void setInputImage(String path);
    public static native void rotate90(boolean clockwise);
    public static native void setApkPath(String apkPath);
    public static native void takePicture();
    public static native void takePictureWithFile(String path);
    public static native void takePictureWithBuffer(byte[] buffer);
    public static native void setCover(String path);
    public static native void setFrame(String path);
    public static native void setEffect(String name);
    public static native String getEffectList();
    public static native void restoreMesh();
    public static native void switchEngine(int type);
    public static native int getEngineType();


    public static void playSound(int soundId) {
        Log.i("libmagic", String.format("playSound callback: %d", soundId));
    }

    public static void playMusic(int MusicId) {
        Log.i("libmagic", String.format("playMusic callback: %d", MusicId));
    }

    public static TakePictureListener onTake = null;

    public static boolean saveImage(byte[] buffer, int w, int h, int format) {
        String status = Environment.getExternalStorageState();
        if (!status.equals(Environment.MEDIA_MOUNTED)) {
            return false;
        }
        String SDPATH = Environment.getExternalStorageDirectory().toString();
        File destDir = new File(SDPATH + "/HaHa");
        if (!destDir.exists()) {
            destDir.mkdirs();
        }
        Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.ARGB_8888);
        bitmap.copyPixelsFromBuffer(ByteBuffer.wrap(buffer));
        String savePath = getAvailableFile(destDir.toString(), "haha_", ".jpg");
        try {
            FileOutputStream fos = new FileOutputStream(savePath);
            bitmap.compress(Bitmap.CompressFormat.JPEG, 80, fos);
        } catch (FileNotFoundException e) {
            e.printStackTrace();
            return false;
        }
        if (onTake != null){
            onTake.onTakePicture(savePath);
        }
        return true;
    }

    public static String getAvailableFile(String path, String prefix, String ext){
        SimpleDateFormat sdf = new SimpleDateFormat("yyMMdd");
        String ds = sdf.format(new Date());
        String s = path+"/"+prefix+ds;
        int count = 0;
        File f = new File(s+ext);
        while (f.exists()){
            f = new File(s+"_"+String.valueOf(count)+ext);
            count++;
            if (count > 999){
                return "";
            }
        }
        return f.toString();
    }

    public interface TakePictureListener{
        void onTakePicture(String picPath);
    }
}
