#!/bin/bash

# Parameters
SERVER_BIN="./myftp"
SERVER_PORT=$1
SERVER_HOST="127.0.0.1"

# Colors
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color
ITALIC='\033[3m'
GREY='\033[1;30m'

############################# Display Functions #############################

print_success() {
    echo -e "${GREEN}✓ $1${NC}"
}

print_failure() {
    echo -e "${RED}✗ $1${NC}"
    echo -e "${RED}  Expected line: $2${NC}"
    echo -e "${RED}  Output: \n${ITALIC}$3${NC}"
}

print_test_header() {
    echo -e "${YELLOW}Testing $1...${NC}"
}

############################## Server Functions ##############################

# Start server
start_server() {
    if [ -z "$SERVER_PORT" ]; then
        echo -e "${RED}Error: No port specified. Usage: $0 <port>${NC}"
        exit 1
    fi
    echo -e "${GREY}Starting server on port $SERVER_PORT...${NC}"
    $SERVER_BIN $SERVER_PORT ./ > /dev/null 2>&1 &
    # $SERVER_BIN $SERVER_PORT ./ &
    SERVER_PID=$!
    sleep 0.1  # Wait for server to start
    if ! kill -0 $SERVER_PID 2>/dev/null; then
        echo -e "${RED}Error: Server failed to start or exited immediately.${NC}"
        exit 1
    fi
}

# Stop server
stop_server() {
    if [ ! -z "$SERVER_PID" ]; then
        kill $SERVER_PID 2>/dev/null
        wait $SERVER_PID 2>/dev/null
    fi
}

# Run FTP commands
run_ftp_commands() {
    if [ "$2" = "background" ]; then
        # Run in background for data connection commands
        echo -ne "$1" | timeout 1 nc $SERVER_HOST $SERVER_PORT &
        echo $!  # Return process ID
    else
        # Run with timeout for normal commands
        echo -ne "$1" | timeout 1 nc $SERVER_HOST $SERVER_PORT
    fi
}

# Parse PASV response to get IP and port
parse_pasv_response() {
    local pasv_response="$1"
    local ip_port=$(echo "$pasv_response" | grep -o "([0-9,]*)" | tr -d '()')
    echo "$ip_port"
}

# Convert PASV format (h1,h2,h3,h4,p1,p2) to IP and port
convert_pasv_to_ip_port() {
    local pasv_nums="$1"
    local IFS=','
    read -ra nums <<< "$pasv_nums"
    local ip="${nums[0]}.${nums[1]}.${nums[2]}.${nums[3]}"
    local port=$((${nums[4]} * 256 + ${nums[5]}))
    echo "$ip:$port"
}

# Connect to data socket and receive data
connect_to_data_socket() {
    local ip_port="$1"
    local ip=$(echo "$ip_port" | cut -d: -f1)
    local port=$(echo "$ip_port" | cut -d: -f2)
    timeout 1 nc "$ip" "$port"
}

################################ Test Login ################################

test_valid_login() {
    print_test_header "Valid login"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
QUIT\r
")
    if echo "$result" | grep -q "230"; then
        print_success "Valid login test passed"
    else
        print_failure "Valid login test failed" "230 Login successful" "$result"
        return 1
    fi
}

