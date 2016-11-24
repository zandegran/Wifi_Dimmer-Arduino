# telnet program example
import socket, select, string, sys
 
#main function
if __name__ == "__main__":
     
    host = "10.0.1.75"
    port = 80
     
    s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    s.settimeout(2)
     
    # connect to remote host
    try :
        s.connect((host, port))
    except :
        print 'Unable to connect'
        sys.exit()
     
    print 'Connected to remote host'
     
   
    socket_list = [sys.stdin, s]
    s.send("b\n")
    sys.exit()
