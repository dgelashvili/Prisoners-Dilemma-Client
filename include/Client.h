#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <winsock2.h>

/**
* This class is the main class which is responsible for communicating server with 'console'.
* It consists of three main phases:
* Authentication: This is where client tries to log in or register
* Main Menu: This is the main page of Server where client has to decide whether quit or start playing
* Game: TODO
*/
class Client {
public:
    Client(std::string  ip, int port);
    ~Client();

    void connectToServer() const;
    void disconnectFromServer() const;
    [[nodiscard]] std::string enterCommand() const;
    [[nodiscard]] bool authenticationPhase() const;
    void gamePhase() const;
    void mainMenuPhase() const;

private:
    static void initializeWinSock();
    void setupConnectingSocket();
    void sendToServer(const std::string& message) const;
    [[nodiscard]] std::string receiveFromServer() const;

    std::string ip;
    int port;
    SOCKET connectSocket;
};

#endif //CLIENT_H
