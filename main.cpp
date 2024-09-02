#include <iostream>
#include <string>
#include "Client.h"
#include <chrono>
#include <thread>
#include <cstdlib>

bool loginPhase(const Client& client) {
    while(true) {
        std::string serverMessage = client.receiveFromServer();
        std::string userInput;
        if (serverMessage.empty()) {
            break;
        }
        std::cout << serverMessage;
        std::cin >> userInput;
        client.sendToServer(userInput);

        system("cls");

        serverMessage = client.receiveFromServer();
        std::cout << serverMessage;
        if (serverMessage == "Goodbye!") {
            break;
        }

        while (serverMessage.find("Enter") != std::string::npos || serverMessage.find("Repeat") != std::string::npos) {
            std::cin >> userInput;
            client.sendToServer(userInput);
            serverMessage = client.receiveFromServer();
            std::cout << serverMessage;
        }

        std::this_thread::sleep_for(std::chrono::seconds(2));
        system("cls");
        if (serverMessage.find("logged in successfully") != std::string::npos) {
            return true;
        }
    }

    return false;
}

[[noreturn]] void gamePhase(const Client& client) {
    const std::string serverMessage = client.receiveFromServer();
    if (serverMessage.empty()) {
        return;
    }
    std::cout << serverMessage;

    while (true) {

    }
}

int main() {;
    const Client client("127.0.0.1", 54000);
    client.connectToServer();

    if(loginPhase(client)) {
        gamePhase(client);
    }

    client.disconnectFromServer();

    return 0;
}
