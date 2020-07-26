from socket import *

http_msg = "HTTP/1.1 200 OK\r\n\
Date: Mon, 27 Jul 2009 12:28:53 GMT\r\n\
Server: Apache/2.2.14 (Win32)\r\n\
Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT \r\n\
Content-Length: 220\r\n\
Content-Type: text/html\r\n\
Connection: Closed\r\n\r\n\
<!doctype html>\r\n\
<html>\r\n\
    <head>\r\n\
        <title>Python is awesome!</title>\r\n\
    </head>\r\n\
    <body>\r\n\
        <h2>Afternerd</h2>\r\n\
        <p>Congratulations! The HTTP Server is working!</p>\r\n\
    </body>\r\n\
</html>\r\n"
serverPort = 12000
serverSocket = socket(AF_INET, SOCK_STREAM)
serverSocket.bind(("", serverPort))
serverSocket.listen(1)
print("The server is ready to receive")
while True:
    connectionSocket, addr = serverSocket.accept()
    print("after accepting connection")
    sentence = connectionSocket.recv(1024)
    print(sentence)
    connectionSocket.send(http_msg)
    connectionSocket.close()