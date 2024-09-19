#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "../header/utility.h"
#include "../header/head.h"
#include "../header/requestLine.h"
#include "../header/entity.h"

int init_server(){
    const int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = 80;
    struct in_addr socket_addr;
    socket_addr.s_addr = htonl(INADDR_LOOPBACK);
    my_addr.sin_addr = socket_addr;
    if(bind(tcp_socket, (struct sockaddr *)&my_addr, sizeof(my_addr)) != 0){
        printf("ERROR BINDING");
        exit(-1);
    }
    
    if(listen(tcp_socket, 20) != 0){
        printf("ERROR ON LISTEN");
        exit(-1);
    }

    return tcp_socket;
}


int main(int argc, char const *argv[]){
    int tcp_socket = init_server();

    for(;;){
        struct sockaddr_in client;

        int len = sizeof(client);
        const int client_socket = accept(tcp_socket, (struct sockaddr *)&client, &len);
        
        if(client_socket == -1){
            printf("ACCEPT FAILED");
            exit(-1);
        }
            
        printf("CONNECTED %d\n", client_socket);
        
        int msg_len;
        char *msg;
        char resp[] = "HTTP/1.1 200 OK\r\n"
                      "Content-Length:5\n"
                      "\r\n"
                      "Hello";
        while(1){
            request_line rl;
            read_request_line(client_socket, &rl);
            printf("METHOD: %s\n", rl.method);
            printf("URI: %s\n", rl.uri);
            printf("VERSION: %s\n", rl.version);
            header_entry* head = read_header(client_socket);
            test(head);
            int entity_len = get_content_len(head);
            char buf[entity_len + 1];
            read_entity_body(client_socket, buf, entity_len, head);
            printf("ENTITY:%s\n", buf);
            break;
        }
        
        
        write(client_socket, resp, sizeof(resp));
        close(client_socket);
    }
    
    close(tcp_socket);
    return 0;
}
