package com.funny.magicamera;

import android.app.Activity;
import android.os.Bundle;

public class MagicActivity extends Activity {
    MagicEngineView mView;


    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mView = new MagicEngineView(getApplication());
        setContentView(mView);
    }


    @Override
    protected void onPause() {
        super.onPause();
        mView.onPause();
    }

    @Override
    protected void onResume() {
        super.onResume();
        mView.onResume();
    }
}