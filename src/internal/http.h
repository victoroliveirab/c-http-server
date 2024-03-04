#ifndef HTTP_H
#define HTTP_H

struct Request {
  char *method;
  char *path;
};

typedef struct Request Request_t;

Request_t *parse_http_request(char *request);
char *handle_request(Request_t *req);

#endif
