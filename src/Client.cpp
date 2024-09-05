#include "Client.h"
#include <iostream>
#include <thread>

//starts initializing WinSock and setups 'connectingSocket'
Client::Client(std::string ip, const int port)
    : ip(std::move(ip)), port(port), connectSocket(INVALID_SOCKET) {
    initializeWinSock();
    setupConnectingSocket();
}

//calls 'disconnectFromServer()' which handles clearing everything
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

// Sets up connection with the server
void Client::connectToServer() const {
    sockaddr_in clientService{};
    clientService.sin_family = AF_INET;
    clientService.sin_addr.s_addr = inet_addr(ip.c_str());
    clientService.sin_port = htons(port);

    if (connect(connectSocket, (SOCKADDR*)(&clientService), sizeof(clientService)) == SOCKET_ERROR) {
        const auto err = WSAGetLastError();
        disconnectFromServer();
        throw std::runtime_error("connection failed: " + std::to_string(err));
    }
}

// Closes 'connectSocket' if one is valid and also cleans up WSA
void Client::disconnectFromServer() const {
    if (connectSocket != INVALID_SOCKET) {
        closesocket(connectSocket);
    }
    WSACleanup();
}

// Wrapper of send method
void Client::sendToServer(const std::string &message) const {
    send(connectSocket, message.c_str(), (int)(message.size()), 0);
}

// Wrapper of receive method
std::string Client::receiveFromServer() const {
    char buffer[512];
    if (const int bytesReceived = recv(connectSocket, buffer, sizeof(buffer) - 1, 0); bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::string result(buffer);
        return result;
    }
    return "";
}

/**
* This method is called when user has to enter some kind of command after which
* the page is going to refresh.
*
* Returns last message from the server
*/
std::string Client::enterCommand() const {
    std::string serverMessage = receiveFromServer();
    std::string userInput;
    if (serverMessage.empty()) {
        return "";
    }
    std::cout << serverMessage;
    std::cin >> userInput;
    sendToServer(userInput);

    system("cls");

    serverMessage = receiveFromServer();
    std::cout << serverMessage;

    return serverMessage;
}


// Handles authentication pages and communication
bool Client::authenticationPhase() const {
    while(true) {
        //Enter command (REG/LOG/EXIT):
        std::string serverMessage = enterCommand();
        if (serverMessage == "Goodbye!" || serverMessage.empty()) {
            break;
        }

        //REG or LOG phase
        while (serverMessage.find("Enter") != std::string::npos ||
                serverMessage.find("Repeat") != std::string::npos) {
            std::string userInput;
            std::cin >> userInput;
            sendToServer(userInput);
            serverMessage = receiveFromServer();
            std::cout << serverMessage;
        }

        //Additional message from server
        std::this_thread::sleep_for(std::chrono::seconds(2));
        system("cls");
        if (serverMessage.find("logged in successfully") != std::string::npos) {
            return true;
        }
    }

    return false;
}

// Handles main menu page and communication
[[noreturn]] void Client::mainMenuPhase() const {
    while(true) {
        //play or exit (P/X):
        std::string serverMessage = enterCommand();
        if (serverMessage == "Goodbye!"  || serverMessage.empty()) {
            break;
        }

        //response of server: either retrying or playing
        if (serverMessage.find("unknown") != std::string::npos) {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            system("cls");
        } else {
            while (true) {

            }
        }
    }
}


