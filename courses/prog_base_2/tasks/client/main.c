#include <winsock2.h>
#include <windows.h>
#include <string.h>
#include <math.h>

#pragma comment(lib, "ws2_32.lib")

#define NO_FLAGS_SET 0
#define PORT 80
#define MAXBUFLEN 20480

int getAvarage(char * arrStr){
    int num = 0;
    float avrg = 0.0f;
    for(char * tok = strtok(arrStr, " "); tok != NULL; tok = strtok(NULL, " ")){
        avrg += atoi(tok);
        num++;
    }
    if(num != 0)
        avrg /= num;
    int roundedAvrg = roundf(avrg);
    return roundedAvrg;
}

int main(void) {
    WSADATA Data;
    SOCKADDR_IN recvSockAddr;
    SOCKET recvSocket;
    int status;
    int numrcv = 0;
    struct hostent * remoteHost;
    char * ip;
    const char * host_name = "pb-homework.appspot.com";
    char buffer[MAXBUFLEN];

    memset(buffer,0,MAXBUFLEN);

    status = WSAStartup(MAKEWORD(2, 2), &Data);
    if(status != 0)
    {
        printf("ERROR: WSAStartup unsuccessful\r\n");
        return 1;
    }

	remoteHost = gethostbyname(host_name);
	ip = inet_ntoa(*(struct in_addr *)*remoteHost->h_addr_list);
    memset(&recvSockAddr, 0, sizeof(recvSockAddr)); // zero the sockaddr_in structure

    recvSockAddr.sin_port=htons(PORT); // specify the port portion of the address
    recvSockAddr.sin_family=AF_INET; // specify the address family as Internet
    recvSockAddr.sin_addr.s_addr= inet_addr(ip); // specify ip address

	recvSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(recvSocket == INVALID_SOCKET)
	{
		printf("ERROR: socket unsuccessful\r\n");
		return 1;
	}

    if(connect(recvSocket,(SOCKADDR*)&recvSockAddr,sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
    {
        printf("ERROR: socket could not connect\r\n");
        closesocket(recvSocket);
        WSACleanup();
        return 1;
    }

    char request[200];
    sprintf(request, "GET /var/15 HTTP/1.1\r\nHost:%s\r\n\r\n", host_name);  // add Host header with host_name value
    send(recvSocket, request, strlen(request), 0);

    numrcv = recv(recvSocket, buffer, MAXBUFLEN, NO_FLAGS_SET);
    if (numrcv == SOCKET_ERROR)
    {
        printf("ERROR: recvfrom unsuccessful\r\n");
        status = closesocket(recvSocket);
        if(status == SOCKET_ERROR)
			printf("ERROR: closesocket unsuccessful\r\n");
        status = WSACleanup();
        if (status == SOCKET_ERROR)
			printf("ERROR: WSACleanup unsuccessful\r\n");
        return 1;
    }
    buffer[numrcv] = '\0';

    strcpy(buffer, strstr(buffer, "secret="));

    sprintf(request, "GET /var/15?%s HTTP/1.1\r\nHost:%s\r\n\r\n", buffer, host_name);
    send(recvSocket, request, strlen(request), 0);

    numrcv = recv(recvSocket, buffer, MAXBUFLEN, NO_FLAGS_SET);
    if (numrcv == SOCKET_ERROR)
    {
        printf("ERROR: recvfrom unsuccessful\r\n");
        status = closesocket(recvSocket);
        if(status == SOCKET_ERROR)
			printf("ERROR: closesocket unsuccessful\r\n");
        status = WSACleanup();
        if (status == SOCKET_ERROR)
			printf("ERROR: WSACleanup unsuccessful\r\n");
        return(1);
    }
    buffer[numrcv] = '\0';

    strcpy(buffer, strstr(buffer, "\r\n\r\n") + 4);

    char result[100];
    sprintf(result, "result=%d", getAvarage(buffer));

    sprintf(request, "POST /var/15 HTTP/1.1\r\nHost:%s\r\nContent-Length: %d\r\n\r\n%s", host_name, strlen(result), result);
    send(recvSocket, request, strlen(request), 0);

    numrcv = recv(recvSocket, buffer, MAXBUFLEN, NO_FLAGS_SET);
    if (numrcv == SOCKET_ERROR)
    {
        printf("ERROR: recvfrom unsuccessful\r\n");
        status = closesocket(recvSocket);
        if(status == SOCKET_ERROR)
			printf("ERROR: closesocket unsuccessful\r\n");
        status = WSACleanup();
        if (status == SOCKET_ERROR)
			printf("ERROR: WSACleanup unsuccessful\r\n");
        system("pause");
        return 1;
    }
    buffer[numrcv] = '\0';

    printf(buffer);

    status = closesocket(recvSocket);
    if(status == SOCKET_ERROR)
        printf("ERROR: closesocket unsuccessful\r\n");
    status = WSACleanup();
    if (status == SOCKET_ERROR)
        printf("ERROR: WSACleanup unsuccessful\r\n");
    return 0;
}
