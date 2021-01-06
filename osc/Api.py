import serial.tools.list_ports
import struct
from .clipper import Line
from typing import List

UNSIGNED_SHORT_MAX = 65535


class Api:
    def __init__(self, port_name: str, baudrate: int):
        self.port = serial.Serial(port_name, baudrate)

    @staticmethod
    def print_ports() -> None:
        for port in serial.tools.list_ports.comports():
            print(port)

    def send(self, lines: List[Line]) -> None:
        self.port.write(self._lines_to_serial_data(lines))

    def _lines_to_serial_data(self, lines: List[Line]) -> bytearray:
        # Flatten line coordinates to a one-dimensional list
        coords = [coord
                  for line in lines
                  for coords in line
                  for coord in coords]

        # Format coordinates as unsigned shorts
        bytes = [struct.pack('<H', self._float_to_unsigned_short(coord))
                 for coord in coords]

        # First four bytes define the length of the list
        bytes.insert(0, len(coords).to_bytes(
            4, byteorder='little', signed=True))

        return bytearray(b''.join(bytes))

    def _float_to_unsigned_short(self, value: float) -> int:
        return int(((value * 0.5) + 0.5) * UNSIGNED_SHORT_MAX)
