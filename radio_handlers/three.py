import socket
import os
import time

# Server Process
server_path = "0.0.0.0"
# Client Process
client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client.connect((server_path, 12345))
# client = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
# client.connect("/tmp/p2p_chat2.sock")

# Both can now use conn.send/recv or client.send/recv


client.send(bytes("hello 3\n", encoding='utf-8'))

resp = client.recv(15)

print ("got resp: ", resp)

time.sleep(10000)