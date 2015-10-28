from xbee import XBee
import serial
import time
import array
import logging
import settings
import RPi.GPIO as io


# config for serial connections
SERIAL_DEV = settings.SERIAL_DEV
BAUDRATE = settings.BAUDRATE


# Xbee ACK constants
SUCCESS = b'\x00'
NO_ACK = b'\x01'


# sample command arrays
CHASE = array.array('B', [18, 8, 5, 8, 8])
BLACK = array.array('B', b'40')


class Master(XBee):

    def __init__(self):
        super().__init__(ser=serial.Serial(SERIAL_DEV, BAUDRATE), callback=self.receive_ack)
        self.ack_received = False
        self.ack_timeout = settings.ACK_TIMEOUT
        self.max_attempts = settings.MAX_ATTEMPTS
        self.frame_counter = b'\x01'

    def send_command(self, address, command_array):
        logging.debug("Sending command to {}\nFrame ID: {}".format(address, self.frame_counter))
        logging.debug("Command array: {}".format(command_array))

        send_attempts = 0
        self.ack_received = False
        result = ''
        while not self.ack_received:
            self.send("tx", dest_addr=address, frame_id=self.frame_counter, data=command_array)

            self.increment_frame_counter()
            time_elapsed = 0
            while not self.ack_received and time_elapsed < self.ack_timeout:
                time.sleep(.001)
                time_elapsed += .001
            if self.ack_timeout <= time_elapsed:
                logging.warning("Timed out.")

            send_attempts += 1
            if send_attempts >= self.max_attempts:
                # raise NoACKException('No ACK received after {} attempts.'.format(send_attempts))
                # print('No ACK received after {} attempts.'.format(send_attempts))
                logging.warning('No ACK received after {} attempts.'.format(send_attempts))
                result = 'No response'
                break

        if self.ack_received:
            logging.debug("Command sent successfully.")
            # self.increment_frame_counter()
            result = 'Success'

        return result

    def receive_ack(self, data):
        status = data['status']
        if status == SUCCESS:
            self.ack_received = True
            # print("ACK received.\n")
            logging.debug("ACK received.")
        elif status == NO_ACK:
            # print("Sent but no ACK.")
            logging.debug("Command sent but no ACK received.")

    def increment_frame_counter(self):
        frame_number = ord(self.frame_counter)
        if frame_number >= 15:
            frame_number = 1
        self.frame_counter = bytes([frame_number + 1])

    def soft_reset(self):
        self.at(command=bytes("FR", "utf-8"), frame_id=self.frame_counter)
        # self.ser.close()
        self.ser = serial.Serial(SERIAL_DEV, BAUDRATE)

    def hard_reset(self):
        # send reset pulse on pin 24
        io.setmode(io.BCM)
        io.setup(settings.RESET_PIN, io.OUT, initial=io.LOW)
        time.sleep(.01)
        io.cleanup()

        # reconnect serial
        self.ser = serial.Serial(SERIAL_DEV, BAUDRATE)


def main():
    master = Master()

    while True:
        try:
            print("Chase")
            master.send_command(b'\x10\x60', CHASE)
            time.sleep(2)

            print("Black")
            master.send_command(b'\x10\x60', BLACK)
            time.sleep(2)

        except KeyboardInterrupt:
            break

    master.halt()
    master.serial.close()


if __name__ == '__main__':
    main()