#include <stdio.h>
#include <assert.h>

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>

#include "Object/Object.h"
#include "App/App.h"
#include "Sources/Sources.h"

#define MAX_OBJECTS 20

ObjectData objects[MAX_OBJECTS];

void renderFunction(AppData* app) {
    SceneIterator iterator;
    App.initSceneIterator(app, &iterator);
    ObjectData* object;
    while((object = App.nextObject(&iterator))) {
        Object.shiftPosition(object, 0, 1);
    }
}

int main(){
    createApp(app, MAX_OBJECTS);

    App.setTitle(app, "Bubbles");
    App.setClearColor(app, 0, 180, 216, 255);
    App.setSize(app, 1280, 720);
    app->logs = 1;
    // App.setRenderFunction(app, renderFunction);

    createSources(sources, 1);
    App.addSources(app, sources);

    Source* bubbleSource = Sources.add(sources, "bubble.png");

    createObject(bubble, bubbleSource);
    
    App.addObject(app, bubble);
    Object.setSize(bubble, 40, 40);
    Object.setPosition(bubble, 40, 40);

    App.start(app);

    return 0;
}