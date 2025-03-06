/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Brian J. Downs
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifdef __cplusplus
extern "C" {
#endif
 
#ifndef __SAFE_ARRAY_H
#define __SAFE_ARRAY_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#if defined(__GNUC__)
#define INLINE __attribute__((always_inline)) inline
#else
#define INLINE inline
#endif

#define NEW_ARRAY(T)                                                     \
typedef bool (*compare_func_t)(const T x, const T y, void *user_data);   \
typedef void (*foreach_func_t)(const T item, void *user_data);           \
typedef int  (*sort_compare_func_t)(const void *x, const void *y);       \
typedef bool (*val_equal_func_t)(const T x, const T y, void *user_data); \
\
typedef struct {            \
    T *items;               \
    uint64_t len;           \
    uint64_t cap;           \
    compare_func_t compare; \
} array_##T; \
\
INLINE void                                        \
array_##T##_init(array_##T *a, const uint64_t cap) \
{                                                  \
    a = (array_##T*)calloc(1, sizeof(array_##T));  \
    a->items = calloc(cap, sizeof(T));             \
    a->len = 0;                                    \
    a->cap = cap;                                  \
} \
\
INLINE void                                                   \
array_##T##_append(array_##T *a, T item)                      \
{                                                             \
    if (a->len == a->cap) {                                   \
        a->cap = a->cap ? a->cap * 2 : 1;                     \
        a->items = (T*)realloc(a->items, sizeof(T) * a->cap); \
    }                                                         \
    a->items[a->len++] = item;                                \
} \
\
INLINE T                                    \
array_##T##_get(array_##T *a, uint64_t idx) \
{                                           \
    if (idx >= 0 && idx < a->len) {         \
        return a->items[idx];               \
    }                                       \
    return a->items[0];                     \
} \
\
INLINE T                          \
array_##T##_first(array_##T *a)   \
{                                 \
    return array_##T##_get(a, 0); \
} \
\
INLINE T                                 \
array_##T##_last(array_##T *a) {         \
    return array_##T##_get(a, a->len-1); \
} \
\
INLINE void array_##T##_reverse(array_##T *a) { \
    if (a->len < 2) {                           \
		return;                                 \
	}                                           \
                                                \
    uint64_t i = a->len - 1;                    \
    uint64_t j = 0;                             \
                                                \
    while(i > j) {                              \
        T temp = a->items[i];                   \
        a->items[i] = a->items[j];              \
        a->items[j] = temp;                     \
        i--;                                    \
        j++;                                    \
    }                                           \
} \
\
INLINE bool                                                            \
array_##T##_compare(array_##T *a1, array_##T *a2, void *user_data)     \
{                                                                      \
	if (a1->len == 0 && a2->len == 0) {                                \
		return true;                                                   \
	}                                                                  \
	if (a1->len != a2->len) {                                          \
		return false;                                                  \
	}                                                                  \
                                                                       \
	if (a1->compare != NULL) {                                         \
		for (uint64_t i = 0; i < a1->len; i++) {                       \
			if (!a1->compare(a1->items[i], a2->items[i], user_data)) { \
				return false;                                          \
			}                                                          \
		}                                                              \
		return true;                                                   \
	}                                                                  \
                                                                       \
	for (uint64_t i = 0; i < a1->len; i++) {                           \
		if (a1->items[i] != a2->items[i]) {                            \
			return false;                                              \
		}                                                              \
	}                                                                  \
	return true;                                                       \
} \
\
INLINE uint64_t                                                  \
array_##T##_copy(array_##T *a1, array_##T *a2, bool overwrite)   \
{                                                                \
	if (a2->len == 0) {                                          \
		return 0;                                                \
	}                                                            \
                                                                 \
	if (overwrite) {                                             \
		if (a1->len != a2->len) {                                \
			a2->cap = a1->cap;                                   \
			a2->items = realloc(a2->items, sizeof(T) * a1->cap); \
		}                                                        \
	}                                                            \
                                                                 \
	for (uint64_t i = 0; i < a1->len; i++) {                     \
		a2->items[i] = a1->items[i];                             \
		a2->len++;                                               \
	}                                                            \
	return a2->len;                                              \
} \
\
INLINE bool                                 \
array_##T##_contains(array_##T *a, T val)   \
{                                           \
    if (a->len == 0) {                      \
		return false;                       \
	}                                       \
                                            \
	for (uint64_t i = 0; i < a->len; i++) { \
		if (a->items[i] == val) {           \
			return true;                    \
		}                                   \
	}                                       \
	return false;                           \
} \
INLINE uint64_t                                      \
array_##T##_delete(array_##T *a, const uint64_t idx) \
{                                                    \
    if (a->len == 0 || idx > a->len) {               \
		return 0;                                    \
	}                                                \
                                                     \
	for (uint64_t i = idx; i < a->len; i++) {        \
		a->items[i] = a->items[i+1];                 \
	}                                                \
	a->len--;                                        \
                                                     \
	return a->len;                                   \
} \
\
INLINE uint8_t                                                           \
array_##T##_replace_by_id(array_##T *a, const uint64_t idx, const T val) \
{                                                                        \
    if (a->len == 0 || idx > a->len) {                                   \
		return -1;                                                       \
	}                                                                    \
	a->items[idx] = val;                                                 \
                                                                         \
	return 0;                                                            \
} \
\
INLINE int8_t                                                                              \
array_##T##_replace_by_val(array_##T *a, const T old_val, const T new_val, uint64_t times) \
{                                                                                          \
	if (a->len == 0) {                                                                     \
		return -1;                                                                         \
	}                                                                                      \
                                                                                           \
	for (uint64_t i = 0; i < a->len && times != 0; i++) {                                  \
		if (a->compare(a->items[i], old_val, NULL)) {                                      \
			a->items[i] = new_val;                                                         \
			times--;                                                                       \
		}                                                                                  \
	}                                                                                      \
	return 0;                                                                              \
} \
INLINE int                                                             \
array_##T##_foreach(array_##T *a, foreach_func_t ift, void *user_data) \
{                                                                      \
	if (a->len == 0) {                                                 \
		return 0;                                                      \
	}                                                                  \
                                                                       \
	for (uint64_t i = 0; i < a->len; i++) {                            \
		ift(a->items[i], user_data);                                   \
	}                                                                  \
	return 0;                                                          \
} \
\
INLINE void                                            \
array_##T##_sort(array_##T *a, sort_compare_func_t sf) \
{                                                      \
	if (a->len < 2) {                                  \
		return;                                        \
	}                                                  \
    qsort(a->items, a->len, sizeof(T), sf);            \
} \
\
INLINE uint64_t                                                     \
array_##T##_repeat(array_##T *a, const T val, const uint64_t times) \
{                                                                   \
    for (uint64_t i = 0; i < times; i++) {                          \
		array_##T##_append(a, val);                                 \
	}                                                               \
	return a->len;                                                  \
} \
INLINE uint64_t                                       \
array_##T##_count(array_##T *a, const T val)          \
{                                                     \
	uint64_t count = 0;                               \
                                                      \
	if (a->len == 0) {                                \
		return count;                                 \
	}                                                 \
                                                      \
	if (a->compare != NULL) {                         \
		for (uint64_t i = 0; i < a->len; i++) {       \
			if (a->compare(a->items[i], val, NULL)) { \
				count++;                              \
			}                                         \
		}                                             \
	} else {                                          \
		for (uint64_t i = 0; i < a->len; i++) {       \
			if (a->items[i] == val) {                 \
				count++;                              \
			}                                         \
		}                                             \
	}                                                 \
	return count;                                     \
} \
INLINE uint64_t                                       \
array_##T##_grow(array_##T *a, const uint64_t size)   \
{                                                     \
    if (size == 0) {                                  \
		return a->cap;                                \
	}                                                 \
                                                      \
    a->cap += size;                                   \
    a->items = realloc(a->items, sizeof(T) * a->cap); \
                                                      \
	return a->cap;                                    \
} \
INLINE uint64_t                                                \
array_##T##_concat(array_##T *a1, const array_##T *a2)         \
{                                                              \
    if (a2->len == 0) {                                        \
		return a1->len;                                        \
	}                                                          \
                                                               \
	a1->cap += a2->len;                                        \
	a1->items = realloc(a1->items, sizeof(T) * a2->len);       \
                                                               \
	for (uint64_t i = 0, j = a1->len; i < a2->len; i++, j++) { \
		a1->items[j] = a2->items[i];                           \
		a1->len++;                                             \
	}                                                          \
                                                               \
	return a1->len;                                            \
} \
INLINE void                              \
array_##T##_free(array_##T *a)           \
{                                        \
    if (a != NULL && a->items != NULL) { \
        free(a->items);                  \
    }                                    \
}

#endif /* end __SAFE_ARRAY_H */
#ifdef __cplusplus
}
#endif
