//
// Created by zpc on 18-12-31.
//

#include "char2hex.h"
#include <string.h>

void key_to_hex(char hex[static WG_KEY_LEN_HEX], const unsigned char key[static WG_KEY_LEN])
{
    unsigned int i;

    for (i = 0; i < WG_KEY_LEN; ++i) {
        hex[i * 2] = 87U + (key[i] >> 4) + ((((key[i] >> 4) - 10U) >> 8) & ~38U);
        hex[i * 2 + 1] = 87U + (key[i] & 0xf) + ((((key[i] & 0xf) - 10U) >> 8) & ~38U);
    }
    hex[i * 2] = '\0';
}

int key_from_hex(unsigned char key[static WG_KEY_LEN], const char *hex)
{
    unsigned char c, c_acc = 0, c_alpha0, c_alpha, c_num0, c_num, c_val;
    volatile unsigned char ret = 0;

    if (strlen(hex) != WG_KEY_LEN_HEX - 1)
        return 0;

    for (unsigned int i = 0; i < WG_KEY_LEN_HEX - 1; i += 2) {
        c = (unsigned char)hex[i];
        c_num = c ^ 48U;
        c_num0 = (c_num - 10U) >> 8;
        c_alpha = (c & ~32U) - 55U;
        c_alpha0 = ((c_alpha - 10U) ^ (c_alpha - 16U)) >> 8;
        ret |= ((c_num0 | c_alpha0) - 1) >> 8;
        c_val = (c_num0 & c_num) | (c_alpha0 & c_alpha);
        c_acc = c_val * 16U;

        c = (unsigned char)hex[i + 1];
        c_num = c ^ 48U;
        c_num0 = (c_num - 10U) >> 8;
        c_alpha = (c & ~32U) - 55U;
        c_alpha0 = ((c_alpha - 10U) ^ (c_alpha - 16U)) >> 8;
        ret |= ((c_num0 | c_alpha0) - 1) >> 8;
        c_val = (c_num0 & c_num) | (c_alpha0 & c_alpha);
        key[i / 2] = c_acc | c_val;
    }

    return 1 & ((ret - 1) >> 8);
}
