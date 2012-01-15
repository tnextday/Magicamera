package com.funny.magicamera.popupviews;

import android.content.Context;
import android.widget.ImageView;
import com.funny.magicamera.R;

import java.util.Timer;
import java.util.TimerTask;

/**
 * User: tNextDay
 * Description:
 */
public class AutoFocusView extends PopupView{
    private ImageView mImageView;
    private Context mContext;
    public AutoFocusView(Context context) {
        super(context);
        mImageView = new ImageView(context);
        mImageView.setScaleType(ImageView.ScaleType.CENTER);
        mImageView.setImageResource(R.drawable.img_autofocus);
        setContentView(mImageView);
        setAnimations(R.style.AutoFoucusAnimation);
    }



    public void beginAutoFocus(){
        mImageView.setImageResource(R.drawable.img_autofocus);
    }

    public void autoFocusCompleted(){
        mImageView.setImageResource(R.drawable.img_autofocus_ok);
        final Timer timer = new Timer();
        timer.schedule(new TimerTask() {
            @Override
            public void run() {
                timer.cancel();
                dismiss();
            }
        }, 200);
    }
}
