#include "server.h"
#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define HTTP_RESPONSE_BUFFER_SIZE 512

/**
 * handle_client - Serves a single client with a static welcome message
 * @client_socket: The socket connected to the client
 */
void handle_client(int client_socket)
{
	char http_response[HTTP_RESPONSE_BUFFER_SIZE];

	build_http_response(http_response, sizeof(http_response),
						"Hello from your C web server!");
	send_http_response(client_socket, http_response);
}

/**
 * run_accept_loop - Continuously accepts and handles client connections
 * @server_socket: The listening socket
 * @port: The bound port (used for logging)
 */
void run_accept_loop(int server_socket, int port)
{
	struct sockaddr_in client_address;
	socklen_t client_address_length;
	int client_socket;
	char client_ip[INET_ADDRSTRLEN];

	printf("[INFO] Listening on port %d...\n", port);

	while (true)
	{
		client_address_length = sizeof(client_address);
		client_socket = accept(server_socket,
							   (struct sockaddr *)&client_address,
							   &client_address_length);
		if (client_socket == -1)
		{
			perror("accept failed");
			continue;
		}

		inet_ntop(AF_INET, &client_address.sin_addr,
				  client_ip, INET_ADDRSTRLEN);
		printf("[INFO] Connected: %s:%d\n",
			   client_ip, ntohs(client_address.sin_port));

		handle_client(client_socket);
		close(client_socket);
		printf("[INFO] Client disconnected.\n");
	}
}
