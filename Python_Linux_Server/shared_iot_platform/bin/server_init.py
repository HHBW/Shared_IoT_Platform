#from config import mysql_config as myqconfig
import sys,os
import threading
sys.path.append(os.path.dirname(sys.path[0]))
from config import socket_config as skconfig
from src import iot_socket as isk_assist
#from src import iot_mysql as imyq_assist

def Socket_Init():
    pass

def Mysql_Init():
    pass

def Server_Init():
    mserver=isk_assist.ThreadedTCPServer((skconfig.SOCKET_SERVER_IP,skconfig.SOCKET_SERVER_PORT),isk_assist.ThreadedTCPRequestHandler)
    ip,port = mserver.server_address
    print("server:",ip,port)
    server_mthread = threading.Thread(target=mserver.serve_forever)
    server_mthread.daemon=True
    server_mthread.start()
    print("Sserver loop running in thread:",server_mthread.name)
