### Day 3 Report: File Serving, MIME Types, and 404 Handling

Project: Tiny HTTP Server
Goal: Transform the parsed request path into actual files served over HTTP.
Date: 30/4/206

What Was Built

On Day 3, the server became a real web server. It now reads files from a www/ directory, sets the correct Content-Type header, and returns a proper 404 Not Found page when a file is missing or inaccessible.

The server:

Maps "/" to "/index.html" automatically.

Validates the path to prevent directory traversal attacks.

Opens files using open() and reads their contents with read().

Determines the MIME type based on the file extension.

Sends an HTTP 200 OK response with the file content.

Serves a canned HTML 404 page for any error or missing file.

New Module: file_handler.c / file_handler.h

The new module encapsulates all file I/O and response building for static file serving. It exposes only one public function:

serve_file(int client_socket, const char *path)

Internal (static) helper functions keep the code modular:

get_mime_type() – maps file extension to MIME string.

send_404_response() – builds and sends a simple 404 page.

map_request_path() – validates the request path and maps it to a www/ file path.

load_file_contents() – opens, stats, reads the file into a dynamically allocated buffer, returning the size.

Changes to Existing Code

src/server.c: The client handler now calls serve_file(client_socket, request.path) instead of the temporary static welcome message. The send_welcome_response() helper was removed.

Makefile: Added src/file_handler.c to the source list.

Clean Code and Clean Architecture

Meaningful names: All variable names are full words (file_descriptor, file_info, file_path, buffer, file_size). No abbreviations like fd or st.

Single responsibility: Each function does one thing – mapping, loading, building, sending.

Data‑behaviour separation: The file handler operates purely on data (paths, buffers); no hybrid objects.

Encapsulation: Internal functions are marked static; the header exposes only serve_file().

Error handling: Every system call is checked; memory is freed on every error path.

Betty Compliance

All source files pass betty with 0 errors, 0 warnings.
The serve_file function was split into smaller helpers to stay under the 40‑line limit.
All code compiles with -std=gnu89 (strict C89) with no mixed declarations.

MIME Types Supported

The get_mime_type function recognises the following extensions:

.html, .htm → text/html

.css → text/css

.js → application/javascript

.png → image/png

.jpg, .jpeg → image/jpeg

.txt → text/plain

All others → application/octet-stream

Directory Traversal Protection

The map_request_path function rejects any request path that:

Contains ".." (parent directory reference).

Does not start with "/".

This ensures that only files inside the www/ directory can be served.

Testing

The server was tested with curl and a web browser.

Commands and results:

curl -v http://localhost:8080/ → 200 OK, index.html returned.

curl -v http://localhost:8080/index.html → same.

curl -v http://localhost:8080/about.html → about page returned.

curl -v http://localhost:8080/style.css → CSS file with correct Content-Type.

curl -v http://localhost:8080/nonexistent → 404 Not Found HTML page.

A real browser loaded the pages correctly, including CSS styling, because the server returns appropriate Content-Type headers.

Lessons Learned

fstat() provides file metadata (size and type); it must be verified that the path is a regular file.

The correct MIME type is essential for browsers to render content correctly.

Dynamic memory allocation (malloc/free) requires careful handling on every error path to avoid leaks.

Separating header and body sends allows serving binary files without corruption.

Blocking ".." is a simple yet effective security measure against directory traversal.

Extracting helpers not only satisfies Betty but also makes error handling clearer and less repetitive.

Next Steps (Day 4)

On Day 4, I will:

Add concurrency support so the server can handle multiple clients simultaneously.

Explore using fork() or select() to manage connections.

Ensure the server continues to serve files correctly under concurrent access.
