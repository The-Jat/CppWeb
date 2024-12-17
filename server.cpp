#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

using namespace std;

#define PORT 8080

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    
    // Read the client's request
    read(clientSocket, buffer, sizeof(buffer));
    cout << "Request:\n" << buffer << endl;

    // HTTP Response (A simple HTML page)
    const char *httpResponse = 
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Content-Length: 113\r\n"
        "\r\n"
        "<html>"
        "<head><title>Test HTTP Server</title></head>"
        "<body><h1>Hello, World!</h1></body>"
        "</html>";
    
    // Send the HTTP response to the client
    send(clientSocket, httpResponse, strlen(httpResponse), 0);

    // Close the client connection
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the IP and port
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        // Accept an incoming connection
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }

        // Handle the client request
        handleClient(clientSocket);
    }

    return 0;
}

