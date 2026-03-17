#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <syslog.h>
#include <errno.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <limits.h>

/*
 * Writer app
 */

static int mkdir_p(const char *dir)
{
    char tmp[PATH_MAX];
    char *p = NULL;
    size_t len;

    if (dir == NULL || *dir == '\0')
    {
        return -1;
    }

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);

    if (tmp[len - 1] == '/')
    {
        tmp[len - 1] = '\0';
    }

    for (p = tmp + 1; *p != '\0'; p++)
    {
        if (*p == '/')
        {
            *p = '\0';

            if (mkdir(tmp, 0755) == -1 && errno != EEXIST)
            {
                return -1;
            }

            *p = '/';
        }
    }

    if (mkdir(tmp, 0755) == -1 && errno != EEXIST)
    {
        return -1;
    }

    return 0;
}

int main(int argc, char *argv[])
{
    openlog("writer", LOG_PID | LOG_CONS, LOG_USER);
    syslog(LOG_INFO, "Program started");

    if (argc != 3)
    {
        fprintf(stderr, "Error: missing arguments.\n");
        fprintf(stderr, "Usage: %s <writefile> <writestr>\n", argv[0]);
        syslog(LOG_ERR, "Invalid number of arguments");
        closelog();
        return EXIT_FAILURE;
    }

    if (strlen(argv[1]) == 0 || strlen(argv[2]) == 0)
    {
        fprintf(stderr, "Error: writefile and writestr must be specified and non-empty.\n");
        syslog(LOG_ERR, "writefile and writestr must be specified and non-empty");
        closelog();
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    char *content = argv[2];

    char *path_copy = strdup(filename);
    if (path_copy == NULL)
    {
        perror("strdup");
        syslog(LOG_ERR, "strdup failed");
        closelog();
        return EXIT_FAILURE;
    }

    char *dir = dirname(path_copy);

    if (mkdir_p(dir) == -1)
    {
        perror("mkdir");
        syslog(LOG_ERR, "Error creating directory path %s", dir);
        free(path_copy);
        closelog();
        return EXIT_FAILURE;
    }

    free(path_copy);

    FILE *file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        syslog(LOG_ERR, "Error opening file %s", filename);
        closelog();
        return EXIT_FAILURE;
    }

    syslog(LOG_DEBUG, "Writing %s to %s", content, filename);

    if (fprintf(file, "%s", content) < 0)
    {
        perror("Error writing file");
        syslog(LOG_ERR, "Error writing to file %s", filename);
        fclose(file);
        closelog();
        return EXIT_FAILURE;
    }

    fclose(file);
    closelog();
    return EXIT_SUCCESS;
}