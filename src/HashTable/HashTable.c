#include "HashTable.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static unsigned long hash(unsigned int key) {

    UintNchars i2c = { key };

    unsigned long hash = 5381;

    int c;
    for(size_t i = 0; i < sizeof(unsigned int); i++) {
        c = i2c.bytes[i];
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

static void init(TableData* table, TableItem* buffer, unsigned int bufferSize) {
    table->buffer = buffer;
    table->maxSize = bufferSize;
    table->size = 0;

    for(unsigned int i = 0; i < bufferSize; i++) {
        buffer[i].filled = 0;
    }
}

static int set(TableData* table, unsigned int key, void* value) {
    if(table->size == table->maxSize) return -1;

    unsigned long hashCode = hash(key);
    unsigned int index = hashCode % table->maxSize;

    unsigned int counter = 0;
    while(1) {
        counter++;
        if(counter == table->maxSize) {
            return -1;
        }
        if(!table->buffer[index].filled) {
            table->buffer[index].key = key;
            table->buffer[index].value = value;
            table->buffer[index].filled = 1;
            table->size++;
            return 0;
        } else {
            index = (index + 1) % table->maxSize;
        }
    }
}

static void* get(TableData* table, unsigned int key) {
    unsigned long hashCode = hash(key);
    unsigned int index = hashCode % table->maxSize;

    unsigned int counter = 0;
    while(1) {
        counter++;
        if(counter == table->maxSize) {
            return NULL;
        }
        if(key == table->buffer[index].key) {
            return table->buffer[index].value;
        } else {
            index = (index + 1) % table->maxSize;
        }
    }
}

static int removeItem(TableData* table, unsigned int key, TableItem* removedItem) {
    unsigned long hashCode = hash(key);
    unsigned int index = hashCode % table->maxSize;

    unsigned int counter = 0;
    while(1) {
        counter++;
        if(counter == table->maxSize) {
            return -1;
        }
        if(table->buffer[index].filled && key == table->buffer[index].key) {
            if(removedItem) {
                removedItem->key = table->buffer[index].key;
                removedItem->value = table->buffer[index].value;
            }
            table->buffer[index].key = 0;
            table->buffer[index].value = NULL;
            table->buffer[index].filled = 0;
            table->size--;
            return 0;
        } else {
            index = (index + 1) % table->maxSize;
        }
    }

    return 0;
}

static void initIterator(TableData* table, HashTableIterator* iterator) {
    iterator->cursor = 0;
    iterator->table = table;
}

static TableItem* next(HashTableIterator* iterator) {
    while(1) {
        if(iterator->cursor == iterator->table->maxSize - 1) {
            return NULL;
        }
        if(iterator->table->buffer[iterator->cursor].filled) {
            iterator->cursor++;
            return &iterator->table->buffer[iterator->cursor - 1];
        }
        iterator->cursor++;
    }
}

static void resetIterator(HashTableIterator* iterator) {
    iterator->cursor = 0;
}

HashTableType HashTable = {
    .init = init,
    .set = set,
    .get = get,
    .remove = removeItem,
    .initIterator = initIterator,
    .next = next,
    .resetIterator = resetIterator,
};
