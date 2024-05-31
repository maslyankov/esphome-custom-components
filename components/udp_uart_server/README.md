# UDP UART Server Component

This custom component for ESPHome creates a UART to UDP server. The component reads data from a UART interface and sends it over UDP to a specified IP address and port. It also listens for incoming UDP packets and writes the data to the UART interface.

## Repository Structure

```
components/udp_uart_server/
├── __init__.py
├── udp_uart_server.h
└── udp_uart_server.cpp
```

- `__init__.py`: Defines the Python interface for the component, including the configuration schema and code generation logic.
- `udp_uart_server.h`: Contains the declaration of the `UDPServer` class.
- `udp_uart_server.cpp`: Contains the implementation of the `UDPServer` class.

## Usage

To use this custom component in your ESPHome project, follow these steps:

### 1. Add the Custom Component

Clone this repository to your local machine or reference it directly from your ESPHome configuration.

### 2. Modify Your ESPHome Configuration

Include the custom component in your ESPHome YAML configuration file.

### Example Configuration

```yaml
esphome:
  name: uart_udp_server
  platform: ESP8266
  board: nodemcuv2

external_components:
  - source:
      type: git
      url: https://github.com/maslyankov/esphome-custom-components
      ref: main  # or specify another branch/tag
    components: [udp_uart_server]

wifi:
  ssid: "your_SSID"
  password: "your_PASSWORD"

# Enable logging
logger:

# Enable Home Assistant API
api:
  password: "your_API_password"

ota:
  password: "your_OTA_password"

# UART Configuration
uart:
  - id: uart1
    tx_pin: GPIO1
    rx_pin: GPIO3
    baud_rate: 9600

  - id: uart2
    tx_pin: GPIO4
    rx_pin: GPIO5
    baud_rate: 9600

udp_uart_server:
  - id: udp_uart_server_1
    uart_id: uart1  # Optional, specify if multiple UARTs are configured
    ip_address: 192.168.1.100
    port: 1234

  - id: udp_uart_server_2
    uart_id: uart2  # Optional, specify if multiple UARTs are configured
    ip_address: 192.168.1.101
    port: 1235
```

### 3. Upload the Configuration

Use the ESPHome dashboard or the command line to upload the configuration to your ESP device:

```bash
esphome run uart_udp_server.yaml
```

## Configuration Options

- `uart_id` (Optional): The ID of the UART hub to use. Required if multiple UART hubs are configured.
- `ip_address` (Required): The target IP address to send the UART data.
- `port` (Required): The target port to send the UART data.

## License

This project is licensed under the MIT License.