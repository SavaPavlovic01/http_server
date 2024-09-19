#ifndef _ENTITY_H
#define _ENTITY_H
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

int read_entity_body(const int client_socket, char* buffer, int len, header_entry* const head){
    if(len == -1) return -1;
    int read_cnt = read(client_socket, buffer, len);
    buffer[read_cnt] = '\0';
    return 0;
}

#endif