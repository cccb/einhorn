
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#include "boot.h"
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

    // Load library
    if (luaL_loadbuffer(engine->L,
                        (const char*)boot_lua,
                        boot_lua_len,
                        "boot") != LUA_OK) {

        engine_lua_error(engine->L);
        return NULL;
    }

    if(lua_pcall(engine->L, 0, 0, 0) != LUA_OK) {
        engine_lua_error(engine->L);
        return NULL;
    }

    // Load program
    if (luaL_dofile(engine->L, config->program_filename) != LUA_OK) {
        engine_lua_error(engine->L);
        return NULL;
    }

    // Initialize network encoding
    engine->conn = net_init(config, engine->framebuffer);
    if (!engine->conn) {
        fprintf(stderr, "Network initialization failed.\n");
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

    // Call render function with framebuffer and time
    return lua_pcall(engine->L, 2, 0, 0);
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
        if(engine_call_render(engine, t) != LUA_OK) {
            engine_lua_error(engine->L);
            return -1;
        }

        // Encode result and transmit packet
        net_update(engine->conn);

        // Wait a bit
        engine_limit_fps(engine->config->fps);
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

