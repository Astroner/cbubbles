#include "Object.h"

#include <stdlib.h>

#include "../App/App.h"

unsigned int initialId = 0;

static void init(ObjectData* data, Source* src) {
    data->__id = initialId++;
    data->src = src;
    
    data->x = 0;
    data->y = 0;
    data->width = 0;
    data->height = 0;

    data->__app = NULL;
}

static void destroy(ObjectData* data) {
    if(data->__app) {
        App.removeObject(data->__app, data);
    }
}

static void setExtraData(ObjectData* obj, void* extraData) {
    obj->extraData = extraData;
}

static void setPosition(ObjectData* data, float x, float y) {
    data->x = x;
    data->y = y;
}

static void shiftPosition(ObjectData* data, float x, float y) {
    data->x += x;
    data->y += y;
}

static void setSize(ObjectData* data, float width, float height) {
    data->width = width;
    data->height = height;
}

ObjectModuleType Object = {
    .init = init,
    .destroy = destroy,
    .setPosition = setPosition,
    .setSize = setSize,
    .shiftPosition = shiftPosition,
    .setExtraData = setExtraData,
};