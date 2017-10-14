import socket
import subprocess
import Global
import multiprocessing
from Processify_Nodes import NewProcess


ip_port=('192.168.2.100',9999)
Node_List=[]
s=socket.socket()
s.bind(ip_port)
s.listen(5)
Global.Node_Connected=0
print('Waiting For Connection...')
'''
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
    Node=NewProcess(conn,str(addr[0]),Global.Node_Connected)
    Global.Node_List.append(Node)
    Global.Node_Connected+=1
    if Global.Node_Connected==Global.Node_Number:
        for i in range(Global.Node_Number):
            Global.Node_List[i].Daemon(True)
            Global.Node_List[i].start()

conn.close()
