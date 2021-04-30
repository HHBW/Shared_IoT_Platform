package com.example.shared_iot_client;

import android.content.Context;
import android.util.Log;
import android.widget.ListView;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

public class Parse_Assist {

    public String[] Parser_Mather(String data){
        String[] pres;
        String mres;
        String pattern="^\\^(.*)\\$$";
        Pattern re=Pattern.compile(pattern);
        Matcher matcher=re.matcher(data);

        if(matcher.find()){
            mres=matcher.group(1);
            //Log.i("res:",mres);
            pres=mres.split("%");
            return pres;  //{{"",""},}
        }
        else {
            return new String[]{};
        }
    }
    private String[] Str_Copy_With_Pos(String[] arg,int pos){

        String[] tmp=new String[arg.length-pos];
        for(int i=0;i+pos<arg.length;i++){
            tmp[i]=arg[i+pos];
        }
        return tmp;
    }
    private String[][] Couple_Pair(String[] arg){
        int pairs=arg.length/2;
        int p=0;
        String[][] tmp=new String[pairs][2];
        for(int i=0;i<pairs;i++){
            StringBuffer sb=new StringBuffer();
            for(int j=0;j<2;j++){
                tmp[i][j]=arg[p];
                sb.append(tmp[i][j]);
                p++;
            }
            //Log.i("devinfo "+i,sb.toString());
        }
        return tmp;
    }

    private List<Map<String, Object>> getData(int resid, String[][] devinfo) {
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        for (String[] strings : devinfo) {
            Map<String, Object> map = new HashMap<String, Object>();
            map.put("devname", strings[0]);
            map.put("ownername", strings[1]);
            map.put("img", resid);
            Log.i("devinfo",map.get("devname")+" "+map.get("ownername"));
            list.add(map);
        }
        return list;
    }
    private List<Map<String, Object>> getMsgBoard(int resid, String[][] devinfo) {
        List<Map<String, Object>> list = new ArrayList<Map<String, Object>>();
        for (String[] strings : devinfo) {
            Map<String, Object> map = new HashMap<String, Object>();
            map.put("sendername", strings[0]);
            map.put("sendermsg", strings[1]);
            map.put("img", resid);
            Log.i("msginfo",map.get("sendername")+" "+map.get("sendermsg"));
            list.add(map);
        }
        return list;
    }
    public List<Map<String,Object>> Refresh_ListView(Context context, ListView listview, String[] data,List<Map<String,Object>> dev_list,int pos){
        String[] tdata;
        String[][] tdevinfo;
        devlist_adapter mddev_adapter;
        tdata=Str_Copy_With_Pos(data,pos);
        tdevinfo=Couple_Pair(tdata);
        dev_list=getData(R.drawable.icon,tdevinfo);
        mddev_adapter=new devlist_adapter(context,R.layout.devlist_pattern,dev_list);
        listview.setAdapter(mddev_adapter);
        return dev_list;
    }
    public List<Map<String,Object>> Refresh_MsgBoardList(Context context, ListView listview, String[] data,List<Map<String,Object>> dev_list,int pos){
        String[] tdata;
        String[][] tdevinfo;
        msgitem_adapter mbview_adapter;
        tdata=Str_Copy_With_Pos(data,pos);
        tdevinfo=Couple_Pair(tdata);
        dev_list=getMsgBoard(R.drawable.icon,tdevinfo);
        mbview_adapter=new msgitem_adapter(context,R.layout.msgitem_pattern,dev_list);
        listview.setAdapter(mbview_adapter);

        return dev_list;
    }
    public String[][] Couple_Pair_Data(String[] data,int pos){
        String[] tdata;
        String[][] tdevinfo;
        tdata=Str_Copy_With_Pos(data,pos);
        tdevinfo=Couple_Pair(tdata);
        return tdevinfo;
    }


}
