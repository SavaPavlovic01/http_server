#ifndef _HEAD_H
#define _HEAD_H
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>
#include "utility.h"

typedef enum read_header_name_status{
    OK,
    DONE_READING,
    ERROR
} read_header_name_status;

typedef struct header_entry header_entry;
struct header_entry{
    char* name;
    char* value;
    header_entry* next;
} ;

char* read_header_name(const int client_socket, int* status){
    int len;
    char* header_name = read_until_char(client_socket, &len, ':', status);
    if(*status == READ_OK) {
        header_name[len - 1] = '\0';
        return header_name;
    }
    if(*status == LF_BEROFE_DELIMETER){
        if(check_if_end(header_name, len) == 0){
            *status = DONE_READING;
        } else{
            *status = ERROR;
        }

        return NULL;
    }

    if(*status == DELIMETER_NOT_FOUND){
        *status = ERROR;
        return NULL;
    }
}

char * read_header_value(const int client_socket, int* status){
    int len;
    char* header_val = read_until_char(client_socket, &len, '\n', status);
    if(*status == READ_OK) {
        header_val[len - 1] = '\0';
        return header_val;
    }
    if((*status = DELIMETER_NOT_FOUND) || (*status = LF_BEROFE_DELIMETER)){
        *status = ERROR;
        return NULL;
    }

}

header_entry* read_header(const int client_socket){
    int status;
    header_entry* header_list_head = NULL;
    header_entry* cur_list_elem = NULL;
    int len;

    while(1){
        char* cur_header = read_header_name(client_socket, &status);
        
        if(status == ERROR){
            printf("MALFORMED HTTP REQUEST\n");
            return NULL;
        } else if(status == DONE_READING){
            return header_list_head;
        } else{
            char* header_val = read_header_value(client_socket, &status);
            if(status == ERROR){
                printf("MALFORMED HTTP REQUEST\n");
                return NULL;
            } else{
                APPEND(header_list_head, cur_list_elem, cur_header, header_val);
            }
        }


    }
}

int get_content_len(header_entry* const head){
    header_entry* cur = head;
    char target[] = "Content-Length\0";
    int target_len = 14;
    for(;cur;cur = cur->next){
        int cur_len = strlen(cur->name);
        if(cur_len != target_len) continue;
        if(strncmp(target, cur->name, target_len) == 0){
            return atoi(cur->value);
        }
    }

    return -1;
}

void test(header_entry* head){
    while(head){
        printf("%s:%s\n", head->name, head->value);
        head = head->next;
    }
}


#endif