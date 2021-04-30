import os,sys
import time
sys.path.append(os.path.dirname(sys.path[0]))
import socket
import threading
import socketserver
from src import iot_mysql as usql
#import iot_mysql as usql
import re
from src import client_qhandle
from src.client_qhandle import Socket_Userreq as ureq
from src.client_qhandle import Socket_Devreq as dreq
from config import mysql_config as myq

clients = {}
client_lock = threading.Lock()

class Socket_Access:
    def __init__(self,sqler,sker):
        self.sqler=sqler
        self.sker=sker
        self.username="" #test
        self.usersk=sker.getpeername()
        self.userip=self.usersk[0]
        self.userport=self.usersk[1]
        print("usersk",self.usersk)
        
    def Access_User(self):
        req=self.sker.recv(1024)
        if (not req) or (len(req)==0):
            return 1
        req=req.decode("utf8")
        print("client_req",req)
        reslist=self.Parse_Protocol(req)
        reshead=reslist[0]
        if reshead=="user_login":
            username=reslist[1]
            password=reslist[2]
            print("username: %s"%reslist[1])
            print("password: %s"%reslist[2])
            try:

                ires=ureq.Identify_User(username,password,self.userip,self.userport,self.sqler) 
                ask="^ask_user_login%"
                if ires==0:
                    ask=ask+"successful"+"%$"
                    self.username=username
                    ures=ureq.Current_Logged(self.sqler)
                    print("current_logged",ures)
                elif ires==1:
                    ask=ask+"is exist"+"%$" #or pwd is error
                elif ires==2:
                    ask=ask+"failed"+"%$"

                self.sker.sendall(ask.encode("utf8"))
            except Exception as e:
                print("identy_err:",e)
            #self.sqler.close()
            #self.shutdown()
            #self.serverclose()
        elif reshead == "user_register":
            username=reslist[1]
            password=reslist[2]
            print("username: %s"%reslist[1])
            print("password: %s"%reslist[2])
            try:
                rres=ureq.Register_User(username,password,self.sqler)
                if rres==0:
                    self.sker.sendall(("%s registed successfully"%username).encode("utf8"))
                elif rres==1:
                    self.sker.sendall(("%s had been registed"%username).encode("utf8"))
                elif rres==2:
                    self.sker.sendall(("%s registed failed"%username).encode("utf8"))
            except Exception as e:
                print("register_err:",e)


        elif reshead =="dev_data": #old send data method
            devinfo=reslist[1]
            reslist=reslist[2:] #drop the reshead
            devinfo=devinfo.split('_')
            devname=devinfo[0]
            devowner=devinfo[1]
            print("devinfo",devinfo)

            self.sker.sendall(("^ask_dev_data%$").encode("utf8")) #send to dev
            ask="^ask_dev_data%"
            ask+=devname
            ask+='%'
            devdata='%'.join(reslist)
            devdata+="%$"
            ask=ask+devdata
            print("devdata",ask)
            ures=ureq.Current_Logged(self.sqler)
            print("current_logged",ures)
            self.Shared_IoT(devname,devowner,ask,ures,self.sker,self.sqler)
            
            
        elif reshead =="dev_register":
            devname=reslist[1]
            ownername=reslist[2]
            print("devname",devname)
            try:
                rres=dreq.Register_Dev(devname,ownername,self.sqler)
                if rres==0:
                    self.sker.sendall(("%s registed successfully"%devname).encode("utf8"))
                elif rres==2:
                    self.sker.sendall(("%s not registed"%ownername).encode("utf8"))
                elif rres==1:
                    self.sker.sendall(("%s had been shared"%devname).encode("utf8"))

            except Exception as e:
                print("dev_register_err",e)

        elif reshead == "user_adddev_page":
            qres=ureq.User_Add_Page_Req(self.sqler)
            if qres!=0:
                qres="^ask_adddev_page%"+qres+'$'

            else:
                qres="^ask_adddev_page%none%none%$"
            self.sker.sendall(qres.encode("utf8"))
        elif reshead == "user_viewdev_page":
            dres=ureq.User_Added_Dev(self.username,self.sqler)
            ask="^ask_added_dev%"
            ask=ask+dres+'$'
            self.sker.sendall(ask.encode("utf8"))

        elif reshead =="user_managedev_page":
            ask_info=ureq.User_Manage_Req(self.username,self.sqler)
            print("ask_info",ask_info)
            qres="^ask_shared_dev%"+ask_info["ask_shared_dev"]+'$'
            self.sker.sendall(qres.encode("utf8"))
            time.sleep(0.5)
            qres="^ask_added_dev%"+ask_info["ask_added_dev"]+'$'
            self.sker.sendall(qres.encode("utf8"))
        
        elif reshead == "user_add_dev":
            devname=reslist[1]
            ownername=reslist[2]
            try:
                ares=ureq.User_Add_Req(self.username,devname,ownername,self.sqler)
                ask="^ask_add_dev%"
                if ares==0:
                    ask=ask+"successful"+"%$"
                elif ares==1:
                    ask=ask+"isexist"+"%$"
                elif ares==2:
                    ask=ask+"not_found"+"%$"
            except Exception as e:
                print("add_dev_err:",e)
                ask=ask+"failed"+"%$"
            finally:
                self.sker.sendall(ask.encode("utf8"))
        elif reshead=="user_delete_dev":
            devname=reslist[1]
            try:
                ares=ureq.User_DeleteDev_Req(self.username,devname,self.sqler)

                ask="^ask_delete_dev%"
                if ares==0:
                    vres=dreq.View_User_Of_Dev(self.username,self.sqler)
                    tmp=""
                    for i in range(len(vres)):
                        tmp+=vres[i]
                        tmp+='%'
                    ask=ask+"successful%"+tmp+'$'
                    print("sask",ask)
                elif ares==1:
                    ask=ask+"not_found"+"%$"
            except Exception as e:
                print("delete_dev_err:",e)
                ask=ask+"failed"+"%$"

            finally:
                self.sker.sendall(ask.encode("utf8"))
            

        elif reshead=="user_cancel_dev":
            devname=reslist[1]
            try:
                ask="^ask_cancel_dev%"
                ares=ureq.User_CancelDev_Req(self.username,devname,self.sqler)
                if ares==0:
                    vres=dreq.View_IoT_Dev(self.username,self.sqler)
                    tmp=""
                    for i in range(len(vres)):
                        tmp+=vres[i]
                        tmp+='%'
                    ask=ask+"successful%"+tmp+'$'
                    print("sask",ask)
                elif ares==1:
                    ask=ask+"not_found"+"%$"
            except Exception as e:
                print("cancel_dev_err:",e)
                ask=ask+"failed"+"%$"

            finally:
                self.sker.sendall(ask.encode("utf8"))

        elif reshead=="user_msgboard_page":
            mres=ureq.Get_Msg_Board(self.username,self.sqler)
            ask="^ask_msgboard_page%"
            if mres==0:
                ask=ask+"none%none%none$"
            else:
                mtmp=[]
                for i in range(len(mres)):
                    mtmp.append(mres[i][0])
                    mtmp.append(mres[i][1])
                mtmp='%'.join(mtmp)
                print("mtmp",mtmp)
                ask=ask+mtmp+"%$"
            self.sker.sendall(ask.encode("utf8"))

        elif reshead=="user_msg_delete":
            recver=reslist[1]
            sender=reslist[2]
            ureq.Delete_Msg_Board(recver,sender,self.sqler)
            
        elif reshead=="reply_msg_req":
            recver=self.username
            sender=reslist[1]
            msg=reslist[2]
            ureq.Reply_Msg_Board(recver,sender,msg,self.sqler)

        elif reshead=="send_msg_req":
            sendto=reslist[1]
            senddata=reslist[2]
            ureq.Send_Msg_Board(sendto,self.username,senddata,self.sqler)


    def Parse_Protocol(self,data):
        pattern=re.compile(r"^\^(.*)\$$")
        res=pattern.match(data)
        if res != None:
            res=res.group(1)
            reslist=re.split("%",res)
            reslist.pop()
            return reslist
        else:
            return None

    
    def Shared_IoT(self,devname,devowner,devdata,ures,sker,sqler):
        
        if len(ures)>0:
            luser=[]
            for i in range(len(ures)):
                luser.append(ures[i][0])
            print("luser",luser)

            uidres=dreq.Prepare_User(devname,devowner,luser,sqler) #all uid that granted with dev
            if uidres ==1:
                print("%s not found"%devname)
                return
            elif uidres==2:
                print("%s not shared"%devname)
                return
            elif len(uidres)>0:
                usk=[]
                for i in range(len(luser)):
                    if luser[i] in uidres:
                        usk.append(ures[i][1:]) #get all ip who logged that have granted by devname
                print("uip",usk)

                with client_lock:
                    for k,v in clients.items():
                        if k in usk:
                            print("k_client",k)
                            dreq.Shared_Data(devdata,v[0])

        else:
            return
            
        

