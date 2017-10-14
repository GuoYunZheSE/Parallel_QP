import threading
import subprocess
import time
import Global
from multiprocessing import Process
class NewProcess(Process):
    """Inherit of class Process and creat a new class NewProcess"""
    def __init__(self, Node_Socket,Node_IP,Node_NO):
        print('Creat process successfully, the %s process has been created'%(Global.Node_Connected))#Message for master
        self.Node_Socket=Node_Socket
        self.Node_IP=Node_IP
        self.Node_NO=Node_NO
        Welcom_msg='Welcome! Your IP address is: '+str(self.Node_IP)+'Your Node ID is: '+str(self.Node_NO)
        self.Node_Socket.send(bytes(Welcom_msg,encoding='utf8'))
        super().__init__()

    def run(self):
        print('Start',self.Node_IP)
        print_time(self.Node_IP)
        while True:
            try:
                #Connecte Successfully
                msg1='Now I am going to assign partitions to you'
                self.Node_Socket.send(bytes(msg1,encoding='utf8'))


                recv_data=self.Node_Socket.recv(1024)
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
                self.Node_Socket.send(bytes(ready_tag,encoding='utf8'))
                feedback=self.Node_Socket.recv(1024)
                feedback=str(feedback,encoding='utf8')

                if feedback.startswith('Start'):
                    self.Node_Socket.send(send_data)
            except Exception:
                Global.Node_List.remove(self)
                break
        print("Exiting", self.Node_IP)


def print_time(Node_Name):
    print ("%s: %s" % (Node_Name, time.ctime(time.time())))