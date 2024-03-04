#include "internal/http.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BREAK "\r\n"
#define MAX_LINES 1024

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

char *handle_request(Request_t *req) {
  if (req == NULL) {
    return "HTTP/1.1 500 Internal Server Error\r\n"
           "Content-Type: text/html\r\n"
           "\r\n"
           "<html><body>Internal Server Error</body></html>";
  }
  char *path = req->path;

  // Just an example
  if (strcmp(path, "/") == 0) {
    return "HTTP/1.1 200 OK\r\n"
           "Content-Type: text/html\r\n\r\n"
           "<html><body><h1>Hello from home page!</h1></body></html>\r\n";
  }
  return "HTTP/1.1 200 OK\r\n"
         "Content-Type: text/html\r\n\r\n"
         "<html><body><h1>Hello from other page.</h1></body></html>\r\n";
}
