#include "dispatcher.h"
#include <unistd.h>    /* fork, close */
#include <stdlib.h>    /* exit */
#include <stdio.h>     /* perror */
#include <sys/types.h> /* pid_t */

/**
 * dispatch_client - Forks a child to handle the client
 * @client_socket: The connected client socket
 * @handler: The function that processes this client
 *
 * The parent process closes its copy of the client socket and returns.
 * The child handles the request, then exits.
 * If fork fails, the client is handled in the parent (degraded mode).
 */
void dispatch_client(int client_socket, client_handler handler)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* Fork failed – fall back to handling in the parent */
		perror("fork failed");
		handler(client_socket);
		return;
	}
	if (child_pid == 0)
	{
		/* Child process: handle the client, then exit */
		handler(client_socket);
		exit(EXIT_SUCCESS);
	}
	/* Parent process: close the client socket (child has its own copy) */
	close(client_socket);
}
