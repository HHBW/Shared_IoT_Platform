import socket
sk = socket.socket()
addresser = ('192.168.31.240',9400)

#sk.bind(addresser)
sk.bind(addresser)
sk.listen(3)
print('waiting...')
while 1:
    connt,addr = sk.accept()
    print(addr)
    while 1:
        try:
            data = connt.recv(1024)
            print(str(data,'utf8'))
        except:
            connt.close()
            print('waiting for connect')
            break
        print('...')
        if not data:
            connt.close()
            print('waiting for connect')
            break
        #message = input('S>>>')
        #connt.send(bytes(message+"\r\n",'utf8'))
sk.close()


