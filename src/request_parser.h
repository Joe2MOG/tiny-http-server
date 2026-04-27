#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct Request - Parsed HTTP request line
 * @method: HTTP method (always uppercase, e.g., "GET")
 * @path: Request path (e.g., "/index.html")
 *
 * This is a pure data structure – no behaviour attached.
 * External functions operate upon it.
 */
struct Request
{
	char method[16];
	char path[256];
};

/**
 * receive_request - Reads the HTTP request from a client socket
 * @client_socket: Connected client socket
 * @buffer: Destination buffer for the raw request
 * @buffer_size: Maximum bytes to read
 * Return: Number of bytes read, 0 on disconnect, -1 on error
 */
int receive_request(int client_socket, char *buffer, size_t buffer_size);

/**
 * extract_request_line - Finds the first line of an HTTP request
 * @raw_request: The full request buffer
 * @line_buffer: Output buffer for the extracted line
 * @line_buffer_size: Size of the output buffer
 * Return: true if a line was found, false otherwise
 */
bool extract_request_line(const char *raw_request, char *line_buffer,
			   size_t line_buffer_size);

/**
 * parse_request_line - Splits a request line into a Request struct
 * @request_line: The first line of the request
 *                (e.g. "GET /index.html HTTP/1.1")
 * @request: Output Request struct that will be filled
 * Return: true on success, false if the line is malformed
 */
bool parse_request_line(const char *request_line, struct Request *request);

#endif /* REQUEST_PARSER_H */
