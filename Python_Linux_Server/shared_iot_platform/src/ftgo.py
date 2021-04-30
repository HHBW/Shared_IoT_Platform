import socket
import os,sys

CoS_Sign='server'


class Socket_Resquest(socket.socket):
    '''
    cmd :  connect -->  indicate the user as a cilent
           bind --> indicate the user as a server
    '''
    listen_num=3
    def __init__(self,*args,**kwargs):  #命令行传参的值
        pass
    @staticmethod
    def Get_Socket(cmd,ip,port):
        addr = (ip, int(port))
        sk = socket.socket()
        sobj=tuple()
        if cmd == 'connect':
            sk.connect(addr)
            sobj = (sk, 'cilent')
        elif cmd == 'bind':
            sk.bind(addr)
            sk.listen(Socket_Resquest.listen_num)
            sobj=(sk,'server')
        return sobj


class File_Transfer(Socket_Resquest):
    '''
    serve terminal function : Response
    cilent termianl function: Upload Download

    '''
    def __init__(self,cmd,ip,port):
        self.Sobj,self.sign=Socket_Resquest.Get_Socket(cmd,ip,port)
        self.Base_Dir=os.path.dirname(__file__)
        self.conn=None
    def Upload(self):
        file_list=os.listdir(self.Base_Dir) #list all file under the base_dir
        for i in file_list:
            print(i)
        file_name=input("which file you want to send>>> ")
        file_path=os.path.join(self.Base_Dir,file_name)
        try:
            file_size=os.path.getsize(file_path)
            fp=open(file_path,'rb')
        except:
            print('file not found')
            return 1
        file_info='Upload|%s|%s'%(file_name,file_size) #发送给服务端的文件信息,及命令名
        self.Sobj.send(file_info.encode('utf8'))
        resp=self.Sobj.recv(1024) #recv the response from server
        if str(resp,'utf8')=='ok':
            print('start uploading...')
            has_sent=0
            while has_sent!=file_size:
                data = fp.read(1024)
                self.Sobj.send(data)
                has_sent += len(data)
            fp.close()
            print('uploaded file %s total size %d '%(file_name,file_size))
            return 0
        else:
            print('server refuse upload request!')
            return 1


    def Download(self):
        file_name = input("which file you want to download>>> ")
        file_info=''
        for i in self.ser_file:
            if i.split('|')[0]==file_name:
                file_info='Download|%s'%i
                break
        if file_info!='':
            self.Sobj.send(file_info.encode('utf8'))
            print('wait for Downloading...')
        else:
            print('%s not found')
            return 1
        resp=self.Sobj.recv(1024) #接收服务器响应消息
        if str(resp,'utf8')=='ok':

            has_recved=0
            file_info=file_info.split('|')
            file_path=os.path.join(self.Base_Dir,file_info[1])
            fp=open(file_path,'wb')
            while has_recved!=int(file_info[2]):
                data=self.Sobj.recv(1024)
                fp.write(data)
                has_recved+=len(data)
            print('Download successfully!')
            return 0


    def Dir(self):
        self.Sobj.send('Dir| '.encode('utf8')) #满足解析格式
        print('Server direction Following...')
        data=self.Sobj.recv(1024) #一般显示一个目录下的文件信息,1024个字节就够了
        self.ser_file=str(data,'utf8').strip().split(' ') #['file_name|file_size',]
        for i,j in enumerate(self.ser_file):
            file_info=j.split('|')
            print('%d %s %s'%(i,file_info[0],file_info[1]))
        return 0
    def ls(self):
        file_list = os.listdir(self.Base_Dir)
        file_info = []
        for i in file_list:
            file_path = os.path.join(self.Base_Dir, i)
            file_size = os.path.getsize(file_path)
            file_info.append('%s %s' % (i, file_size))
        for i,j in enumerate(file_info):
            print('%d %s'%(i,j))
    def exit(self):
        self.Sobj.send('exit| '.encode('utf8'))
        data=self.Sobj.recv(1024)
        if str(data,'utf8')=='ok':
            return 'exit'
        return 1

    def Response(self):
        data=self.conn.recv(1024) #用于接收命令
        data=str(data,'utf8')
        data_info=data.strip().split('|')
        if data_info[0]=='Upload':
            resp=input('if you want recv the file %s total size %d>>>[ok/cancel]'%(data_info[1],int(data_info[2])))
            self.conn.send(resp.encode('utf8'))
            if resp=='ok':
                print('start recving...')
                file_path=os.path.join(self.Base_Dir,data_info[1])
                fp=open(file_path,'wb')
                has_recv=0
                while has_recv!=int(data_info[2]):
                    data=self.conn.recv(1024)
                    fp.write(data)
                    has_recv+=len(data)
                fp.close()
                print('recved file %s total size %d ' % (data_info[1], int(data_info[2])))
                return 0
            else:
                return 1
        elif data_info[0]=='Dir':
            file_list=os.listdir(self.Base_Dir)
            file_info=[]
            for i in file_list:
                file_path=os.path.join(self.Base_Dir,i)
                file_size=os.path.getsize(file_path)
                file_info.append('%s|%s'%(i,file_size))
            file_info=' '.join(file_info)
            self.conn.send(file_info.encode('utf8'))
            return 0

        elif data_info[0]=='Download':
            resp = input('if you want send the file %s total size %d>>>[ok/cancel]' % (data_info[1], int(data_info[2])))
            self.conn.send(resp.encode('utf8')) #回发服务器的响应消息
            if resp == 'ok':
                print('start sending...')
                file_path = os.path.join(self.Base_Dir, data_info[1])
                fp = open(file_path, 'rb')
                has_sent = 0
                while has_sent != int(data_info[2]):
                    data=fp.read(1024)
                    self.conn.send(data)
                    has_sent += len(data)
                fp.close()
                print('sended file %s total size %d ' % (data_info[1], int(data_info[2])))
                return 0
            else:
                return 1
        elif data_info[0]=='exit':
            print('%s %s is exiting...'%(self.addr[0],self.addr[1]))
            self.conn.send('ok'.encode('utf8'))
            return 'cilent_exit'

def Serve_Func(Sobj):
    while True:
        print('wait for connecting')
        Sobj.conn, Sobj.addr = Sobj.Sobj.accept()
        print('%s %d connected'%(Sobj.addr[0],Sobj.addr[1]))
        while True:
            rest = Sobj.Response()
            if rest == 0:
                print("work successfully!")
            elif rest=='cilent_exit':
                Sobj.conn.close()
                break
            elif rest==1:
                print('work failed!')
            else:
                break
def Cilent_Func(Sobj):
    while True:
        inp=input('please enter your cmd>>>')
        func=getattr(Sobj,inp)
        rest=func()
        if rest==0:
            print("work successfully!")
        elif rest=='exit':
            print('exit successfully!')
            break
        else:
            print('work failed!')

if __name__=="__main__":
    # argv_cmd='connect'
    # argv_ip='192.168.31.240'
    # argv_port='9090'
    argv_cmd = sys.argv[1]
    argv_ip = sys.argv[2]
    argv_port = sys.argv[3]
    obj=File_Transfer(argv_cmd,argv_ip,argv_port)
    print('Corrent Path>>: %s'%obj.Base_Dir)
    print('%s is running...'%obj.sign)
    if obj.sign=='server':
        Serve_Func(obj)
    elif obj.sign=='cilent':
        Cilent_Func(obj)
        obj.Sobj.close()

