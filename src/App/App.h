#if !defined(APP_H)
#define APP_H

#include <stdlib.h>

#include "../Object/Object.h"
#include "../Sources/Sources.h"

#include "../HashTable/HashTable.h"

#define createApp(name, maxObjectsCount)\
    AppData name##__data;\
    AppData* name = &name##__data;\
    TableData name##__objects__data;\
    TableItem name##__objects__buffer[maxObjectsCount];\
    TableData* name##__objects = &name##__objects__data;\
    HashTable.init(name##__objects, name##__objects__buffer, maxObjectsCount);\
    App.init(name, name##__objects);\

typedef struct TickData {
    unsigned int time;
    unsigned int frame;
} TickData;

typedef struct AppData {
    void* extraData;

    TableData* objects;
    SourcesData* sources;

    char* title;
    unsigned int width;
    unsigned int height;

    unsigned char clearColor[4];
    unsigned int logs;

    unsigned int FPS;
    
    void (*tick)(struct AppData* app, TickData* tickData);
    void (*init)(struct AppData* app);
} AppData;

typedef struct {
    HashTableIterator iterator;
} SceneIterator;

typedef struct {
    void (*init)(AppData*, TableData* objects);
    void (*setTitle)(AppData*, char* title);
    void (*setSize)(AppData*, unsigned int width, unsigned int height);
    void (*setClearColor)(AppData*, unsigned char R, unsigned char G, unsigned char B, unsigned char A);
    void (*setExtraData)(AppData* app, void* extraData);
    void (*setFPS)(AppData* app, unsigned int FPS);

    int (*addObject)(AppData*, ObjectData* object);
    void (*removeObject)(AppData*, ObjectData* object);
    
    void (*addSources)(AppData* app, SourcesData* sources);

    int (*playMusic)(AppData* app, Source* music, int loops);
    int (*playAudio)(AppData* app, Source* audio, int loops);
    int (*start)(AppData*);

    void (*initSceneIterator)(AppData* app, SceneIterator* iterator);
    ObjectData* (*nextObject)(SceneIterator* iterator);
    void (*resetSceneIterator)(SceneIterator* iterator);

    void (*setTickFunction)(AppData* app, void (*tick)(AppData* app, TickData* tickData));
    void (*setInitFunction)(AppData* app, void (*init)(AppData* app));
} AppModule;

AppModule App;

#endif // APP_H
