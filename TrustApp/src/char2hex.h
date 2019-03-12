//
// Created by zpc on 18-12-31.
//

#ifndef COM_WIREGUARD_ANDROID_1231_CHAR2HEX_H
#define COM_WIREGUARD_ANDROID_1231_CHAR2HEX_H

#define WG_KEY_LEN 32
#define WG_KEY_LEN_HEX (WG_KEY_LEN * 2 + 1)
//typedef unsigned char unsigned char;

int key_from_hex(unsigned char key[static WG_KEY_LEN], const char *hex);

void key_to_hex(char hex[static WG_KEY_LEN_HEX], const unsigned char key[static WG_KEY_LEN]);

#endif //COM_WIREGUARD_ANDROID_1231_CHAR2HEX_H
