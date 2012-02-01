package com.funny.magicamera.popupviews;

import android.content.Context;
import android.graphics.drawable.ColorDrawable;
import android.view.KeyEvent;
import android.view.MotionEvent;
import android.view.View;
import android.view.ViewGroup;
import android.widget.PopupWindow;

/**
 * User: tNextDay
 * Description:
 */
public class PopupView extends PopupWindow implements View.OnTouchListener, View.OnKeyListener {
    private boolean mCanceledOnTouchOutside = false;


    public PopupView(View contentView) {
        super();
        init();
        setContentView(contentView);

    }

    public PopupView(){
        super();
        init();
    }

    public PopupView(Context context){
        super(context);
        init();
    }

    protected void init(){
        setFocusable(true);
        setTouchable(true);
        setOutsideTouchable(true);
        setTouchInterceptor(this);
        setBackgroundDrawable(new ColorDrawable(0));
        setWidth(ViewGroup.LayoutParams.WRAP_CONTENT);
        setHeight(ViewGroup.LayoutParams.WRAP_CONTENT);
    }

    @Override
    public void setContentView(View contentView) {

        contentView.measure(getWidth(), getHeight());
        super.setContentView(contentView);
        contentView.setOnKeyListener(this);
    }

    //设置触摸对话框意外的地方取消对话框
    private void setCanceledOnTouchOutside(boolean b) {
        mCanceledOnTouchOutside = b;
    }



    //设置窗口弹出动画
    public void setAnimations(int anim){
        setAnimationStyle(anim);
    }

    
    public int getMeasuredHeight(){
        return getContentView().getMeasuredHeight();
    }
    
    public int getMeasuredWidth(){
        return getContentView().getMeasuredWidth();
    }


    @Override
    public boolean onTouch(View view, MotionEvent event) {
        if (mCanceledOnTouchOutside 
                && event.getAction() == MotionEvent.ACTION_DOWN) {
            dismiss();
            return true;
        }
        return false;
    }

    //返回键取消
    @Override
    public boolean onKey(View view, int i, KeyEvent keyEvent) {
        if(keyEvent.getKeyCode() == KeyEvent.KEYCODE_BACK
                && keyEvent.getAction() == KeyEvent.ACTION_DOWN){
            dismiss();
            return true;
        }
        return false;
    }
}
