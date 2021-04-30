package com.example.shared_iot_client;

import android.annotation.SuppressLint;
import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;
import java.util.Map;

public class devlist_adapter extends BaseAdapter {

    private final List<Map<String,Object>> mapList;

    private final int resId;
    private final LayoutInflater mInflater;
    public devlist_adapter(Context context, int resid, List<Map<String,Object>> aplist) {
        this.mInflater=LayoutInflater.from(context);
        this.resId = resid;
        this.mapList=aplist;
    }

    @Override
    public int getCount() {
        return mapList.size();
    }

    @Override
    public Object getItem(int position) {
        return null;
    }

    @Override
    public long getItemId(int position) {
        return 0;
    }

    //@SuppressLint("ViewHolder")
    @Override
    public View getView(final int position, View converetView, ViewGroup parent) {
        // TODO Auto-generated method stub
        mViewHolder mholder=null;
        if (converetView == null) {
            mholder = new mViewHolder();
            converetView=mInflater.inflate(resId,null);
            mholder.dev_name = (TextView) converetView.findViewById(R.id.text_devname);
            mholder.owner_name = (TextView) converetView.findViewById(R.id.text_ownername);
            mholder.imgpattern = (ImageView) converetView.findViewById(R.id.image_devpattern);
            converetView.setTag(mholder);
        }else{
            mholder = (mViewHolder) converetView.getTag();
        }
        mholder.imgpattern.setBackgroundResource((Integer)mapList.get(position).get("img"));
        mholder.dev_name.setText((String)mapList.get(position).get("devname"));
        mholder.owner_name.setText((String)mapList.get(position).get("ownername"));

        return converetView;
    }

    public static final class mViewHolder{
        public ImageView imgpattern;
        public TextView dev_name;
        public TextView owner_name;
      }
}
