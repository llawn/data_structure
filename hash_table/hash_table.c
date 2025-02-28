#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash_table.h"

Node *create_node(const char *key, int value) {
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->key = strdup(key);
    new_node->value = value;
    return new_node;
}

HashTable *create_table(int initial_capacity, float load_factor) {
    HashTable *ht = (HashTable *)malloc(sizeof(HashTable));
    ht->capacity = initial_capacity;
    ht->size = 0;
    ht->load_factor = load_factor;
    ht->table = (Node **)calloc(initial_capacity, sizeof(Node *));
    return ht;
}

void free_table(HashTable *ht) {
    for (int i = 0; i < ht->capacity; i++) {
        Node *current = ht->table[i];
        while (current) {
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(ht->table);
    free(ht);
}

void print_table(HashTable *ht){
    printf("[");
    int first = 1;
    for (int i=0; i < ht->capacity; i++) {
        Node *current = ht->table[i];
        while (current) {
            if (!first) {
                printf(", ");
            } else {
                first = 0;
            }
            printf("(%s, %d)", current->key, current->value);
            current = current->next;
        }
    }
    printf("]\n");
}

// djb2 hash function
unsigned long hash(const char *key, int capacity)  {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % capacity;
}

void insert(HashTable *ht, const char *key, int value) {
    if ((float)ht->size / ht->capacity > ht->load_factor) {
        resize(ht);
    }

    int index = hash(key, ht->capacity);
    Node *current = ht->table[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            current->value = value;
            return;
        }
        current = current->next;
    }

    // create node at the head of the chained array
    Node *new_node = create_node(key, value);
    new_node->next = ht->table[index];
    ht->table[index] = new_node;
    ht->size++;
}

void resize(HashTable *ht) {
    int old_capacity = ht->capacity;
    Node **old_table = ht->table;

    ht->capacity *= 2;
    ht->size = 0;
    ht->table = (Node **)calloc(ht->capacity, sizeof(Node *));

    for (int i = 0; i < old_capacity; i++) {
        Node *current = old_table[i];
        while (current) {
            insert(ht, current->key, current->value);
            Node *temp = current;
            current = current->next;
            free(temp->key);
            free(temp);
        }
    }
    free(old_table);
}

int search(HashTable *ht, const char *key) {
    int index = hash(key, ht->capacity);
    Node *current = ht->table[index];

    while (current) {
        if (strcmp(current->key, key) == 0) {
            return current->value;
        }
        current = current->next;
    }

    fprintf(stderr, "KeyError: %s not found\n", key);
    exit(EXIT_FAILURE);
}

void remove_key(HashTable *ht, const char *key) {
    int index = hash(key, ht->capacity);
    Node *current = ht->table[index];
    Node *previous = NULL;

    while (current) {
        if (strcmp(current->key, key) == 0) {
            if (previous) {
                previous->next = current->next;
            } else {
                ht->table[index] = current->next;
            }
            free(current->key);
            free(current);
            ht->size--;
            return;
        }
        previous = current;
        current = current->next;
    }

    fprintf(stderr, "KeyError: %s not found\n", key);
    exit(EXIT_FAILURE);
}

void test_hash_table() {
    HashTable *ht = create_table(4, 0.7);
    print_table(ht);
    printf("Capacity: %d\n", ht->capacity);

    insert(ht, "apple", 3);
    insert(ht, "banana", 2);
    insert(ht, "cherry", 5);
    insert(ht, "blueberry", 5);
    
    print_table(ht);
    printf("Capacity after insertion: %d\n", ht->capacity);

    int size = 4;
    if (ht->size != size){
        fprintf(stderr, "Size mismatch, expected %d, got %d\n", size, ht->size);
    }

    if (search(ht, "apple") == 3) {
        printf("Found apple: %d\n", search(ht, "apple"));
    } else {
        fprintf(stderr, "apple not found\n");
    }

    if (search(ht, "banana") == 2) {
        printf("Found banana: %d\n", search(ht, "banana"));
    } else {
        fprintf(stderr, "banana not found\n");
    }

    insert(ht, "banana", 4);
    print_table(ht);
    if (search(ht, "banana") == 4) {
        printf("Updated banana: %d\n", search(ht, "banana"));
    } else {
        fprintf(stderr, "banana not found after update\n");
    }

    remove_key(ht, "apple");
    size--;
    print_table(ht);
    if (ht->size != size) {
        fprintf(stderr, "Size mismatch after removal: expected %d, got %d\n", size, ht->size);
    }
    free_table(ht);
}

int main() {
    test_hash_table();
    printf("All tests passed!\n");
    return 0;
}
