import socket


def run_client():
    recv_buff_bytes = 10000

    while True:
        inp = input("Please, enter the server's addres in format 'host:port': ")
        addr_list = inp.split(sep=':')
        addr = (addr_list[0], int(addr_list[1]))

        sock = socket.socket()
        try:
            sock.connect(addr)
        except:
            continue
        break

    while True:
        data = sock.recv(recv_buff_bytes)
        print("SERVER:")
        print(data.decode("utf-8"))
        print("_________________")
        inp = input("Enter your command:\n")
        print("_________________")
        sock.send(bytes(inp, "utf-8"))

    sock.close()

run_client()