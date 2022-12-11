#include "Object.h"

#include <stdlib.h>

#include "../App/App.h"

unsigned int initialId = 0;

static void init(ObjectData* data, Source* src) {
    data->__id = initialId++;
    data->src = src;
    
    data->box.x = 0;
    data->box.y = 0;
    data->box.w = 0;
    data->box.h = 0;

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

static void setPosition(ObjectData* data, int x, int y) {
    data->box.x = x;
    data->box.y = y;
}

static void shiftPosition(ObjectData* data, int x, int y) {
    data->box.x += x;
    data->box.y += y;
}

static void setSize(ObjectData* data, int width, int height) {
    data->box.w = width;
    data->box.h = height;
}

ObjectModuleType Object = {
    .init = init,
    .destroy = destroy,
    .setPosition = setPosition,
    .setSize = setSize,
    .shiftPosition = shiftPosition,
    .setExtraData = setExtraData,
};