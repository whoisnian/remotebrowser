#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char SERVER_ADDR[] = "10.0.2.2:4543";
//char LOCAL_BROWSER[] = "C:\Program Files\Mozilla Firefox\firefox.exe";
char LOCAL_BROWSER[] = "start microsoft-edge:";

int main(int argc, char *argv[])
{
    char command[4096] = "curl \"";
    strcat(command, SERVER_ADDR);
    strcat(command, "/?url=");
    if(argc > 1)
        strcat(command, argv[1]);
    else
        strcat(command, "about:blank");
    strcat(command, "\"");

    int res = system(command);
    if(res != 0)
    {
        system("color 04");
        printf("Open remotely failed.\n");
        printf("Press Enter to open locally.\n");
        getchar();
        strcpy(command, LOCAL_BROWSER);
        if(argc > 1)
        {
            strcat(command, "\"");
            strcat(command, argv[1]);
            strcat(command, "\"");
        }
        system(command);
    }
    return 0;
}

