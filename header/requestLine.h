#ifndef _REQUEST_LINE_H
#define _REQUEST_LINE_H
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "utility.h"

typedef struct request_line{
    char* method;
    char* uri;
    char* version;
    int method_len;
    int uri_len;
    int verison_len;
}  request_line;

int read_request_line(const int client_socket, request_line* dest){
    int status;
    dest->method = read_until_char(client_socket, &dest->method_len, ' ', &status);
    dest->method[dest->method_len - 1] = '\0';
    dest->uri = read_until_char(client_socket, &dest->uri_len, ' ', &status);
    dest->uri[dest->uri_len - 1] = '\0';
    dest->version = read_until_char(client_socket, &dest->verison_len, '\n', &status);
    dest->version[dest->verison_len - 1] = '\0';
    return 0;     
}

#endif