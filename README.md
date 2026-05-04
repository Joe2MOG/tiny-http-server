# JosephHTTP – A Minimal HTTP/1.1 Server in C

A clean, modular, Betty-compliant web server written from scratch using
only POSIX sockets and C89. It serves static files, dynamic CGI content,
and handles multiple clients concurrently via a fork‑based dispatcher.

## Features

- **HTTP/1.1 compliant**: parses request line, headers, query strings.
- **Static content**: serves HTML, CSS, JavaScript, images, and plain text
  from a `www/` directory with correct MIME types.
- **Dynamic content**: runs CGI scripts in `www/cgi-bin/` with
  `QUERY_STRING` environment variable support.
- **Concurrency**: each client request is handled in a separate process
  using `fork()` behind a clean dispatcher interface.
- **Robust IO**: retries on short writes (`send_all`) and ignores
  `SIGPIPE` to survive broken connections.
- **Error handling**: returns proper status codes:
  `200 OK`, `403 Forbidden`, `404 Not Found`, `501 Not Implemented`.
- **Clean architecture**: layered modules with single responsibilities
  (socket setup, request parsing, file handler, CGI handler, error
  handler, dispatcher).
- **Betty compliant**: zero Betty warnings, strict C89 (`-std=gnu89`),
  all functions ≤ 40 lines and files ≤ 5 functions.
- **Documented**: every function has a Betty‑style doc comment.
- **Build system**: `Makefile` with `all`, `clean`, and `betty` targets.

## Project Structure
tiny-http-server/
├── src/
│ ├── main.c # Entry point, signal setup
│ ├── server.c / .h # Accept loop, request dispatch
│ ├── socket_utils.c / .h # Socket creation, bind, listen
│ ├── request_parser.c / .h# HTTP request parsing
│ ├── file_handler.c / .h # Static file serving
│ ├── response.c / .h # HTTP response building
│ ├── dispatcher.c / .h # Concurrency (fork)
│ ├── error_handler.c / .h# Error responses (404, 403, 501)
│ ├── cgi_handler.c / .h # CGI execution
│ └── io_utils.c / .h # Robust send_all
├── www/ # Document root
│ ├── index.html
│ ├── about.html
│ ├── style.css
│ └── cgi-bin/ # CGI scripts
│ └── test
├── docs/ # Daily reports
│ ├── day1-report.md
│ ├── day2-report.md
│ ├── day3-report.md
│ └── day4-report.md
├── Makefile
├── README.md
└── .gitignore

text

## Build and Run

### Prerequisites

- GCC (supports C89)
- GNU Make
- Linux / Unix environment

### Build

```bash
make
To check Betty style:

bash
make betty
Run
bash
./server <PORT>
Example:

bash
./server 8080
The server will listen on all interfaces. Open a browser and visit
http://localhost:8080 or use curl.

Test Endpoints
GET / → serves index.html

GET /about.html → serves the about page

GET /style.css → serves CSS with correct MIME type

GET /cgi-bin/test?name=value → runs the test CGI script

POST / → returns 501 Not Implemented

GET /missing → returns 404 Not Found

Architecture
The server follows a layered, procedural design where each module has
a single responsibility:

socket_utils.c – creates and configures the listening socket.

request_parser.c – reads raw bytes from a connection and
transforms them into a struct Request (method, path, query string).

server.c – orchestrates the accept loop, calls the dispatcher,
and routes requests (static vs CGI).

dispatcher.c – executes the request handler concurrently using
fork() (or a future strategy) without the rest of the server
knowing how.

file_handler.c – maps request paths to files inside www/,
loads them, and sends them with proper headers.

cgi_handler.c – forks a child, sets environment, redirects I/O,
and execs the CGI program.

response.c – low‑level HTTP response formatting.

io_utils.c – robust send_all that handles short writes.

error_handler.c – sends consistent HTML error pages.

All data flows through pure C structs (like Request); no global
variables are used. The dispatcher interface allows swapping the
concurrency model without changing the rest of the server.

Clean Code & Conventions
Meaningful names: no abbreviations (file_descriptor, not fd;
server_socket, not listenfd).

Single Responsibility: every function does exactly one thing.

Data‑behaviour separation: data structures (Request) are pure
containers; functions operate on them from the outside.

No magic numbers: all constants are named.

Consistent vocabulary: socket, address, request, response.

Betty style: K&R braces, 80‑column limit, 40‑line function limit,
5‑functions‑per‑file limit.

Credits
This project was built as a 5‑day challenge to solidify C programming
fundamentals after taking C Programming 1 from Elmwood Institute (SET), inspired by the textbook Computer Systems: A Programmer's
Perspective (Bryant & O'Hallaron). The architecture follows principles
from Clean Code by Robert C. Martin.

License
MIT – feel free to use, modify, and share.
