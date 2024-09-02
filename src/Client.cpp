#include "Client.h"
#include <iostream>

Client::Client(std::string ip, const int port)
    : ip(std::move(ip)), port(port), connectSocket(INVALID_SOCKET) {
    initializeWinSock();
    setupConnectingSocket();
}

Client::~Client() {
    disconnectFromServer();
}

void Client::initializeWinSock() {
    WSADATA wsaData;
    if (const int result = WSAStartup(MAKEWORD(2, 2), &wsaData); result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }
}

void Client::setupConnectingSocket() {
    connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        const auto err = WSAGetLastError();
        disconnectFromServer();
        throw std::runtime_error("Socket creation failed: " + std::to_string(err));
    }
}

void Client::connectToServer() const {
    sockaddr_in clientService{};
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(ip.c_str());
    clientService.sin_port = htons(port);

    if (connect(connectSocket, reinterpret_cast<SOCKADDR *>(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
        const auto err = WSAGetLastError();
        disconnectFromServer();
        throw std::runtime_error("connection failed: " + std::to_string(err));
    }
}


void Client::disconnectFromServer() const {
    if (connectSocket != INVALID_SOCKET) {
        closesocket(connectSocket);
    }
    WSACleanup();
}

void Client::sendToServer(const std::string &message) const {
    send(connectSocket, message.c_str(), static_cast<int>(message.size()), 0);
}

std::string Client::receiveFromServer() const {
    char buffer[512];
    if (const int bytesReceived = recv(connectSocket, buffer, sizeof(buffer) - 1, 0); bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string result(buffer);
        return result;
    }
    return "";
}
