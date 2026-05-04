#include "response.h"
#include "io_utils.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/**
 * build_http_response - Constructs an HTTP 200 OK response
 * @buffer: Destination buffer
 * @buffer_size: Size of the buffer
 * @body: The response body string
 */
void build_http_response(char *buffer, size_t buffer_size, const char *body)
{
	int content_length = strlen(body);

	snprintf(buffer, buffer_size,
		 "HTTP/1.1 200 OK\r\n"
		 "Content-Type: text/plain\r\n"
		 "Content-Length: %d\r\n"
		 "\r\n"
		 "%s",
		 content_length, body);
}

/**
 * send_http_response - Sends the HTTP response using send_all
 * @client_socket: Connected client socket
 * @response: Full HTTP response string
 */
void send_http_response(int client_socket, const char *response)
{
	if (send_all(client_socket, response, strlen(response)) == -1)
		perror("send_http_response failed");
}
