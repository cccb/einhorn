
#include "framebuffer.h"

/*
 * Framebuffer datastructure layout
 * 
 * To keep things simple, the framebuffer is just an
 * array of doubles with the length of: width * height + 2.
 *
 * This first two doubles are the width and height of the
 * framebuffer.
 *
 * Convenience functions for access in lua are implemented.
 */

void framebuffer_init(lua_State* L)
{


}

/*
 * Lua Library Functions
 */


