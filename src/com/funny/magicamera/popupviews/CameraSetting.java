package com.funny.magicamera.popupviews;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import com.funny.magicamera.R;

import static android.view.View.inflate;

/**
 * User: tNextDay
 * Description:
 */
public class CameraSetting extends PopupView {
    private Context mContext;
    private View mView;
    public CameraSetting(Context context) {
        super(context);
        mContext = context;
        mView = inflate(mContext, R.layout.camera_setting, null);
        setContentView(mView);
        setWidth(ViewGroup.LayoutParams.MATCH_PARENT);
        setAnimations(R.style.BottomUpAnimation);
    }


}
