#include <iostream>
#include <map>
#include <mutex>
#include <cstring>
#include <thread>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

std::map<int, int> clients; // Map client IDs to their socket file descriptors
std::mutex clients_lock;

void handle_client(int client_socket, int client_id) {
    {
        std::lock_guard<std::mutex> lock(clients_lock);
        // Pair clients sequentially: 1 with 2, 3 with 4, etc.
        int paired_client_id = client_id % 2 != 0 ? client_id + 1 : client_id - 1;
        clients[client_id] = client_socket;
        std::cout << "Client " << client_id << " connected. Paired with client " << paired_client_id << "." << std::endl;
    }

    char buffer[1024];
    try {
        while (true) {
            memset(buffer, 0, 1024);
            ssize_t bytes_received = recv(client_socket, buffer, 1024, 0);
            if (bytes_received <= 0) {
                break; // Client disconnected
            }
            std::cout << "Coordinates from Client " << client_id << ": " << buffer << std::endl;

            // Relay coordinates to the paired client
            std::lock_guard<std::mutex> lock(clients_lock);
            int paired_client_socket = clients[client_id % 2 != 0 ? client_id + 1 : client_id - 1];
            if (send(paired_client_socket, buffer, strlen(buffer), 0) == -1) {
                std::cerr << "Error sending message to client." << std::endl;
            } else {
                std::cout << "Sent coordinates to Client " << std::endl;
                //std::cout << (client_id % 2 != 0 ? client_id + 1 : client_id - 1) << ": " << buffer << std::endl;
            }
        }
    } catch (...) {
        std::cerr << "An error occurred with Client " << client_id << std::endl;
    }

    {
        std::lock_guard<std::mutex> lock(clients_lock);
        clients.erase(client_id);
    }
    close(client_socket);
    std::cout << "Client " << client_id << " disconnected." << std::endl;
}

void start_server(int port) {
    int server_socket, client_socket;
    sockaddr_in server_addr{}, client_addr{};
    socklen_t client_addr_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        std::cerr << "Could not create socket" << std::endl;
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Bind failed" << std::endl;
        exit(1);
    }

    listen(server_socket, 5);
    std::cout << "Server started on port " << port << ". Waiting for connections..." << std::endl;

    int client_id = 1;
    client_addr_size = sizeof(client_addr);
    while ((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_size))) {
        std::thread client_thread(handle_client, client_socket, client_id);
        client_thread.detach();
        client_id++;
    }

    close(server_socket);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <PORT>" << std::endl;
        return 1;
    }

    int port = std::stoi(argv[1]);
    start_server(port);
    return 0;
}
