#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>

#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = 0; // Автоматический выбор порта

    // Привязка сокета к адресу
    if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка привязки сокета");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Получение номера порта
    int port;
    socklen_t len = sizeof(server_addr);
    getsockname(sockfd, (struct sockaddr *)&server_addr, &len);
    port = ntohs(server_addr.sin_port);
    std::cout << "Сервер запущен на порту: " << port << std::endl;

    while (true) {
        // Получение сообщения от клиента
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        buffer[n] = '\0'; // Завершение строки

        // Вывод информации на экран
        std::cout << "Получено сообщение от клиента " 
                  << inet_ntoa(client_addr.sin_addr) << ":" 
                  << ntohs(client_addr.sin_port) << " - " 
                  << buffer << std::endl;

        // Преобразование сообщения (например, добавление "Echo: ")
        std::string response = "Echo: " + std::string(buffer);

        // Отправка ответа клиенту
        sendto(sockfd, response.c_str(), response.size(), 0, (const struct sockaddr *)&client_addr, addr_len);
    }

    close(sockfd);
    return 0;
}