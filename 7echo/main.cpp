#include <os>
#include <iostream>
#include <net/interfaces>

void Service::start()
{
        auto& inet = net::Interfaces::get(0);
        auto& tcp = inet.tcp().listen(7);
        auto& udp = inet.udp().bind(7);

        tcp.on_connect([] (auto conn) {
                // Log incomming connections on the console:
                std::cout << "TCP from " << conn->to_string() << "\n";
                // When data is received, echo back
                conn->on_read(1024, [conn] (auto buf) {
                        conn->write(buf);
                });
        });

        udp.on_read([&] (auto addr, auto port, auto buf, auto len) {
                // Log incomming connections on the console:
                std::cout << "UDP from " << addr.to_string() << "\n";
                // When data is received, echo back
                udp.sendto(addr, port, buf, len);
        });
}
