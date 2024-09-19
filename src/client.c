#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>



int main(){
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = 80;
    struct in_addr serv_sin_addr;
    serv_sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    server_addr.sin_addr = serv_sin_addr;
    connect(tcp_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)); 
    const char to_send[15] = "GET myWebsite\r\n";
    int n = write(tcp_socket, to_send, sizeof(to_send));
    printf("n = %d", n);
    close(tcp_socket);
}