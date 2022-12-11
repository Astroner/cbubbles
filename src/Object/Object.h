#if !defined(OBJECT_H)
#define OBJECT_H

#include<SDL2/SDL.h>

#include "../Sources/Sources.h"

#define createObject(name, source)\
    ObjectData name##__data;\
    ObjectData* name = &name##__data;\
    Object.init(name, source);\

typedef struct {
    SDL_Rect box;
    unsigned int __id;
    Source* src;
    void* __app;
    void* extraData;
} ObjectData;

typedef struct {
    void (*init)(ObjectData* data, Source* sprite);
    void (*destroy)(ObjectData*);
    void (*setPosition)(ObjectData*, int x, int y);
    void (*shiftPosition)(ObjectData*, int x, int y);
    void (*setSize)(ObjectData*, int width, int height);
    void (*setExtraData)(ObjectData* obj, void* extraData);
} ObjectModuleType;

ObjectModuleType Object;

#endif // OBJECT_H
