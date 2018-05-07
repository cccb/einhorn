#ifndef ENGINE_H
#define ENGINE_H

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "config.h"
#include "net.h"

typedef struct {
    lua_State* L;
    einhorn_config* config;

    double* framebuffer;

    int         net_socket;
    net_packet* net_packet;
} einhorn_engine;


einhorn_engine* engine_init(einhorn_config* config);
int engine_run(einhorn_engine* engine);

#endif
