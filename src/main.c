
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

void usage(const char* progname)
{
    printf("Einhorn\t\t\t\t\t\tv.%s\n", EINHORN_VERSION);
    printf("\nUsage: ");
    printf("%s -h <hostname> -p <port> <program_filename>\n", progname);
}

einhorn_config* parse_args(int argc, const char** argv)
{

    return NULL;
}

int main(int argc, const char** argv)
{
    einhorn_config* config = parse_args(argc, argv);
    if (!config) {
        usage(argv[0]);

        return -1;
    }

    return 0;
}

