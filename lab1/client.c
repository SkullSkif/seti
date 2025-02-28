#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <arpa/inet.h>
#include <chrono>
#include <thread>

#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <IP адрес сервера> <порт>" << std::endl;
        return EXIT_FAILURE;
    }

    const char *server_ip = argv[1];
    int server_port = std::atoi(argv[2]);
    int sockfd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Создание сокета
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Ошибка при создании сокета");
        exit(EXIT_FAILURE);
    }

    // Настройка адреса сервера
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    for (int i = 1; i <= 5; ++i) { // Отправка 5 сообщений
        std::string message = "Сообщение " + std::to_string(i);
        sendto(sockfd, message.c_str(), message.size(), 0, (const struct sockaddr *)&server_addr, sizeof(server_addr));

        // Получение ответа от сервера
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, nullptr, nullptr);
        buffer[n] = '\0'; // Завершение строки
        std::cout << "Ответ от сервера: " << buffer << std::endl;

        // Задержка в i секунд
        std::this_thread::sleep_for(std::chrono::seconds(i));
    }

    close(sockfd);
    return 0;
}