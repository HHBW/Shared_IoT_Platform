#from src import iot_socket
#from iot_socket import Socket_Assist
#import re
import sys,os
sys.path.append(os.path.dirname(sys.path[0]))
from src.iot_mysql import Sqltable_Assist as sqlt

class Socket_Userreq:
    def __init__(self):
        pass
    @classmethod    
    def Identify_User(cls,username,userpwd,userip,userport,usql):
        sqler=sqlt("registed_user",usql)
        userinfo=(username,userpwd)
        ures=sqler.Table_Select("name","pwd")
        qres=userinfo in ures
        if qres==True:
            lres=cls.Login_Successe(username,userip,userport,usql)
            if lres==0:
                return 0
            elif lres==1:
                return 1
            else:
                return 2
        else:
            lres=cls.Login_Failed()
            return lres

    @staticmethod
    def Login_Successe(username,userip,userport,usql):
        try:
            sqler1=sqlt("logged_user",usql)
            sqler2=sqlt("registed_user",usql)
            # name='username' need add a '' wrapper it
            ures=sqler1.Table_Select("uid")
            uid=sqler2.Table_Select("nid",condition="name='%s'"%username)
            uid=uid[0][0]
            for i in range(len(ures)):
                if uid == ures[i][0]:
                    return 1
            sqler1.Table_Insert("uid,ip,port",value1="%s,'%s',%s"%(str(uid),userip,str(userport)))
            return 0
        except Exception as e:
            print("logsucce_err:",e)
            return 2

    @staticmethod
    def Login_Failed():
        return 1

    @classmethod
    def Register_User(cls,username,userpwd,usql):
        ires=cls.User_Isexist(username,usql)
        if ires!=1:
            rres=cls.Reg_Successe(username,userpwd,ires)
            return rres
        else:
            return 1

    @staticmethod
    def User_Isexist(username,usql):
        sqler=sqlt("registed_user",usql)
        ures=sqler.Table_Select("name")
        for i in range(len(ures)):
            if username == ures[i][0]:
                return 1
        return sqler
    @staticmethod
    def Reg_Successe(username,pwd,usql):
        try:
            usql.Table_Insert("name","pwd",value1="'%s','%s'"%(username,pwd))
            return 0
        except Exception as e:
            print("reg_success_err:",e)
            return 2

    @staticmethod
    def User_Exit(username,usql):
        sqler1=sqlt("registed_user",usql)
        sqler2=sqlt("logged_user",usql)
        ures=sqler1.Table_Select("nid",condition="name='%s'"%username)
        if len(ures[0])!=0:
            uid=ures[0][0]
            sqler2.Table_Delete(condition="uid=%s"%str(uid))

    @staticmethod
    def Current_Logged(usql):
        sqler=sqlt("logged_user",usql)
        ures=sqler.Table_Select("uid,ip,port")
        return ures 
    @classmethod
    def User_Add_Page_Req(cls,usql):
        sqler=sqlt("iot_dev",usql)
        dres=sqler.Table_Select("dev_name","dev_owner")
        if len(dres)>0:
            dres=cls.Dev_Info_Pair(dres)
            return dres
        else:
            return 0
        
    @staticmethod
    def Dev_Info_Pair(devinfo):
        tmp=[]
        for i in range(len(devinfo)):
            tmp.append('%'.join(devinfo[i]))
        stmp='%'.join(tmp)
        stmp+='%'
        return stmp
    
    @classmethod
    def User_Manage_Req(cls,username,usql):
        ask_info={}
        sqler1=sqlt("iot_dev",usql)
        sqler2=sqlt("user_of_dev",usql)
        sqler3=sqlt("registed_user",usql)
        dres=sqler1.Table_Select("dev_name","dev_owner",condition="dev_owner='%s'"%username)  #shared dev
        if len(dres)>0:
            dres=cls.Dev_Info_Pair(dres)
        else:
            dres="none%none%"
        ask_info.update({"ask_shared_dev":dres})
        ures=sqler3.Table_Select("nid",condition="name='%s'"%username)
        uid=ures[0][0]
        dres=sqler2.Table_Select("dev_sign",condition="uid=%s"%str(uid))
        if len(dres)>0:
            devinfo=[]
            for i in range(len(dres)):
                did=dres[i][0] #dev_sign
                ires=sqler1.Table_Select("dev_name","dev_owner",condition="dev_sign='%s'"%did)  #shared dev
                if len(ires)>0:
                    devinfo.append(ires[0])
            dres=cls.Dev_Info_Pair(devinfo)
        else:
            dres="none%none%"
        ask_info.update({"ask_added_dev":dres})
        return ask_info

    @classmethod
    def User_Added_Dev(cls,username,usql):
        sqler1=sqlt("iot_dev",usql)
        sqler2=sqlt("user_of_dev",usql)
        sqler3=sqlt("registed_user",usql)
        ures=sqler3.Table_Select("nid",condition="name='%s'"%username)
        uid=ures[0][0]
        dres=sqler2.Table_Select("dev_sign",condition="uid=%s"%str(uid))
        if len(dres)>0:
            devinfo=[]
            for i in range(len(dres)):
                did=dres[i][0] #dev_sign
                ires=sqler1.Table_Select("dev_name","dev_owner",condition="dev_sign='%s'"%did)  #shared dev
                if len(ires)>0:
                    devinfo.append(ires[0])
            dres=cls.Dev_Info_Pair(devinfo)
        else:
            dres="none%none%"
        return dres

    @staticmethod
    def User_Add_Req(username,devname,ownername,usql):
        sqler1=sqlt("iot_dev",usql)
        sqler2=sqlt("user_of_dev",usql)
        sqler3=sqlt("registed_user",usql)
        devsign=devname+'_'+ownername
        dres=sqler1.Table_Select("dev_sign",condition="dev_sign='%s'"%devsign)
        if len(dres)>0:
            did=dres[0][0]
        else:
            did="none"
        ures=sqler3.Table_Select("nid",condition="name='%s'"%username)
        if len(ures)>0:
            uid=ures[0][0]
        else:
            uid="none"
        sres=sqler2.Table_Select("uid,dev_sign")
        if (uid,did) in sres:
            return 1
        elif did=="none":
            return 2
        sqler2.Table_Insert("devname","dev_sign","uid",value1="'%s','%s',%s"%(devname,devname+'_'+ownername,str(uid)))
        return 0

    @staticmethod
    def User_DeleteDev_Req(username,devname,usql): #delete added dev
        sqler2=sqlt("user_of_dev",usql)
        sqler3=sqlt("registed_user",usql)
        dres=sqler2.Table_Select("devname",condition="devname='%s'"%devname)
        if len(dres)>0:
            did=dres[0][0]
        else:
            did="none"
        ures=sqler3.Table_Select("nid",condition="name='%s'"%username)
        if len(ures)>0:
            uid=ures[0][0]
        else:
            uid="none"
        sres=sqler2.Table_Select("uid,devname")
        if (uid,did) in sres:
            sqler2.Table_Delete(condition="uid=%s and devname='%s'"%(str(uid),did))
            return 0
        return 1

    @staticmethod
    def User_CancelDev_Req(username,devname,usql): #cancel shared dev
        sqler=sqlt("iot_dev",usql)
        dres=sqler.Table_Select("dev_name",condition="dev_name='%s'"%devname)
        if len(dres)>0:
            sqler.Table_Delete(condition="dev_name='%s' and dev_owner='%s'"%(devname,username))
            return 0
        else:
            return 1

    @staticmethod
    def Get_Msg_Board(username,usql):
        sqler=sqlt("msg_board",usql)
        mres=sqler.Table_Select("sender,msg",condition="recver='%s'"%username)
        if len(mres)>0:
            return mres

        else:
            return 0

    @staticmethod
    def Delete_Msg_Board(recver,sender,usql):
        sqler=sqlt("msg_board",usql)
        sqler.Table_Delete(condition="sender='%s'and recver='%s'"%(sender,recver))
        return sqler

    @classmethod
    def Reply_Msg_Board(cls,recver,sender,msg,usql):
        sqler=cls.Delete_Msg_Board(recver,sender,usql)
        sqler.Table_Insert("sender","recver","msg",values="'%s','%s','%s'"%(recver,sender,msg))
        
    @staticmethod
    def Send_Msg_Board(recver,sender,msg,usql):
        sqler=sqlt("msg_board",usql)
        sqler.Table_Insert("sender","recver","msg",values="'%s','%s','%s'"%(sender,recver,msg))

