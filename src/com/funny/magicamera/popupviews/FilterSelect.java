package com.funny.magicamera.popupviews;

import android.content.Context;
import android.content.res.AssetManager;
import android.content.res.Resources;
import android.view.View;
import android.view.ViewGroup;
import android.widget.*;
import com.funny.magicamera.MagicJNILib;
import com.funny.magicamera.R;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.Map;

import static android.view.View.inflate;

/**
 * User: tNextDay
 * Description:
 */
public class FilterSelect extends PopupView implements AdapterView.OnItemSelectedListener {
    private FilterSelected mFilterSelected = null;
    private Context mContext;
    private View mView;
    private ArrayList<String> m_frames;
    private ArrayList<String> m_overlay;
    private ArrayList<String> m_filters;
    
    public static final String Type_Filter = "filter";
    public static final String Type_Cover = "cover";
    public static final String Type_Frame = "frame";

    public FilterSelect(Context context,FilterSelected callback) {
        super(context);
        mFilterSelected = callback;
        mContext = context;
        mView = inflate(mContext, R.layout.filter_select, null);

        getAllFilters();
        setupTabHost();
        setContentView(mView);
        setWidth(ViewGroup.LayoutParams.MATCH_PARENT);
        setAnimations(R.style.BottomUpAnimation);

        mView.findViewById(R.id.btn_hide).setOnClickListener(new View.OnClickListener(){

            @Override
            public void onClick(View view) {
                dismiss();
            }
        });

    }

    public void getAllFilters(){
        String sl[];
        try {
            AssetManager am = mContext.getResources().getAssets();
            sl = am.list("covers");
            m_overlay = new ArrayList<String>(sl.length+1);
            m_overlay.add("none");
            m_overlay.addAll(Arrays.asList(sl));
            sl = am.list("frames");
            m_frames = new ArrayList<String>(sl.length+1);
            m_frames.add("none");
            m_frames.addAll(Arrays.asList(sl));
        } catch (IOException e) {
            e.printStackTrace();
        }
        String effectList = MagicJNILib.getEffectList();
        sl = effectList.split(",");
        m_filters = new ArrayList<String>(sl.length+1);
        m_filters.add("none");
        m_filters.addAll(Arrays.asList(sl));
    }

    final static int tabTitleId[] = {
            R.string.str_effect_filter,
            R.string.str_effect_overlay,
            R.string.str_effect_frame
    };
    final static int tabContentId[] = {
            R.id.gallery_filter,
            R.id.gallery_overlay,
            R.id.gallery_frame,
    };

    private final static String[] data_from ={
            "preview", "title",
    };
    private final static int[] set_to = {
            R.id.im_preview, R.id.tv_title
    };

    public void setupTabHost(){
        TabHost tabHost = (TabHost) mView.findViewById(R.id.tabhost);
        tabHost.setup();
        LinearLayout ll = (LinearLayout)tabHost.getChildAt(0);
        TabWidget tw = (TabWidget)ll.getChildAt(0);
        LinearLayout tabIndicator;
        TextView tvTab;
        View v = inflate(mContext, R.layout.effect_tab_content,
                tabHost.getTabContentView());
        ArrayList<String> EffectType = null;
        String EffectName[] = {"filter_", "overlay_", "frame_"};
        Resources res = mContext.getResources();
        int resId;
        String packageName = mContext.getPackageName();
        for(int i = 0; i < tabTitleId.length; i++){
            tabIndicator = (LinearLayout) inflate(mContext, R.layout.effect_tab_indicator, null);
            tvTab = (TextView)tabIndicator.findViewById(R.id.title );
            tvTab.setText(mContext.getString(tabTitleId[i]));
            tabHost.addTab(tabHost.newTabSpec(String.format("tab_%d", i))
                    .setIndicator(tabIndicator)
                    .setContent(tabContentId[i]));

            //设置各个Gallery项

            Gallery g = (Gallery)v.findViewById(tabContentId[i]);
            g.setCallbackDuringFling(false);
            g.setOnItemSelectedListener(this);
            ArrayList<Map<String, ?>> effects = new ArrayList<Map<String, ?>>();
            Map<String, Object> item;
            switch (i){
                case 0:
                    EffectType = m_filters;
                    break;
                case 1:
                    EffectType = m_overlay;
                    break;
                case 2:
                    EffectType = m_frames;
                    break;
            }
            for (String str : EffectType){
                item = new HashMap<String, Object>();
                str = str.replace(".pk", "").toLowerCase();
                //设置标题
                resId = res.getIdentifier(EffectName[i]+str, "string", packageName);
                if (resId != 0){
                    item.put("title", res.getString(resId));
                }else{
                    item.put("title", str);
                }
                resId = res.getIdentifier(EffectName[i]+str, "drawable", packageName);
                if (resId == 0)
                    resId = R.drawable.noeffect;
                item.put("preview", resId);
                effects.add(item);
            }
            g.setAdapter(new SimpleAdapter(mContext, effects,
                    R.layout.gallery_item, data_from, set_to));
        }
    }

    @Override
    public void onItemSelected(AdapterView<?> adapterView, View view, int pos, long l) {
        if (mFilterSelected == null) return;
        int vid = adapterView.getId();
        String type = null;
        String name = null;
        if (vid == R.id.gallery_filter){
            type = Type_Filter;
            name = m_filters.get(pos);
        }else if (vid == R.id.gallery_overlay){
            type = Type_Cover;
            name = "res://covers/"+m_overlay.get(pos);
        }else if (vid == R.id.gallery_frame){
            type = Type_Frame;
            name = "res://frames/"+m_frames.get(pos);
        }
        mFilterSelected.OnFilterSelected(type, name);
    }

    @Override
    public void onNothingSelected(AdapterView<?> adapterView) {
        //pass
    }


    public interface FilterSelected{
        public void OnFilterSelected(String type, String name);
    }
}
