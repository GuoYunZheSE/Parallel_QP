import socket
import subprocess
import Global
from Threadify_Nodes import Threadify_Nodes


ip_port=('127.0.0.1',9999)
Node_List=[]
s=socket.socket()
s.bind(ip_port)
s.listen(5)
Count=1

print('Waiting For Connection...')
while True:
    conn,addr=s.accept()
    while True:
        try:
            conn.send(bytes('Welcome ~',encoding='utf8'))
            recv_data=conn.recv(1024)
            if len(recv_data)==0:
                break
            p=subprocess.Popen(str(recv_data,encoding='utf8'),shell=True,stdout=subprocess.PIPE)

            res=p.stdout.read()
            if len(res)==0:
                send_data='cmd error'
            else:
                send_data=str(res,encoding='gbk')
            send_data=bytes(send_data,encoding='utf8')

            ready_tag='Ready|%s'%len(send_data)
            conn.send(bytes(ready_tag,encoding='utf8'))
            feedback=conn.recv(1024)
            feedback=str(feedback,encoding='utf8')

            if feedback.startswith('Start'):
                conn.send(send_data)
        except Exception:
            break
    conn.close()
'''
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
'''