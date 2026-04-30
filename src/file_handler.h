#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

/**
 * serve_file - Sends the requested file (or a 404) over the socket
 * @client_socket: The connected client socket
 * @path: The clean request path (e.g., "/index.html")
 *
 * The function handles mapping "/" to "/index.html",
 * opens the file inside "www/", sends it with correct headers,
 * and falls back to a 404 page if the file doesn't exist.
 */
void serve_file(int client_socket, const char *path);

#endif /* FILE_HANDLER_H */
