#if !defined(SOURCES_H)
#define SOURCES_H

#include<SDL2/SDL.h>

#define createSources(name, maxSources)\
    SourcesData name##__data;\
    SourcesData* name = &name##__data;\
    Source name##__buffer[maxSources];\
    Sources.initSources(name, name##__buffer, maxSources);\

typedef struct Source {
    unsigned int id;
    char* path;
    struct Texture {
       int width;
       int height;
       SDL_Texture* __texture; 
    } texture;
} Source; 

typedef struct {
    unsigned int max;
    unsigned int length;
    Source* buffer;
} SourcesData;

typedef struct {
    void (*initSources)(SourcesData* sources, Source* buffer, unsigned int size);
    Source* (*add)(SourcesData* sources, char* path);
} SourcesType;

SourcesType Sources;

#endif // SOURCES_H
