from socket import *
serverName = "localhost"
serverPort = 12000
clientSocket = socket(AF_INET, SOCK_STREAM)
clientSocket.connect((serverName, serverPort))
sentence = input("Input: \n")
f = clientSocket.makefile('wr')
f.write(sentence)
buffer = f.readlines()
for line in buffer:
    print(line)
#clientSocket.send(sentence.encode())
clientSocket.close()
