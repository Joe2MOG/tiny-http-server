#include "server.h"
#include "request_parser.h"
#include "file_handler.h"
#include "error_handler.h"
#include "cgi_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "dispatcher.h"

#define REQUEST_BUFFER_SIZE 4096

/**
 * route_request - Validates headers, checks method, and serves content
 * @client_socket: Connected client socket
 * @request: Parsed request
 * @raw_request: The raw request buffer (used for header validation)
 */
static void route_request(int client_socket, const struct Request *request,
			  const char *raw_request)
{
	/* Check that the full HTTP headers were received */
	if (skip_headers(raw_request) == NULL)
	{
		printf("[WARN] Incomplete headers\n");
		return;
	}

	/* Only GET is implemented */
	if (strcmp(request->method, "GET") != 0)
	{
		send_501_response(client_socket);
		return;
	}

	/* CGI request? */
	if (strncmp(request->path, "/cgi-bin/", 9) == 0)
	{
		char script_path[1024];

		snprintf(script_path, sizeof(script_path), "www%s",
			 request->path);
		serve_cgi(client_socket, script_path, request->query_string);
	}
	else
	{
		serve_file(client_socket, request->path);
	}
}

/**
 * handle_client - Receives, parses, and dispatches a client request
 * @client_socket: Socket connected to the client
 */
void handle_client(int client_socket)
{
	char raw_request[REQUEST_BUFFER_SIZE];
	char request_line[REQUEST_BUFFER_SIZE];
	struct Request request;
	int bytes_received;
	bool success;

	bytes_received = receive_request(client_socket,
					 raw_request,
					 sizeof(raw_request));
	if (bytes_received <= 0)
		return;

	printf("[INFO] Received %d bytes:\n%s\n",
	       bytes_received, raw_request);

	success = extract_request_line(raw_request, request_line,
				       sizeof(request_line));
	if (!success)
	{
		printf("[WARN] Malformed request (no CRLF found)\n");
		return;
	}
	printf("[INFO] Request line: %s\n", request_line);

	success = parse_request_line(request_line, &request);
	if (!success)
	{
		printf("[WARN] Could not parse method/path\n");
		return;
	}
	printf("[INFO] Method: %s, Path: %s, Query: %s\n",
	       request.method, request.path, request.query_string);

	route_request(client_socket, &request, raw_request);
}

/**
 * accept_client_connection - Accepts and logs a new client connection
 * @server_socket: The listening socket
 * @client_socket: Output – the new client socket
 * Return: true on success, false on failure (skip this client)
 */
bool accept_client_connection(int server_socket, int *client_socket)
{
	struct sockaddr_in client_address;
	socklen_t client_address_length = sizeof(client_address);
	char client_ip[INET_ADDRSTRLEN];

	*client_socket = accept(server_socket,
				(struct sockaddr *)&client_address,
				&client_address_length);
	if (*client_socket == -1)
	{
		perror("accept failed");
		return (false);
	}

	inet_ntop(AF_INET, &client_address.sin_addr,
		  client_ip, INET_ADDRSTRLEN);
	printf("[INFO] Connected: %s:%d\n",
	       client_ip, ntohs(client_address.sin_port));
	return (true);
}

/**
 * run_accept_loop - Continuously accepts and dispatches clients
 * @server_socket: The listening socket
 * @port: The bound port (used for logging)
 */
void run_accept_loop(int server_socket, int port)
{
	int client_socket;

	printf("[INFO] Listening on port %d...\n", port);

	while (true)
	{
		if (!accept_client_connection(server_socket, &client_socket))
			continue;

		dispatch_client(client_socket, handle_client);
		printf("[INFO] Client dispatched.\n");
	}
}
