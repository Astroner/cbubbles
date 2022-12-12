#include "App.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_mixer/SDL_mixer.h>
#include <math.h>

static char* DEFAULT_TITLE = "Title";

static void init(AppData* app, TableData* objects) {
    app->width = 400;
    app->height = 300;
    app->title = DEFAULT_TITLE;

    app->objects = objects;
    app->sources = NULL;

    app->clearColor[0] = 255;
    app->clearColor[1] = 255;
    app->clearColor[2] = 255;
    app->clearColor[3] = 255;

    app->FPS = 30;

    app->logs = 0;

    app->tick = NULL;
    app->init = NULL;
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

static void setInitFunction(AppData* app, void (*init)(AppData* app)) {
    app->init = init;
}

static int playMusic(AppData* app, Source* music, int loops) {
    if(music->type != SOURCE_MUSIC) {
        if(app->logs) SDL_Log("Cannot play source '%s'. It's not a music type.\n", music->path);
        return -1;
    }
    Mix_PlayMusic(music->data.music.__music, loops);
    return 0;
}

static int playAudio(AppData* app, Source* audio, int loops) {
    if(audio->type != SOURCE_AUDIO) {
        if(app->logs) SDL_Log("Cannot play source '%s'. It's not a audio type.\n", audio->path);
        return -1;
    }
    Mix_PlayChannel(-1, audio->data.audio.__audio, loops);
    return 1;
}

static int start(AppData* app) {
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        return -1;
    }
    if(MIX_INIT_MP3 != Mix_Init(MIX_INIT_MP3)) {
        return -1;
    }

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 640);
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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

    if(app->sources) {
        if(app->logs) SDL_Log("Loading sources\n");
        for(unsigned int i = 0; i < app->sources->length; i++) {
            Source* src = &app->sources->buffer[i];
            if(src->type == SOURCE_TEXTURE) {
                SDL_Surface* surface = IMG_Load(src->path);
                if(!surface) {
                    if(app->logs) SDL_Log("Failed to loaded image source '%s'\n", src->path);
                    return -1;
                }
                src->data.texture.width = surface->w;
                src->data.texture.height = surface->h;
                src->data.texture.__texture = SDL_CreateTextureFromSurface(renderer, surface);
                if(app->logs) SDL_Log("Loaded image source '%s'. Width: %d, Height: %d\n", src->path, surface->w, surface->h);
                SDL_FreeSurface(surface);
            } else if(src->type == SOURCE_MUSIC) {
                src->data.music.__music = Mix_LoadMUS(src->path);
                if(!src->data.music.__music) {
                    if(app->logs) SDL_Log("Failed to loaded music source '%s'\n", src->path);
                    return -1;
                }
                if(app->logs) SDL_Log("Loaded music source '%s'.\n", src->path);
            } else if(src->type == SOURCE_AUDIO) {
                src->data.audio.__audio = Mix_LoadWAV(src->path);
                if(!src->data.audio.__audio) {
                    if(app->logs) SDL_Log("Failed to loaded audio source '%s'\n", src->path);
                    return -1;
                }
                Mix_VolumeChunk(src->data.audio.__audio, (int)(MIX_MAX_VOLUME * src->data.audio.volume));
                if(app->logs) SDL_Log("Loaded audio source '%s'.\n", src->path);
            }
        }
    } else if(app->logs) {
        SDL_Log("Sources was not provided\n");
    }

    if(app->init) {
        app->init(app);
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
                object->src->data.texture.__texture, 
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

    // freeing music
    if(app->sources) {
        for(unsigned int i = 0; i < app->sources->length; i++) {
            Source* current = &app->sources->buffer[i];
            if(current->type == SOURCE_MUSIC) {
                Mix_FreeMusic(current->data.music.__music);
            }
            if(current->type == SOURCE_AUDIO) {
                Mix_FreeChunk(current->data.audio.__audio);
            }
        }
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
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
    .setInitFunction = setInitFunction,
    .playMusic = playMusic,
    .playAudio = playAudio,
};

