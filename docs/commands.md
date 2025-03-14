# FTP Commands Documentation

## Overview
This document details all supported FTP commands, their usage, and response codes.

## Authentication Commands

### USER
- **Usage**: `USER <username>`
- **Description**: Specifies the user for authentication
- **Response Codes**:
  - 331: Username OK, need password
  - 530: Not logged in
- **Example**:
```
> USER anonymous
< 331 Please specify the password.
```

### PASS
- **Usage**: `PASS <password>`
- **Description**: Authenticates the user with a password
- **Response Codes**:
  - 230: User logged in
  - 530: Login incorrect
  - 500: Already logged in
- **Example**:
```
> PASS anonymous
< 230 Login successful.
```

## Directory Navigation

### CWD
- **Usage**: `CWD <path>`
- **Description**: Changes the current working directory
- **Response Codes**:
  - 250: Directory changed successfully
  - 550: Failed to change directory
  - 530: Not logged in
- **Example**:
```
> CWD /home/user
< 250 Directory successfully changed.
```

### CDUP
- **Usage**: `CDUP`
- **Description**: Changes to parent directory
- **Response Codes**:
  - 250: Directory changed successfully
  - 550: Failed to change directory
  - 530: Not logged in
- **Example**:
```
> CDUP
< 250 Directory successfully changed.
```

### PWD
- **Usage**: `PWD`
- **Description**: Prints current working directory
- **Response Codes**:
  - 257: Path returned
  - 530: Not logged in
- **Example**:
```
> PWD
< 257 "/home/user" is the current directory.
```

## File Operations

### DELE
- **Usage**: `DELE <filename>`
- **Description**: Deletes specified file
- **Response Codes**:
  - 250: File deleted successfully
  - 550: File deletion failed
  - 530: Not logged in
- **Example**:
```
> DELE test.txt
< 250 File successfully deleted.
```

### RETR
- **Usage**: `RETR <filename>`
- **Description**: Downloads specified file
- **Response Codes**:
  - 150: File status okay; opening data connection
  - 226: Transfer complete
  - 550: File unavailable
  - 530: Not logged in
- **Example**:
```
> RETR document.pdf
< 150 Opening BINARY mode data connection.
< 226 Transfer complete.
```

### STOR
- **Usage**: `STOR <filename>`
- **Description**: Uploads file to server
- **Response Codes**:
  - 150: File status okay; opening data connection
  - 226: Transfer complete
  - 550: Failed to store file
  - 530: Not logged in
- **Example**:
```
> STOR upload.txt
< 150 Opening BINARY mode data connection.
< 226 Transfer complete.
```

## Connection Modes

### PASV
- **Usage**: `PASV`
- **Description**: Enters passive mode
- **Response Codes**:
  - 227: Entering Passive Mode (h1,h2,h3,h4,p1,p2)
  - 530: Not logged in
- **Example**:
```
> PASV
< 227 Entering Passive Mode (127,0,0,1,156,124).
```

### PORT
- **Usage**: `PORT <h1,h2,h3,h4,p1,p2>`
- **Description**: Specifies address and port for active mode
- **Response Codes**:
  - 200: Command successful
  - 500: Invalid port command
  - 530: Not logged in
- **Example**:
```
> PORT 127,0,0,1,156,124
< 200 PORT command successful.
```

## Data Type

### TYPE
- **Usage**: `TYPE <type-code>`
- **Description**: Sets transfer mode (ASCII/Binary)
- **Parameters**:
  - A: ASCII mode
  - I: Binary mode
- **Response Codes**:
  - 200: Command successful
  - 500: Invalid type code
  - 530: Not logged in
- **Example**:
```
> TYPE I
< 200 Switching to Binary mode.
```

## Directory Listing

### LIST
- **Usage**: `LIST [path]`
- **Description**: Lists directory contents
- **Response Codes**:
  - 150: Opening data connection
  - 226: Transfer complete
  - 550: Failed to list directory
  - 530: Not logged in
- **Example**:
```
> LIST
< 150 Here comes the directory listing.
< 226 Directory send OK.
```

## Other Commands

### HELP
- **Usage**: `HELP [command]`
- **Description**: Shows available commands or specific command help
- **Response Codes**:
  - 214: Help information
  - 500: Command not recognized
- **Example**:
```
> HELP PASV
< 214 PASV: Enter passive mode.
```

### NOOP
- **Usage**: `NOOP`
- **Description**: No operation (keepalive)
- **Response Codes**:
  - 200: Command successful
- **Example**:
```
> NOOP
< 200 NOOP ok.
```

### QUIT
- **Usage**: `QUIT`
- **Description**: Closes the connection
- **Response Codes**:
  - 221: Goodbye
- **Example**:
```
> QUIT
< 221 Goodbye.
```

## General Response Codes

| Code | Category | Description |
|------|----------|-------------|
| 150  | File     | File status okay; about to open data connection |
| 200  | Success  | Command okay |
| 214  | Help     | Help message |
| 220  | Service  | Service ready for new user |
| 221  | Service  | Service closing control connection |
| 226  | Transfer | Closing data connection, requested file action successful |
| 227  | Passive  | Entering Passive Mode |
| 230  | Auth     | User logged in |
| 250  | File     | Requested file action okay, completed |
| 331  | Auth     | Username okay, need password |
| 500  | Error    | Syntax error, command unrecognized |
| 530  | Auth     | Not logged in |
| 550  | File     | Requested action not taken; file unavailable |