#include <string>
#include "Client.h"

int main() {;
    const Client client("127.0.0.1", 54000);
    client.connectToServer();

    if(client.authenticationPhase()) {
        client.mainMenuPhase();
    }

    client.disconnectFromServer();

    return 0;
}
