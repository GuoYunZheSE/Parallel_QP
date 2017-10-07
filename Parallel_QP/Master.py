import socket
import subprocess
import Global
from Threadify_Nodes import Threadify_Nodes


ip_port=('192.168.2.100',9999)
Node_List=[]
s=socket.socket()
s.bind(ip_port)
s.listen(5)
Count=1

print('Waiting For Connection...')
while True: 
    conn,addr=s.accept()
    Node=Threadify_Nodes(conn,str(addr[0]),Count)
    Count+=1
    Global.Node_List.append(Node)
    if Count==Global.Node_Number:
        for i in range(Global.Node_Number):
            Global.Node_List[i].daemon(True)
            Global.Node_List[i].start()

conn.close()