class Socket_Devreq:
    def __init__(self):
        pass

    
    @staticmethod
    def Shared_Data(data,sker):
        try:
            sker.sendall(data.encode("utf8"))
        except Exception as e:
            print("shared_data_err:",e)

    @classmethod
    def Prepare_User(cls,devname,devowner,luser,usql):
        ures=cls.Granted_Dev_Of_User(devname,devowner,usql)
        if ures==0:
            return 1
        elif ures==1:
            return 2
        else:
            ulist=[]
            for i in range(len(ures)):
                ulist.append(ures[i][0])  #ALL user who granted privilege with dev
            uidres=[]
            j=0
            for i in range(len(luser)):  #get the uid who logged which have granted privilege
                if luser[i] in ulist:
                    uidres.append(luser[i]) 
                    j+=1

            print("uidres",uidres)

            return uidres  #uid [1,2,3]

    @staticmethod
    def Granted_Dev_Of_User(devname,devowner,usql):
        sqler1=sqlt("iot_dev",usql)
        sqler2=sqlt("user_of_dev",usql)
        dev_sign=devname+'_'+devowner
        dres=sqler1.Table_Select("dev_sign",condition="dev_sign='%s'"%dev_sign)
        if len(dres)>0:
            if len(dres[0])==1:
                dres=dres[0][0]
                ures=sqler2.Table_Select("uid",condition="dev_sign='%s'"%dres)
                if len(ures)>0:

                    print("%s granted to:"%devname,ures)
                    return ures  #((1,),(2,),)
                else:
                    return 1 # this dev have not shared
            else:
                return 0 # not found the dev which called devname
        else:
            return 1

    @classmethod
    def Register_Dev(cls,devname,ownername,usql):
        sqler=sqlt("iot_dev",usql)
        sqler2=sqlt("registed_user",usql)
        if cls.Dev_Isexist(devname,sqler)==0:
            ures=sqler2.Table_Select("name",condition="name='%s'"%ownername)
            if len(ures)==1:
                sqler.Table_Insert("dev_name","dev_owner,permission,dev_sign",value1="'%s','%s','%s','%s'"%(devname,ownername,"free",devname+'_'+ownername))

                return 0
            else:
                return 2
        else:
            return 1

    @staticmethod
    def Dev_Isexist(devname,usql):
        dres=usql.Table_Select("dev_name",condition="dev_name='%s'"%devname)
        if len(dres)>0:
            return 1
        else:
            return 0

    @staticmethod
    def View_User_Of_Dev(username,usql):
        sqler1=sqlt("user_of_dev",usql)
        sqler2=sqlt("registed_user",usql)
        ures=sqler2.Table_Select("nid",condition="name='%s'"%username)
        uid=ures[0][0]
        dres=sqler1.Table_Select("dev_sign",condition="uid=%s"%str(uid))
        dev_info=[]
        if len(dres)>0:
            for i in range(len(dres)):
                did=dres[i][0]
                did=did.split('_')
                print("did",did)
                dev_info.append(did[0])
                dev_info.append(did[1])
        else:
            dev_info.append("none")
            dev_info.append("none")

        print("user_dev_name",dev_info)
        return dev_info

    @staticmethod
    def View_IoT_Dev(username,usql):
        sqler=sqlt("iot_dev",usql)
        dres=sqler.Table_Select("dev_name",condition="dev_owner='%s'"%username)
        dev_info=[]
        if len(dres)>0:
            for i in range(len(dres)):
                dev_info.append(dres[i][0])
                dev_info.append(username)
        else:
            dev_info.append("none")
            dev_info.append("none")
        print("iot_dev_name",dev_info)

        return dev_info


    


if __name__=="__main__":
    test="^user_login%username%userpwd%$"
    pattern=re.compile(r"^\^(.*)\$$")
    res=pattern.match(test)
    test2=res.group(1)
    pattern=re.compile(r"%")
    res=pattern.split(test2)
    print(res)

