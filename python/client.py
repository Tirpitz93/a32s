__author__ = 'Lonja'
import socket
import sys
import time

HOST, PORT = "localhost", 9999
data = " ".join(sys.argv[1:])

# Create a socket (SOCK_STREAM means a TCP socket)
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
try:
    sock.connect((HOST, PORT))
except:
    pass
    print("could not connect to socket")


while True:
    data = {}
    data["sysTime"] = time.strftime("%H:%M:%S")

    try:

        for key, value in data:
            sock.sendall((str(key) + "," + str(value)).encode("utf-8"))


        # Connect to server and send data

        #sock.sendall((data + "\n").encode('utf-8'))

        # Receive data from the server
        received = sock.recv(1024)

    except:
        print("could not send data")



print("Sent:     {}".format(data))
print("Received: {}".format(received))
