#ifndef __GDBWIRE_STRING_H__
#define __GDBWIRE_STRING_H__

#ifdef __cplusplus 
extern "C" { 
#endif 

/**
 * A dynamic string representation.
 *
 * To create and destroy a string use gdbwire_string_create() and
 * gdbwire_string_destroy() respectively.
 *
 * This string is an abstraction of a low level C string. It supports being
 * used as a NULL terminated c string and also as an arbitrary array of
 * bytes. You can append to this string in either of these modes using
 * gdbwire_string_append_cstr() or gdbwire_string_append_data(). This string
 * automatically grows as you append data to it. Please note, the size of
 * the string will not include the NULL terminated character when using
 * the gdbwire_string_append_cstr() function to append data.
 *
 * To get access to the underlying bytes associated with this string
 * call gdbwire_string_data(). It is OK to modify the result as long as
 * you are careful to stay in it's valid bounds.
 *
 * The size (or length) of the string can be accessed through the
 * gdbwire_string_size() function. The character pointer returned from
 * gdbwire_string_data() is valid from the index range of 0 to
 * gdbwire_string_size() - 1.
 */
struct gdbwire_string;

/**
 * Create a string instance.
 *
 * @return
 * A valid string instance or NULL on error.
 */
struct gdbwire_string *gdbwire_string_create(void);

/**
 * Destroy the string instance and it's resources.
 *
 * @param string
 * The string to destroy.
 */
void gdbwire_string_destroy(struct gdbwire_string *string);

/**
 * Clear the contents of a string.
 *
 * Sets the string back to an empty string which also changes it's
 * size back to zero.
 *
 * The capacity remains unchanged.
 *
 * @param string
 * The string to clear
 */
void gdbwire_string_clear(struct gdbwire_string *string);

/**
 * Append a c string to the string instance.
 *
 * @param string
 * The string instance to append the c string to.
 *
 * @param cstr
 * The c string to append to the string instance.
 *
 * @return
 * 0 on success or -1 on failure.
 */
int gdbwire_string_append_cstr(struct gdbwire_string *string, const char *cstr);

/**
 * Append a sequence of bytes to the string instance.
 *
 * @param string
 * The string instance to append the sequence of bytes to.
 *
 * @param data
 * The sequence of bytes to append to the string instance. This may
 * contain NUL characters.
 *
 * @param size
 * The number of bytes in data to append to the string instance.
 *
 * @return
 * 0 on success or -1 on failure.
 */
int gdbwire_string_append_data(struct gdbwire_string *string,
        const char *data, size_t size);

/**
 * Get the data associated with this string.
 *
 * The data could be formatted as a NULL terminated C string or
 * as an arbitrary array of bytes. Use gdbwire_string_size() to
 * determine the size (or length) of the result of this function.
 * 
 * Modifying the return value of this function is acceptable as long as you
 * stay in the string's valid bounds.
 *
 * @param string
 * The string index to get the pointer data from.
 *
 * @return
 * The data that has been added to this string instance or "" after
 * creation or clear. The result is gdbwire_string_size() bytes long.
 */
char *gdbwire_string_data(struct gdbwire_string *string);

/**
 * Determine the size (the number of bytes) this string instance represents.
 *
 * Please note, the result of this function will not include the NULL
 * terminated character when using the gdbwire_string_append_cstr() function
 * to append data.
 *
 * @param string
 * The string instance to get the size for.
 *
 * @return
 * The number of bytes contained in this string instance. To access these
 * bytes see gdbwire_string_data(). Will be 0 after creation or clear.
 */
size_t gdbwire_string_size(struct gdbwire_string *string);

/**
 * Determine the maximum capacity (number of bytes) this string may hold.
 *
 * The max capacity of the string is automatically increased when data
 * is appended to this string through the gdbwire_string_append_*()
 * family of functions.
 *
 * @param string
 * The string to determine the capacity of.
 *
 * @return
 * The max number of bytes this string may hold.
 */
size_t gdbwire_string_capacity(struct gdbwire_string *string);

#ifdef __cplusplus 
}
#endif 

#endif
