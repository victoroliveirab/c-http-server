#include "trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

short get_index(char c) {
  if (c == ':') {
    return 27;
  }
  if (c == '/') {
    return 26;
  }
  return c - 'a';
}

char get_char(short index) {
  if (index == 26) {
    return '/';
  }
  if (index == 27) {
    return ':';
  }
  return index + 97;
}

TrieNode_t *trie_create_node() {
  TrieNode_t *node = malloc(sizeof(TrieNode_t));
  if (node == NULL) {
    return NULL;
  }
  for (int i = 0; i < CHILDREN_LENGTH; ++i) {
    node->children[i] = NULL;
  }
  node->dynamic = 0;
  node->end = 0;
  node->value = NULL;
  return node;
}

TrieNode_t *create_trie() { return trie_create_node(); }

void trie_insert(TrieNode_t *trie, const char *key, void *value) {
  if (trie == NULL) {
    trie = trie_create_node();
  }
  TrieNode_t *curr = trie;
  unsigned char *ukey = (unsigned char *)key;
  while (*ukey) {
    if (*ukey == ':') {
      // 0-25 = a-z, 26 = /, 27 = :
      if (!curr->children[CHILDREN_LENGTH - 1]) {
        curr->children[CHILDREN_LENGTH - 1] = trie_create_node();
        curr->dynamic = 1;
      }
      short i = 0;
      while (*ukey && *ukey != '/') {
        if (i == MAX_IDENTIFIER_LEN) {
          printf("ERROR: too large identifier found at %s. Max identifier's "
                 "length: %d\n",
                 key, MAX_IDENTIFIER_LEN);
          exit(1);
        }
        curr->identifier[i++] = *ukey;
        ++ukey;
      }
      curr->identifier[i] = '\0';
    } else {
      // map a-z and / to 0-26
      short index = get_index(*ukey);
      TrieNode_t *next = curr->children[index];
      if (!next) {
        curr->children[index] = trie_create_node();
        next = curr->children[index];
      }
      curr = next;
      ++ukey;
    }
  }
  curr->end = 1;
  curr->value = value;
}

// TODO: Check how we can handle to trailing slashes
// Ex.: registering /posts/:id/comments matches /posts/1/comments but not /posts/1/comments/
// registering /posts/:id/comments/ matches /posts/1/comments/ but not /posts/1/comments
void *trie_search(TrieNode_t *trie, const char *key) {
  TrieNode_t *curr = trie;
  unsigned char *ukey = (unsigned char *)key;
  while (*ukey && curr) {
    if (curr->dynamic) {
      char dynamic_value[32];
      short i = 0;
      while (*ukey && *ukey != '/') {
        dynamic_value[i++] = *ukey;
        ++ukey;
      }
      dynamic_value[i] = '\0';
      printf("Key-Value pair: %s => %s\n", curr->identifier, dynamic_value);
      curr = curr->children[get_index('/')];
      ++ukey;
      continue;
    }
    short index = get_index(*ukey);
    if (index < 0 || index >= CHILDREN_LENGTH) {
      return NULL;
    }
    TrieNode_t *next = curr->children[index];
    if (!next) {
      return NULL;
    }
    ++ukey;
    curr = next;
  }
  if (!curr || !curr->end) {
    return NULL;
  }
  return curr->value;
}

void all(TrieNode_t *trie, char **elements, int *spot, char *str, int index) {
  if (trie == NULL) {
    return;
  }

  for (int i = 0; i < CHILDREN_LENGTH - 1; ++i) {
    if (trie->children[i] != NULL) {
      int str_offset = 0;
      str[index + str_offset++] = get_char(i);
      char *identifier = trie->children[i]->identifier;
      if (identifier[0]) {
        int j = 0;
        while (identifier[j]) {
          str[index + str_offset++] = identifier[j];
          ++j;
        }
      }
      if (trie->children[i]->end) {
        elements[*spot] = malloc(50 * sizeof(char));
        strncpy(elements[*spot], str, index + 1);
        *spot = *spot + 1;
      }
      all(trie->children[i], elements, spot, str, index + str_offset);
    }
  }
}

char **trie_dump(TrieNode_t *trie) {
  char **strs = malloc(20 * sizeof(char *));
  char *str = malloc(50 * sizeof(char));
  int *spot = malloc(sizeof(int));
  *spot = 0;

  all(trie, strs, spot, str, 0);

  free(spot);
  free(str);

  return strs;
}
