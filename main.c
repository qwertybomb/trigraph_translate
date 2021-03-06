#include <errno.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define WRITE_BUFFER(x, size)                                            \
    do {                                                                 \
        memcpy(buffer + BUFFER_SIZE + write_buffer_length, (x), (size)); \
        write_buffer_length += (size);                                   \
    } while(0)                                                           \

int main(int argc, char **argv)
{
    int j;

    /* ignore the first argument */
    --argc;
    ++argv;

    /* check for invalid arguments */
    if(argc % 2 != 0) {
        fprintf(stderr, "Error: invalid arguments\n");
        fprintf(stderr, "Usage: %s input output ...\n", argv[-1]);
        fprintf(stderr, "Example: %s main.c result.c \n", argv[-1]);
        return -1;
    }

    for(j = 0; j < argc; j += 2) {
        char buffer[BUFFER_SIZE + BUFFER_SIZE * 3];
        FILE *read_file, *write_file;
        size_t bytes_read = BUFFER_SIZE;

        if(strcmp(argv[j], argv[j + 1]) == 0) {
            printf("Warning: using the same file for input and output is not supported\n");
            continue;
        }

        /* open a file for reading */
        if((read_file = fopen(argv[j], "r")) == NULL) {
            fprintf(stderr, "Error: could not open %s\n%s", argv[j], strerror(errno));
            return -2;
        }

        /* open a file for writing */
        if((write_file = fopen(argv[j + 1], "w")) == NULL) {
            fprintf(stderr, "Error: could not open %s\n%s", argv[j + 1], strerror(errno));
            return -3;
        }

        /* read the file in BUFFER_SIZE chunks */
        while (bytes_read == BUFFER_SIZE) {
            size_t i, write_buffer_length;
            bytes_read = fread(buffer, 1, BUFFER_SIZE, read_file);

            /* process each character in the buffer
             * and if needed convert it to a trigraph
             */

            write_buffer_length = 0;
            for (i = 0; i < bytes_read; ++i) {
                char const ch = buffer[i];

                switch (ch) {
                    case '#':
                        WRITE_BUFFER("\?\?=", 3);
                        break;
                    case '[':
                        WRITE_BUFFER("\?\?(", 3);
                        break;
                    case ']':
                        WRITE_BUFFER("\?\?)", 3);
                        break;
                    case '{':
                        WRITE_BUFFER("\?\?<", 3);
                        break;
                    case '}':
                        WRITE_BUFFER("\?\?>", 3);
                        break;
                    case '\\':
                        WRITE_BUFFER("\?\?/", 3);
                        break;
                    case '^':
                        WRITE_BUFFER("\?\?'", 3);
                        break;
                    case '~':
                        WRITE_BUFFER("\?\?-", 3);
                        break;
                    case '|':
                        WRITE_BUFFER("\?\?!", 3);
                        break;
                    default:
                        WRITE_BUFFER(&ch, 1);
                        break;
                }
            }

            fwrite(buffer + BUFFER_SIZE, 1, write_buffer_length, write_file);
        }

        fclose(read_file);
        fclose(write_file);
    }

    return 0;
}