test_unauthenticated() {
    print_test_header "Unauthenticated"
    result=$(run_ftp_commands "HELP\r
QUIT\r
")
    if echo "$result" | grep -q "530"; then
        print_success "Unauthenticated test passed"
    else
        print_failure "Unauthenticated test failed" "530 Not logged in" "$result"
        return 1
    fi
}

################################# Test USER #################################

test_invalid_user() {
    print_test_header "Invalid user"
    result=$(run_ftp_commands "USER invalid\r
PASS\r
QUIT\r
")
    if echo "$result" | grep -q "530"; then
        print_success "Invalid user test passed"
    else
        print_failure "Invalid user test failed"
        return 1
    fi
}

################################# Test PASS #################################

test_invalid_pass() {
    print_test_header "Invalid password"
    result=$(run_ftp_commands "USER anonymous\r
PASS invalid\r
QUIT\r
")
    if echo "$result" | grep -q "530"; then
        print_success "Invalid password test passed"
    else
        print_failure "Invalid password test failed" "530 Login incorrect" "$result"
        return 1
    fi
}

################################# Test HELP #################################

test_help_command() {
    print_test_header "HELP command"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
HELP\r
QUIT\r
")
    if echo "$result" | grep -q "214"; then
        print_success "HELP command test passed"
    else
        print_failure "HELP command test failed" "214 Help message" "$result"
        return 1
    fi
}

################################# Test QUIT #################################

test_quit_command() {
    print_test_header "QUIT command"
    result=$(run_ftp_commands "quit\r
")
    if echo "$result" | grep -q "221"; then
        print_success "QUIT command test passed"
    else
        print_failure "QUIT command test failed" "221 Goodbye" "$result"
        return 1
    fi
}

################################# Test CWD #################################

test_cwd_command(){
    print_test_header "CWD command"
    result=$(run_ftp_commands "USER anonymous\r
PASS \r
CWD ..\r
QUIT\r
")
    if echo "$result" | grep -q "250"; then
        print_success "CWD command test passed"
    else
        print_failure "CWD command test failed" "250 Changed directory" "$result"
        return 1
    fi
}

test_cwd_command_invalid(){
    print_test_header "CWD command invalid"
    result=$(run_ftp_commands "USER anonymous\r
PASS \r
CWD invalid_directory\r
QUIT\r
")
    if echo "$result" | grep -q "550"; then
        print_success "CWD command invalid test passed"
    else
        print_failure "CWD command invalid test failed" "550 Failed to change directory" "$result"
        return 1
    fi
}

################################# Test PWD #################################

test_pwd_command(){
    print_test_header "PWD command"
    result=$(run_ftp_commands "USER anonymous\r
PASS \r
PWD\r
QUIT\r
")
    if echo "$result" | grep -q "257"; then
        print_success "PWD command test passed"
    else
        print_failure "PWD command test failed" "257 Current directory" "$result"
        return 1
    fi
}

################################# Test CDUP #################################

test_cdup_command(){
    print_test_header "CDUP command"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
CDUP\r
QUIT\r
")
    if echo "$result" | grep -q "250"; then
        print_success "CDUP command test passed"
    else
        print_failure "CDUP command test failed" "250: Directory changed successfully" "$result"
        return 1
    fi
}

################################# Test NOOP #################################

test_noop_command(){
    print_test_header "NOOP command"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
NOOP\r
QUIT\r
")
    if echo "$result" | grep -q "200"; then
        print_success "NOOP command test passed"
    else
        print_failure "NOOP command test failed" "200 No operation" "$result"
        return 1
    fi
}

################################# Test DELE #################################

test_dele_command(){
    print_test_header "DELE command"
    # Create a test file
    touch test.txt
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
DELE test.txt\r
QUIT\r
")
    if echo "$result" | grep -q "250"; then
        print_success "DELE command test passed"
    else
        print_failure "DELE command test failed" "250 Deleted file" "$result"
        return 1
    fi
}

test_dele_command_invalid(){
    print_test_header "DELE command invalid"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
DELE invalid_file.txt\r
QUIT\r
")
    if echo "$result" | grep -q "550"; then
        print_success "DELE command invalid test passed"
    else
        print_failure "DELE command invalid test failed" "550 Failed to delete file" "$result"
        return 1
    fi
}

################################# Test PASV #################################

test_pasv_command(){
    print_test_header "PASV command"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
PASV\r
QUIT\r
")
    if echo "$result" | grep -q "227"; then
        print_success "PASV command test passed"
    else
        print_failure "PASV command test failed" "227 Entering Passive Mode" "$result"
        return 1
    fi
}

################################# Test PORT #################################

test_port_command(){
    print_test_header "PORT command"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
PORT 127,0,0,1,10,20\r
QUIT\r
")
    if echo "$result" | grep -q "200"; then
        print_success "PORT command test passed"
    else
        print_failure "PORT command test failed" "200 Port command successful" "$result"
        return 1
    fi
}

test_port_command_invalid(){
    print_test_header "PORT command invalid"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
PORT invalid\r
QUIT\r
")
    if echo "$result" | grep -q "500"; then
        print_success "PORT command invalid test passed"
    else
        print_failure "PORT command invalid test failed" "500 Syntax error" "$result"
        return 1
    fi
}

################################# Test LIST #################################

test_list_command(){
    print_test_header "LIST command"

    # Create a temporary file to store PASV response
    local pasv_file=$(mktemp)

    # Start control connection and get PASV info
    # Using process substitution to capture the server's response in real-time
    exec 3> >( nc $SERVER_HOST $SERVER_PORT > "$pasv_file" )

    # Send login commands and PASV
    echo -e "USER anonymous\r" >&3
    sleep 0.1
    echo -e "PASS\r" >&3
    sleep 0.1
    echo -e "PASV\r" >&3
    sleep 0.1  # Give server time to respond with PASV info

    # Read response so far
    pasv_output=$(cat "$pasv_file")

    # Parse PASV response
    pasv_nums=$(parse_pasv_response "$pasv_output")
    if [ -z "$pasv_nums" ]; then
        print_failure "LIST command failed - Cannot parse PASV response" "227 Entering Passive Mode" "$pasv_output"
        exec 3>&-  # Close FD 3
        rm -f "$pasv_file"
        return 1
    fi

    # Convert to IP and port
    ip_port=$(convert_pasv_to_ip_port "$pasv_nums")
    ip=$(echo "$ip_port" | cut -d: -f1)
    port=$(echo "$ip_port" | cut -d: -f2)

    # Start data connection in background
    data_file=$(mktemp)
    timeout 2 nc "$ip" "$port" > "$data_file" &
    data_pid=$!

    # Small delay to ensure the data connection is established
    sleep 0.2

    # Now send LIST on the same control connection
    echo -e "LIST\r" >&3
    sleep 1  # Give server time to send the directory listing

    # Send QUIT to close the connection
    echo -e "QUIT\r" >&3
    exec 3>&-  # Close FD 3

    # Wait for data connection to finish
    wait $data_pid 2>/dev/null

    # Read the data that was received
    data=$(cat "$data_file")

    # Clean up
    rm -f "$pasv_file" "$data_file"

    # Check if we got any data
    if [ ! -z "$data" ]; then
        print_success "LIST command test passed"
        return 0
    else
        print_failure "LIST command failed - No data received" "Directory listing" "$pasv_output"
        return 1
    fi
}

test_list_command_directory(){
    print_test_header "LIST command directory"

    # Create a temporary file to store PASV response
    local pasv_file=$(mktemp)

    # Start control connection and get PASV info
    # Using process substitution to capture the server's response in real-time
    exec 3> >( nc $SERVER_HOST $SERVER_PORT > "$pasv_file" )

    # Send login commands and PASV
    echo -e "USER anonymous\r" >&3
    sleep 0.1
    echo -e "PASS\r" >&3
    sleep 0.1
    echo -e "PASV\r" >&3
    sleep 0.1

    # Read response so far
    pasv_output=$(cat "$pasv_file")

    # Parse PASV response
    pasv_nums=$(parse_pasv_response "$pasv_output")
    if [ -z "$pasv_nums" ]; then
        print_failure "LIST command failed - Cannot parse PASV response" "227 Entering Passive Mode" "$pasv_output"
        exec 3>&-  # Close FD 3
        rm -f "$pasv_file"
        return 1
    fi

    # Convert to IP and port
    ip_port=$(convert_pasv_to_ip_port "$pasv_nums")
    ip=$(echo "$ip_port" | cut -d: -f1)
    port=$(echo "$ip_port" | cut -d: -f2)

    # Start data connection in background
    data_file=$(mktemp)
    timeout 2 nc "$ip" "$port" > "$data_file" &
    data_pid=$!

    # Small delay to ensure the data connection is established
    sleep 0.2

    # Now send LIST on the same control connection
    echo -e "LIST src\r" >&3
    sleep 1

    # Send QUIT to close the connection
    echo -e "QUIT\r" >&3
    exec 3>&-  # Close FD 3

    # Wait for data connection to finish
    wait $data_pid 2>/dev/null

    # Read the data that was received
    data=$(cat "$data_file")

    # Clean up
    rm -f "$pasv_file" "$data_file"
    # Check if we got any data
    if [ ! -z "$data" ]; then
        print_success "LIST command directory test passed"
        return 0
    else
        print_failure "LIST command failed - No data received" "Directory listing" "$pasv_output"
        return 1
    fi
}

test_list_command_invalid(){
    print_test_header "LIST command invalid"

    # Create a temporary file to store PASV response
    local pasv_file=$(mktemp)
    # Start control connection and get PASV info
    # Using process substitution to capture the server's response in real-time
    exec 3> >( nc $SERVER_HOST $SERVER_PORT > "$pasv_file" )
    # Send login commands and PASV
    echo -e "USER anonymous\r" >&3
    sleep 0.1
    echo -e "PASS\r" >&3
    sleep 0.1
    echo -e "CWD invalid_directory\r" >&3  # Invalid directory for LIST
    sleep 0.1
    echo -e "PASV\r" >&3
    sleep 0.1

    # Read response so far
    pasv_output=$(cat "$pasv_file")

    # Parse PASV response
    pasv_nums=$(parse_pasv_response "$pasv_output")
    if [ -z "$pasv_nums" ]; then
        print_failure "LIST command failed - Cannot parse PASV response" "227 Entering Passive Mode" "$pasv_output"
        exec 3>&-  # Close FD 3
        rm -f "$pasv_file"
        return 1
    fi

    # Convert to IP and port
    ip_port=$(convert_pasv_to_ip_port "$pasv_nums")
    ip=$(echo "$ip_port" | cut -d: -f1)
    port=$(echo "$ip_port" | cut -d: -f2)

    # Start data connection in background
    data_file=$(mktemp)
    timeout 2 nc "$ip" "$port" > "$data_file" &
    data_pid=$!

    # Small delay to ensure the data connection is established
    sleep 0.2

    # Send LIST on the same control connection
    echo -e "LIST\r" >&3
    sleep 1

    # Send QUIT to close the connection
    echo -e "QUIT\r" >&3
    exec 3>&-  # Close FD 3

    # Wait for data connection to finish
    wait $data_pid 2>/dev/null

    # Read the data that was received
    data=$(cat "$data_file")
    rm -f "$pasv_file" "$data_file"

    # Check if we received error message (550)
    if echo "$pasv_output" | grep -q "550"; then
        print_success "LIST command invalid test passed"
        return 0
    else
        print_failure "LIST command invalid test failed" "550 Failed to access directory" "$pasv_output"
        return 1
    fi
}

test_list_command_no_data_socket(){
    print_test_header "LIST command no data socket"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
LIST\r
QUIT\r
")
    if echo "$result" | grep -q "425"; then
        print_success "LIST command no data socket test passed"
    else
        print_failure "LIST command no data socket test failed" "425 Can't open data connection" "$result"
        return 1
    fi
}

################################# Test RETR #################################
test_retr_command(){
    print_test_header "RETR command"

    # Create a temporary file to store PASV response
    local pasv_file=$(mktemp)
    # Start control connection and get PASV info
    # Using process substitution to capture the server's response in real-time
    exec 3> >( nc $SERVER_HOST $SERVER_PORT > "$pasv_file" )
    # Create a test file
    echo "This is a test file" > test_file.txt

    # Send login commands and PASV
    echo -e "USER anonymous\r" >&3
    sleep 0.1
    echo -e "PASS\r" >&3
    sleep 0.1
    echo -e "PASV\r" >&3
    sleep 0.1

    # Read response so far
    pasv_output=$(cat "$pasv_file")

    # Parse PASV response
    pasv_nums=$(parse_pasv_response "$pasv_output")
    if [ -z "$pasv_nums" ]; then
        print_failure "RETR command failed - Cannot parse PASV response" "227 Entering Passive Mode" "$pasv_output"
        exec 3>&-  # Close FD 3
        rm -f "$pasv_file"
        return 1
    fi

    # Convert to IP and port
    ip_port=$(convert_pasv_to_ip_port "$pasv_nums")
    ip=$(echo "$ip_port" | cut -d: -f1)
    port=$(echo "$ip_port" | cut -d: -f2)

    # Start data connection in background
    data_file=$(mktemp)
    timeout 2 nc "$ip" "$port" > "$data_file" &
    data_pid=$!

    # Small delay to ensure the data connection is established
    sleep 0.2

    # Now send RETR on the same control connection
    echo -e "RETR test_file.txt\r" >&3
    sleep 1

    # Send QUIT to close the connection
    echo -e "QUIT\r" >&3
    exec 3>&-  # Close FD 3

    # Wait for data connection to finish
    wait $data_pid 2>/dev/null

    # Read the data that was received
    data=$(cat "$data_file")

    # Clean up
    rm -f "$pasv_file" "$data_file" "test_file.txt"

    # Check if we got any data
    if [ ! -z "$data" ] && [[ "$data" == *"This is a test file"* ]]; then
        print_success "RETR command test passed"
        return 0
    else
        print_failure "RETR command failed - Data received does not match" "This is a test file" "$data"
        return 1
    fi
}

test_retr_command_invalid(){
    print_test_header "RETR command invalid"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
PASV\r
RETR invalid_file.txt\r
QUIT\r
}
")
    if echo "$result" | grep -q "550"; then
        print_success "RETR command invalid test passed"
    else
        print_failure "RETR command invalid test failed" "550 Failed to retrieve file" "$result"
        return 1
    fi
}

test_retr_command_no_data_socket(){
    print_test_header "RETR command no data socket"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
RETR test_file.txt\r
QUIT\r
")
    if echo "$result" | grep -q "425"; then
        print_success "RETR command no data socket test passed"
    else
        print_failure "RETR command no data socket test failed" "425 Can't open data connection" "$result"
        return 1
    fi
}
test_retr_command_no_file(){
    print_test_header "RETR command no file"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
PASV\r
RETR\r
QUIT\r
")
    if echo "$result" | grep -q "550"; then
        print_success "RETR command no file test passed"
    else
        print_failure "RETR command no file test failed" "550 Failed to retrieve file" "$result"
        return 1
    fi
}

################################# Test STOR #################################

test_stor_command(){
    print_test_header "STOR command"

    # Create a temporary file to store PASV response
    local pasv_file=$(mktemp)
    # Start control connection and get PASV info
    # Using process substitution to capture the server's response in real-time
    exec 3> >( nc $SERVER_HOST $SERVER_PORT > "$pasv_file" )
    # Create a test file
    echo "This is a test file" > test_file.txt

    # Send login commands and PASV
    echo -e "USER anonymous\r" >&3
    sleep 0.1
    echo -e "PASS\r" >&3
    sleep 0.1
    echo -e "PASV\r" >&3
    sleep 0.1

    # Read response so far
    pasv_output=$(cat "$pasv_file")

    # Parse PASV response
    pasv_nums=$(parse_pasv_response "$pasv_output")
    if [ -z "$pasv_nums" ]; then
        print_failure "STOR command failed - Cannot parse PASV response" "227 Entering Passive Mode" "$pasv_output"
        exec 3>&-  # Close FD 3
        rm -f "$pasv_file"
        return 1
    fi

    # Convert to IP and port
    ip_port=$(convert_pasv_to_ip_port "$pasv_nums")
    ip=$(echo "$ip_port" | cut -d: -f1)
    port=$(echo "$ip_port" | cut -d: -f2)

    # Start data connection in background
    data_file=$(mktemp)
    timeout 2 nc "$ip" "$port" < test_file.txt &
    data_pid=$!

    # Small delay to ensure the data connection is established
    sleep 0.2

    # Now send STOR on the same control connection
    echo -e "STOR test_file.txt\r" >&3
    sleep 1

    # Send QUIT to close the connection
    echo -e "QUIT\r" >&3
    exec 3>&-  # Close FD 3

    # Wait for data connection to finish
    wait $data_pid 2>/dev/null

    # Clean up
    rm -f "$pasv_file" "$data_file"
    # Check if the file was stored successfully
    if [ -f "test_file.txt" ]; then
        rm -f "test_file.txt"
        print_success "STOR command test passed"
        return 0
    else
        print_failure "STOR command failed - File not stored" "File test_file.txt" "$pasv_output"
        return 1
    fi
}

test_stor_command_no_data_socket(){
    print_test_header "STOR command no data socket"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
STOR test_file.txt\r
QUIT\r
")
    if echo "$result" | grep -q "425"; then
        print_success "STOR command no data socket test passed"
    else
        print_failure "STOR command no data socket test failed" "425 Can't open data connection" "$result"
        return 1
    fi
}

test_stor_command_no_file(){
    print_test_header "STOR command no file"
    result=$(run_ftp_commands "USER anonymous\r
PASS\r
PASV\r
STOR\r
QUIT\r
")
    if echo "$result" | grep -q "550"; then
        print_success "STOR command no file test passed"
    else
        print_failure "STOR command no file test failed" "550 Failed to store file" "$result"
        return 1
    fi
}

################################ Main Test Runner ############################

# Main test runner
run_tests() {
    local failed=0

    start_server

    test_valid_login || failed=1
    test_unauthenticated || failed=1
    test_invalid_user || failed=1
    test_invalid_pass || failed=1
    test_help_command || failed=1
    test_quit_command || failed=1
    test_cwd_command || failed=1
    test_cwd_command_invalid || failed=1
    test_pwd_command || failed=1
    test_cdup_command || failed=1
    test_noop_command || failed=1
    test_dele_command || failed=1
    test_dele_command_invalid || failed=1
    test_pasv_command || failed=1
    test_port_command || failed=1
    test_port_command_invalid || failed=1
    test_list_command || failed=1
    test_list_command_directory || failed=1
    test_list_command_invalid || failed=1
    test_list_command_no_data_socket || failed=1
    test_retr_command || failed=1
    test_retr_command_invalid || failed=1
    test_retr_command_no_data_socket || failed=1
    test_retr_command_no_file || failed=1
    test_stor_command || failed=1
    test_stor_command_no_data_socket || failed=1
    test_stor_command_no_file || failed=1

    if [ $failed -eq 0 ]; then
        echo "All tests passed! 🎉"
        exit 0
    else
        echo "Some tests failed! 😢"
        exit 1
    fi
}

# Cleanup on script exit
trap stop_server EXIT

# Run all tests
run_tests