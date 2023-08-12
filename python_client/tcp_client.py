import socket
import time
from google.protobuf.timestamp_pb2 import Timestamp
from output_pb2 import Output

SERVER_ADDRESS = "127.0.0.1"
SERVER_PORT = 8080


def create_message(message_id):
    message = Output()
    message.id = message_id

    current_time = time.time()
    seconds = int(current_time)
    nanoseconds = int((current_time - seconds) * 1e9)

    timestamp = Timestamp()
    timestamp.seconds = seconds
    timestamp.nanos = nanoseconds
    message.timestamp.CopyFrom(timestamp)

    message.content = "Hello Seoul Robotics"

    return message.SerializeToString()


def main():
    try:
        client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client_socket.connect((SERVER_ADDRESS, SERVER_PORT))

        message_id = 1
        while True:
            message_data = create_message(message_id)

            try:
                client_socket.sendall(message_data)

                print(f"Sent message with ID: {message_id}")
                message_id += 1

                time.sleep(0.1)
            except Exception as e:
                print(f"Error while sending message: {e}")
                break
    except Exception as ex:
        print(f"Failed due to {ex}")

    client_socket.close()


if __name__ == "__main__":
    main()
