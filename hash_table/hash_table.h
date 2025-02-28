#ifndef HASH_TABLE_H
#define HASH_TABLE_H

typedef struct Node {
    char *key;
    int value;
    struct Node *next;
} Node;

typedef struct HashTable {
    int capacity;
    int size;
    Node **table;
    float load_factor;
} HashTable;

Node *create_node(const char *key, int value);
HashTable *create_table(int initial_capacity, float load_factor);
void free_table(HashTable *ht);
void print_table(HashTable *ht);
unsigned long hash(const char *key, int capacity);
void insert(HashTable *ht, const char *key, int value);
void resize(HashTable *ht);
int search(HashTable *ht, const char *key);
void remove_key(HashTable *ht, const char *key);

#endif
