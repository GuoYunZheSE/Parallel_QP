# -*- coding: utf-8 -*-
# Server.py
import subprocess
from multiprocessing.connection import Listener
from threading import Thread

def rpc_server(handler, address, authkey):

    sock = Listener(address, authkey=authkey)
    while True:
        client = sock.accept()
        t = Thread(target=handler.handle_connection, args = (client,))
        t.daemon = True
        t.start()


if __name__ == '__main__':

    # 写几个测试方法
    def add(x, y):
        return x+y

    def printdict(**kwargs):
        cnt = 0
        for k, v in kwargs.items():
            print(''.join(['"', str(k), '":"', str(v), '"']))
            cnt += 1
        return cnt

    def RunQP():
        pname=r'C:\Users\user\Documents\Visual Studio 2017\Projects\Print\Debug\Print.exe'
        #source=bytes(parameter,encoding='utf-8')
        p = subprocess.Popen(['C:/Users/user/Documents/Visual Studio 2017/Projects/Print/Debug/Print.exe','51','2','2','232'],shell=True,stdout=subprocess.PIPE)
        result=p.communicate()[0]
        print(result)
        return result
    # 新建一个handler类实例, 并将add, printdict方法注册到handler里面
    from Handler import RPCHandler
    rpc_handler = RPCHandler()
    rpc_handler.register_function(add)
    rpc_handler.register_function(printdict)

    # 运行server
    RunQP()
   # rpc_server(rpc_handler, ('192.168.2.100', 17000), authkey=bytes('Node_0',encoding='utf8'))