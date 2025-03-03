#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstdio>

using namespace std;

#define PORT 8080

void handleClient(int clientSocket) {
    char buffer[1024] = {0};
    
    // Read the client's request
    int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        close(clientSocket);
        return;
    }
    buffer[bytesRead] = '\0';  // Ensure null termination
    cout << "Request:\n" << buffer << endl;
    
    // Parse the HTTP request line to extract the method and route
    char method[10], route[1024];
    sscanf(buffer, "%s %s", method, route);

    string response;

    // Route handling logic
    if (strcmp(route, "/") == 0) {
        // Default home route
        const char *body = "<html><head><title>Home</title></head>"
                           "<body><h1>Welcome to the Home Page!</h1></body></html>";
        char header[256];
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\nContent-Length: %zu\r\n\r\n", strlen(body));
        response = string(header) + body;
    }
    else if (strcmp(route, "/hello") == 0) {
        // A '/hello' route that returns plain text
        const char *body = "Hello, world!";
        char header[256];
        sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: %zu\r\n\r\n", strlen(body));
        response = string(header) + body;
    }
    else {
        // If the route isn't recognized, return a 404 response
        const char *body = "<html><head><title>404 Not Found</title></head>"
                           "<body><h1>404 Not Found</h1></body></html>";
        char header[256];
        sprintf(header, "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nContent-Length: %zu\r\n\r\n", strlen(body));
        response = string(header) + body;
    }

    // Send the HTTP response to the client
    send(clientSocket, response.c_str(), response.length(), 0);
    
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

