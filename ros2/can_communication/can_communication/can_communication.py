import rclpy
from rclpy.node import Node
from custom_messages.msg import CanMsg
import serial
import time

class can_communication(Node) :
    def __init__(self) :
        super().__init__('can_communication')
        
        self.ser = serial.Serial()
        self.ser.port = '/dev/serial/by-id/usb-TMCIT_S_USBtoCAN.ver1-if00'
        self.ser.baudrate = 115200
        self.ser.parity = serial.PARITY_NONE
        self.ser.bytesize = serial.EIGHTBITS
        self.ser.stopbits = serial.STOPBITS_ONE
        self.ser.timeout = 0.1
        self.ser.write_timeout = 0.1
        self.ser.inter_byte_timeout = 0.1

        self.setup_serial() 
        
        self.pub = self.create_publisher(CanMsg, '/can_rx', 1)
        self.sub = self.create_subscription(CanMsg, '/can_tx', self.can_msg_callback, 20)

        self.create_timer(0.001, self.rxloop)

    def setup_serial(self) :
        if self.ser.is_open :
            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()
            self.ser.close()
        
        while not self.ser.is_open :
            try :
                self.ser.open()
            except Exception as e :
                self.get_logger().error('Setup Error')
                self.get_logger().error(str(e))
                time.sleep(1)

    def transmit_data(self, data) :
        tx = b'\x02' + data + b'\x03'
        # self.get_logger().info('tx: ' + str([*tx]))
        try :
            self.ser.write(tx)
            self.ser.flush()
        except Exception as e :
            self.get_logger().error('TX Error')
            self.get_logger().error(str(e))
            self.setup_serial()

    def receive_data(self) :
        rx = []
        try :
            while True :
                if (reading := self.ser.read()) != b'' :
                    rx += [reading]
                    # self.get_logger().info('RX: ' + str(reading))
                if len(rx) >= 6 and (int(rx[1]) or len(rx) == 6 + int(rx[5])) :
                    return b''.join(rx)
        except ValueError as e :
            self.get_logger().warn('RX Value Error: ' + str(rx))
        except Exception as e :
            self.get_logger().error('RX Error: ' + str(rx))
            self.get_logger().error(str(e))
            self.setup_serial()

    def rxloop(self) :
        if rx := self.receive_data() :
            if rx[2:4] == b'7f' :
                # self.get_logger().error('unchi')
                return
            # self.get_logger().info('rx_raw: ' + str([*rx]))
            self.publish_can_msg(rx)
    
    def publish_can_msg(self, rx) :
        try :
            msg = CanMsg()
            msg.channel = bool(int(chr(rx[0])))
            msg.frametype = bool(int(chr(rx[1])))
            msg.id = int(rx[2:5], 16)
            msg.dlc = bytes([rx[5]])
            msg.data = [bytes([b]) for b in rx[6:]] if not msg.frametype else b''
            # self.get_logger().info('rx: ' + str(msg))
            self.pub.publish(msg)
        except Exception as e:
            self.get_logger().error('Publish CAN Messages Error')
            self.get_logger().error(str(e))
    
    def can_msg_callback(self, msg) :
        # self.get_logger().info('sub: ' + str(msg))
        try :
            tx = str(int(msg.channel)).encode() + str(int(msg.frametype)).encode() + format(msg.id, '03x').encode() + str(int(msg.dlc)).encode() + b''.join(msg.data)
            self.transmit_data(tx)
        except Exception as e :
            self.get_logger().error('Subscribe CAN Messages Error')
            self.get_logger().error(str(e))

def main() :
    rclpy.init()
    node = can_communication()
    
    try :
        rclpy.spin(node)
    except KeyboardInterrupt :
        node.get_logger().error('Keyborad Interrupt')
        time.sleep(0.1)
    else :
        node.destroy_node()
        rclpy.shutdown()

if __name__ == '__main__' :
    main()