#ifndef _SERVER_H
#define _SERVER_H

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
#include "../header/response.h"

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
        printf("ERROR BINDING\n");
        exit(-1);
    }
    
    if(listen(tcp_socket, 20) != 0){
        printf("ERROR ON LISTEN\n");
        exit(-1);
    }

    return tcp_socket;
}
typedef struct endpoint endpoint;
struct endpoint{
    char* name;
    response* (*value)(char*);
    endpoint* next;
};

static endpoint* endpoint_head = NULL;
static endpoint* endpoint_end = NULL;

void add_endpoint(char* name, response* (*value)(char*)){
    APPEND(endpoint_head, endpoint_end, name, value, endpoint);
}

int execute_endpoint(const char* name, char* data, int client_socket){
    endpoint* cur = endpoint_head;
    int target_len = strlen(name);
    for(;cur;cur = cur->next){
        int len = strlen(cur->name);
        if(len != target_len) continue;
        if(strncmp(cur->name, name, len) == 0) break;
    }
    if(cur == NULL){
        send_response_s(client_socket, CODE_404);
        return -1;
    }

    response* resp = cur->value(data);
    send_response(client_socket, resp);
    if(resp) free(resp);
        
    
    
    
    return 0;
}

void run_server(int tcp_socket){
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
        int code = CODE_200;
        
        request_line rl;
        if(read_request_line(client_socket, &rl) == -1){
            code = CODE_404;
            send_response_s(client_socket, code);
            close(client_socket);
            continue;
        }
        printf("METHOD: %s\n", rl.method);
        printf("URI: %s\n", rl.uri);
        printf("VERSION: %s\n", rl.version);
        header_entry* head = read_header(client_socket);
        if(!head){
            code = CODE_404;
            send_response_s(client_socket, code);
            close(client_socket);
            continue;
        }
        test(head);
        int entity_len = get_content_len(head);
        char buf[entity_len + 1];
        read_entity_body(client_socket, buf, entity_len, head);
        execute_endpoint(rl.uri, buf, client_socket);
        close(client_socket);
    }

}

void go(){
    int tcp_socket = init_server();

    run_server(tcp_socket);

    close(tcp_socket);
}

#endif