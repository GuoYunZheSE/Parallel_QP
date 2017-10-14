import socket
ip_port=('127.0.0.1',9999)
s=socket.socket()

s.connect(ip_port)
welcom_msg=s.recv(200).decode()

print(welcom_msg)
while True:
    send_data=input(">>: ").strip()#strip() 方法用于移除字符串头尾指定的字符(默认为空格)
    if send_data=='exit':
        break
    if len(send_data)==0:
        continue
    s.send(bytes(send_data,encoding='utf8'))
    read_tag=s.recv(1024)
    read_tag=str(read_tag,encoding='utf8')
    if read_tag.startswith('Ready'):
        msg_size=int(read_tag.split('|')[-1])
    start_tag='Start'
    s.send(bytes(start_tag,encoding='utf8'))

    recv_size=0
    recv_msg=b''
    while recv_size<msg_size:
        recv_data=s.recv(1024)
        recv_msg+=recv_data
        recv_size+=len(recv_data)
        print('MSG SIZE %s RECE SIZE %s'%(msg_size,recv_size))

    print(str(recv_msg,encoding='utf8'))

s.close()