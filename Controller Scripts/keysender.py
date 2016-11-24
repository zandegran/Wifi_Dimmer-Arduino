class _Getch:
    """Gets a single character from standard input.  Does not echo to the
screen."""
    def __init__(self):
        try:
            self.impl = _GetchWindows()
        except ImportError:
            self.impl = _GetchUnix()

    def __call__(self): return self.impl()


class _GetchUnix:
    def __init__(self):
        import tty, sys

    def __call__(self):
        import sys, tty, termios
        fd = sys.stdin.fileno()
        old_settings = termios.tcgetattr(fd)
        try:
            tty.setraw(sys.stdin.fileno())
            ch = sys.stdin.read(1)
        finally:
            termios.tcsetattr(fd, termios.TCSADRAIN, old_settings)
        return ch


class _GetchWindows:
    def __init__(self):
        import msvcrt

    def __call__(self):
        import msvcrt
        return msvcrt.getch()


getch = _Getch()

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
     
    print '..######.##.....#....###...##....#.########.##......###.#######.########...########.###.##.....#.##.....#.#######.########.'
    print '.##....#.##.....#...##.##..###...#.##.....#.##.......##.##......##.....#...##.....#..##.###...##.###...##.##......##.....##'
    print '.##......##.....#..##...##.####..#.##.....#.##.......##.##......##.....#...##.....#..##.####.###.####.###.##......##.....##'
    print '.##......########.##.....#.##.##.#.##.....#.##.......##.######..########...##.....#..##.##.###.#.##.###.#.######..########.'
    print '.##......##.....#.########.##..###.##.....#.##.......##.##......##...##....##.....#..##.##.....#.##.....#.##......##...##..'
    print '.##....#.##.....#.##.....#.##...##.##.....#.##.......##.##......##....##...##.....#..##.##.....#.##.....#.##......##....##.'
    print '..######.##.....#.##.....#.##....#.########.#######.###.#######.##.....#...########.###.##.....#.##.....#.#######.##.....##'
    
    print '\nConnected to remote host'
    print 'n to on, f to turn off \n+ to increase Brightness\n- to decrease Brightness'
    while 1:
        socket_list = [sys.stdin, s]
        import msvcrt
        #msg = sys.stdin.readline()
        thing = msvcrt.getch()
        #msg = thing.getch()
        s.send(thing)
        s.send("\n")


