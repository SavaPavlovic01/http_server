#ifndef _RESPONSE_H
#define _RESPONSE_H
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "utility.h"
#include "requestLine.h"
#include "head.h"

typedef enum code_pos{
    CODE_200,
    CODE_404
} code_pos;

typedef struct response{
    int code;
    char* data;
    int data_len;
} response;

const char * status_msg[] = {"OK\0", "NOT FOUND\0"};
const char * status[] = {"200 ", "404 "};
//  request_line* req_data, header_entry* head
int send_response(const int client_socket, response* resp){
    const char response_line[] = "HTTP/1.1 ";
    write(client_socket, response_line, 9);
    write(client_socket, status[resp->code], 4);
    int len = strlen(status_msg[resp->code]);
    write(client_socket, status_msg[resp->code], len);
    write(client_socket, " \r\n", 3);
    //end of request line
    
    if(resp->data != NULL){
        write(client_socket, "Content-Length:", 15);
        int digit_cnt = numPlaces(resp->data_len);
        char code_string[digit_cnt];
        sprintf(code_string, "%d", resp->data_len);
        write(client_socket, code_string, digit_cnt);
        write(client_socket, "\n", 1);
        write(client_socket, "\r\n", 2);
        write(client_socket, resp->data, resp->data_len);
    }
    
    return 0;
}

int send_response_s(const int client_socket, int code){
    response* resp = malloc(sizeof(response));
    resp->code = code;
    resp->data = NULL;
    send_response(client_socket, resp);
    free(resp);
}

#endif