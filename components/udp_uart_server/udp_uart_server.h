#include "esphome.h"
#include <WiFiUdp.h>

namespace esphome {
namespace udp_uart_server {

class UDPServer : public Component, public uart::UARTDevice {
 public:
  void set_target_ip(const std::string &ip) { target_ip_ = ip; }
  void set_target_port(uint16_t port) { port_ = port; }
  void set_uart_parent(uart::UARTComponent *parent) { this->parent_ = parent; }
  void set_client_connected_sensor(binary_sensor::BinarySensor *client_connected) { client_connected_ = client_connected; }
  void set_client_count_sensor(sensor::Sensor *client_count) { client_count_ = client_count; }

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

    if (available()) {
      std::string uart_data = read_string();
      udp_.beginPacket(target_ip_.c_str(), port_);
      udp_.print(uart_data.c_str());
      udp_.endPacket();
    }

    bool client_connected = udp_.connected();
    int client_count = udp_.remoteIP() == target_ip_ ? 1 : 0;  // Simplified client count logic

    if (client_connected_ != nullptr) {
      client_connected_->publish_state(client_connected);
    }
    if (client_count_ != nullptr) {
      client_count_->publish_state(client_count);
    }
  }

 protected:
  WiFiUDP udp_;
  std::string target_ip_;
  uint16_t port_;
  binary_sensor::BinarySensor *client_connected_{nullptr};
  sensor::Sensor *client_count_{nullptr};
};

}  // namespace udp_uart_server
}  // namespace esphome