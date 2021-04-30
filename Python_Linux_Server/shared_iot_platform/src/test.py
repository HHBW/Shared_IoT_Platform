import sys,os

current_file=__file__
#print(os.path.dirname(sys.path[0]))
a={}
a.update(table_name="iot_dev")

def test1(*args,**kwargs):
    sql="select "
    args_len=len(args)
    print(args)
    for i in range(args_len):
        sql+=args[i]
        if i==args_len-1:
            sql+=' '
        else:
            sql+=','
    sql+="from "
    args_len=len(kwargs)
    for k,v in kwargs.items():
        if k=="table_name":
            sql+=v
            if args_len>1:
                sql+=' '
        elif k=="limit":  #limit is always at the end
            if type(v)==int:
                sql+="limit %s;"%str(v)
            elif type(v)==tuple:
                sql+="limit %s,%s;"%(str(v[0]),str(v[1]))
            break
        elif k=="condition":
            sql+="where "
            sql+=v
            if args_len>1:
                sql+=' '
        args_len-=1
        if args_len==0:
            sql+=';'
            break;



    print(sql)

if __name__=="__main__":
    #test1("nid","demo","test",table_name="iot_dev",condition="nid=3",limit=3)

    #test1("da18b20","haha",table_name="iot")
    #test1("slfj",table_name="jeoijf",limit=(3,4))
    #test1("slfj",table_name="jeoijf",condition="asldfja asf safj")
