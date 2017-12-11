import Proxy
import  os
import numpy
import re
from multiprocessing.connection import Client


def system_initial(config_file,Nodes,Proxies):
    print('System Initialing...')
    config_file=open(config_path,'r')
    Node_Number = 0
    Node_IP=[]
    Node_Port=[]
    for eachline in config_file.readlines():
        if eachline[:12]=='Node_Number=':
            Node_Number=eachline[12:].strip('{}\n')
            Node_Number=int(Node_Number)
        if eachline[:18]=='Nodes_Information=':
            temp=eachline[18:].strip('{}\n')
            temp=temp.split(';')
            for i in range(Node_Number):
                ip,port=temp[i].strip('[]').split(',')
                ip=ip.strip("''")
                port=port.strip("''")
                Node_ID = 'Node_%d' % (i)
                Proxy_ID = 'Proxy_%d' % (i)
                try:
                    Nodes.append(Client((ip, int(port)), authkey=bytes(str(Node_ID), encoding='utf8')))
                    try:
                        Proxies.append(Proxy.RPCProxy(Nodes[i]))
                    except Exception as e1:
                        print(str(Node_ID) + ':')
                        raise e1
                except Exception as e2:
                    print(str(Node_ID) + ':')
                    raise e2
    print('Initial Completed. Now You have %d Nodes to Use.'%(len(Nodes)))
if __name__ == '__main__':
    config_path='/home/lucas/Codes/Projects/Parallel_QP/Parallel_QP/RPC_Version/config.txt'
    Nodes=[]
    Proxies=[]
    system_initial(config_path,Nodes,Proxies)
    while True:
        print('Please input the Command: ')
        Command = input('<<<')
        if Command != 'exit':
            print(eval(Command))
        else:
            break