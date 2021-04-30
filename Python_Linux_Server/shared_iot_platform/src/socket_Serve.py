import socketserver
class myServe(socketserver.BaseRequestHandler):
    def handle(self):
        print('server starting')
        while 1:
            print('waiting for connecting')
            conn = self.request
            addr = self.client_address
            while 1:
                try:
                    data = conn.recv(1024)
                    print(str(data,'utf8'))
                    inp = input('server>>')
                    conn.send(bytes(inp,'utf8'))
                except:
                    break

if __name__ == '__main__':
    server = socketserver.ThreadingTCPServer(('172.25.93.249',9400),myServe)
    server.serve_forever()

