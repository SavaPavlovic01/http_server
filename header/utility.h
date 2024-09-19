#ifndef UTILITY_H
#define UTILITY_H
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#define BUFFER_SIZE 80

#define APPEND(head, cur, name_real, val_real) \
    if(!head){\
        head = malloc(sizeof(header_entry));\
        head->name = name_real;\
        head->value = val_real;\
        head->next = NULL;\
    }\
    if(cur){\
        cur->next = malloc(sizeof(header_entry));\
        cur = cur->next;\
    }\
    else{\
        cur = malloc(sizeof(header_entry));\
        head->next = cur;\
    }\
    cur->name = name_real;\
    cur->value = val_real;\
    cur->next = NULL;\

typedef enum read_status{
    READ_OK,
    LF_BEROFE_DELIMETER,
    DELIMETER_NOT_FOUND
} read_status;

char* read_until_char(const int client_socket, int* msg_len, const char delimeter, int* status){
    *msg_len = 0;
    char* msg = malloc(BUFFER_SIZE * sizeof(char));
    int cur_number_of_in = 0;
    int resizes = 1;
    *status = READ_OK;
    do{
        if(*msg_len == BUFFER_SIZE * resizes)
            msg = realloc(msg, (++resizes) * BUFFER_SIZE);
        
        cur_number_of_in = recv(client_socket, msg + *msg_len, sizeof(char), 0); 
        
        if(cur_number_of_in == 0){
            *status = DELIMETER_NOT_FOUND;
            break;
        } 

        *msg_len += cur_number_of_in;

        if(msg[*msg_len - 1] == '\n' && delimeter != '\n'){
            *status = LF_BEROFE_DELIMETER;
            break;
        }
        
        
    } while(msg[*msg_len - 1] != delimeter && msg[*msg_len - 1] != '\n');

    return realloc(msg, *msg_len);
}

// 0 true, else false
int check_if_end(char* src, int len){
    if(len != 2) return 1;
    return strncmp(src, "\r\n", 2);
}






#endif