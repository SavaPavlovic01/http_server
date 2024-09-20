#include "../header/server.h"

response* hello(char* data){
    response* resp = malloc(sizeof(response));
    resp->code = CODE_200;
    resp->data = malloc(6);
    resp->data = "Hello\0";
    resp->data_len = 5;
    return resp;
}
//asd
int main(int argc, char const *argv[]){
    add_endpoint("/hello\0", &hello);
    go();
    return 0;
}
