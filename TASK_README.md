# Coding Assignment for Seoul Robotics Project Engineer

## Task description

Thank you for your interest in joining our journey. The goal of the assignment is to develop a pair of server and client applications using TCP protocol. C++ is a must and python is an extra for this assignment. You can refer to any public materials (e.g., open source) as well as integrate third-party libraries if needed. You would need to specify all the references if exist.

For simplicity, the data flow is one-directional: the server only receives data from the client, and the client only sends data to the server every 100ms.

### Client application

1. Create a TCP client sending any data every 100ms.

1. Generate a protocol buffer class of [output.proto](./output.proto).

1. Fill in the message `sr_test::Output` with the following contents:

    1. `timestamp` is the current timestamp.

    1. `id` starts from 0 and increases every time.

    1. `content` can contain any text.

1. Serialize and send `sr_test::Output` to the TCP server

### Server application

1. Create a TCP server waiting for incoming packets from a TCP client. It should be able to receive messages of variable lengths.

1. Deserialize and print out the serialized `sr_test::Output` message.

1. Enable the server to reconnect to the client when it restarts.

### Extra tasks

1. Write either server or client in python for heterogeneous communication (e.g., C++ server <> Python client.) Please note that you would need to keep the code files in both C++ and python.

## Requirements

1. We care a lot about code quality. Please submit us a clear, well-organized repository.

1. Please include a readme which contains how to build and the references. It would be great if you explain your approach with pros and cons in the document.

1. Please DO NOT share the assignment. We want to have a fair recruitment process.

## Copyright

All rights reserved by Seoul Robotics, 2022.