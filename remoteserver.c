/*************************************************************************
    > File Name: remoteserver.c
    > Author: nian
    > Blog: https://whoisnian.com
    > Mail: zhuchangbao1998@gmail.com
    > Created Time: 2019年03月02日 星期六 15时00分51秒
 ************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<sys/types.h> 
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>

const unsigned long port = 4543;
char BROWSER[] = "/usr/bin/firefox";

int main(void)
{
    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 10);
    while(1)
    {
        int client_socket;
        struct sockaddr_in client_addr;
        socklen_t client_addr_len;
        client_addr_len = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_addr_len);

        char request[4096];
        read(client_socket, request, sizeof(request));

        time_t Time;
        char time_of_now[50];
        time(&Time);
        strftime(time_of_now, 50, "%a, %d %b %Y %H:%I:%S %Z", gmtime(&Time));
        
        char resource[4096];
        sscanf(request, "%*s%s", resource);
        if(strstr(resource, "/?url=") == resource)
        {
            char response[256];
            memset(response, 0, sizeof(response));
            sprintf(response, "HTTP/1.1 200 OK\nDate: %s\nServer: C++ Server\n\n", time_of_now);
            strcat(response, "Open browser Successfully.");
            write(client_socket, response, strlen(response));
            close(client_socket);

            char *address = &resource[6];
            printf("%s: %s\n", time_of_now, address);

            char command[4096];
            strcpy(command, BROWSER);
            strcat(command, " \"");
            strcat(command, address);
            strcat(command, "\"");
            system(command);
            system("xdotool search --desktop 0 --name firefox windowactivate");
        }
        else
        {
            char response[128];
            memset(response, 0, sizeof(response));
            sprintf(response, "HTTP/1.1 404 Not Found\nDate: %s\nServer: C++ Server\n\n", time_of_now);
            write(client_socket, response, strlen(response));
            close(client_socket);
        }
    }
    close(server_socket);
    return 0;
}

