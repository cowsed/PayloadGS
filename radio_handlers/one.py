import socket
import os

# Server Process
server_path = "/tmp/p2p_chat.sock"
if os.path.exists(server_path): os.remove(server_path)

server = socket.socket(socket.AF_UNIX, socket.SOCK_STREAM)
server.bind(server_path)
# server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# server.bind(("0.0.0.0", 44444))


while True:
    try:
        server.listen(0)
        conn, _ = server.accept() # Peer 2 connects here


        while True:
            msg = conn.recv(15)
            if len(msg) == 0:
                print("empty recv, continuing")
                conn.send(bytes("TEST\n", 'utf-8'))
                continue
            print("Got message: ", msg.decode('utf-8'))
            conn.send(bytes("echo "+msg.decode(), encoding='utf-8'))
    except Exception as e:
        print("Sock error, relistening", e)
