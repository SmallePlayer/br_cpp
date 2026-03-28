import socket
import struct

HOST = '127.0.0.1'
PORT = 8080
with socket.create_connection((HOST, PORT)) as s:
    print(f'Connected to server at {HOST}:{PORT}')
    value = 5
    s.sendall(struct.pack('!I', value))
    resp = s.recv(1024)
    print(f'Received response: {resp.decode()}')
