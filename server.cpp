#include "server.h"

MSTServer::MSTServer(boost::asio::io_context& io_context, int port)
    : io_context(io_context), acceptor(io_context, tcp::endpoint(tcp::v4(), port)) {
    startAccept();
}

void MSTServer::startAccept() {
    auto socket = std::make_shared<tcp::socket>(io_context);
    acceptor.async_accept(*socket, [this, socket](boost::system::error_code ec) {
        if (!ec) {
            handleClient(socket);
        }
        startAccept();  // המשך לקבל חיבורים נוספים
    });
}

void MSTServer::handleClient(std::shared_ptr<tcp::socket> socket) {
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

std::string MSTServer::processRequest(const std::string& request) {
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
