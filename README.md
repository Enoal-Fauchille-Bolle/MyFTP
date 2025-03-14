# MyFTP Server

![Language](https://img.shields.io/badge/Language-C-blue)
![Network](https://img.shields.io/badge/Network-TCP%2FIP-brightgreen)
![Protocol](https://img.shields.io/badge/Protocol-FTP%20RFC959-yellow)
![Build](https://img.shields.io/badge/Build-Makefile-orange)
![POSIX](https://img.shields.io/badge/POSIX-Compliant-gold)
![Socket](https://img.shields.io/badge/Socket-Berkeley-red)
![Architecture](https://img.shields.io/badge/Architecture-Client%2FServer-lightblue)

[![Last Commit](https://img.shields.io/github/last-commit/Enoal-Fauchille-Bolle/MyFTP)](https://github.com/Enoal-Fauchille-Bolle/MyFTP/commits/master)
![Repo Size](https://img.shields.io/github/repo-size/Enoal-Fauchille-Bolle/MyFTP)
![License](https://img.shields.io/github/license/Enoal-Fauchille-Bolle/MySokoban)
![Lines of Code](https://tokei.rs/b1/github/Enoal-Fauchille-Bolle/MyFTP)
[![wakatime](https://wakatime.com/badge/user/018bbded-63e4-4a01-bd5e-21d050739656/project/116ad2d1-337b-4271-b42f-01cf796f5d9d.svg)](https://wakatime.com/badge/user/018bbded-63e4-4a01-bd5e-21d050739656/project/116ad2d1-337b-4271-b42f-01cf796f5d9d)


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

## Commands
See the detailed [commands documentation](docs/commands.md) for a list of supported FTP commands and their usage.

## Architecture
See the detailed [architecture documentation](docs/architecture.md) for a visual representation of the system components.

## Protocols
See the detailed [protocols documentation](docs/protocols.md) for a summary of the FTP protocol and its components.

## RFC959 Compliance
See the detailed [RFC959 documentation](docs/RFC959.md) for a summary of the protocol and its requirements.

## Bonus: FTP Client
The project also includes a bonus FTP client that can be used to connect to the server. The client is designed to be simple and easy to use, allowing you to test the server's functionality.
See the detailed [client documentation](docs/client.md) for usage instructions.

## License
This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
