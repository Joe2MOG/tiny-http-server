#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <stdbool.h>
#include <stddef.h>

/**
 * struct Request - Parsed HTTP request line
 * @method: HTTP method (always uppercase)
 * @path: Request path without query string
 * @query_string: Everything after '?' or empty string
 */
struct Request
{
	char method[16];
	char path[256];
	char query_string[256];
};

int receive_request(int client_socket, char *buffer, size_t buffer_size);
bool extract_request_line(const char *raw_request, char *line_buffer,
			   size_t line_buffer_size);
bool parse_request_line(const char *request_line, struct Request *request);
const char *skip_headers(const char *raw_request);

#endif /* REQUEST_PARSER_H */
