#include "App.h"

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <math.h>

static char* DEFAULT_TITLE = "Title";

static void init(AppData* app, TableData* objects) {
    app->width = 400;
    app->height = 300;
    app->title = DEFAULT_TITLE;

    app->objects = objects;

    app->clearColor[0] = 255;
    app->clearColor[1] = 255;
    app->clearColor[2] = 255;
    app->clearColor[3] = 255;

    app->FPS = 30;

    app->logs = 0;

    app->tick = NULL;
    app->sources = NULL;
}

static void addSources(AppData* app, SourcesData* sources) {
    app->sources = sources;
}


static void setTitle(AppData* app, char* title) {
    app->title = title;
}

static void setSize(AppData* app, unsigned int width, unsigned int height) {
    app->width = width;
    app->height = height;
}

static void removeObject(AppData* app, ObjectData* object) {
    HashTable.remove(app->objects, object->__id, NULL);
    object->__app = NULL;
}

static int addObject(AppData* app, ObjectData* object) {
    HashTable.set(app->objects, object->__id, object);

    object->__app = app;

    return 0;
}

static void setClearColor(AppData* app, unsigned char R, unsigned char G, unsigned char B, unsigned char A) {
    app->clearColor[0] = R;
    app->clearColor[1] = G;
    app->clearColor[2] = B;
    app->clearColor[3] = A;
}

static void initSceneIterator(AppData* app, SceneIterator* iterator) {
    HashTable.initIterator(app->objects, &iterator->iterator);
}

static ObjectData* nextObject(SceneIterator* iterator) {
    TableItem* item = HashTable.next(&iterator->iterator);
    if(item) {
        return item->value;
    }
    return NULL;
}

static void resetSceneIterator(SceneIterator* iterator) {
    HashTable.resetIterator(&iterator->iterator);
}

static void setTickFunction(AppData* app, void (*tick)(struct AppData* app, TickData* data)) {
    app->tick = tick;
}

static void setExtraData(AppData* app, void* extraData) {
    app->extraData = extraData;
}

static void setFPS(AppData* app, unsigned int FPS) {
    app->FPS = FPS;
}

static int start(AppData* app) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow(
        app->title,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        app->width,
        app->height,
        SDL_WINDOW_OPENGL
    );

    if(!window) {
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    if(app->sources) {
        if(app->logs) SDL_Log("Loading sources\n");
        for(unsigned int i = 0; i < app->sources->length; i++) {
            Source* src = &app->sources->buffer[i];
            SDL_Surface* surface = IMG_Load(src->path);
            if(!surface) {
                return -1;
            }
            src->texture.width = surface->w;
            src->texture.height = surface->h;
            src->texture.__texture = SDL_CreateTextureFromSurface(renderer, surface);
            if(app->logs) SDL_Log("Loaded source '%s'. Width: %d, Height: %d\n", src->path, surface->w, surface->h);
            SDL_FreeSurface(surface);
        }
    } else if(app->logs) {
        SDL_Log("Sources was not provided\n");
    }

    int quit = 0;
    SDL_Event e;
    int once = 0;

    TickData tickData = {
        .frame = 0,
        .time = 0,
    };

    unsigned int initialTime;

    while(!quit) {
        float msPerFrame = 1000.0f / (float)app->FPS;
        Uint64 start = SDL_GetPerformanceCounter();

        while(SDL_PollEvent(&e)) {
            if(e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        if(!once) {
            initialTime = SDL_GetTicks();
        }
        tickData.time = SDL_GetTicks() - initialTime;
        if(app->tick) {
            app->tick(app, &tickData);
        }

        SDL_SetRenderDrawColor(
            renderer, 
            app->clearColor[0],
            app->clearColor[1],
            app->clearColor[2],
            app->clearColor[3]
        );
        SDL_RenderClear(renderer);

        SceneIterator iterator;
        App.initSceneIterator(app, &iterator);

        ObjectData* object;
        while((object = App.nextObject(&iterator))) {
            SDL_FRect rect = {
                .x = object->x,
                .y = object->y,
                .w = object->width,
                .h = object->height,
            };

            

            SDL_RenderCopyF(
                renderer, 
                object->src->texture.__texture, 
                NULL, 
                &rect
            );
        }
        
        SDL_RenderPresent(renderer);
        once = 1;
        tickData.frame++;
        Uint64 end = SDL_GetPerformanceCounter();
        
        float timeForFrame = (end - start) * 1000.0f / (float)SDL_GetPerformanceFrequency();

        if(timeForFrame < msPerFrame) {
            SDL_Delay(msPerFrame - timeForFrame);
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

AppModule App = {
    .init = init,
    .removeObject = removeObject,
    .addObject = addObject,
    .setTitle = setTitle,
    .setSize = setSize,
    .start = start,
    .setClearColor = setClearColor,
    .initSceneIterator = initSceneIterator,
    .nextObject = nextObject,
    .resetSceneIterator = resetSceneIterator,
    .setTickFunction = setTickFunction,
    .setExtraData = setExtraData,
    .addSources = addSources,
    .setFPS = setFPS,
};

