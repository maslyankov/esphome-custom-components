#ifndef UDP_UART_SERVER_H
#define UDP_UART_SERVER_H

#include "esphome.h"
#include <WiFiUdp.h>

namespace esphome {
namespace udp_uart_server {

class UDPServer : public Component, public uart::UARTDevice {
 public:
  void set_target_ip(const std::string &ip) { target_ip_ = ip; }
  void set_target_port(uint16_t port) { port_ = port; }
  void set_uart_parent(uart::UARTComponent *parent) { this->parent_ = parent; }

  void setup() override {
    udp_.begin(port_);
  }

  void loop() override {
    char incomingPacket[255];
    int packetSize = udp_.parsePacket();
    if (packetSize) {
      int len = udp_.read(incomingPacket, 255);
      if (len > 0) {
        incomingPacket[len] = 0;
        ESP_LOGD("uart", "Received UDP packet: %s", incomingPacket);
        write_array((uint8_t*)incomingPacket, len);
      }
    }

    while (available()) {
      std::string uart_data;
      read_line(&uart_data);
      udp_.beginPacket(target_ip_.c_str(), port_);
      udp_.print(uart_data.c_str());
      udp_.endPacket();
    }
  }

 protected:
  WiFiUDP udp_;
  std::string target_ip_;
  uint16_t port_;
};

}  // namespace udp_uart_server
}  // namespace esphome

#endif  // UDP_UART_SERVER_H