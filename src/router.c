#include "../include/router.h"
#include "internal/trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Router_t *create_router() {
  Router_t *router = malloc(sizeof(Router_t));
  return router;
}

void register_route(Router_t *router, enum Method method, const char *path,
                    void *handler) {
  if (router == NULL) {
    return;
  }

  if (router->routes[method] == NULL) {
    router->routes[method] = create_trie();
  }

  TrieNode_t *method_routes = router->routes[method];
  trie_insert(method_routes, path, handler);
}

char **get_registered_routes(Router_t *router) {
  return trie_dump(router->routes[GET]);
}

void *match_route(Router_t *router, enum Method method, const char *raw_path) {
  return trie_search(router->routes[method], raw_path);
}
