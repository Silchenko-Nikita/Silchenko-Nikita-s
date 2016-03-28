#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

int main(int argc , char *argv[])
{
    WSADATA wsa;
    SOCKET s;
    struct sockaddr_in server;
    char *message , server_reply[2000], gotStr[200];
    int recv_size;

    printf("\nInitialising Winsock...");
    if (WSAStartup(MAKEWORD(2,2),&wsa) != 0)
    {
        printf("Failed. Error Code : %d",WSAGetLastError());
        return 1;
    }
    printf("Initialised.\n");

    //Create a socket
    if((s = socket(AF_INET , SOCK_STREAM , 0 )) == INVALID_SOCKET)
    {
        printf("Could not create socket : %d" , WSAGetLastError());
    }

    printf("Socket created.\n");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons( 3333 );

    //Connect to remote server
    int errorCode;
    if ((errorCode = connect(s, (struct sockaddr *)&server , sizeof(server))) < 0)
    {
        printf("Error code: %d", errorCode);
        return 1;
    }
    puts("Connected");

    while(1){
    //Receive a reply from the server
        if((recv_size = recv(s , server_reply , 2000 , 0)) == SOCKET_ERROR)
        {
            puts("recv failed");
        }
        puts("\nReply received");

        //Add a NULL terminating character to make it a proper string before printing
        server_reply[recv_size] = '\0';
        puts(server_reply);

        gets(gotStr);
        send(s, gotStr, strlen(gotStr), 0);
    }

    closesocket(s);
    WSACleanup();

    return 0;
}
