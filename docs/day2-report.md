Day 2 Report: HTTP Request Parsing & Client Handler
Day 2 Report: HTTP Request Parsing & Client Handler
Project: Tiny HTTP Server
Goal: Enable the server to read and interpret the raw HTTP request sent by a client.
Date: 27/04/2026

1. What Was Built
On Day 2, I extended the server so it no longer ignores what the client says. The server now:
•	Receives the complete HTTP request using recv().
•	Locates the request line (the first line that ends with \r\n).
•	Splits the request line into the HTTP method (e.g., GET) and the requested path (e.g., /index.html).
•	Validates that the method is all uppercase letters.
•	Prints the parsed method and path for debugging.
•	Continues to send a static welcome response (file serving comes on Day 3).
All parsing logic is isolated in a new module: request_parser.c / request_parser.h.

2. New Files and Changes
New module
•	src/request_parser.h – defines the Request data structure and declares the parsing functions.
•	src/request_parser.c – implements receive_request, extract_request_line, parse_request_line.
Modified files
•	src/server.c:
o	Added send_welcome_response – a tiny helper that builds and sends the static response (keeps handle_client under 40 lines).
o	Added accept_client_connection – handles the accept() call and client logging (keeps run_accept_loop short).
o	handle_client now calls the parsing functions before dispatching the response.
•	Makefile: added src/request_parser.c to the source list.

3. Architecture Overview
The flow of a single client connection is now:
1.	accept_client_connection() – accepts the connection and logs the client IP.
2.	handle_client():
o	receive_request() → raw bytes
o	extract_request_line() → first line of the request
o	parse_request_line() → fills a struct Request with method and path
o	send_welcome_response() → sends a static HTTP 200 OK
The Request data structure is pure data:
c
struct Request {
    char method[16];
    char path[256];
};
All functions that work on it are external procedures – no hybrid object/behaviour mixing.

4. Clean Code Principles Applied
•	Meaningful names: Every function is a verb phrase (receive_request, extract_request_line). Variables like raw_request, line_end, line_length leave no ambiguity.
•	Single responsibility: Each function does exactly one thing:
o	receive_request – reads and null terminates.
o	extract_request_line – finds and copies the first line.
o	parse_request_line – splits into method and path, validates.
•	Data vs. behaviour separation: The Request struct contains only data; parsing logic lives in separate functions – no hidden side effects.
•	Consistent vocabulary: The words request, line, buffer, method, path are used everywhere; there is no mix of query, url, or fd.
•	No hidden side effects: Functions either return a success/failure status and fill an output buffer, or they send data; they don’t do both unnoticed.

5. Betty Compliance & C89 Strictness
•	All source files pass the betty style checker with 0 errors, 0 warnings.
•	The code is fully compatible with C89 (-std=gnu89):
o	All variables are declared at the top of their blocks.
o	for loops use a pre declared index variable (no for (int i = 0; ...)).
•	Helper functions were extracted to keep every function ≤ 40 lines, satisfying Betty’s size rule.

6. Testing
Tests were performed with two tools:
Tool	Command	What it proved
curl	curl -v http://localhost:8080/index.html	Server correctly parsed the request and logged Method: GET, Path: /index.html.
telnet	telnet localhost 8080 (typed a raw HTTP request)	Even a manually typed request was parsed accurately.
The server still replies with the static “Hello from your C web server!” message – the full file serving feature is reserved for Day 3.

7. Key Functions and System Calls Introduced
Function / Call	Purpose
recv()	Receive data from a socket (returns number of bytes read, 0 on disconnect).
strstr()	Find a substring; used to locate the \r\n that ends the request line.
sscanf()	Parse formatted input from a string; width specifiers (%15s, %255s) prevent buffer overflow.
Pointer subtraction	Calculate the length of the first line by subtracting two pointers.
snprintf()	Build the HTTP response string safely.
send()	Transmit the response data over the socket.

8. Lessons Learned
•	An HTTP request is simply text over TCP – parsing it is pure string manipulation.
•	Every buffer that holds string data must be explicitly null terminated before using string functions.
•	sscanf with width specifiers is a safe, concise way to extract tokens.
•	Extracting even small helper functions makes the code more readable, easier to debug, and Betty compliant.
•	A data structure like Request cleanly bundles related information, reducing the number of function parameters.
•	Adhering to C89 rules forces careful variable placement and helps avoid subtle bugs.

9. Next Steps (Day 3)
On Day 3 I will:
•	Create a www/ directory with sample HTML and CSS files.
•	Map the parsed path (like /index.html) to a real file on disk.
•	Use open(), stat(), and read() to load file contents.
•	Build a proper HTTP response with the correct Content-Type based on the file extension.
•	Return 404 Not Found when the file doesn't exist.
•	Replace the temporary send_welcome_response with a new function that serves actual files.

