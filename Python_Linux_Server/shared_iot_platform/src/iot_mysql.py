import pymysql,sys,os

class Sql_Assist:
    def __init__(self,
            host,
            user_name,
            user_pwd,
            db_name,
            port=3306,
            charset='utf8'):
        self.user_name=user_name
        self.user_pwd=user_pwd
        self.db_name=db_name
        self.sql=pymysql.connect(host=host,port=port,user=user_name,
                password=user_pwd,db=db_name,charset=charset,
                autocommit=True)
    
    def Get_Sql_Cursor(self,cursortype):
        self.cursor=self.sql.cursor(cursortype)
    def Sql_Execute(self,sql_sentence):
        self.cursor.execute(sql_sentence)

    def Sql_Select(self,sql_sentence,size):
        self.Sql_Execute(sql_sentence)
        if size=='all':
            return self.cursor.fetchall()
        elif size==1:
            return self.cursor.fetchone()
        elif type(size)=='int':
            return self.cursor.fetchmany(size)
        self.sql.commit()

    def Sql_Insert(self,sql_sentence):
        self.Sql_Execute(sql_sentence)
        self.sql.commit()

    def Sql_Delete(self,sql_sentence):
        self.Sql_Execute(sql_sentence)
        self.sql.commit()

    def Sql_Updata(self,sql_sentence):
        self.Sql_Execute(sql_sentence)
        self.sql.commit()

    #############Talbe Operation########################
    def Table_Select(self,*args,**kwargs): # *item table_name size
        sql="select "
        args_len=len(args)
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
            elif k=="limit":
                if type(v) == int:
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
        sql=self.Sql_Select(sql,"all")
        return sql
    def Table_Insert(self,*args,**kwargs): # *item table_name size
        sql="insert into "
        sql+=kwargs["table_name"]
        kwargs.pop("table_name")
        sql+='('
        args_len=len(args)
        for i in range(args_len):
            sql+=args[i]
            if i==args_len-1:
                sql+=')'
            else:
                sql+=','
        sql+=' values'
        args_len=len(kwargs)
        for k,v in kwargs.items():
            sql+="(%s)"%kwargs[k]
            args_len-=1
            if args_len==0:
                sql+=';'
                break;
            else:
                sql+=','
        #print(sql)
        self.Sql_Insert(sql)
    def Table_Delete(self,*args,**kwargs):
        sql="delete from %s"%kwargs["table_name"]
        if kwargs.__contains__("condition"):
            sql+=' where %s'%kwargs["condition"]
        sql+=';'
        self.Sql_Delete(sql)
    

'''
every table class can be diffrent,they have the assotiated functions
'''
class Sqltable_Assist(Sql_Assist): 
    def __init__(self,table_name,sqler):
        self.table_name=table_name
        self.sqler=sqler
        
    def Table_Select(self,*args,**kwargs):
        if kwargs.__contains__("limit"):
            if kwargs.__contains__("condition"):

                sql=self.sqler.Table_Select(*args,table_name=self.table_name,condition=kwargs["condition"],limit=kwargs["limit"])
                return sql
            else:
                sql=self.sqler.Table_Select(*args,table_name=self.table_name,limit=kwargs["limit"])
                return sql
        elif kwargs.__contains__("condition"):
            if kwargs.__contains__("limit"):

                sql=self.sqler.Table_Select(*args,table_name=self.table_name,condition=kwargs["condition"],limit=kwargs["limit"])
                return sql
            else:
                sql=self.sqler.Table_Select(*args,table_name=self.table_name,condition=kwargs["condition"])
                return sql
        else:
            sql=self.sqler.Table_Select(*args,table_name=self.table_name)
            return sql

    def Table_Insert(self,*args,**kwargs):
        kwargs.update({"table_name":self.table_name})
        self.sqler.Table_Insert(*args,**kwargs)

    def Table_Delete(self,*args,**kwargs):
        
        kwargs.update(table_name=self.table_name)
        self.sqler.Table_Delete(*args,**kwargs)



if __name__ == "__main__":

    sys.path.append(os.path.dirname(sys.path[0]))  #put this line to bin/setup.py
    from config import mysql_config as myq
 
    sqldemo=Sql_Assist(myq.MYSQL_SERVER_HOST,myq.MYSQL_ROOT_NAME,
            myq.MYSQL_ROOT_PWD,myq.MYSQL_IOT_DB)
    sqldemo.Get_Sql_Cursor(None)
    #ip="192.168.3.1"
    sqliot=Sqltable_Assist("user_of_dev",sqldemo)
    #sqliot.Table_Insert("uid,ip",value1="%s,'%s'"%(str(1),ip))
    #dres=sqliot.Table_Select("nid",condition="dev_name='%s'"%devname)
    dres=1
    ures=sqliot.Table_Select("uid",condition="did=%s"%str(dres))
    print(ures)
    #res=sqliot.Table_Select("dev_name","dev_owner",limit=1,condition="dev_name='dht11'")

    #sqliot.Table_Insert("dev_name","dev_owner",value1="'haha','sldfjl'",value2="'sldfj','weoifjo'")
    #sqliot.Table_Delete(condition="dev_name='sldfj'")
    #sql1="delete from iot_dev"
    #test=('root','roo:w')
    #sqliot.Table_Insert("name","pwd",value1="'jiangwei','jiang'",value2="'root','root'")
    #res=sqliot.Table_Delete(condition="name='jiangwei'")
    sqldemo.cursor.close()
    sqldemo.sql.close()



