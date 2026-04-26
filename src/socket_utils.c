#include "socket_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

/**
 * create_socket - Creates an IPv4 TCP socket
 * Return: new socket file descriptor on success, -1 on error
 */
int create_socket(void)
{
	int new_socket;

	new_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (new_socket == -1)
		perror("socket creation failed");
	return (new_socket);
}

/**
 * set_socket_reuse_address - Enables SO_REUSEADDR on a socket
 * @server_socket: The socket to modify
 * Return: 0 on success, -1 on error
 */
int set_socket_reuse_address(int server_socket)
{
	int enable_reuse = 1;

	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR,
				   &enable_reuse, sizeof(enable_reuse)) == -1)
	{
		perror("setsockopt failed");
		return (-1);
	}
	return (0);
}

/**
 * bind_socket_to_address - Binds a socket to a port
 * @server_socket: Socket to bind
 * @port: Port number in host byte order
 * Return: 0 on success, -1 on error
 */
int bind_socket_to_address(int server_socket, int port)
{
	struct sockaddr_in server_address;

	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = INADDR_ANY;
	server_address.sin_port = htons(port);

	if (bind(server_socket, (struct sockaddr *)&server_address,
			 sizeof(server_address)) == -1)
	{
		perror("bind failed");
		return (-1);
	}
	printf("[INFO] Bound to port %d.\n", port);
	return (0);
}

/**
 * start_listening - Marks a socket as passive
 * @server_socket: Socket to listen on
 * Return: 0 on success, -1 on error
 */
int start_listening(int server_socket)
{
	if (listen(server_socket, SOMAXCONN) == -1)
	{
		perror("listen failed");
		return (-1);
	}
	printf("[INFO] Listening on socket.\n");
	return (0);
}

/**
 * create_listening_socket - Sets up a fully configured listening socket
 * @port: Port number to bind to
 * Return: listening socket file descriptor, or -1 on error
 */
int create_listening_socket(int port)
{
	int server_socket;

	server_socket = create_socket();
	if (server_socket == -1)
		return (-1);
	printf("[INFO] Server socket created.\n");

	if (set_socket_reuse_address(server_socket) == -1)
	{
		close(server_socket);
		return (-1);
	}

	if (bind_socket_to_address(server_socket, port) == -1)
	{
		close(server_socket);
		return (-1);
	}

	if (start_listening(server_socket) == -1)
	{
		close(server_socket);
		return (-1);
	}
	return (server_socket);
}
