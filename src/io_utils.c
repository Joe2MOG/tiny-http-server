#include "io_utils.h"
#include <stdio.h>          /* for perror */
#include <unistd.h>         /* for send */
#include <errno.h>          /* for errno, EINTR */
#include <sys/socket.h>     /* for send */

/**
 * send_all - Sends every byte from a buffer over the socket
 * @client_socket: Connected client socket
 * @buffer: Data to send
 * @length: Number of bytes to send
 * Return: 0 on success, -1 on error
 */
int send_all(int client_socket, const void *buffer, size_t length)
{
	const char *data;
	size_t remaining;
	ssize_t sent;

	data = (const char *)buffer;
	remaining = length;
	while (remaining > 0)
	{
		sent = send(client_socket, data, remaining, 0);
		if (sent < 0)
		{
			if (errno == EINTR)
				continue;
			perror("send_all failed");
			return (-1);
		}
		remaining -= sent;
		data += sent;
	}
	return (0);
}
