
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "engine.h"
#include "framebuffer.h"

void engine_lua_error(lua_State *L);


/*
 * Initialize the engine and pass the configuration
 */
einhorn_engine* engine_init(einhorn_config* config)
{
    einhorn_engine* engine = malloc(sizeof(einhorn_engine));
    if (!engine) {
        fprintf(stderr, "Could not allocate space.");
        return NULL;
    }

    // Initialize engine
    engine->config = config;

    // Initialize Lua state    
    engine->L = luaL_newstate();
    luaL_openlibs(engine->L);

    // Initialize framebuffer
    engine->framebuffer = framebuffer_init(engine);
    if (!engine->framebuffer) {
        fprintf(stderr, "Allocating the framebuffer failed.\n");
        return NULL;
    }

    // Load program
    if (luaL_loadfile(engine->L, config->program_filename) 
        || lua_pcall(engine->L, 0, 0, 0)) {
            engine_lua_error(engine->L);
        return NULL;
    }

    return engine;
}

/*
 * Helper: Get time delta
 */
double engine_get_timedelta(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) + (t1.tv_usec - t0.tv_usec) / 1000000.0;
}

/*
 * Helper: Limit FPS
 */
void engine_limit_fps(double fps)
{
    double timeout = 1.0 / fps;
    // Convert to timeval
    double nsec = fmod(timeout, 1.0) * 1000000000.0; 
    struct timespec timeout_val = {
        .tv_sec = (int)timeout,
        .tv_nsec = (int)nsec,
    };
    nanosleep(&timeout_val, NULL);
}

/*
 * Call the lua render function
 */
int engine_call_render(einhorn_engine* engine, double t)
{
    // Push args on the stack
    lua_getglobal(engine->L, "render");
    lua_pushlightuserdata(engine->L, engine->framebuffer);
    luaL_setmetatable(engine->L, "framebuffer");
    lua_pushnumber(engine->L, t);

    // Call render function with two arguments (fb and time)
    int res = lua_pcall(engine->L, 2, 0, 0); 
    if (res != 0) {
        engine_lua_error(engine->L);
        return -1;
    }

    // We don't expect it to return anything.
    // Updates to the framebuffer are done in place.

    return 0;
}

/*
 * Main rendering loop
 */
int engine_run(einhorn_engine* engine)
{
    struct timeval t0;
    struct timeval t1;

    gettimeofday(&t0, NULL);

    while(42) {
        gettimeofday(&t1, NULL);
        double t = engine_get_timedelta(t0, t1);
        int ret = engine_call_render(engine, t); 
        if (ret != 0) {
            return -1;
        }

        engine_limit_fps(60.0);
    }
      
    return 0;
}

/*
 * Helper: Print lua error
 */
void engine_lua_error(lua_State *L)
{
    const char* error = lua_tostring(L, -1);

    fprintf(stderr, "----BEGIN SCRIPT ERROR----\n");
    fprintf(stderr, "%s\n", error);
    fprintf(stderr, "----END SCRIPT ERROR----\n");

    lua_close(L);
}

