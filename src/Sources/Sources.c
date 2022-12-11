#include "Sources.h"

#include<stdlib.h>

static void initSources(SourcesData* sources, Source* buffer, unsigned int size) {
    sources->buffer = buffer;
    sources->length = 0;
    sources->max = size;
}

static Source* add(SourcesData* sources, char* path) {
    if(sources->length == sources->max) return NULL;

    sources->buffer[sources->length].path = path;
    sources->buffer[sources->length].id = sources->length;
    sources->buffer[sources->length].texture.width = 0;
    sources->buffer[sources->length].texture.height = 0;
    sources->buffer[sources->length].texture.__texture = NULL;
    sources->length++;

    return &sources->buffer[sources->length];
}

SourcesType Sources = {
    .initSources = initSources,
    .add = add,
};
