#include "error_handler.h"
#include "io_utils.h"
#include <stdio.h>
#include <string.h>

/**
 * send_error_response - Builds and sends a generic error page
 * @client_socket: Connected client socket
 * @status_code: HTTP status code string (e.g., "404")
 * @status_message: Human-readable message (e.g., "Not Found")
 */
static void send_error_response(int client_socket, const char *status_code,
			      const char *status_message)
{
	char header[512];
	char body[512];
	char full_response[1024];
	size_t body_len;

	snprintf(body, sizeof(body),
		 "<html><body><h1>%s %s</h1></body></html>",
		 status_code, status_message);
	body_len = strlen(body);

	snprintf(header, sizeof(header),
		 "HTTP/1.1 %s %s\r\n"
		 "Server: JosephHTTP/1.0\r\n"
		 "Content-Type: text/html\r\n"
		 "Content-Length: %lu\r\n"
		 "\r\n", status_code, status_message,
		 (unsigned long)body_len);

	strcpy(full_response, header);
	strcat(full_response, body);
	send_all(client_socket, full_response, strlen(full_response));
}

/**
 * send_404_response - Sends an HTTP 404 Not Found error
 * @client_socket: Connected client socket
 */
void send_404_response(int client_socket)
{
	send_error_response(client_socket, "404", "Not Found");
}

/**
 * send_403_response - Sends an HTTP 403 Forbidden error
 * @client_socket: Connected client socket
 */
void send_403_response(int client_socket)
{
	send_error_response(client_socket, "403", "Forbidden");
}

/**
 * send_501_response - Sends an HTTP 501 Not Implemented error
 * @client_socket: Connected client socket
 */
void send_501_response(int client_socket)
{
	send_error_response(client_socket, "501", "Not Implemented");
}
