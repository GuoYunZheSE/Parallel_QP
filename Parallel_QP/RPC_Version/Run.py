if __name__ == '__main__':
    import Proxy
    from multiprocessing.connection import Client
    Nodes=[]
    Proxies=[]
    print('System Initialing...')

    Node_Number=int(input('Please input the number of nodes:'))
    for i in range(Node_Number):
        Node_ID='Node_%d'%(i)
        Proxy_ID='Proxy_%d'%(i)
        print('Please input IP adress for %s'%(Node_ID))
        IP=input()
        print('Please input port number for %s'%(Node_ID))
        Port=input()
        try:
            Nodes.append(Client((str(IP), int(Port)), authkey=bytes(str(Node_ID),encoding='utf8')))
            try:
                Proxies.append(Proxy.RPCProxy(Nodes[i]))
            except Exception as e1:
                print(str(Node_ID)+':')
                raise e1
        except Exception as e2:
            print(str(Node_ID)+':')
            raise e2
    
    print('Initial Completed. Now You have %d Nodes to Use.'%(len(Nodes)))

while True:
    print('Please input the Command: ')
    Command=input('<<<')
    if Command != 'exit':
        print(eval(Command))
    else:
        break