/*
 * Copyright (c) 2022 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include "shell_util.h"

static size_t whitespace_trim(char *out, size_t len, const char *str)
{
	if (len == 0) {
		return 0;
	}

	const char *end;
	size_t out_size;

	while (str[0] == ' ') {
		str++;
	}

	if (*str == 0) {
		*out = 0;
		return 1;
	}

	end = str + strlen(str) - 1;
	while (end > str && (end[0] == ' ')) {
		end--;
	}
	end++;

	out_size = (end - str) + 1;

	if (out_size > len) {
		return 0;
	}

	memcpy(out, str, out_size - 1);
	out[out_size - 1] = 0;

	return out_size;
}

int bt_mesh_str2l(const char *str, long *val)
{
	char *endptr = NULL;
	char c;

	errno = 0;
	*val = strtol(str, &endptr, 10);
	if (errno == ERANGE) {
		return -ERANGE;
	} else if (errno || endptr == str) {
		return -EINVAL;
	}


	while ((c = *endptr++)) {
		if (!isspace(c)) {
			return -EINVAL;
		}
	}

	return 0;
}

int bt_mesh_str2ul(const char *str, unsigned long *val)
{
	char *endptr = NULL;
	char c;

	while (isspace(*str)) {
		str++;
	}
	if (*str == '-') {
		return -EINVAL;
	}

	errno = 0;
	*val = strtoul(str, &endptr, 10);
	if (errno == ERANGE) {
		return -ERANGE;
	} else if (errno || endptr == str) {
		return -EINVAL;
	}

	while ((c = *endptr++)) {
		if (!isspace(c)) {
			return -EINVAL;
		}
	}

	return 0;
}

int bt_mesh_str2dbl(const char *str, double *val)
{
	char trimmed_buf[22] = { 0 };
	long intgr;
	unsigned long frac;
	double frac_dbl;
	int err = 0;

	size_t len = whitespace_trim(trimmed_buf, sizeof(trimmed_buf), str);

	if (len < 2) {
		return -EINVAL;
	}

	int comma_idx = strcspn(trimmed_buf, ".");
	int frac_len = strlen(trimmed_buf + comma_idx + 1);

	/* Covers corner case "." input */
	if (strlen(trimmed_buf) < 2 && trimmed_buf[comma_idx] != 0) {
		return -EINVAL;
	}

	trimmed_buf[comma_idx] = 0;

	/* Avoid fractional overflow by losing one precision point */
	if (frac_len > 9) {
		trimmed_buf[comma_idx + 10] = 0;
		frac_len = 9;
	}

	/* Avoid doing str2long if intgr part is empty or only single sign char*/
	if (trimmed_buf[0] == '\0' ||
	    (trimmed_buf[1] == '\0' && (trimmed_buf[0] == '+' || trimmed_buf[0] == '-'))) {
		intgr = 0;
	} else {
		err = bt_mesh_str2l(trimmed_buf, &intgr);

		if (err) {
			return err;
		}
	}

	/* Avoid doing str2ulong if fractional part is empty */
	if ((trimmed_buf + comma_idx + 1)[0] == '\0') {
		frac = 0;
	} else {
		err = bt_mesh_str2ul(trimmed_buf + comma_idx + 1, &frac);

		if (err) {
			return err;
		}
	}

	frac_dbl = (double)frac;

	for (int i = 0; i < frac_len; i++) {
		frac_dbl /= 10;
	}

	*val = (trimmed_buf[0] == '-') ? ((double)intgr - frac_dbl) :
					       ((double)intgr + frac_dbl);

	return err;
}

int bt_mesh_str2bool(const char *str, bool *val)
{
	long temp;

	if (!strcmp(str, "on") || !strcmp(str, "enable") || !strcmp(str, "true")) {
		*val = true;
		return 0;
	}

	if (!strcmp(str, "off") || !strcmp(str, "disable") || !strcmp(str, "false")) {
		*val = false;
		return 0;
	}

	int err = bt_mesh_str2l(str, &temp);

	if (err) {
		return err;
	}

	*val = (bool)temp;
	return 0;
}
