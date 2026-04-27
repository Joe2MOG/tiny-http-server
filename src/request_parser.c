#include "request_parser.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

/**
 * receive_request - Reads the HTTP request from a client socket
 * @client_socket: Connected client socket
 * @buffer: Destination buffer
 * @buffer_size: Maximum bytes to read
 * Return: Bytes read, 0 on disconnect, -1 on error
 */
int receive_request(int client_socket, char *buffer, size_t buffer_size)
{
	int bytes_read;

	bytes_read = recv(client_socket, buffer, buffer_size - 1, 0);
	if (bytes_read < 0)
	{
		perror("recv failed");
		return (-1);
	}
	buffer[bytes_read] = '\0';
	return (bytes_read);
}

/**
 * extract_request_line - Finds the first line of an HTTP request
 * @raw_request: The full request buffer
 * @line_buffer: Output buffer
 * @line_buffer_size: Size of the output buffer
 * Return: true if a line was found, false otherwise
 */
bool extract_request_line(const char *raw_request, char *line_buffer,
			   size_t line_buffer_size)
{
	const char *line_end;
	size_t line_length;

	line_end = strstr(raw_request, "\r\n");
	if (!line_end)
		return (false);

	line_length = line_end - raw_request;
	if (line_length >= line_buffer_size)
		return (false);

	strncpy(line_buffer, raw_request, line_length);
	line_buffer[line_length] = '\0';
	return (true);
}

/**
 * parse_request_line - Splits a request line into a Request struct
 * @request_line: The extracted first line
 * @request: Output struct to fill
 * Return: true on success, false if malformed
 */
bool parse_request_line(const char *request_line, struct Request *request)
{
	int i;

	if (!request)
		return (false);

	if (sscanf(request_line, "%15s %255s",
		   request->method, request->path) != 2)
		return (false);

	for (i = 0; request->method[i]; i++)
	{
		if (request->method[i] < 'A' || request->method[i] > 'Z')
			return (false);
	}
	return (true);
}
