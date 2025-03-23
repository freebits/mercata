// mercata_server.cpp - Simple TCP inventory server
#include <asio.hpp>
#include <iostream>
#include <fstream>
#include <array>
#include <vector>
#include <thread>
#include <cstring>

using asio::ip::tcp;

struct Item {
    uint32_t item_id;
    uint32_t quantity;
    uint32_t price_cents;
};

const std::vector<Item> inventory = {
    {1, 100, 10000},
    {2, 50, 5000},
    {3, 10, 2000}
};

void handle_client(tcp::socket socket) {
    try {
        std::array<char, 1024> buffer;
        auto bytes = socket.read_some(asio::buffer(buffer));
        std::string command(buffer.data(), bytes);

        std::ofstream log("trades.log", std::ios::app);
        log << "Received: " << command << "\n";

        if (command.starts_with("L")) {
            for (const auto& item : inventory) {
                std::array<char, 13> response{};
                response[0] = 'I';
                memcpy(&response[1], &item.item_id, 4);
                memcpy(&response[5], &item.quantity, 4);
                memcpy(&response[9], &item.price_cents, 4);
                asio::write(socket, asio::buffer(response));
            }
        } else {
            asio::write(socket, asio::buffer("OK\n"));
        }
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << "\n";
    }
}

int main() {
    try {
        asio::io_context io;
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 8443));
        std::cout << "mercata server running on port 8443\n";

        while (true) {
            tcp::socket socket(io);
            acceptor.accept(socket);
            std::thread(handle_client, std::move(socket)).detach();
        }
    } catch (std::exception& e) {
        std::cerr << "Server error: " << e.what() << "\n";
    }
}
