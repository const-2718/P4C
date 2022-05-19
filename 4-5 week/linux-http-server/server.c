#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define LISTEN_QUEUE_MAX 16

enum METHOD{GET, POST, DELETE, METHOD_COUNT};

char *recieve_request(int);
char *make_response(char *);

int main(int argc, char* argv[])
{
    if(argc != 2) {
        printf("%s [port]", argv[0]);
        return 1;
    }
    int server_port = atoi(argv[1]);

    int server_sock, client_sock;

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(server_sock == -1) {
        printf("sock() failed\n");
        return 1;
    }

    struct sockaddr_in server_sock_addr, client_sock_addr;
    socklen_t sock_addr_len = sizeof(server_sock_addr);

    memset(&server_sock_addr, '0', sizeof(server_sock_addr));
    server_sock_addr.sin_family = AF_INET;
    server_sock_addr.sin_port = htons(server_port);
    server_sock_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(server_sock, (struct sockaddr *)&server_sock_addr, sock_addr_len) == -1) {
        printf("bind() failed\n");
        return 1;
    }

    while(1)
    {
        if(listen(server_sock, LISTEN_QUEUE_MAX) == -1) {
            printf("listen() failed\n");
            return 1;
        }

        client_sock = accept(server_sock, (struct sockaddr *)&client_sock_addr, &sock_addr_len);
        if(client_sock == -1) {
            printf("accept() failed\n");
            return 1;
        }

        //recv
        char request[1024] = {};
        int recv_length = recv(client_sock, request, sizeof(request), 0);
        if(recv_length == -1) {
            printf("recv() failed\n");
            return 1;
        }

        printf("request:\n%s\n", request);


        //send
        
        char* response = make_response(request);

        printf("response: \n%s\n", response);

        if(response != NULL) {
            send(client_sock, response, (int)strlen(response), 0);
        }

        
        close(client_sock);
        //free(request);
    }

    close(server_sock);
    return 0;
}

int check_version(char *version)
{
    if(strcmp(version, "HTTP/1.1")) {
        return -1;
    }
    return 0;
}

int check_path(char* path)
{
    if(strcmp(path, "/") == 0) {
        strcpy(path, "/redirection.html");
    }

    FILE *fp = fopen(path + 1, "r");

    while(*path != '.') {
        path++;
    }
    
    if(fp == NULL && strcmp(path, ".html") == 0) {
        return -2;
    } else if(fp == NULL) {
        return -1;
    } else {   
        fseek(fp, 0, SEEK_END);
        int file_length = ftell(fp);
        return file_length;
    }
}

enum METHOD get_method_enum(char* method)
{
    enum METHOD met;
    if(strcmp(method, "GET") == 0) {
        return GET;
    } else if(strcmp(method, "POST") == 0) {
        return POST;
    } else if(strcmp(method, "DELETE") == 0) {
        return DELETE;
    } else {
        return -1;
    }
}

char* get_string_enum(int method_enum)
{
    switch(method_enum) {
    case POST:
        return "POST";
    case GET:
        return "GET";
    case DELETE:
        return "DELETE";
    }
}

void set_http_status(char** response, int status_code)
{
    if(*response != NULL) {
        free(*response);
    }
    *response = malloc(sizeof(char) * 1024);

    switch(status_code) {
    case 200:
        strcpy(*response, "HTTP/1.1 200 OK\r\n");
        break;
    case 308:
        strcpy(*response, "HTTP/1.1 308 Moved Redirect\r\n");
        break;
    case 404:
        strcpy(*response, "HTTP/1.1 404 Not Found\r\n");
        break;
    case 405:
        strcpy(*response, "HTTP/1.1 405 Method Not Allowed\r\n");
        break;
    default:
        break;
    }
    
}

char* load_html_file(char* path)
{
    FILE* fp = fopen(path, "r");
    fseek(fp, 0, SEEK_END);
    int file_length = ftell(fp);
    
    fseek(fp, 0, SEEK_SET);

    char* buffer = malloc(sizeof(char) * 1024);
    memset(buffer, 0, sizeof(buffer));

    int c = 0;
    int i = 0;
    while((c = fgetc(fp)) != EOF) {
        buffer[i++] = c;
    }
    
    fclose(fp);

    return buffer;
}

int check_allowed_mathod(int method_enum) {
    switch(method_enum) {
        case GET:
        case POST:
            return 1;
        case DELETE:
            return 0;
        default:
            return 0;
    }
}

void set_http_header(char** response, int status_code, int content_length)
{
    switch(status_code) {
    case 200:
    case 308:
        strcat(*response, "Location: /index.html\r\n");
    case 404:
        strcat(*response, "Content-Type: text/html; charset=utf-8\r\n");
        strcat(*response, "Content-Length: ");
        
        char tmp[10]; 
        sprintf(tmp,"%d", content_length);
        strcat(*response, tmp);

        break;
    case 405:
        
        strcat(*response, "Allow: ");
        for(int i = GET; i < METHOD_COUNT - 1 ; i++) {
            if(!check_allowed_mathod(i)) {
                continue;
            }
            char* method = get_string_enum(i);
            strcat(*response, method);
            strcat(*response, ", ");
        }
        strcat(*response, get_string_enum(METHOD_COUNT - 1));

        break;
    default:
        break;
    }
    
    strcat(*response, "\r\n\r\n\r\n");
}

char *make_response(char *request)
{
    char *response = NULL;
    
    char *method, *path, *version;

    method = strtok(request,  " \t\r\n");
    path = strtok(NULL, " \t");
    version = strtok(NULL, " \t\r\n"); 

    printf("method: %s\n", method);
    printf("path   : %s\n", path);
    printf("version  : %s\n", version);

    if(check_version(version) == -1) {
        printf("version error: %s is not supported\n", version);
        return NULL;
    }

    int content_length = check_path(path);
    if(content_length == -1) {
        printf("the file format is not supported\n");
        return NULL;
    } else if(content_length == -2) {
        printf("worong path: %s\n", path);
        strcpy(path, "/notfound.html");
    }

    switch (get_method_enum(method)) {
    case GET:
        if(strcmp(path, "/notfound.html") == 0) {
            set_http_status(&response, 404);
            set_http_header(&response, 404, content_length);
        } else if(strcmp(path, "/redirection.html") == 0) {
            set_http_status(&response, 308);
            set_http_header(&response, 308, content_length);
        }
        else {
            set_http_status(&response, 200);
            set_http_header(&response, 200, content_length);
        }

        printf("path: %s\n", path);
        char* html = load_html_file(path + 1);
        printf("load file path sucess!!\n");
        strcat(response, html);
        free(html);

        printf("response sucess!\n");
        break;
    case POST:
        
        break;
    case DELETE:
        set_http_status(&response, 405);
        set_http_header(&response, 405, content_length);
        break;
    default:
        printf("unkown method: %s\n", method);
        break;
    }

    return response;
}