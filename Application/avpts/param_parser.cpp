#define _CRT_SECURE_NO_WARNINGS

#include <zstack.h>

#include <3rdparty/getopt.h>

#include "main.h"

#ifdef _WIN32
#include <Windows.h>
#else
#include <time.h>
#endif

enum {
    OPTION_VERSION = 1,
    OPTION_HELP,
    OPTION_LOG,
    OPTION_CHANNEL,
    OPTION_DUMP,
};

static struct option opts[] = {
    {"version", no_argument, 0, OPTION_VERSION},
    {"help", no_argument, 0, OPTION_HELP},
    {"log", required_argument, 0, OPTION_LOG},
    {"channel", required_argument, 0, OPTION_CHANNEL},
    {"dump", required_argument, 0, OPTION_DUMP},
    {0, 0, 0, 0}
};

int param_parser(int argc, char *argv[], struct application *app)
{
    int c;
    
#ifdef _WIN32
    SYSTEMTIME time;
#else
    time_t seconds;
    struct tm *tm;
#endif

    if (argc <= 1)
        return -1;

    while((c=getopt_long(argc, argv, "", opts, NULL))!=-1){
        switch (c) {
        case OPTION_VERSION:
            log_info("Version: %s\n", version);
            return -1;
        case OPTION_HELP:
            log_info(usage);
            return -1;
        case OPTION_LOG:
            if (strlen(optarg) >= LOG_CONFIG_LENGTH) {
                log_info("log config out of range (0, %d)\n", LOG_CONFIG_LENGTH);
                return -1;
            }
            strncpy(app->param.log_config, optarg, LOG_CONFIG_LENGTH);
            break;
        case OPTION_CHANNEL:
            app->param.channel = atoi(optarg);
            break;
        case OPTION_DUMP:
            app->param.dump_flag = atoi(optarg);
            break;
        default:
            return -1;
        }
    }

    if (optind < argc) {
        if (strlen(argv[optind]) >= 16)
            return -1;
        memcpy(app->param.ip, argv[optind], strlen(argv[optind]));
    }

    if (optind + 1 < argc) {
        if (strlen(argv[optind + 1]) >= FILENAME_MAX)
            return -1;
        memcpy(app->param.input_filename, argv[optind + 1], strlen(argv[optind + 1]));
    }

    // Do param validation
    if (0 == app->param.output_filename[0]) {
#ifdef _WIN32
        GetLocalTime(&time);
        _snprintf(app->param.output_filename, FILENAME_MAX, "avpts_%d-%02d-%02d_%02d-%02d-%02d.log",
            time.wYear,
            time.wMonth,
            time.wDay,
            time.wHour,
            time.wMinute,
            time.wSecond
            );
#else
        seconds = time(NULL);
        tm = localtime(&seconds);
        snprintf(app->param.output_filename, FILENAME_MAX, "avpts_%d-%02d-%02d_%02d-%02d-%02d.log",
            tm->tm_year + 1900,
            tm->tm_mon + 1,
            tm->tm_day,
            tm->tm_hour,
            tm->tm_min,
            tm->tm_sec
            );
#endif
    }

    return 0;
}
