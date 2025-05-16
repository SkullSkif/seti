#include <iostream>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    int sockMain, sockServ;
    struct sockaddr_in serv;
    sockMain = socket(AF_INET, SOCK_STREAM, 0);
    if (sockMain < 0)
    {
        std::cerr << "SOCKET OPEN ERROR" << '\n';
        close(sockMain);
        return 1;
    }
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    int time = atoi(argv[2]);
    serv.sin_port = htons(atoi(argv[1]));
    socklen_t s_size = sizeof(serv);
    if ((sockServ = connect(sockMain, (struct sockaddr*) &serv, s_size)) == -1)
    {
        std::cerr << "CONNECT ERROR" << '\n';
        close(sockServ);
        close(sockMain);
        return 1;
    }
    std::cout << "CONNECTED TO SERVER" << '\n';
    for(int i = 0; i <= time ; i++)
    {
        std::cout << "send: " << time << '\n';
        if (send(sockMain, &time, 4, 0) < 0)
        {
            std::cerr << "SENDING RROR\n";
            exit(1);
        }
        sleep(time);
        recv(sockMain, &time, sizeof(time), 0);
        std::cout << "received: " << time << '\n';
    }
    close(sockServ);
    close(sockMain);
    return 0;
}