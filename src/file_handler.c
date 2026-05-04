#include "file_handler.h"
#include "error_handler.h"
#include "io_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 * get_mime_type - Returns MIME type for file extension
 * @path: File path
 * Return: MIME string
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
 * map_request_path - Validates and maps request path to a www/ file path
 * @request_path: Original request path
 * @file_path: Output buffer
 * @size: Size of output buffer
 * Return: true if safe and mapped, false otherwise
 */
static bool map_request_path(const char *request_path,
			     char *file_path, size_t size)
{
	if (strstr(request_path, "..") || request_path[0] != '/')
		return (false);

	if (strcmp(request_path, "/") == 0)
		snprintf(file_path, size, "www/index.html");
	else
		snprintf(file_path, size, "www%s", request_path);

	return (true);
}

/**
 * load_file_contents - Opens, stats, checks permission, reads file into buffer
 * @file_path: Path on disk
 * @buffer: Output pointer to allocated buffer
 * @file_size: Output file size
 * Return: true on success, false on error
 */
static bool load_file_contents(const char *file_path,
			       char **buffer, size_t *file_size)
{
	int file_descriptor;
	struct stat file_info;

	file_descriptor = open(file_path, O_RDONLY);
	if (file_descriptor == -1)
		return (false);

	if (fstat(file_descriptor, &file_info) == -1 ||
	    !S_ISREG(file_info.st_mode))
	{
		close(file_descriptor);
		return (false);
	}

	if (!(file_info.st_mode & S_IRUSR))
	{
		close(file_descriptor);
		return (false);
	}

	*file_size = file_info.st_size;
	*buffer = malloc(*file_size);
	if (!*buffer)
	{
		close(file_descriptor);
		return (false);
	}

	if (read(file_descriptor, *buffer, *file_size) < 0)
	{
		free(*buffer);
		close(file_descriptor);
		return (false);
	}

	close(file_descriptor);
	return (true);
}

/**
 * serve_file - Serves a static file from the www/ directory
 * @client_socket: Connected client socket
 * @path: The original request path
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
		 "Server: JosephHTTP/1.0\r\n"
		 "Content-Type: %s\r\n"
		 "Content-Length: %lu\r\n"
		 "\r\n", mime, (unsigned long)file_size);

	send_all(client_socket, header, strlen(header));
	if (send_all(client_socket, buffer, file_size) == -1)
		perror("send body failed");
	free(buffer);
}
