#if !defined(SOURCES_H)
#define SOURCES_H

#include <SDL2/SDL.h>
#include <SDL2_mixer/SDL_mixer.h>

#define createSources(name, maxSources)\
    SourcesData name##__data;\
    SourcesData* name = &name##__data;\
    Source name##__buffer[maxSources];\
    Sources.initSources(name, name##__buffer, maxSources);\

typedef unsigned int SourceType;

#define SOURCE_TEXTURE 0
#define SOURCE_MUSIC 1
#define SOURCE_AUDIO 2

typedef struct Source {
    unsigned int id;
    char* path;
    SourceType type;

    union SourceData{
        struct Texture {
            int width;
            int height;
            SDL_Texture* __texture; 
        } texture;
        struct Music {
            Mix_Music* __music;
        } music;

        struct Audio {
            Mix_Chunk* __audio;
            float volume;
        } audio;
    } data;
} Source; 

typedef struct {
    unsigned int max;
    unsigned int length;
    Source* buffer;
} SourcesData;

typedef struct {
    void (*initSources)(SourcesData* sources, Source* buffer, unsigned int size);
    Source* (*add)(SourcesData* sources, char* path, SourceType type);
    int (*setAudioVolume)(Source* src, float volume);
} SourcesType;

SourcesType Sources;

#endif // SOURCES_H
