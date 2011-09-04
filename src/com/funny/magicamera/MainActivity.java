package com.funny.magicamera;

import android.app.Activity;
import android.content.Intent;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.database.Cursor;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore;
import android.util.Log;
import android.view.View;

import java.io.File;

/**
 * User: tNextDay
 * Description:
 */
public class MainActivity extends Activity implements View.OnClickListener{
    private static final int REQUEST_SELECT_PHOTO = 1;
    
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.main);
        findViewById(R.id.btn_pic).setOnClickListener(this);
        findViewById(R.id.btn_cam).setOnClickListener(this);
        MagicJNILib.APK_PATH = getAPKPath();
    }

    @Override
    public void onClick(View view) {

        if(view.getId() == R.id.btn_cam){
            Intent intent = new Intent(this, MagicActivity.class);
            startActivity(intent);
        }else if (view.getId() == R.id.btn_pic){
            selectPhoto();
        }
    }

    void selectPhoto(){
        Intent innerIntent = new Intent(Intent.ACTION_GET_CONTENT); //"android.intent.action.GET_CONTENT"
        innerIntent.setType("image/*"); //查看类型  详细的类型在 com.google.android.mms.ContentType 中
        Intent wrapperIntent = Intent.createChooser(innerIntent, null);
        startActivityForResult(wrapperIntent, REQUEST_SELECT_PHOTO);
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (data == null) return;
        if (requestCode == REQUEST_SELECT_PHOTO){
            String path = getPath(data.getData());
            if(path != null && new File(path).exists()){
                Intent intent = new Intent(this, MagicActivity.class);
                intent.putExtra("PicPath", path);
                startActivity(intent);
            }
        }
    }

    public String getPath(Uri uri) {
        String[] projection = { MediaStore.Images.Media.DATA };
        if (uri == null) return null;
        Cursor cursor = managedQuery(uri, projection, null, null, null);
        startManagingCursor(cursor);
        int column_index = cursor.getColumnIndexOrThrow(MediaStore.Images.Media.DATA);
        cursor.moveToFirst();
        return cursor.getString(column_index);
    }

    public String getAPKPath(){
        String packageName = getApplication().getPackageName();
        String apkFilePath = "";
        ApplicationInfo appInfo = null;
        PackageManager packMgmr = getApplication().getPackageManager();
        try {
            appInfo = packMgmr.getApplicationInfo(packageName, 0);
        } catch (PackageManager.NameNotFoundException e) {
            e.printStackTrace();
            throw new RuntimeException("Unable to locate assets, aborting...");
        }
        apkFilePath = appInfo.sourceDir;
        Log.i("apk path", apkFilePath);
        return  apkFilePath;
    }
}