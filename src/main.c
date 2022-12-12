#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <time.h>

#if defined(__MINGW)
    #include <windows.h>
#endif // __MINGW

#include "HashTable/HashTable.h"
#include "Object/Object.h"
#include "App/App.h"
#include "Sources/Sources.h"

#define WIDTH 1280
#define HEIGHT 720

#define MAX_OBJECTS 300

typedef struct AppExtra {
    TableData* bubbles;
    ObjectData* bubblesBuffer;
    Source* bubblesSource;
    Source* bgmSource;
    Source* popSource;
} AppExtra;

#define bubblesPerSecond 10

float msUntilNextBubble = 1000 / bubblesPerSecond;

unsigned int nextSecondToAdd = 0;

unsigned int lastSpawn = 0;

void init(AppData* app) {
    AppExtra* appData = app->extraData;

    App.playMusic(app, appData->bgmSource, -1);
}

void tick(AppData* app, TickData* data) {
    static int counter = 0;

    AppExtra* appData = app->extraData;
    
    if((float)data->time - (float)lastSpawn >= msUntilNextBubble) {
        float sizeModifier = (float)rand() / (float)RAND_MAX;
        
        ObjectData* bubble = &appData->bubblesBuffer[counter];

        Object.init(bubble, appData->bubblesSource);
        Object.setSize(bubble, 50 + 100.0f * sizeModifier, 50 + 100.0f * sizeModifier);
        Object.setPosition(bubble, 50 + rand() % (WIDTH - 250), -150);

        App.addObject(app, bubble);

        counter = (counter + 1) % MAX_OBJECTS;
        lastSpawn = data->time;
    }

    SceneIterator iterator;
    App.initSceneIterator(app, &iterator);
    ObjectData* object;
    while((object = App.nextObject(&iterator))) {
        float speedRatio = 1.0f - object->width / 150.0f;
        Object.shiftPosition(object, 0, 1 + 1 * speedRatio);
    }

    App.resetSceneIterator(&iterator);
    while((object = App.nextObject(&iterator))) {
        if(object->y + object->height >= (float)HEIGHT) {
            Object.destroy(object);
            App.playAudio(app, appData->popSource, 0);
        }
    }
}

int main(int argc, char* argv[]){
    #if defined(__MINGW)
        HWND hWnd = GetConsoleWindow();
        ShowWindow( hWnd, SW_HIDE );
    #endif // __MINGW
    

    srand(time(NULL));
    createTable(bubbles, MAX_OBJECTS);

    ObjectData bubblesBuffer[MAX_OBJECTS];

    createApp(app, MAX_OBJECTS);

    App.setTitle(app, "Bubbles");
    App.setClearColor(app, 0, 180, 216, 255);
    App.setSize(app, WIDTH, HEIGHT);
    app->logs = 1;
    App.setTickFunction(app, tick);
    App.setInitFunction(app, init);
    App.setFPS(app, 60);
    createSources(sources, 3);

    App.addSources(app, sources);

    AppExtra appData = {
        .bubbles = bubbles,
        .bubblesBuffer = bubblesBuffer,
        .bubblesSource = Sources.add(sources, "bubble.png", SOURCE_TEXTURE),
        .bgmSource = Sources.add(sources, "bgm.mp3", SOURCE_MUSIC),
        .popSource = Sources.add(sources, "pop.mp3", SOURCE_AUDIO),
    };

    Sources.setAudioVolume(appData.popSource, 0.05f);

    App.setExtraData(app, &appData);

    App.start(app);

    return 0;
}