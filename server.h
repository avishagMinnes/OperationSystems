#ifndef SERVER_H
#define SERVER_H

#include "graph.h"
#include "mst_factory.h"
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <sstream>

using boost::asio::ip::tcp;

class MSTServer {
public:
    MSTServer(boost::asio::io_context& io_context, int port);
    void startAccept();

private:
    void handleClient(std::shared_ptr<tcp::socket> socket);
    std::string processRequest(const std::string& request);

    boost::asio::io_context& io_context;
    tcp::acceptor acceptor;
};

#endif // SERVER_H
