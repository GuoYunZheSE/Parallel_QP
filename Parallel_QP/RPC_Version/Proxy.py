# -*- coding: utf-8 -*-

import pickle

class RPCProxy(object):

    def __init__(self, connection):
        self._connection = connection

    def __getattr__(self, name):
        # 通过name，得到一个函数
        def do_rpc(*args, **kwargs):
            self._connection.send(pickle.dumps((name, args, kwargs)))
            result = pickle.loads(self._connection.recv())
            if isinstance(result, Exception):
                raise result
            return result

        return do_rpc

# 远程连接并且调用
if __name__ == '__main__':
    #仅在实验时直接运行此程序
    from multiprocessing.connection import Client
    try:
        rpc_client_1 = Client(('192.168.2.100', 17000), authkey=bytes('Node1',encoding='utf8'))
    except Exception as e:
        print(e)
    rpc_client_2 = Client(('192.168.2.101', 17000), authkey=bytes('Node2',encoding='utf8'))
    Node1_proxy = RPCProxy(rpc_client_1)
    Node2_proxy = RPCProxy(rpc_client_2)
    print (Node1_proxy.add(2,3))
    print (Node1_proxy.printdict(**{"Node1":"rpc", "github":"https://github.com/GuoYunZheSE/Parallel_QP"}))
    print (Node2_proxy.add(2,3))
