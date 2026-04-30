#include "file_handler.h"
#include "response.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/socket.h>

/**
 * get_mime_type - Returns the MIME type for a given file extension
 * @path: The file path
 * Return: A MIME string (e.g., "text/html")
 */
const char *get_mime_type(const char *path)
{
	const char *dot;

	dot = strrchr(path, '.');
	if (!dot)
		return ("application/octet-stream");

	if (strcmp(dot, ".html") == 0 || strcmp(dot, ".htm") == 0)
		return ("text/html");
	if (strcmp(dot, ".css") == 0)
		return ("text/css");
	if (strcmp(dot, ".js") == 0)
		return ("application/javascript");
	if (strcmp(dot, ".png") == 0)
		return ("image/png");
	if (strcmp(dot, ".jpg") == 0 || strcmp(dot, ".jpeg") == 0)
		return ("image/jpeg");
	if (strcmp(dot, ".txt") == 0)
		return ("text/plain");

	return ("application/octet-stream");
}

/**
 * send_404_response - Sends a canned 404 Not Found page
 * @client_socket: The connected client socket
 */
void send_404_response(int client_socket)
{
	const char *body;
	size_t body_len;
	char header[512];
	char full_response[4096];

	body = "<html><body><h1>404 Not Found</h1></body></html>";
	body_len = strlen(body);

	snprintf(header, sizeof(header),
		 "HTTP/1.1 404 Not Found\r\n"
		 "Content-Type: text/html\r\n"
		 "Content-Length: %lu\r\n"
		 "\r\n", (unsigned long)body_len);

	strcpy(full_response, header);
	strcat(full_response, body);
	send_http_response(client_socket, full_response);
}

/**
 * map_request_path - Validates and maps the request path to a www/ path
 * @request_path: The original request path (e.g., "/" or "/index.html")
 * @file_path: Output buffer for the mapped file path
 * @size: Size of the output buffer
 * Return: true if the path is safe and was mapped, false otherwise
 */
static bool map_request_path(const char *request_path,
			     char *file_path, size_t size)
{
	/* Block directory traversal */
	if (strstr(request_path, "..") || request_path[0] != '/')
		return (false);

	if (strcmp(request_path, "/") == 0)
		snprintf(file_path, size, "www/index.html");
	else
		snprintf(file_path, size, "www%s", request_path);

	return (true);
}

/**
 * load_file_contents - Opens a file and loads its contents into memory
 * @file_path: Path to the file on disk
 * @buffer: Output pointer; the caller receives a malloc'd buffer
 * @file_size: Output; the size of the loaded file
 * Return: true on success, false on error
 *
 * On success the caller must free(*buffer).
 */
static bool load_file_contents(const char *file_path,
			       char **buffer, size_t *file_size)
{
	int file_descriptor;
	struct stat file_info;
	size_t size;

	file_descriptor = open(file_path, O_RDONLY);
	if (file_descriptor == -1)
		return (false);

	if (fstat(file_descriptor, &file_info) == -1 ||
	    !S_ISREG(file_info.st_mode))
	{
		close(file_descriptor);
		return (false);
	}

	size = file_info.st_size;
	*buffer = malloc(size);
	if (!*buffer)
	{
		close(file_descriptor);
		return (false);
	}

	if (read(file_descriptor, *buffer, size) < 0)
	{
		free(*buffer);
		close(file_descriptor);
		return (false);
	}

	close(file_descriptor);
	*file_size = size;
	return (true);
}

/**
 * serve_file - Serves a file from the www/ directory
 * @client_socket: The connected client socket
 * @path: The original HTTP request path
 */
void serve_file(int client_socket, const char *path)
{
	char file_path[1024];
	char *buffer;
	size_t file_size;
	const char *mime;
	char header[2048];

	if (!map_request_path(path, file_path, sizeof(file_path)))
	{
		send_404_response(client_socket);
		return;
	}

	if (!load_file_contents(file_path, &buffer, &file_size))
	{
		send_404_response(client_socket);
		return;
	}

	mime = get_mime_type(file_path);
	snprintf(header, sizeof(header),
		 "HTTP/1.1 200 OK\r\n"
		 "Content-Type: %s\r\n"
		 "Content-Length: %lu\r\n"
		 "\r\n", mime, (unsigned long)file_size);

	send_http_response(client_socket, header);
	if (send(client_socket, buffer, file_size, 0) == -1)
		perror("send body failed");

	free(buffer);
}
