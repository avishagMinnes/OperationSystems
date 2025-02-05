#include "graph.hpp"
#include <iostream>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>
#include <pthread.h>
#include <queue>

// Constants
#define MAX_THREADS 4
#define MAX_QUEUE_SIZE 100

// Global Variables
Graph* g = nullptr;
pthread_mutex_t graphMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t graphCond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t queueMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queueCond = PTHREAD_COND_INITIALIZER;

std::queue<int> requestQueue;
bool serverRunning = true;
int listener;  // Global listener socket

// Function Prototypes
void* leaderThread(void* arg);
void* workerThread(void* arg);
void processClient(int clientSocket);
void notifyGraphUpdated();

void notifyGraphUpdated() {
    pthread_mutex_lock(&graphMutex);
    pthread_cond_signal(&graphCond);
    pthread_mutex_unlock(&graphMutex);
}

// Leader thread accepts connections (Leader-Follower Pattern)
void* leaderThread(void* arg) {
    while (serverRunning) {
        struct sockaddr_in clientAddr;
        socklen_t clientLen = sizeof(clientAddr);
        int clientSocket = accept(listener, (struct sockaddr*)&clientAddr, &clientLen);

        if (!serverRunning) {
            close(listener);  // Ensure listener socket is closed
            break;
        }

        if (clientSocket < 0) {
            if (!serverRunning) break;  // Ensure shutdown
            perror("accept");
            continue;
        }

        std::cout << "Accepted connection from " << inet_ntoa(clientAddr.sin_addr) << std::endl;

        pthread_mutex_lock(&queueMutex);
        if (requestQueue.size() < MAX_QUEUE_SIZE) {
            requestQueue.push(clientSocket);
            pthread_cond_signal(&queueCond);
        } else {
            close(clientSocket);
        }
        pthread_mutex_unlock(&queueMutex);
    }

    std::cout << "Leader thread exiting...\n";
    return nullptr;
}

// Worker thread processes client requests (Pipeline Pattern)
void* workerThread(void* arg) {
    while (serverRunning) {
        int clientSocket;

        pthread_mutex_lock(&queueMutex);
        while (requestQueue.empty() && serverRunning) {
            pthread_cond_wait(&queueCond, &queueMutex);
        }

        if (!serverRunning) {
            pthread_mutex_unlock(&queueMutex);
            break;
        }

        clientSocket = requestQueue.front();
        requestQueue.pop();
        pthread_mutex_unlock(&queueMutex);

        processClient(clientSocket);
    }

    std::cout << "Worker thread exiting...\n";
    return nullptr;
}

// Process a client request
void processClient(int sockfd) {
    char buffer[1024];
    int fd = sockfd;

    while (true) {
        int nbytes = read(fd, buffer, sizeof(buffer) - 1);
        if (nbytes <= 0) {
            if (nbytes == 0) {
                std::cout << "Connection closed by client" << std::endl;
            } else {
                perror("recv");
            }
            close(fd);
            return;
        }

        buffer[nbytes] = '\0';
        std::string command(buffer);
        std::string response;

        if (command.find("Newgraph") == 0) {
            int n;
            sscanf(command.c_str(), "Newgraph %d", &n);
            pthread_mutex_lock(&graphMutex);
            delete g;
            g = new Graph(n);
            pthread_mutex_unlock(&graphMutex);
            response = "New graph created\n";
        } 
        else if (command.find("Newedge") == 0) {
            int u, v, weight;
            sscanf(command.c_str(), "Newedge %d,%d,%d", &u, &v, &weight);
            pthread_mutex_lock(&graphMutex);
            if (g != nullptr) {
                g->addEdge(u, v, weight);
            }
            pthread_mutex_unlock(&graphMutex);
            response = "Edge added\n";
            notifyGraphUpdated();
        } 
        else if (command.find("Removeedge") == 0) {
            int u, v;
            sscanf(command.c_str(), "Removeedge %d,%d", &u, &v);
            pthread_mutex_lock(&graphMutex);
            if (g != nullptr) {
                g->removeEdge(u, v);
            }
            pthread_mutex_unlock(&graphMutex);
            response = "Edge removed\n";
            notifyGraphUpdated();
        } 
        else if (command.find("MST") == 0) {
            int root;
            char algoType[10];
            sscanf(command.c_str(), "MST %s %d", algoType, &root);

            MSTAlgorithm algo = (strcmp(algoType, "Prim") == 0) ? MSTAlgorithm::PRIM : MSTAlgorithm::BORUVKA;
            pthread_mutex_lock(&graphMutex);
            if (g != nullptr) {
                g->computeMST(algo);
                response = "MST Computed:\n";
                response += "Total MST Weight: " + std::to_string(g->getMSTTotalWeight()) + "\n";
                response += "Longest Distance in MST: " + std::to_string(g->getMSTLongestDistance()) + "\n";
                response += "Average Distance in MST: " + std::to_string(g->getMSTAverageDistance()) + "\n";
                response += "Shortest Distance in MST: " + std::to_string(g->getMSTShortestDistance()) + "\n";
            }
            pthread_mutex_unlock(&graphMutex);
        } 
        else if (command.find("exit") == 0) {  // Gracefully shutdown server
            std::cout << "Shutting down server..." << std::endl;
            serverRunning = false;

            pthread_cond_broadcast(&queueCond);  // Wake up all worker threads
            shutdown(listener, SHUT_RDWR);
            close(listener);

            close(fd);
            return;
        } 
        else {
            response = "Unknown command\n";
        }

        send(fd, response.c_str(), response.size(), 0);
    }
}

// Server main function
int main() {
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if (listener == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = INADDR_ANY;
    serveraddr.sin_port = htons(9034);

    if (bind(listener, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) == -1) {
        perror("bind");
        close(listener);
        return 1;
    }

    if (listen(listener, 10) == -1) {
        perror("listen");
        close(listener);
        return 1;
    }

    std::cout << "Server is running on port 9034" << std::endl;

    // Create leader thread
    pthread_t leader;
    if (pthread_create(&leader, nullptr, leaderThread, nullptr) != 0) {
        perror("pthread_create (leader)");
        return 1;
    }

    // Create worker threads
    pthread_t workers[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_create(&workers[i], nullptr, workerThread, nullptr) != 0) {
            perror("pthread_create (worker)");
            return 1;
        }
    }

    // Join threads
    pthread_join(leader, nullptr);
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(workers[i], nullptr);
    }

    std::cout << "Cleaning up resources..." << std::endl;
    close(listener);
    delete g;
    pthread_mutex_destroy(&graphMutex);
    pthread_mutex_destroy(&queueMutex);
    pthread_cond_destroy(&queueCond);
    std::cout << "Server has shut down." << std::endl;

    return 0;
}







