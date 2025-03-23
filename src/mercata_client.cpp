// mercata_client.cpp - Simple TCP client to request inventory
#include <asio.hpp>
#include <iostream>
#include <array>
#include <cstdint>
#include <cstring>

using asio::ip::tcp;

void read_inventory(tcp::socket& socket) {
    std::array<char, 13> buffer;
    while (true) {
        asio::error_code ec;
        size_t len = asio::read(socket, asio::buffer(buffer), asio::transfer_exactly(13), ec);
        if (ec) break;
        if (buffer[0] != 'I') break;

        uint32_t item_id, quantity, price;
        memcpy(&item_id, &buffer[1], 4);
        memcpy(&quantity, &buffer[5], 4);
        memcpy(&price, &buffer[9], 4);

        std::cout << "Item ID: " << item_id
                  << ", Qty: " << quantity
                  << ", Price: $" << price / 100.0 << "\n";
    }
}

int main() {
    try {
        asio::io_context io;
        tcp::resolver resolver(io);
        auto endpoints = resolver.resolve("127.0.0.1", "8443");
        tcp::socket socket(io);
        asio::connect(socket, endpoints);

        asio::write(socket, asio::buffer("L"));
        read_inventory(socket);
    } catch (std::exception& e) {
        std::cerr << "Client error: " << e.what() << "\n";
    }
}
