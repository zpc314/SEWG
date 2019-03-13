// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2015-2018 Jason A. Donenfeld <Jason@zx2c4.com>. All Rights Reserved.
 */

#include <errno.h>
#include <stdio.h>
#include <ctype.h>

#include "curve25519.h"
#include "encoding.h"
#include "subcommands.h"

int pubkey_main(int argc, char *argv[])
{
	uint8_t key[WG_KEY_LEN];
	int keyid;
	char base64[WG_KEY_LEN_BASE64];
	int trailing_char;

	if (argc != 1) {
		fprintf(stderr, "Usage: %s %s\n", PROG_NAME, argv[0]);
		return 1;
	}
	//modified by zpc
	if (fread(&keyid, 1, 3, stdin) != 3) {
		errno = EINVAL;
		fprintf(stderr, "%s: Key is not the correct length or format\n", PROG_NAME);
		return 1;
	}
	//base64[WG_KEY_LEN_BASE64 - 1] = '\0';

	for (;;) {
		trailing_char = getc(stdin);
		if (!trailing_char || isspace(trailing_char) || isblank(trailing_char))
			continue;
		if (trailing_char == EOF)
			break;
		fprintf(stderr, "%s: Trailing characters found after key\n", PROG_NAME);
		return 1;
	}

	//if (!key_from_base64(key, base64)) {
	//	fprintf(stderr, "%s: Key is not the correct length or format\n", PROG_NAME);
	//	return 1;
	//}
	Core_GetPublickeyByID(key, keyid);
	key_to_base64(base64, key);
	puts(base64);
	return 0;
}
