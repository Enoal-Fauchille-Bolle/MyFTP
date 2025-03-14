# MyFTP Server

## Project Description
My FTP Server is an FTP server project built in C that implements the FTP protocol as defined in RFC959. The project supports handling multiple client connections and long-running processes.

## Build Instructions
The project is built using a Makefile. You can compile the server using the following commands:
- `make` - Compile the project.
- `make re` - Recompile the project.
- `make clean` - Remove intermediate files.
- `make fclean` - Remove all generated files, including the binary.

## Usage
To run the FTP server, use the following command:
```
./myftp <port> <path>
```
- `<port>` is the port number on which the server listens.
- `<path>` is the path to the home directory for the anonymous user.

## Handling Multiple Clients
The server uses `poll()` to monitor multiple file descriptors for incoming data and connections. For each accepted connection, the server uses `fork()` (or plans to use it) to handle data transfers in a separate process. This design ensures that even when a time-consuming operation is running for one client, the server can continue to accept and process new connections.

## Protocol Compliance
The FTP server is designed to be compliant with RFC959. It implements the required command sequences and follows the protocol details, ensuring compatibility with standard FTP clients. Bonus features may also extend its functionality closer to full RFC959 compliance.

## Testing
You can test the server using any standard FTP client or even telnet. For example:
- Start the server:
  ```
  ./myftp 4242 /path/to/home
  ```
- Connect using telnet:
  ```
  telnet 127.0.0.1 4242
  ```
This will allow you to observe the server's responses and verify that it properly accepts connections, processes commands, and handles multiple clients.

## Architecture
See the detailed [architecture documentation](docs/architecture.md) for a visual representation of the system components.

## Commands
See the detailed [commands documentation](docs/commands.md) for a list of supported FTP commands and their usage.

## RFC959 Compliance
See the detailed [RFC959 documentation](docs/RFC959.md) for a summary of the protocol and its requirements.

## Bonus: FTP Client
The project also includes a bonus FTP client that can be used to connect to the server. The client is designed to be simple and easy to use, allowing you to test the server's functionality.
See the detailed [client documentation](docs/client.md) for usage instructions.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
