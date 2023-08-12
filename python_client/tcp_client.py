import socket
import time
from google.protobuf.timestamp_pb2 import Timestamp
from output_pb2 import Output
import logging
import argparse


class TcpClient:
    def __init__(self, server_address: str, server_port: int) -> None:
        try:
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            self.client_socket.connect((server_address, server_port))
            self.send()
        except socket.error as e:
            logging.error(f"Error during initialization: {e}")

    def create_message(self, message_id: int) -> bytes:
        """
        Create a message using Output proto.

        Args:
            message_id (int): The ID of the message.

        Returns:
            bytes: Serialized message data.
        """
        sr_test_output = Output()
        sr_test_output.id = message_id

        current_time = time.time()
        seconds = int(current_time)
        nanoseconds = int((current_time - seconds) * 1e9)

        timestamp = Timestamp()
        timestamp.seconds = seconds
        timestamp.nanos = nanoseconds
        sr_test_output.timestamp.CopyFrom(timestamp)

        sr_test_output.content = "Hello Seoul Robotics"

        sr_test_output_serialized = sr_test_output.SerializeToString()
        sr_test_output_serialized = 0

        if not sr_test_output_serialized:
            self.client_socket.close()
            raise ValueError("Serialization failed. Empty serialized data")

        return sr_test_output.SerializeToString()

    def send(self) -> None:
        """
        Send the client to send messages.
        """
        message_id = 1
        while True:
            try:
                message_data = self.create_message(message_id)
                self.client_socket.sendall(message_data)
                logging.info(f"Sent message with ID: {message_id}")
                message_id += 1
                time.sleep(0.1)
            except socket.error as e:
                self.client_socket.close()
                logging.error(f"Error while sending message: {e}")
                break

        self.client_socket.close()


if __name__ == "__main__":
    logging.basicConfig(level=logging.ERROR)

    parser = argparse.ArgumentParser(
        description="TCP Client for sending messages to server."
    )
    parser.add_argument(
        "--address", type=str, default="127.0.0.1", help="Server Address"
    )
    parser.add_argument("--port", type=int, default=8080, help="Server Port")

    args = parser.parse_args()

    client = TcpClient(args.address, args.port)
