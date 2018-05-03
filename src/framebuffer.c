
#include "framebuffer.h"

/*
 * Framebuffer datastructure layout
 * 
 * To keep things simple, the framebuffer is just an
 * array of doubles with the length of: 
 * width * height * 4 + 2.
 *
 * This first two doubles are the width and height of the
 * framebuffer.
 *
 * Convenience functions for access in lua are implemented.
 */

#include <stdio.h>
#include <stdlib.h>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include "framebuffer.h"

int l_framebuffer_debug(lua_State* L);
int l_framebuffer_setpixel(lua_State* L);

double* framebuffer_init(einhorn_engine* engine)
{
    size_t bufsize = 2 + (4 * engine->config->fb_width *
                              engine->config->fb_height);
    double* buffer = (double*)calloc(bufsize, sizeof(double));

    buffer[0] = engine->config->fb_width;
    buffer[1] = engine->config->fb_height;

    // Register framebuffer library
    lua_newtable(engine->L);
    lua_pushstring(engine->L, "debug");
    lua_pushcfunction(engine->L, l_framebuffer_debug);
    lua_settable(engine->L, -3);

    lua_pushstring(engine->L, "setpixel");
    lua_pushcfunction(engine->L, l_framebuffer_setpixel);
    lua_settable(engine->L, -3);

    lua_setglobal(engine->L, "framebuffer");

    return buffer;
}

/*
 * Helper functions: get framebuffer value
 */
double framebuffer_get(double* buffer, size_t x, size_t y, size_t offset)
{
    size_t width = buffer[0];
    size_t height = buffer[1];

    size_t index = ((x % width) * 4) + ((y % height) * width * 4) + offset;
    if (index > width * height * 4) {
        return -1;
    }
    
    return buffer[index + 2];
}

/*
 * Helper functions: set framebuffer value
 */
int framebuffer_set(double* buffer,
                       size_t x,
                       size_t y,
                       size_t offset,
                       double value)
{
    size_t width = buffer[0];
    size_t height = buffer[1];

    size_t index = ((x % width) * 4) + ((y % height) * width * 4) + offset;
    if (index > width * height * 4) {
        return -1;
    }

    buffer[index + 2] = value;
    
    return 0;
}


/*
 * Lua Library Functions
 */

/*
 * Debug framebuffer, prints the framebuffer content
 */
int l_framebuffer_debug(lua_State* L)
{
    // Get framebuffer
    double* buffer = (double*)lua_touserdata(L, 1);
    if(!buffer) {
        fprintf(stderr, "Getting the framebuffer pointer failed.\n");
        return 0;
    }

    size_t width = (size_t)buffer[0];
    size_t height = (size_t)buffer[1];

    for(size_t y = 0; y < height; y++) {
        for(size_t x = 0; x < width; x++) {
            printf("%f %f %f %f  ", framebuffer_get(buffer, x, y, 0),
                                    framebuffer_get(buffer, x, y, 1),
                                    framebuffer_get(buffer, x, y, 2),
                                    framebuffer_get(buffer, x, y, 3));
                                    
        }
    }

    printf("\n");

    return 0;
}

/*
 * Helper: get number from table
 */
double lua_table_get_number(lua_State* L, int table, int index)
{
    lua_pushnumber(L, index);
    lua_gettable(L, table);

    return luaL_optnumber(L, -1, 0.0);
}

/*
 * Set framebuffer value
 */
int l_framebuffer_setpixel(lua_State* L)
{
    // Get framebuffer
    double* buffer = (double*)lua_touserdata(L, 1);
    luaL_argcheck(L, buffer != NULL, 1, "expected a framebuffer");
 
    size_t width = (size_t)buffer[0];
    size_t height = (size_t)buffer[1];

    // Second param should be a table with x, y coordinates
    // third a table with rgba values
    luaL_checktype(L, 2, LUA_TTABLE);
    luaL_checktype(L, 3, LUA_TTABLE);
    
    // Get position
    int x = (int)lua_table_get_number(L, 2, 1) - 1;
    int y = (int)lua_table_get_number(L, 2, 2) - 1; 

    luaL_argcheck(L, x >= 0 && x < width,  2, "invalid index for position.x");
    luaL_argcheck(L, y >= 0 && y < height, 2, "invalid index for position.y");

    // Get r, g, b
    double r = lua_table_get_number(L, 3, 1);
    double g = lua_table_get_number(L, 3, 2); 
    double b = lua_table_get_number(L, 3, 3); 
    double a = lua_table_get_number(L, 3, 4); 

    framebuffer_set(buffer, x, y, 0, r);
    framebuffer_set(buffer, x, y, 1, g);
    framebuffer_set(buffer, x, y, 2, b);
    framebuffer_set(buffer, x, y, 3, a);

    return 0;
}

int l_framebuffer_getpixel(lua_State* L)
{
    return 0;
}


