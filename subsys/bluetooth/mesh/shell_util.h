/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef BT_MESH_SHELL_UTIL_H__
#define BT_MESH_SHELL_UTIL_H__

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * @brief Convert string to long param.
 *
 * @note On failure the passed value reference will not be altered.
 *
 * @param str Input string
 * @param val Converted value
 *
 * @return 0 on success.
 * @return -EINVAL on invalid string input.
 * @return -ERANGE if numeric string input is to large to convert.
 */
int bt_mesh_str2l(const char *str, long *val);

/*
 * @brief Convert string to unsigned long param.
 *
 * @note On failure the passed value reference will not be altered.
 *
 * @param str Input string
 * @param val Converted value
 *
 * @return 0 on success.
 * @return -EINVAL on invalid string input.
 * @return -ERANGE if numeric string input is to large to convert.
 */
int bt_mesh_str2ul(const char *str, unsigned long *val);

/*
 * @brief Convert string to double param.
 *
 * @note On failure the passed value reference will not be altered.
 *
 * @param str Input string
 * @param val Converted value
 *
 * @return 0 on success.
 * @return -EINVAL on invalid string input.
 * @return -ERANGE if numeric string input is to large to convert.
 */
int bt_mesh_str2dbl(const char *str, double *val);

/*
 * @brief Convert string to bool param.
 *
 * @note On failure the passed value reference will not be altered.
 *
 * @param str Input string
 * @param val Converted value
 *
 * @return 0 on success.
 * @return -EINVAL on invalid string input.
 * @return -ERANGE if numeric string input is to large to convert.
 */
int bt_mesh_str2bool(const char *str, bool *val);

#ifdef __cplusplus
}
#endif

#endif /* BT_MESH_SHELL_UTIL_H__ */
