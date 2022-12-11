#if !defined(HASH_TABLE_H)
#define HASH_TABLE_H

#define createTable(name, size)\
    TableData name##__data;\
    TableItem name##__buffer[size];\
    TableData* name = &name##__data;\
    HashTable.init(name, name##__buffer, size);\

typedef union UintNchars {
    unsigned int number;
    char bytes[sizeof(unsigned int)];
} UintNchars;

typedef struct {
    int filled;
    unsigned int key;
    void* value;
} TableItem;

typedef struct {
    unsigned int maxSize;
    unsigned int size;
    TableItem* buffer;
} TableData;

typedef struct {
    TableData* table;
    unsigned int cursor;
} HashTableIterator;

typedef struct {
    void (*init)(TableData* table, TableItem* buffer, unsigned int bufferSize);
    int (*set)(TableData* table, unsigned int key, void* value);
    void* (*get)(TableData* table, unsigned int key);
    int (*remove)(TableData* table, unsigned int key, TableItem* removedItem);
    void (*initIterator)(TableData* table, HashTableIterator* iterator);
    TableItem* (*next)(HashTableIterator* iterator);
    void (*resetIterator)(HashTableIterator* iterator);
} HashTableType;

HashTableType HashTable;

#endif // HASH_TABLE_H
