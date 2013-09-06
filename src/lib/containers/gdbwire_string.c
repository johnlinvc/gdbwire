#include <string.h>
#include <stdlib.h>
#include "gdbwire_string.h"

struct gdbwire_string {
    /* The bytes that make up the string. May contain NUL characters. */
    char *data;
    /* The number of bytes occuring in data at the moment. */
    size_t size;
    /* The max capacity of the string */
    size_t capacity;
};

struct gdbwire_string *
gdbwire_string_create(void)
{
    struct gdbwire_string *string;

    string = calloc(1, sizeof (struct gdbwire_string));
    if (string) {
        if (gdbwire_string_append_cstr(string, "") == -1) {
            gdbwire_string_destroy(string);
            string = NULL;
        }
    }

    return string;
}

void
gdbwire_string_destroy(struct gdbwire_string *string)
{
    if (string) {
        if (string->data) {
            free(string->data);
            string->data = NULL;
        }
        string->size = 0;
        string->capacity = 0;
        free(string);
    }
}

void
gdbwire_string_clear(struct gdbwire_string *string)
{
    if (string) {
        string->size = 0;
        string->data[0] = '\0';
    }
}

/**
 * Increase the size of the string capacity.
 *
 * @param string
 * The string to increase the capacity.
 *
 * @return
 * 0 on success or -1 on error.
 */
static int
gdbwire_string_increase_capacity(struct gdbwire_string *string)
{
    // The algorithm chosen to increase the capacity is arbitrary.
    // It starts at 128 bytes. It then doubles it's size in bytes like this,
    //   128, 256, 512, 1024, 2048, 4096
    // After it reaches 4096 it then grows by 4096 bytes at a time.
    if (string->capacity == 0) {
        string->capacity = 128;
    } else if (string->capacity < 4096) {
        string->capacity *= 2;
    } else {
        string->capacity += 4096;
    }

    // At this point string->capacity is set to the new size, so realloc
    string->data = (char*)realloc(string->data, string->capacity);

    return (string->data) ? 0 : -1;
}

int
gdbwire_string_append_cstr(struct gdbwire_string *string, const char *cstr)
{
    int result;

    if (string && cstr) {
        size_t length = strlen(cstr) + 1;
        result = gdbwire_string_append_data(string, cstr, length);
        /* Do not include the NUL character in the size for NULL terminated
         * strings. This is documented in the interface. */
        if (result == 0) {
            string->size--;
        }
    } else {
        result = -1;
    }

    return result;
}

int
gdbwire_string_append_data(struct gdbwire_string *string, const char *data,
        size_t size)
{
    int result = (string && data) ? 0 : -1;
    int data_index = 0;

    for (; string && data && data_index < size; ++data_index, ++string->size) {
        if (string->size >= string->capacity) {
            result = gdbwire_string_increase_capacity(string);
            if (result == -1) {
                break;
            }
        }

        string->data[string->size] = data[data_index];
    }

    return result;
}

char *
gdbwire_string_data(struct gdbwire_string *string)
{
    char *result = NULL;

    if (string) {
        result = string->data;
    }

    return result;
}

size_t
gdbwire_string_size(struct gdbwire_string *string)
{
    return string->size;
}

size_t
gdbwire_string_capacity(struct gdbwire_string *string)
{
    return string->capacity;
}
