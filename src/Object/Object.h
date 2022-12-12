#if !defined(OBJECT_H)
#define OBJECT_H

#include<SDL2/SDL.h>

#include "../Sources/Sources.h"

#define createObject(name, source)\
    ObjectData name##__data;\
    ObjectData* name = &name##__data;\
    Object.init(name, source);\

typedef struct {
    unsigned int __id;

    float x;
    float y;
    float width;
    float height;

    Source* src;

    void* __app;

    void* extraData;
} ObjectData;

typedef struct {
    void (*init)(ObjectData* data, Source* sprite);
    void (*destroy)(ObjectData*);
    void (*setPosition)(ObjectData*, float x, float y);
    void (*shiftPosition)(ObjectData*, float x, float y);
    void (*setSize)(ObjectData*, float width, float height);
    void (*setExtraData)(ObjectData* obj, void* extraData);
} ObjectModuleType;

ObjectModuleType Object;

#endif // OBJECT_H
