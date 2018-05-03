
#include <stdlib.h>

#include "engine.h"

/*
 * Initialize the engine and pass the configuration
 */
einhorn_engine* engine_init(einhorn_config* config)
{
    einhorn_engine* engine = malloc(sizeof(einhorn_engine));
    if (!engine) {
        sprintf(stderr, "Could not allocate space.");
        return NULL;
    }

}


