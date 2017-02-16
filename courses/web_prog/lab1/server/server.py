import socket, os, json, re

class ConnectionAccumulator:

    def __init__(self, conn):
        self.conn = conn
        self.mess = ""

    def append_text(self, text, newline=True):
        self.mess += text + '\n' if newline else ''

    def send(self, addition=''):
        to_send = bytes(self.mess)
        if isinstance(addition, str):
            to_send = bytes("{0}{1}".format(self.mess, addition), "utf-8")
        elif isinstance(addition, bytes):
            to_send += addition

        self.conn.send(to_send)

    def send_and_print(self, text=''):
        fin_mess = "{0}{1}".format(self.mess, text)
        self.conn.send(bytearray(fin_mess, "utf-8"))
        print("SERVER:\n{0}\n_________________".format(fin_mess))

    def clear(self):
        self.mess = ""


commands_dict = {
    r"break conn": "to break the connection between server and client",
    r"get JSON": "to get the the server's JSON films file",
    r"get JSON sorted by (name|producer|duration|budget|date)": "to get the server's JSON films file sorted by some key",
    r"get JSON objs num": "to get number of the server's JSON films file objects",
    r"get clients info": "to get addresses of connected to the server clients"
}

commands_patterns = [
    re.compile(comm) for comm in commands_dict.keys()
]


def some_pattern_matches(string):
    matches = False
    for patt in commands_patterns:
        if re.match(patt, string):
            matches = True
    return matches


def run_server():
    f = open(os.path.join(os.getcwd(), "server\\films.json"), "r")
    JSON_content = json.load(f)
    f.close()

    host = 'localhost'
    port = 9000
    addr = (host, port)
    recv_buff_bytes = 10000

    sock = socket.socket()
    sock.bind(addr)
    sock.listen(50)
    print("Server is listening...")
    conn, addr = sock.accept()

    conn_accum = ConnectionAccumulator(conn)
    conn_accum.append_text("Hello, you connected to server with address '{0}:{1}'\n".format(host, port))
    conn_accum.append_text("Type commands:")

    i = 0
    for comm, descr in commands_dict.items():
        conn_accum.append_text("{0}) '{1}' {2};".format(chr(ord('a') + i), str(comm), descr))
        i += 1

    conn_accum.send_and_print()
    conn_accum.clear()

    while True:
        data = conn.recv(recv_buff_bytes)
        command = data.decode("utf-8")

        print("CLIENT:", command, sep='\n')
        print("_________________")

        if re.match(re.compile(r"^break conn$"), command):
            conn.close()
            break
        elif re.match(re.compile(r"^get JSON$"), command):
            conn_accum.send_and_print(json.dumps(JSON_content, separators=(',', ':')))
        elif re.match(re.compile(r"^get JSON sorted by (name|producer|duration|budget|date)$"), command):
            s = "get JSON sorted by "
            conn_accum.send_and_print(json.dumps(sorted(JSON_content, key=lambda d: d[command[command.index(s) + len(s):]]), separators=(',', ':')))
        elif re.match(re.compile(r"^get JSON objs num$"), command):
            conn_accum.send_and_print(str(len(JSON_content)))
        elif re.match(re.compile(r"^get clients info$"), command):
            pass
        else:
            conn_accum.send_and_print("Command is not correct")


    conn.close()

run_server()