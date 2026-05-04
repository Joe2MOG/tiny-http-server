### Day 5 Report: Final Enhancements and Professional Polish

Project: Tiny HTTP Server
Goal: Elevate the server to a professional-grade HTTP/1.1 web server
with robust error handling, CGI support, and complete documentation.
Date: 5/04/2026

What Was Built

On the final day, I added the following features to bring the server
in line with the textbook "Tiny" server and beyond:

Robust sending with send_all() – guarantees all bytes are transmitted,
retries on short writes and interrupts.

SIGPIPE ignored – prevents server crash when a client disconnects
prematurely.

Request header validation – ensures a complete HTTP header block
is received before processing.

Method checking – returns 501 Not Implemented for non‑GET requests.

403 Forbidden error – when a file exists but is not readable.

Server response header – identifies the server as "JosephHTTP/1.0".

CGI dynamic content – full support for CGI programs located in
www/cgi-bin/, with QUERY_STRING environment variable, stdout/stderr
redirection, and proper error handling.

Error handler module – centralized error page generation for 404,
403, and 501.

New module io_utils.c – provides send_all for all network writes.

Refined Makefile and project structure – all new source files added.

New and Modified Files

New modules:

src/io_utils.c / .h – robust send_all().

src/error_handler.c / .h – send_404_response(), send_403_response(),
send_501_response().

src/cgi_handler.c / .h – serve_cgi() for dynamic content.

Modified files:

src/request_parser.h – added query_string field to struct Request.

src/request_parser.c – parse_request_line splits query string;
added skip_headers().

src/file_handler.c – load_file_contents checks read permission;
serve_file uses send_all and adds Server header.

src/server.c – added route_request() static helper; handle_client
calls route_request; includes new headers.

src/response.c – send_http_response uses send_all.

src/main.c – added signal(SIGPIPE, SIG_IGN).

Makefile – added new source files.

README.md – rewritten with build instructions, architecture, and
features.

www/ directory – added cgi-bin/test script.

Architecture Summary

The final architecture is a layered, procedural system with clean
boundaries:

main.c
└─ create_listening_socket() [socket_utils.c]
└─ run_accept_loop() [server.c]
└─ accept_client_connection()
└─ dispatch_client(..., handle_client) [dispatcher.c]
└─ handle_client() [server.c]
├─ receive_request() [request_parser.c]
├─ extract_request_line()
├─ parse_request_line()
└─ route_request() [server.c]
├─ skip_headers() [request_parser.c]
├─ method check (GET only)
├─ if static: serve_file() [file_handler.c]
└─ if CGI: serve_cgi() [cgi_handler.c]

All I/O uses send_all() from io_utils.c. Error pages are built by
error_handler.c. The dispatcher hides the fork() concurrency strategy.

Betty Compliance & Code Quality

All source files pass betty with 0 errors, 0 warnings.

Every function remains at or below 40 lines; every file contains at
most 5 functions.

Strict C89 enforced: variables declared at top of blocks, no C99
loop initializations.

Meaningful naming throughout (file_descriptor, server_socket,
client_ip, raw_request).

No global variables, no memory leaks (verified with Valgrind on
simple requests).

Testing

The server was tested with curl and a browser for all supported
scenarios:

GET / → 200 OK, index.html returned with Server header.

GET /style.css → 200 OK, Content-Type: text/css.

GET /about.html → 200 OK.

GET /nonexistent → 404 Not Found.

POST / → 501 Not Implemented.

chmod 000 www/about.html; GET /about.html → 403 Forbidden
(then restored permissions).

GET /cgi-bin/test?name=Joseph → 200 OK, HTML showing
"CGI works! Args: name=Joseph".

Parallel requests via backgrounded curl calls confirmed
concurrent handling (interleaved logs).

Lessons Learned

Robust network programming requires handling short writes
(send_all) and signals (SIGPIPE).

CGI is a simple protocol: set environment, redirect standard
streams, exec the program.

Permission checks should happen as early as possible to return
proper status codes.

A single static helper for all error pages eliminates duplication.

Adding features to a well‑architected project is easy – no
existing code needed restructuring, only new modules were added.

The open‑closed principle works: the dispatcher interface allowed
adding CGI without changing server.c's accept loop.

Final Reflections

Over five days, I built a fully functional HTTP/1.1 web server from
absolute scratch. Starting from socket() and progressing through
request parsing, file serving, concurrency, and dynamic content, I
applied nearly every concept from my C programming course and the
Clean Code principles. The server is now a polished, professional‑grade
project that I can share with my lecturer, mentors, and future
employers.

The architecture, clean naming, and strict adherence to Betty style
show that I can write code that is not only functional but also
readable and maintainable. Reading CS:APP's Chapter 11 validated
every design decision and helped me understand the deeper systems
concepts at play.

This project has transformed me from a C student into a confident
systems programmer. I can now explain sockets, HTTP, process
management, and clean architecture with authority.

Next Steps (Future Enhancements)

Implement select() or epoll dispatcher for non‑blocking
concurrency.

Add support for more HTTP methods (HEAD, POST with request body).

Parse request headers and use them (e.g., Content-Length for POST).

Implement mmap for large file serving.

Add logging to a file instead of stdout.

Write a proper test suite with shell scripts.

The foundation is so well structured that any of these can be added
without rewriting existing code.
