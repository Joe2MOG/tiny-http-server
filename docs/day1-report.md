Day 1 Report: Minimal HTTP Server in C
Project: Tiny HTTP Server
Goal: Build a modular, well named, Betty compliant HTTP/1.1 server from scratch using only C and POSIX sockets.
Date: (fill in your date)

1. Overview
On Day 1, I built a bare bones HTTP/1.1 server that listens on a given port, accepts TCP connections, sends a static 200 OK response, and closes the connection. The code was written following strict Clean Code naming principles and formatted in Betty style, then split into modular source files to comply with the 5 function per file rule.
The server currently:
•	Creates an IPv4 TCP socket
•	Binds to a user specified port
•	Listens for incoming connections
•	Accepts a client and logs its IP address
•	Sends a minimal HTTP response (Hello from your C web server!)
•	Runs indefinitely until interrupted
The real HTTP request parsing and file serving will be added on Days 2–3.

2. Files & Project Structure
text
tiny-http-server/
├── .gitignore
├── Makefile
├── README.md
└── src/
    ├── main.c
    ├── server.c
    ├── server.h
    ├── response.c
    ├── response.h
    ├── socket_utils.c
    └── socket_utils.h
Why modular?
•	Each file handles one clear area (socket setup, response building, server loop, entry point).
•	Betty’s “max 5 functions per file” rule is satisfied.
•	Extensibility: tomorrow I can add a request_parser.c without touching existing code.

3. Code Architecture (The Story)
Reading the source top down tells the story:
1.	main.c – parse the port argument, call create_listening_socket(), then run_accept_loop().
2.	socket_utils.c – implement the TCP server lifecycle:
socket() → setsockopt(SO_REUSEADDR) → bind() → listen().
These are wrapped into tiny, well named functions.
3.	server.c – run_accept_loop() continuously accepts clients; for each, calls handle_client().
4.	response.c – handle_client() uses build_http_response() and send_http_response() to craft and deliver the static reply.
Every function has a Betty doc comment (/** ... */) explaining its purpose, parameters, and return value.


4. Clean Code & Naming Conventions
All variable and function names follow the “Tick Boxes for Meaningful Naming”:
Principle	How Applied
No encodings	No fd, addr, sz – use full words: socket, address, size
Pronounceable	Every name can be read aloud (e.g., server_socket)
Searchable	No single letter names outside tiny loops (none exist yet)
Consistent vocabulary	Always socket (not fd), address (not addr), client/server
Intention revealing	enable_reuse instead of opt, content_length instead of magic 28
Verb phrase functions	create_listening_socket, handle_client, build_http_response
Constants are named (HTTP_RESPONSE_BUFFER_SIZE). Magic numbers are eliminated.


5. Betty Style Compliance
Each .c file passes 0 errors, 0 warnings under betty. To achieve this:
•	Functions ≤ 40 lines
•	No more than 5 functions per source file (hence the modular split)
•	K&R indentation, 80 column maximum, proper brace placement
•	Betty style doc comments on every function and at the top of files (where applicable)
The Makefile includes a betty target to check all sources with a single command.


6. Build System (Makefile)
make
CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic -std=gnu89 -g
SRC = src/main.c src/server.c src/socket_utils.c src/response.c
OBJ = $(SRC:.c=.o)
TARGET = server

all: $(TARGET)
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -f $(OBJ) $(TARGET)
betty:
	betty src/*.c src/*.h
.PHONY: all clean betty
Key flags:
•	-Wall -Wextra -Werror treat all warnings as errors.
•	-pedantic -std=gnu89 enforce strict C89 with GNU extensions (matching the course standard).
•	-g embeds debug symbols for GDB (Day 4).


7. Version Control (Git)
•	Initialised a .gitignore to exclude binaries (server, *.o) and editor temp files.
•	Committed every logical step with descriptive messages.
•	Pushed the full Day 1 work to GitHub, creating a clean public portfolio.


8. Testing & Verification
Tested with two tools:
1.	curl -v http://localhost:8080/ – verified the HTTP status line, headers, and body.
2.	telnet localhost 8080 – manually typed a raw HTTP request and saw the exact bytes returned.
Both confirmed the server responds correctly.


9. Key System Calls Used
Call	Purpose
socket()	Create a communication endpoint
setsockopt()	Set SO_REUSEADDR to avoid “Address already in use”
bind()	Assign an IP address and port to the socket
listen()	Mark the socket as passive (ready to accept connections)
accept()	Block until a client connects, return a new socket for that client
send()	Write data to the client socket
close()	Release the socket
All calls are wrapped with error checking; failures print a message and exit or continue gracefully.


10. Lessons Learned
•	How the TCP server lifecycle works at the system call level.
•	The difference between host byte order and network byte order (htons).
•	Why SO_REUSEADDR is essential for quick server restarts.
•	The importance of checking every return value in C (no exceptions!).
•	How to apply Clean Code naming to C, even when the language lacks classes.
•	How to structure a multi file C project that passes Betty.


11. What’s Next (Day 2)
Tomorrow I will:
•	Read the incoming HTTP request using recv()
•	Parse the request line (method, path, version)
•	Handle edge cases (empty requests, large requests, malformed input)
•	Print the parsed method and path (still serving a static response for now)
•	Refactor into a new request_parser.c module if needed


Commit that closes Day 1:
text
Complete Day 1: modular Betty-compliant HTTP server with clean naming, Makefile, and .gitignore

