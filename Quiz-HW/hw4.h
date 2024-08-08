#include <stdlib.h>
#include <string>
using namespace std;

#define SIZE 109

struct bucket {
    string key;
    void *binding;
    struct bucket *next;
};

struct symbol_table {
    struct bucket **table;
    int size;
};

unsigned int Hash(char *s) {
    unsigned int h = 0;
    char *p;
    for(p = s; *p; p++)
        h = h * 65599 + *p;
    return h;
}

struct bucket *Bucket(string key, void *binding, struct bucket *next) {
    struct bucket *b = (struct bucket *)malloc(sizeof(struct bucket));
    b->key = key;
    b->binding = binding;
    b->next = next;
    return b;
}

void insert(struct symbol_table *st, string key, void *binding) {
    int index = Hash(const_cast<char*>(key.c_str())) % st->size;
    st->table[index] = Bucket(key, binding, st->table[index]);
}

void *lookup(struct symbol_table *st, string key) {
    int index = Hash(const_cast<char*>(key.c_str())) % st->size;
    struct bucket *b;
    for(b = st->table[index]; b; b = b->next) {
        if (strcmp(b->key.c_str(), key.c_str()) == 0)
            return b->binding;
    }
    return NULL;
}

/**
 * Resizes the symbol table by doubling its size and rehashing all the elements.
 * 
 * @param st The symbol table to be resized.
 */
void resize(struct symbol_table *st) {
    int old_size = st->size;
    struct bucket **old_table = st->table;

    st->size *= 2;
    st->table = (struct bucket **)malloc(st->size * sizeof(struct bucket *));
    if (st->table == NULL) {
        // alloc error
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < st->size; i++) {
        st->table[i] = NULL;
    }

    // rehash
    for (int i = 0; i < old_size; i++) {
        struct bucket *b = old_table[i];
        while (b != NULL) {
            struct bucket *next = b->next;
            insert(st, b->key, b->binding);
            free(b);
            b = next;
        }
    }
    free(old_table);
}
