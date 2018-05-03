
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "config.h"

enum {
    CONF_OPT_NONE = 0,
    CONF_OPT_HOST,
    CONF_OPT_PORT,
    CONF_OPT_WIDTH,
    CONF_OPT_HEIGHT,
};

enum {
    CONF_HOST_MISSING_ERROR = 1,
    CONF_PROGRAM_MISSING_ERROR = 2,
    CONF_PROGRAM_FILE_NOT_FOUND = 4,
};


/*
 * Config Helper: Check config validity
 */
int config_validate(einhorn_config* config)
{
    int errors = 0;

    if(!config->host) {
        errors |= CONF_HOST_MISSING_ERROR; 
    }
    if(!config->program_filename) {
        errors |= CONF_PROGRAM_MISSING_ERROR;
    }

    return errors;
}

/*
 * Config Helper: Print errors
 */
void config_print_errors(int errors)
{
    if(errors == 0) {
        return;
    }

    fprintf(stderr, "Errors:\n");

    if(errors & CONF_HOST_MISSING_ERROR) {
        fprintf(stderr, "\tHostname is missing\n");
    }
    if(errors & CONF_PROGRAM_MISSING_ERROR) {
        fprintf(stderr, "\tNo program file was specified\n");
    }
}

/*
 * Config Helper: Print configuration
 */
void config_print(einhorn_config* config)
{
    printf("Framebuffer dimensions:\t%d x %d\n", config->fb_width,
                                                config->fb_height);
    printf("Rendering program:\t%s\n", config->program_filename);
    printf("Sending packets to:\t%s:%d\n", config->host, config->port);
}

/*
 * Parse commandline arguments
 */
einhorn_config* config_parse_args(int argc, const char** argv)
{
    einhorn_config* config = calloc(1, sizeof(einhorn_config));
    if(!config) {
        printf("Could not allocate space.\n");
        return NULL;
    }

    // Defaults:
    config->port = CONF_DEFAULT_PORT;
    config->fb_width = CONF_DEFAULT_FB_WIDTH;
    config->fb_height = CONF_DEFAULT_FB_HEIGHT;

    int opt = CONF_OPT_NONE;
    for(int i = 1; i < argc; i++) {
        // In case arg is an opt switch:
        if (argv[i][0] == '-') {
            if (strlen(argv[i]) < 2) {
                continue;
            }

            // Determine opt type
            switch(argv[i][1]) {
                case 't':
                    opt = CONF_OPT_HOST;
                    break;
                case 'p':
                    opt = CONF_OPT_PORT;
                    break;
                case 'w':
                    opt = CONF_OPT_WIDTH;
                    break;
                case 'h':
                    opt = CONF_OPT_HEIGHT;
                    break;
                default:
                    opt = CONF_OPT_NONE;
            }
        }
        else {
            // Arg is a param
            switch(opt) {
                case CONF_OPT_NONE:
                    // We only accept the filename as param
                    config->program_filename = argv[i];
                    break;
                case CONF_OPT_HOST:
                    config->host = argv[i];
                    break;
                case CONF_OPT_PORT:
                    config->port = atoi(argv[i]);
                    break;
                case CONF_OPT_WIDTH:
                    config->fb_width = atoi(argv[i]);
                    break;
                case CONF_OPT_HEIGHT:
                    config->fb_height = atoi(argv[i]);
                    break;
            }

            opt = CONF_OPT_NONE;
        }
    }

    return config;
}



