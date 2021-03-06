#ifndef CONFIG_H
#define CONFIG_H

#define CONF_DEFAULT_PORT 3123
#define CONF_DEFAULT_FB_WIDTH 1
#define CONF_DEFAULT_FB_HEIGHT 14
#define CONF_DEFAULT_FPS 60.0

typedef struct {
    const char* program_filename;
    const char* host;
    unsigned int port;

    double fps;
    
    unsigned int fb_width;
    unsigned int fb_height;
} einhorn_config;

einhorn_config* config_parse_args(int argc, const char** argv);
int config_validate(einhorn_config* config);
void config_print(einhorn_config* config);
void config_print_errors(int errors);

#endif
