package com.funny.magicamera.popupviews;

import android.hardware.Camera;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
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
        View v =  mView.findViewById(R.id.btn_switch_camera);
        v.setOnClickListener(this);
        v.setEnabled(mContext.canSwitchCamera());
        mView.findViewById(R.id.btn_delay).setOnClickListener(this);
        mView.findViewById(R.id.btn_flash_mode).setOnClickListener(this);
        mView.findViewById(R.id.btn_mute).setOnClickListener(this);
    }
   

    @Override
    public void showAtLocation(View parent, int gravity, int x, int y) {
        updateFlashModeIcon();
        updateDelayTimeIcon();
        updateMuteIcon();
        super.showAtLocation(parent, gravity, x, y);
    }
    
    private void updateFlashModeIcon(){
        String flashMode = mContext.getCameraFlashMode();
        Button btn_flash_mode = (Button)mView.findViewById(R.id.btn_flash_mode);
        btn_flash_mode.setEnabled(flashMode!=null);
        if (flashMode == null || Camera.Parameters.FLASH_MODE_OFF.equals(flashMode)){
            btn_flash_mode.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_flash_off,0,0);
        } else if (Camera.Parameters.FLASH_MODE_AUTO.equals(flashMode)){
            btn_flash_mode.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_flash_auto,0,0);
        }else if (Camera.Parameters.FLASH_MODE_ON.equals(flashMode)){
            btn_flash_mode.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_flash_on,0,0);
        }
    }

    private void updateDelayTimeIcon(){
        Button btn_delay = (Button)mView.findViewById(R.id.btn_delay);
        switch (mContext.getShootDelayTime()){
            case 3:
                btn_delay.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_timer_3,0,0);
                break;
            case 5:
                btn_delay.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_timer_5,0,0);
                break;
            case 10:
                btn_delay.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_timer_10,0,0);
                break;
            default:
                btn_delay.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_timer_off,0,0);
        }
    }
    
    private void updateMuteIcon(){
        Button btn_mute = (Button)mView.findViewById(R.id.btn_mute);
        if (mContext.isMute()){
            btn_mute.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_gps_off, 0, 0);
        }else{
            btn_mute.setCompoundDrawablesWithIntrinsicBounds(0, R.drawable.ic_gps_on, 0, 0);
        }
    }


    @Override
    public void onClick(View view) {
        if(view.getId() == R.id.btn_switch_camera){
            mContext.switchCamera();
        }else if(view.getId() == R.id.btn_delay){
            mContext.setNextDelayTime();
            updateDelayTimeIcon();
        }else if(view.getId() == R.id.btn_flash_mode){
            mContext.setNextFlashMode();
            updateFlashModeIcon();
        }else if (view.getId() == R.id.btn_mute){
            if (mContext.isMute())
                mContext.unMute();
            else
                mContext.mute();
            updateMuteIcon();
        }
            
    }
}