class ThreadedTCPServer(socketserver.ThreadingMixIn,socketserver.TCPServer):

    pass

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):
    
    
    def setup(self):
        self.Login_Status=0
        self.sqler=usql.Sql_Assist(myq.MYSQL_SERVER_HOST,myq.MYSQL_ROOT_NAME,myq.MYSQL_ROOT_PWD,myq.MYSQL_IOT_DB)
        self.sqler.Get_Sql_Cursor(None)
        self.sker=self.request
        with client_lock:
            clients[self.client_address]=(self.sker,self.sqler)
        print("current client number:",len(clients))

    def handle(self):
        self.sk_assist=Socket_Access(self.sqler,self.sker)
        
        while True:
            try:
                
                ares=self.sk_assist.Access_User()
                if ares==1:
                    raise UserWarning("client %s closed"%self.client_address[0])
            except UserWarning as e:
                print("userwarning:",e)
                break

            except Exception as e:
                print("handler_err:",str(e))
                break

    def finish(self):
        #print("client close")
        with client_lock:
            clients.pop(self.client_address)
            self.sker.close()
        if len(self.sk_assist.username)!=0:
            ureq.User_Exit(self.sk_assist.username,self.sqler)
        


if __name__ == "__main__":

    host="172.25.93.249"
    port=9400
    
    server = ThreadedTCPServer((host, port), ThreadedTCPRequestHandler)
    ip,port=server.server_address
    print("server:",ip,port)
    server_thread = threading.Thread(target=server.serve_forever)
    server_thread.daemon = True
    server_thread.start()
    print("Server loop running in thread:", server_thread.name)
    while 1:
        pass

    #server.shutdown()
    #server.server_close()

'''
class Socket_Assist(socket.socket):
    
    def __init__(self,*args,**kwargs):  #命令行传参的值
        self.ip=kwargs["ip"]
        self.port=kwargs["port"]
        self.sk=None
        self.listen_num=kwargs["listen"]
    def Get_Socket(self,ip,port):
        addr = (ip, int(port))
        self.sk = socket.socket()
        self.sk.bind(addr)
        self.sk.listen(self.listen_num)
    def Close_Socket(self):
        self.sk.close()


class Socket_Sqltable(Socket_Assist):
    def __init__(self,sqler,sker):
        self.sqler=sqler
        self.sker=sker


if __name__=="__main__":

    sa=Socket_Assist(port=9400,ip="172.25.93.249",listen=100)
    sa.Get_Socket(sa.ip,sa.port)
    print(sa.sk)
    sa.Close_Socket()
''' 
