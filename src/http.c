#include "internal/http.h"
#include "../include/router.h"
#include "../include/server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BREAK "\r\n"
#define MAX_LINES 1024

enum Method get_method(const char *method) {
  if (strncmp(method, "GET", 3) == 0)
    return GET;
  if (strncmp(method, "POST", 4) == 0)
    return POST;
  if (strncmp(method, "PUT", 3) == 0)
    return PUT;
  if (strncmp(method, "DELETE", 6) == 0)
    return DELETE;
  return UNKNOWN;
}

Request_t *parse_http_request(char *request) {
  char *lines[MAX_LINES];
  char *line = strtok(request, LINE_BREAK);
  int line_count = 0;
  while (line != NULL && line_count < MAX_LINES) {
    lines[line_count++] = line;
    line = strtok(NULL, LINE_BREAK);
  }
  if (line_count == 0) {
    return NULL;
  }
  // for (int i = 0; i < line_count; ++i) {
  //   printf("Line #%d: %s\n", i, lines[i]);
  // }

  // For now, let's just look at the method and the path in order to serve our
  // resources
  char *first_line = lines[0];
  Request_t *req = malloc(sizeof(Request_t));
  req->method = strtok(first_line, " ");
  req->path = strtok(NULL, " ");
  return req;
}

char *handle_request(Server_t *server, Request_t *req) {
  if (req == NULL) {
    return "HTTP/1.1 500 Internal Server Error\r\n"
           "Content-Type: text/html\r\n"
           "\r\n"
           "<html><body>Internal Server Error</body></html>";
  }

  enum Method method = get_method(req->method);

  if (method == UNKNOWN) {
    return "HTTP/1.1 405 Method Not Allowed\r\n"
           "Content-Type: text/html\r\n"
           "\r\n"
           "<html><body>Method Not Allowed</body></html>";
  }

  const char *path = req->path;

  void *handler = match_route(server->router, method, path);

  if (handler == NULL) {
    return "HTTP/1.1 404 OK\r\n"
           "Content-Type: text/html\r\n\r\n"
           "<html><body><h1>Content not found</h1></body></html>\r\n";
  }

  char *(*handler_fn)() = (char *(*)())handler;
  char *response = handler_fn();
  return response;
}
