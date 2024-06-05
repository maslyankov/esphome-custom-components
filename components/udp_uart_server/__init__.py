import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, binary_sensor, sensor
from esphome.const import CONF_ID, CONF_IP_ADDRESS, CONF_PORT, CONF_UART_ID, CONF_CLIENT_CONNECTED, CONF_CLIENT_COUNT

MULTI_CONF = True
DEPENDENCIES = ['uart']

udp_uart_server_ns = cg.esphome_ns.namespace('udp_uart_server')
UDPServer = udp_uart_server_ns.class_('UDPServer', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UDPServer),
    cv.Optional(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_IP_ADDRESS): cv.ipv4,
    cv.Required(CONF_PORT): cv.port,
    cv.Optional(CONF_CLIENT_CONNECTED): cv.use_id(binary_sensor.BinarySensor),
    cv.Optional(CONF_CLIENT_COUNT): cv.use_id(sensor.Sensor),
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    if CONF_UART_ID in config:
        uart_component = yield cg.get_variable(config[CONF_UART_ID])
        cg.add(var.set_uart_parent(uart_component))
    cg.add(var.set_target_ip(str(config[CONF_IP_ADDRESS])))
    cg.add(var.set_target_port(config[CONF_PORT]))
    if CONF_CLIENT_CONNECTED in config:
        client_connected = yield cg.get_variable(config[CONF_CLIENT_CONNECTED])
        cg.add(var.set_client_connected_sensor(client_connected))
    if CONF_CLIENT_COUNT in config:
        client_count = yield cg.get_variable(config[CONF_CLIENT_COUNT])
        cg.add(var.set_client_count_sensor(client_count))
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)