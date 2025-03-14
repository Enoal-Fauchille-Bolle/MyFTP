# MyFTP Architecture

## Components Overview

This diagram shows the main components of the FTP server and their interactions.

```mermaid
graph TB
    subgraph Main
        A[Main Program]
        B[Signal Handler]
    end

    subgraph Server
        C[Server Socket Setup]
        D[Connection Manager]
    end

    subgraph Client Handler
        E[Client Handler]
        F[Command Parser]
        G[Command Executor]
    end

    subgraph Data Transfer
        H[Passive Mode Socket]
        I[Active Mode Socket]
        J[Data Socket Executor]
    end

    subgraph Commands
        K[FTP Commands]
        subgraph Authentication
            K1[USER]
            K2[PASS]
        end
        subgraph Navigation
            K3[CWD]
            K4[CDUP]
            K5[PWD]
        end
        subgraph Transfer
            K6[RETR]
            K7[STOR]
            K8[LIST]
        end
        subgraph Connection
            K9[PASV]
            K10[PORT]
            K11[QUIT]
        end
    end

    subgraph Utils
        L[String Utils]
        M[Port Utils]
        N[Resource Cleanup]
    end

    %% Main connections
    A -->|Setup| B
    A -->|Initialize| C
    C -->|Process| D

    %% Client handling
    D -->|Handle Client| E
    E -->|Parse Input| F
    F -->|Execute| G

    %% Command execution
    G -->|Route Command| K
    K -->|Use| H
    K -->|Use| I
    K -->|Execute Transfer| J

    %% Utils usage
    K -->|Use| L
    K -->|Use| M
    D -->|Cleanup| N
    E -->|Cleanup| N

    %% Data flow
    H -->|Transfer Data| J
    I -->|Transfer Data| J

```

## Components Description

### Main Program
- Entry point of the application
- Handles server initialization and signal setup

### Server
- Manages socket configuration
- Handles client connections

### Client Handler
- Manages client communication
- Implements FTP protocol logic
- Parses and executes client commands

### Data Transfer
- Handles file transfers in both active and passive modes
- Manages data socket connections
- Provides data streaming capabilities

### Commands
- Implements standard FTP commands
- Handles authentication, navigation, and file operations
- Manages connection state changes

### Utils
- Provides helper functions for string manipulation
- Handles port calculations and socket operations
- Manages proper resource cleanup
