#ifndef ENGINE_H
#define ENGINE_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "config.h"

typedef struct {
    lua_State* L;
    einhorn_config* config;
} einhorn_engine;


einhorn_engine* engine_init(einhorn_config* config);
void engine_run(einhorn_engine* self);


#endif
