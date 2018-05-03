
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include "engine.h"

void engine_lua_error(lua_State *L, const char *fmt, ...);


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

    // Load program
    if (luaL_loadfile(engine->L, config->program_filename) 
        || lua_pcall(engine->L, 0, 0, 0)) {
            engine_lua_error(engine->L,
                             "Error while loading program: %s\n",
                             lua_tostring(engine->L, -1));

        return NULL;
    }

    return engine;
}

/*
 * Main rendering loop
 */
int engine_run(einhorn_engine* engine)
{

    return 0;
}

/*
 * Helper: Print lua error
 */
void engine_lua_error(lua_State *L, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    vfprintf(stderr, fmt, args);
    va_end(args);

    lua_close(L);
}

