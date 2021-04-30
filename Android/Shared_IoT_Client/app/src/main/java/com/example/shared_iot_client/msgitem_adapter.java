package com.example.shared_iot_client;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.ImageView;
import android.widget.TextView;

import java.util.List;
import java.util.Map;

public class msgitem_adapter extends BaseAdapter {
    private final List<Map<String,Object>> mapList;

    private final int resId;
    private final LayoutInflater mInflater;
    public msgitem_adapter(Context context, int resid, List<Map<String,Object>> aplist) {
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

    @Override
    public View getView(int position, View convertView, ViewGroup parent) {
        // TODO Auto-generated method stub
        mViewHolder mholder=null;
        if (convertView == null) {
            mholder = new mViewHolder();
            convertView = mInflater.inflate(resId, null);
            mholder.sender_name = (TextView) convertView.findViewById(R.id.text_msgsender);
            mholder.imgpattern = (ImageView) convertView.findViewById(R.id.image_devpattern);
            convertView.setTag(mholder);
        }else{
            mholder = (mViewHolder) convertView.getTag();
        }
        mholder.imgpattern.setBackgroundResource((Integer)mapList.get(position).get("img"));
        mholder.sender_name.setText((String)mapList.get(position).get("sendername"));

        return convertView;
    }

    public static class mViewHolder{
        public ImageView imgpattern;
        public TextView sender_name;
    }
}
