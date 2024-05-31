#include "udp_uart_server.h"
#include "esphome/core/log.h"

namespace esphome {
namespace udp_uart_server {

static const char *TAG = "udp_uart_server";

void UDPServer::setup() {
  ESP_LOGCONFIG(TAG, "Setting up UDPServer...");
  udp_.begin(port_);
}

void UDPServer::loop() {
  char incomingPacket[255];
  int packetSize = udp_.parsePacket();
  if (packetSize) {
    int len = udp_.read(incomingPacket, 255);
    if (len > 0) {
      incomingPacket[len] = 0;
      ESP_LOGD(TAG, "Received UDP packet: %s", incomingPacket);
      write_array((uint8_t*)incomingPacket, len);
    }
  }

  if (available()) {
    std::string uart_data = read_string();
    udp_.beginPacket(target_ip_.c_str(), port_);
    udp_.print(uart_data.c_str());
    udp_.endPacket();
  }
}

}  // namespace udp_uart_server
}  // namespace esphome