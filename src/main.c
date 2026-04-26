#include "socket_utils.h"
#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * main - Entry point for the HTTP server
 * @argc: Argument count
 * @argv: Argument vector
 * Return: EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char *argv[])
{
	int port;
	int server_socket;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <PORT>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	port = atoi(argv[1]);

	server_socket = create_listening_socket(port);
	if (server_socket == -1)
		exit(EXIT_FAILURE);

	run_accept_loop(server_socket, port);
	close(server_socket);   /* unreachable */
	return (EXIT_SUCCESS);
}
