__author__ = 'Lonja'
#todo, impliment heartbeat
#todo impliment system properties when not in game
import time
#print('hello world')

app = ""
import socket
import socketserver
import serial
import time
import math
#
# data_Names = {"altASL": "ALT:", "TOT": "TOT:", "tgt_alt": "TGT:", "rel_tgt_alt": "REL:", "tfarFreqLr":"FR:"}
# data = {
#     "altASL":800,
#     "TOT": 60,
#     "tgt_alt": 254,
#
# }
#
# current_LCD_Data = ["alt_ASL", "tgt_alt"]
#
# packet_Template = "\x01%s\x02%s\x04"
# lcd = []
# try:
#     ser = serial.Serial('COM5', 500000, timeout=0)
#
# except:
#     pass
#
# testMsg = "\x01\x00\x02\x20\x20\x20\x30\x31\x32\x32\x34\x35\x04".encode("utf-8")
# #print(testMsg)
# #content2 = data_Names["tfarFreqsw"] + str(data["tfarFreqsw"]).rjust(4)
# #content2 = content2.ljust(8)
# #content2 += data_Names["rel_tgt_alt"] + str(data["alt_ASL"] - data["tgt_alt"]).rjust(4)
# #content2 = content2.ljust(16)
# #packet2 = packet_Template % ("\x01", content2)
# #print(content2.encode())
# #print( str(data["alt_ASL"] - data["tgt_alt"]))
#
#
#
# class MyTCPHandler(socketserver.BaseRequestHandler):
#     """
#     The RequestHandler class for our server.
#
#     It is instantiated once per connection to the server, and must
#     override the handle() method to implement communication to the
#     client.
#     """
#
#
#
#     def sendData(self):
#         try:
#
#             #print(ser.readline().decode("utf-8"))
#             content = data_Names["TOT"] + str(data["TOT"]).rjust(4)
#             content = content.ljust(8)
#             content += data_Names["altASL"] + str(int(math.floor(float(data["altASL"])))).rjust(4)
#             content = content.ljust(16)
#
#             #content2 = data_Names["tfarFreqLr"] + str(data["tfarFreqLr"]).rjust(4)w
#             #content2 = content2.ljust(8)
#             # content2 += data_Names["rel_tgt_alt"] + str(int(data["alt_ASL"]) - int(data["tgt_alt"])).rjust(4)
#             #content2 = content2.ljust(16)
#             #packet2 = packet_Template % ("\x01", content2)
#             #print(packet2.encode())
#             packet = packet_Template % ("\x00", content)
#             print(packet.encode())
#             if ser:
#                 ser.write(packet.encode("utf-8")) #test message
#             #ser.write(packet2.encode("utf-8")) #test message
#
#         except:
#             print('Data could not be read')
#
#
#
#
#     def handle(self):
#         # self.request is the TCP socket connected to the client
#         self.incoming = self.request.recv(1024).strip()
#         self.request.sendall("Python has heard\n\0".rjust(1024).encode("utf-8"))
#         print("{} wrote:".format(self.client_address))
#
# #        do stuff with data
#         print(self.incoming)
#         self.finish()
#         #category, content = self.incoming.split(",", 1)
#
#         #data[category] = content
#         #print()
#         #print(data["altASL"])
#
#         #self.sendData()
#
# #       just send back the same data, but upper-cased
#
# #       send return data back
#         #self.request.sendall("Python has heard\n\0".encode("utf-8"))
#
#
# if __name__ == "__main__":
#     HOST, PORT = "localhost", 9999
#
#     # Create the server, binding to localhost on port 9999
#     server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)
#
#     # Activate the server; this will keep running until you
#     # interrupt the program with Ctrl-C
#     print('server running')
#     server.serve_forever()


#!/usr/bin/env python

"""
A simple echo server
"""

import socket

host = ''
port = 9999
backlog = 5
size = 1024
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((host,port))
s.listen(backlog)
client, address = s.accept()
while 1:
    try:
        data = ""
        #client, address = s.accept()
        data = client.recv(size)
        if data:
            print("data recieved: ")
            #time.sleep(3)
            client.send((("python heard: " +data.decode()+"\0")).encode("utf-8"))
            print (data.decode().encode("utf-8"))
            print(address)
            print(data)
        #client.close()
    except:
        pass