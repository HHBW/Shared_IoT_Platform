import sys,os
sys.path.append(os.path.dirname(sys.path[0]))

import server_init

def main():
    server_init.Server_Init()
    while 1:
        pass

    return


if __name__=="__main__":
    main()

   
    #sa=isk_assist.Socket_Assist(ip=skconfig.SOCKET_SERVER_IP,port=skconfig.SOCKET_SERVER_PORT,listen=skconfig.SOCKET_LISTEN_NUM)


