#include <stdio.h>
#include <winsock2.h>

char* make_response(char*, char*);

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("%s [port]", argv[0]);
        return 1;
    }

    WSADATA wsaData;

    SOCKET server_sock, client_sock;

    struct sockaddr_in server_sock_addr, client_sock_addr;

    if(WSAStartup(MAKEWORD(2, 0), &wsaData) != 0)
    {
        return 1;
    }

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1) {
        printf("socket() failed\n");
        return 1;
    }
    printf("socket() sucess\n");

    int port = atoi(argv[1]);
    
    memset(&server_sock_addr, '0', sizeof(server_sock_addr));
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(port);
    server_sock_addr.sin_addr.S_un.S_addr = INADDR_ANY;

    //set socket option

    //BOOL valid = 1;
    //setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, (const char*)&valid, sizeof(valid));


    if(bind(server_sock, (struct sockaddr *)&server_sock_addr, sizeof(server_sock_addr)) != 0)
    {
        printf("bind() failed\n");
        return 1;
    }
    printf("bind() sucess\n");

    //listen
    if(listen(server_sock, 5) != 0)
    {
        printf("listen() failed\n");
        return 1;
    }

    while(1)
    {
        int sock_addr_length = sizeof(client_sock_addr);
        client_sock = accept(server_sock, (struct sockaddr*)&client_sock_addr, &sock_addr_length);

        if(client_sock == -1) {
            printf("accept() failed\n");
            return -1;
        }

        //char* request_buffer;
        char request_buffer[1024] = {};
        int recv_length = recv(client_sock, request_buffer, sizeof(request_buffer), 0);
        if(recv_length == -1) {
            printf("recv() failed\n");
            return -1;
        }

        printf("recv_length: %d\n", recv_length);
        printf("%s", request_buffer);

        request_buffer[recv_length] = '\0';


        // 클라이언트에 테스트용 HTTP 메세지 송신

        //char* response_buffer;

        char *method = NULL, *path = NULL, *version = NULL;
        
        method = strtok(request_buffer,  " \t\r\n");
        path    = strtok(NULL, " \t");
        version   = strtok(NULL, " \t\r\n"); 

        printf("method: %s\n", method);
        printf("path   : %s\n", path);
        printf("version  : %s\n", version);


        char* response = make_response(method, path);
        printf("response : %s\n", response);
        if(response != NULL) {
            send(client_sock, response, (int)strlen(response), 0);
        }

        closesocket(client_sock);
    }

}

char* parse_uri(char* uri)
{
    printf("uri: %s\n", uri + 1);
    return uri + 1;
}

char* make_response(char* method, char* uri)
{
    if(strcmp(method, "GET") == 0) {

    } else if(strcmp(method, "POST") == 0) {

    }
    char *response_buffer = malloc(sizeof(char) * 1024);
    strcpy(response_buffer, "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=utf-8\r\nContent-Length: ");
    
    const char* path = parse_uri(uri);

    FILE* fp = fopen(path, "r");

    if(fp == NULL) {
        printf("wrong uri\n");
        return NULL;
    }

    char file_buffer[1024];

    char ch;
    int i = 0;
    while(EOF != (ch = fgetc(fp)))
    {
        file_buffer[i++] = ch;
    }

    printf("html length: %d\n", strlen(file_buffer));

    char s[5]; 
    sprintf(s,"%d", strlen(file_buffer));
    strcat(response_buffer, s);
    strcat(response_buffer, "\r\n\r\n");
    strcat(response_buffer, file_buffer);


    fclose(fp);
    return response_buffer;
}
