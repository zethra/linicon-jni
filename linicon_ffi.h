/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/. */

#ifndef linicon_ffi_h
#define linicon_ffi_h

/* Generated with cbindgen:0.13.1 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * The type of icon returned i.e. file type
 */
typedef enum {
  PNG,
  SVG,
  XMP,
} linicon_IconType;

/**
 * An iterator over the icons that match the input parameters given
 * to [`lookup_icon`](fn.lookup_icon.html) or
 * [`lookup_icon_with_extra_paths`](fn.lookup_icon_with_extra_paths.html)
 */
typedef struct IconIter IconIter;

/**
 * Opaque iterator over icon paths
 */
typedef IconIter linicon_IconIter;

/**
 * A path to an icon and meta data about that icon
 */
typedef struct {
  /**
   * The path to the icon
   */
  char *path;
  /**
   * The name of the theme to icon is from
   */
  char *theme;
  /**
   * The type of icon i.e. the file type
   */
  linicon_IconType icon_type;
} linicon_IconPath;

/**
 * Free an IconIter
 */
void linicon_free_icon_iter(linicon_IconIter *ptr);

/**
 * Free an IconPath*
 */
void linicon_free_icon_path(linicon_IconPath *ptr);

/**
 * Checks if there was an error before.
 *
 * # Returns
 *
 * `0` if there was no error, `1` if error had occured.
 */
int linicon_have_last_error(void);

/**
 * Get the next IconPath from an IconIter
 */
linicon_IconPath *linicon_iter_next(linicon_IconIter *ptr);

/**
 * Gets error message length if any error had occurred.
 *
 * # Returns
 *
 * If there was no error before, returns `0`,
 * otherwise returns message length including trailing `\0`.
 */
int linicon_last_error_length(void);

/**
 * Fills passed buffer with an error message.
 *
 * Buffer length can be get with [battery_last_error_length](fn.battery_last_error_length.html) function.
 *
 * # Returns
 *
 * Returns `-1` is passed buffer is `NULL` or too small for error message.
 * Returns `0` if there was no error previously.
 *
 * In all other cases returns error message length.
 */
int linicon_last_error_message(char *buffer,
                               int length);

/**
 * Lookup and icons given a name, size, and scale and the name theme
 * you want to look the icon up from.
 */
linicon_IconIter *linicon_lookup_icon(char *theme_name, char *name, uint16_t size, uint16_t scale);

/**
 * Works the same as lookup_icon expect
 * you can provide a list of additional paths to the default list
 * of paths in with to search for icon themes.
 */
linicon_IconIter *linicon_lookup_icon_with_extra_paths(char *theme_name,
                                                       char *name,
                                                       uint16_t size,
                                                       uint16_t scale,
                                                       char **extra_search_paths,
                                                       int extra_search_paths_len);

#endif /* linicon_ffi_h */
