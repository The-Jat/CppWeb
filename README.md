# CppWebServer

A lightweight, fast, and simple HTTP server written in C++. This project demonstrates how to handle HTTP requests and responses using basic networking features in C++.

## Features

- Handles basic HTTP GET and POST requests.
- Supports basic routing and URL handling.
- Lightweight and minimalistic design, focused on performance.
- Easily extensible to support additional HTTP methods and features.

## Requirements

- C++17 or later
- A C++ compiler (GCC, Clang, MSVC, etc.)
- Basic networking libraries like `iostream`, `sys/socket.h`, `netinet/in.h`, etc.

## Installation

### Prerequisites

Make sure you have a C++17 compatible compiler installed (like GCC or Clang) and the required networking libraries are available.

### Steps to Build

1. Clone this repository:
   ```bash
   git clone https://github.com/The-Jat/CppWeb.git
   cd CppWebServer
2. Build the project (using g++):
   ```bash
   g++ -o server server.cpp
   ```
3. Run the server:
   ```c
   ./server
   ```
4. Your HTTP server will start and listen on port 8080 by default. You can access it via a web browser or tools like curl:
5. ```bash
   curl http://localhost:8080
   ```
   or Visit the brower at this link: http://localhost:8080