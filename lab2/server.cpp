#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

void rpr(int sign)
{
    int stat;
    while(wait3(&stat, WNOHANG, (struct rusage *)0) >= 0);
}

int main (int argc, char **argv)
{
    int sockMain, sockBind, check, sockClient, stat, childs;
    struct sockaddr_in serv;
    sockMain = socket(AF_INET, SOCK_STREAM, 0);
    if (sockMain < 0)
    {
        std::cerr << "SOCKET ERROR" << '\n';
        return 1;
    }
    childs = 0;
    socklen_t size = sizeof(struct sockaddr);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = 0;
    signal(SIGCHLD, rpr);
    sockBind = bind(sockMain, (struct sockaddr *) &serv, sizeof(serv));
    if (sockBind == -1)
    {
        std::cerr << "BIND ERROR" << '\n';
        return 1;
    }
    if (getsockname(sockMain, (struct sockaddr *) &serv, &size))
    {
        std::cerr << "GET SOCKET NAME ERROR" << '\n';
        return 1;
    }
    std::cout << "PORT: " << ntohs(serv.sin_port) << '\n';
    while (1)
    {
        if ((check = listen(sockMain, 5)) == -1)
        {
            std::cerr << "LISTEN ERROR" << '\n';
            return 1;
        }
        if ((sockClient = accept(sockMain, (struct sockaddr *) 0, 0)) == -1)
        {
            std::cerr << "ACCEPT ERROR" << '\n';
            return 1;
        }
        std::cout << "| connected |" << '\n';
        childs++;
        pid_t im = fork();
        switch (im)
        {
            case -1:
                std::cerr << "FORK ERROR" << '\n';
                wait(&stat);
                return -1;
                break;
            case 0:
                while (1) {
                    int buf = 0;
                    ssize_t tmp = 0;
                    tmp = recv(sockClient, &buf, 4, 0);
                    std::cout << "received: " << buf << '\n';
                    if (tmp == 0)
                    {
                        close(sockClient);
                        exit(0);
                    }
                    send(sockClient, &buf, 4, 0);
                }
                break;
            default:
            if(childs >= 5)
            {
                wait(&stat);
                close(sockMain);
                close(sockClient);
                exit(0);
            }
            break;
        }
    }
    return 0;
}