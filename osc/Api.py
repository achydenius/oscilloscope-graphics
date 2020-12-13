import serial.tools.list_ports
import struct


UNSIGNED_SHORT_MAX = 65535


class Api:
    def __init__(self, port_name, baudrate):
        self.port = serial.Serial(port_name, baudrate)

    @staticmethod
    def print_ports():
        for port in serial.tools.list_ports.comports():
            print(port)

    def send(self, lines):
        self.port.write(self.lines_to_serial_data(lines))

    def lines_to_serial_data(self, lines):
        # Flatten and pack lines to one-dimensional list of binary values
        bytes = [self._pack_coords(coords)
                 for line in lines
                 for coords in line]

        # First four bytes define the number of lines
        bytes.insert(0, len(lines).to_bytes(
            4, byteorder='little', signed=True))

        return bytearray(b''.join(bytes))

    def _float_to_unsigned_short(self, value):
        return int(((value * 0.5) + 0.5) * UNSIGNED_SHORT_MAX)

    def _pack_coords(self, coords):
        return struct.pack('<HHf', *(self._float_to_unsigned_short(coord)
                                     for coord in coords))
