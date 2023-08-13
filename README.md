# TCP Client-Server Application

This repository contains an example implementation of a TCP client-server communication using C++, Python  and Protobuf.

## Dependencies

- [Protobuf](https://developers.google.com/protocol-buffers)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)
- [gtest](https://github.com/google/googletest)

## Install the dependencies

```
sudo apt-get install protobuf-compiler libprotobuf-dev
```

Make sure you have these dependencies installed before proceeding.

## Build

To build the TCP client-server example, follow these steps:

1. Clone the repository:
   ```sh
   git clone https://github.com/Keshavraj024/tcp_server_client_application.git
   cd tcp_server_client_application
   ```

2. Create a build directory and navigate to it:
   ```sh
   mkdir build
   cd build
   ```

3. Generate the build files using CMake:
   ```sh
   cmake ..
   ```

4. Build the project:
   ```sh
   make
   ```

## Run

### Server

To run the server, execute the following command from the build directory:

```sh
./server
```

The server will start listening for incoming connections.

### Client (C++)

To run the client, execute the following command from the build directory:

```sh
./client
```
The client will connect to the server and send messages.
### Client (Python)

You can also establish a heterogeneous connection using a Python client. An example Python client can be found in the `python_client` directory. To run the Python client, follow these steps:

1. Navigate to the `python_client` directory:
   ```sh
   cd python_client
   ```

2. Install the required Python dependencies:
   ```sh
   pip install -r requirements.txt
   ```

3. Run the Python client script (configurations specific to this task):
   ```sh
   python3 tcp_client.py 
   ```

4. Run the Python client script (with user-defined configurations):
   
   - configure the yaml file(config.yaml):
      ```
      server:
         address (str): The IP address the server should bind to.
         port (int): The port number on which the server will listen.
      interval (int): Time interval in seconds for periodic tasks.
      max_attempts (int): Maximum number of connection attempts.
      message_interval_ms (int): Interval in milliseconds between sending messages.
      message_to_send (str): The message to be sent by the server.
      ```
   - Run the client using
      ```sh
      python3 tcp_client.py --file_path ../config.yaml
      ```

   

The Python client will establish a connection to the server and send messages.

## Configuration

You can configure the client and server settings using a YAML configuration file found at config.yaml.

## Possible Issue

```
File "tcp_server_client_application/python_client/tcp_client.py", line 4, in <module>
    from output_pb2 import Output
  File "tcp_server_client_application/python_client/output_pb2.py", line 38, in <module>
    _descriptor.FieldDescriptor(
  File "tcp_server_client_application/python_client/venv/lib/python3.10/site-packages/google/protobuf/descriptor.py", line 561, in __new__
    _message.Message._CheckCalledFromGeneratedFile()
TypeError: Descriptors cannot not be created directly.
If this call came from a _pb2.py file, your generated code is out of date and must be regenerated with protoc >= 3.19.0.
If you cannot immediately regenerate your protos, some other possible workarounds are:
 1. Downgrade the protobuf package to 3.20.x or lower.
 2. Set PROTOCOL_BUFFERS_PYTHON_IMPLEMENTATION=python (but this will use pure-Python parsing and will be much slower)
```
Install requirments.txt should sove the issue if not then run the following command

```
pip3 install --upgrade "protobuf<=3.20.1"
```

## Running Tests
You can verify the simple tests both the TcpServer and TcpClient classes using Google Test. To run the tests, use CTest:

```
   ctest
```


| Pros | Cons |
|----------|----------|
| Efficiency and Performance: Using C++ for your server and client allows you to leverage its low-level memory management and performance optimizations, making the application more efficient and responsive. |Resource Usage: Handling multiple clients concurrently can lead to resource-intensive operations, such as managing threads or handling asynchronous events. Proper management is required to avoid resource exhaustion.  |
|Efficient Serialization: Protocol Buffers provide efficient serialization and deserialization of data, which can lead to faster communication compared to plaintext serialization formats. | Protocol Evolution: If you need to evolve your message format over time, you'll need to manage backward and forward compatibility to ensure old and new versions of your application can still communicate.| 

# References
- [Cmake Function](https://cmake.org/cmake/help/latest/command/function.html)
- [gtest](http://google.github.io/googletest/quickstart-cmake.html)
- [protobuf](https://medium.com/geekculture/protocol-buffers-in-c-d60865ae7782)
- [protobuf-version-issue](https://github.com/deepmind/alphafold/issues/478)
- [tcp-server-client](https://www.geeksforgeeks.org/socket-programming-cc/)
