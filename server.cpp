/*
MST Server Implementation in C++
Using Boost.Asio for Asynchronous Networking
Supports Kruskal and Prim Algorithms
Implements Design Patterns: Factory Pattern, Leader-Follower Thread Pool
*/

#include "graph.h"
#include "mst_factory.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>

using boost::asio::ip::tcp;

class MSTServer {
public:
    MSTServer(boost::asio::io_context& io_context, int port, int num_threads)
        : io_context(io_context), acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
        startAccept();
        for (int i = 0; i < num_threads; ++i) {
            worker_threads.emplace_back([this]() { io_context.run(); });
        }
    }

    ~MSTServer() {
        for (auto& thread : worker_threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }

private:
    void startAccept() {
        auto socket = std::make_shared<tcp::socket>(io_context);
        acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
            if (!ec) {
                handleClient(socket);
            }
            startAccept();  // Continue accepting new connections
        });
    }

    void handleClient(std::shared_ptr<tcp::socket> socket) {
        auto buffer = std::make_shared<boost::asio::streambuf>();
        boost::asio::async_read_until(*socket, *buffer, "\n",
            [this, socket, buffer](boost::system::error_code ec, std::size_t) {
                if (!ec) {
                    std::istream stream(buffer.get());
                    std::string request;
                    std::getline(stream, request);
                    
                    std::string response = processRequest(request) + "\n";
                    boost::asio::async_write(*socket, boost::asio::buffer(response),
                        [socket](boost::system::error_code, std::size_t) {});
                }
            });
    }

    std::string processRequest(const std::string& request) {
        std::istringstream ss(request);
        std::string command;
        ss >> command;

        if (command == "MST") {
            std::string algorithm;
            ss >> algorithm;
            
            Graph g;
            int u, v;
            double w;
            
            while (ss >> u >> v >> w) {
                g.addEdge(u, v, w);
            }
            
            MST mst = (algorithm == "KRUSKAL") ? 
                      MSTFactory::computeMST(g, KRUSKAL) : 
                      MSTFactory::computeMST(g, PRIM);

            std::ostringstream output;
            for (const auto& [u, v, weight] : mst.getEdges()) {
                output << u << " - " << v << " : " << weight << "\n";
            }

            return output.str();
        }
        return "Invalid Command";
    }

    boost::asio::io_context& io_context;
    tcp::acceptor acceptor;
    std::vector<std::thread> worker_threads;
};

int main() {
    try {
        const int num_threads = std::thread::hardware_concurrency();
        boost::asio::io_context io_context;
        MSTServer server(io_context, 12345, num_threads);
        io_context.run();
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }
    return 0;
}



