#include "Sources.h"

#include<stdlib.h>

static void initSources(SourcesData* sources, Source* buffer, unsigned int size) {
    sources->buffer = buffer;
    sources->length = 0;
    sources->max = size;
}

static Source* add(SourcesData* sources, char* path, SourceType type) {
    if(sources->length == sources->max) return NULL;

    Source* src = &sources->buffer[sources->length];

    src->path = path;
    src->id = sources->length;
    src->type = type;

    sources->length++;

    if(type == SOURCE_TEXTURE) {
        src->data.texture.height = 0;
        src->data.texture.width = 0;
        src->data.texture.__texture = NULL;
    } else if(type == SOURCE_MUSIC) {
        src->data.music.__music = NULL;
    } else if(type == SOURCE_AUDIO) {
        src->data.audio.__audio = NULL;
        src->data.audio.volume = 1.0f;
    }

    return src;
}

static int setAudioVolume(Source* src, float volume) {
    if(src->type != SOURCE_AUDIO) {
        return -1;
    }
    src->data.audio.volume = volume;
    if(src->data.audio.__audio) {
        Mix_VolumeChunk(src->data.audio.__audio, (int)(MIX_MAX_VOLUME * volume));
    }
    return 0;
}

SourcesType Sources = {
    .initSources = initSources,
    .add = add,
    .setAudioVolume = setAudioVolume,
};
