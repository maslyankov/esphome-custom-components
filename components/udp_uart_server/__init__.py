import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart
from esphome.const import CONF_ID, CONF_IP_ADDRESS, CONF_PORT, CONF_UART_ID

MULTI_CONF = True
DEPENDENCIES = ['uart']

udp_uart_server_ns = cg.esphome_ns.namespace('udp_uart_server')
UDPServer = udp_uart_server_ns.class_('UDPServer', cg.Component, uart.UARTDevice)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(UDPServer),
    cv.Optional(CONF_UART_ID): cv.use_id(uart.UARTComponent),
    cv.Required(CONF_IP_ADDRESS): cv.ipv4,
    cv.Required(CONF_PORT): cv.port,
}).extend(cv.COMPONENT_SCHEMA).extend(uart.UART_DEVICE_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    if CONF_UART_ID in config:
        uart_component = yield cg.get_variable(config[CONF_UART_ID])
        cg.add(var.set_uart_parent(uart_component))
    cg.add(var.set_target_ip(config[CONF_IP_ADDRESS].str()))
    cg.add(var.set_target_port(config[CONF_PORT]))
    yield cg.register_component(var, config)
    yield uart.register_uart_device(var, config)