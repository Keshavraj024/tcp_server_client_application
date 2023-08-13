import socket
import time
from google.protobuf.timestamp_pb2 import Timestamp
from output_pb2 import Output
import logging
import argparse
import yaml


class TcpClient:
    def __init__(self, file_path=str) -> None:
        try:
            config = self.read_config(file_path)
            if not config:
                return
            server_address = config["server"]["address"]
            server_port = config["server"]["port"]
            self.message_interval_ms = config["message_interval_ms"]
            self.message_to_send = config["message_to_send"]
            max_attempts = config["max_attempts"]
            interval = config["interval"]

            # end index to slice the message at for variable lengths
            self.end_index = 0

            self.init_connection(server_address, server_port, max_attempts, interval)
            self.send()

        except socket.error as e:
            logging.error(f"Error during initialization: {e}")

    def init_connection(
        self, server_address: str, server_port: int, max_attempts: int, interval: int
    ) -> None:
        """
        Initialize a connection to the server with reconnection attempts.

        This function attempts to establish a connection to the specified server address
        and port. If the connection fails, it retries for a maximum number of attempts
        with a specified interval between attempts.

        Args:
            server_address (str): The IP address or hostname of the server.
            server_port (int): The port number of the server.
            max_attempts (int): The maximum number of connection attempts.
            interval (float): The time interval (in seconds) between connection attempts.

        Returns:
            bool: True if the connection is successfully established, False otherwise.
        """
        reconnection_attempts = 0
        connection_successful = False

        while reconnection_attempts < max_attempts and not connection_successful:
            try:
                self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
                self.client_socket.connect((server_address, server_port))
                connection_successful = True
            except socket.error as e:
                logging.error(f"Error during initialization: {e}. Retrying.....")
                time.sleep(interval)
                reconnection_attempts += 1

    def read_config(self, file_path: str) -> dict:
        """
        Read the configuration from the specified YAML file.

        Args:
            file_path (str): Path to the YAML configuration file.

        Returns:
            dict: Dictionary containing the configuration data.
        """
        try:
            with open(file_path, "r") as config_file:
                config = yaml.safe_load(config_file)
                return config
        except FileNotFoundError:
            print("Error: Config file not found:", file_path)
            return None
        except yaml.YAMLError as e:
            print(f"Error while reading config: {e}")
            return None

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
        nanoseconds = int((current_time - seconds) * 1e3)

        timestamp = Timestamp()
        timestamp.seconds = seconds
        timestamp.nanos = nanoseconds
        sr_test_output.timestamp.CopyFrom(timestamp)

        if self.end_index == len(self.message_to_send):
            self.end_index = 0
        sr_test_output.content = self.message_to_send[0 : self.end_index]
        self.end_index += 1

        sr_test_output_serialized = sr_test_output.SerializeToString()

        if not sr_test_output_serialized:
            self.client_socket.close()
            raise ValueError("Serialization failed. Empty serialized data")

        return sr_test_output.SerializeToString()

    def send(self) -> None:
        """
        Send the client to send messages.
        """
        message_id = 0
        while True:
            try:
                message_data = self.create_message(message_id)
                message_size = len(message_data)
                self.client_socket.sendall(message_size.to_bytes(8, byteorder="little"))
                self.client_socket.sendall(message_data)

                logging.info(f"Sent message with ID: {message_id}")
                message_id += 1
                time.sleep(self.message_interval_ms / 1000)
            except socket.error as e:
                self.client_socket.close()
                logging.error(f"Error while sending message: {e}")
                break

        self.client_socket.close()


if __name__ == "__main__":
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser(
        description="TCP Client for sending messages to server."
    )
    parser.add_argument(
        "--file_path",
        type=str,
        default="../config.yaml",
        help="Path to YAML configuration file",
    )
    args = parser.parse_args()

    client = TcpClient(args.file_path)
