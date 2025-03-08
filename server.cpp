#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

#define PORT 8080

// Utility function to read the contents of a file into a string.
string readFileContent(const string &filename) {
    ifstream file(filename, ios::binary); // Read in binary mode for all file types
    if (!file.is_open()) {
        cerr << "Could not open file: " << filename << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}


bool endsWith(const std::string &str, const std::string &suffix) {
    if (str.size() < suffix.size()) return false;
    return std::equal(suffix.rbegin(), suffix.rend(), str.rbegin());
}

std::string getContentType(const std::string &filepath) {
    if (endsWith(filepath, ".html")) return "text/html";
    if (endsWith(filepath, ".css")) return "text/css";
    if (endsWith(filepath, ".js")) return "application/javascript";
    return "text/plain"; // Default type
}


void handleClient(int clientSocket) {
    char buffer[1024] = {0};

    // Read the client's request.
    int bytesRead = read(clientSocket, buffer, sizeof(buffer) - 1);
    if (bytesRead <= 0) {
        close(clientSocket);
        return;
    }
    buffer[bytesRead] = '\0';  // Ensure null termination.
    cout << "Request:\n" << buffer << endl;

    // Parse the request line to extract the method and route.
    char method[10], route[1024];
    sscanf(buffer, "%s %s", method, route);

    string filepath = route;
    
    // If root is requested, serve index.html
    if (strcmp(route, "/") == 0) {
        filepath = "index.html";
    } else if (strcmp(route, "/about") == 0) {
        filepath = "about.html";
    } else {
        // Remove leading '/' and use it as file path
        filepath = route + 1;
    }

    // Read file content from disk.
    string fileContent = readFileContent(filepath);
    if (fileContent.empty()) {
        // If file not found, serve 404 page.
        fileContent = "<html><body><h1>404 Not Found</h1></body></html>";
        filepath = "404.html"; // Fallback to setting proper type for response
    }

    // Determine content type.
    string contentType = getContentType(filepath);

    // Build HTTP response.
    stringstream responseStream;
    responseStream << "HTTP/1.1 200 OK\r\n";
    responseStream << "Content-Type: " << contentType << "\r\n";
    responseStream << "Content-Length: " << fileContent.length() << "\r\n";
    responseStream << "\r\n";
    responseStream << fileContent;

    string httpResponse = responseStream.str();

    // Send the response.
    send(clientSocket, httpResponse.c_str(), httpResponse.length(), 0);

    // Close the client connection.
    close(clientSocket);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);

    // Create a socket.
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address.
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // Bind the socket to the IP and port.
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections.
    if (listen(serverSocket, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    cout << "Server listening on port " << PORT << "...\n";

    while (true) {
        // Accept an incoming connection.
        if ((clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen)) < 0) {
            perror("Accept failed");
            exit(EXIT_FAILURE);
        }
        // Handle the client request.
        handleClient(clientSocket);
    }

    return 0;
}

