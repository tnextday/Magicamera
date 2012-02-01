package com.funny.magicamera.popupviews;

import android.view.View;
import android.view.ViewGroup;
import com.funny.magicamera.MagicActivity;
import com.funny.magicamera.R;

import static android.view.View.inflate;

/**
 * User: tNextDay
 * Description:
 */
public class CameraSetting extends PopupView implements View.OnClickListener {
    private MagicActivity mContext;
    private View mView;
    public CameraSetting(MagicActivity context) {
        super(context);
        mContext = context;
        mView = inflate(mContext, R.layout.camera_setting, null);
        
        setWidth(ViewGroup.LayoutParams.MATCH_PARENT);
        setContentView(mView);
        setAnimations(R.style.BottomUpAnimation);
        mView.findViewById(R.id.btn_switch_camera).setOnClickListener(this);
        
    }


    @Override
    public void onClick(View view) {
        if(view.getId() == R.id.btn_switch_camera){

        }
    }
}
