#ifndef IO_UTILS_H
#define IO_UTILS_H

#include <stddef.h>

/**
 * send_all - Sends exactly @n bytes from @buffer over the socket
 * @client_socket: The connected client socket
 * @buffer: Data to send
 * @length: Number of bytes to send
 * Return: 0 on success, -1 on error
 */
int send_all(int client_socket, const void *buffer, size_t length);

#endif /* IO_UTILS_H */
