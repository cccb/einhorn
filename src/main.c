
/*
 * Einhorn
 * -------
 *
 * Execute lua programs for generating nice patterns on the Treppe2000.
 *
 * (c) 2018 Chaos Computer Club Berlin e.V.
 */

#include <stdio.h>

#include "version.h"
#include "config.h"
#include "engine.h"

void usage(const char* progname)
{
    printf("Einhorn\t\t\t\t\t\tv.%s\n", EINHORN_VERSION);
    printf("\nUsage: ");
    printf("%s -t <hostname> -p <port> <program_filename>\n", progname);
    printf("Optional Parameters:\n");
    printf("\t-w <width>\t default: %d\tThe width of the framebuffer\n",
        CONF_DEFAULT_FB_WIDTH);
    printf("\t-h <height>\t default: %d\tThe height of the framebuffer\n",
        CONF_DEFAULT_FB_HEIGHT);
}

int main(int argc, const char** argv)
{
    // Initialize config
    einhorn_config* config = config_parse_args(argc, argv);
    if (!config) {
        usage(argv[0]);

        return -1;
    }

    int errors = config_validate(config);
    if (errors) {
        usage(argv[0]);
        config_print_errors(errors);

        return -1;
    }

    config_print(config);

    // Initialize engine
    einhorn_engine* engine = engine_init(config);

    // Start rendering
    engine_run(engine);

    return 0;
}

