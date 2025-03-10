#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>

void scanPort(const char* ip, int port, std::vector<int>& openPorts) {
    int sock;
    struct sockaddr_in server;

    // Создаем сокет
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        std::cerr << "Ошибка при создании сокета" << std::endl;
        return;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    inet_pton(AF_INET, ip, &server.sin_addr);

    // Устанавливаем тайм-аут
    struct timeval timeout;
    timeout.tv_sec = 1;  // 1 секунда
    timeout.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));

    // Пытаемся подключиться
    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) == 0) {
        openPorts.push_back(port); // Сохраняем открытый порт
    }

    close(sock);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <IP-адрес>" << std::endl;
        return 1;
    }

    const char* targetIP = argv[1];
    
    std::vector<int> openPorts; // Вектор для хранения открытых портов

    for (int port = 1; port <= 1024; port++) {
        scanPort(targetIP, port, openPorts);
    }

    // Выводим открытые порты
    std::cout << "Открытые порты на IP " << targetIP << ":" << std::endl;
    for (int port : openPorts) {
        std::cout << "Порт " << port << " открыт" << std::endl;
    }

    return 0;
}
