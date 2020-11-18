import socket
from datetime import datetime

UDP_HOST = '0.0.0.0'
UDP_PORT = 5005
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_HOST, UDP_PORT))

# client_socket, client_address = sock.accept()
while True:
    data = sock.recv(1024)
    #if not data:
    #    break
    # strdata = data.decode("utf-8").strip("\x00").split()
    # strdata.append(datetime.now().strftime("%d/%m/%Y %H:%M:%S"))
    # print(strdata)
    print(data)

sock.close()
