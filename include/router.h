#ifndef ROUTER_H
#define ROUTER_H

#include "../src/internal/trie.h"

#define MAX_PATH_LENGTH 127
#define MAX_NUM_OF_ROUTES 30

struct Router {
  TrieNode_t *routes[4];
};

enum Method { GET, POST, PUT, DELETE, UNKNOWN };

typedef struct Router Router_t;

Router_t *create_router();
void register_route(Router_t *router, enum Method method, const char *path,
                    void *handler);
void *match_route(Router_t *router, enum Method method, const char *raw_path);
char **get_registered_routes(Router_t *router);

#endif
