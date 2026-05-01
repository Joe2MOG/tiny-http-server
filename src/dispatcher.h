#ifndef DISPATCHER_H
#define DISPATCHER_H

/**
 * client_handler - Function pointer type for a client handler
 * @client_socket: The connected client socket
 *
 * A handler takes a socket and does its work (e.g., handle_client).
 */
typedef void (*client_handler)(int client_socket);

/**
 * dispatch_client - Executes a handler concurrently for the client
 * @client_socket: The connected client socket
 * @handler: The function that will process this client
 *
 * The current implementation uses fork().
 * The parent process returns immediately; the child handles the client,
 * then exits.
 */
void dispatch_client(int client_socket, client_handler handler);

#endif /* DISPATCHER_H */
