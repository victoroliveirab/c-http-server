#ifndef TRIE_H
#define TRIE_H

// words can only contain a-z or : or / (for now)
#define CHILDREN_LENGTH 28
#define MAX_IDENTIFIER_LEN 16

struct TrieNode {
  struct TrieNode *children[CHILDREN_LENGTH];
  char identifier[MAX_IDENTIFIER_LEN];
  short end;
  short dynamic;
  void *value;
};

typedef struct TrieNode TrieNode_t;

TrieNode_t * create_trie();
void trie_insert(TrieNode_t *trie, const char *key, void *value);
void *trie_search(TrieNode_t *trie, const char *key);
char ** trie_dump(TrieNode_t *trie);

#endif
