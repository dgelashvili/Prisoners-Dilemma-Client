#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <winsock2.h>

class Client {
public:
    Client(std::string  ip, int port);
    ~Client();

    void connectToServer() const;
    void disconnectFromServer() const;

    void sendToServer(const std::string& message) const;
    [[nodiscard]] std::string receiveFromServer() const;

private:
    static void initializeWinSock();
    void setupConnectingSocket();

    std::string ip;
    int port;
    SOCKET connectSocket;
};

#endif //CLIENT_H
