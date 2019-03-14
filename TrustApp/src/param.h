#ifndef PARAM_H
#define PARAM_H

#include <stdint.h>
#include "def.h"

#define SET_PIN_UI													9
#define INPUT_PIN_UI												10
#define SIGN_UI														11
#define AUTH_UI														12

#define Initialize_CMD												1
#define GenerateKeyPair_CMD                                         2
#define GetPublickeyByID_CMD                                        3
#define CurveMul_CMD												4
#define Auth_CMD													5

#define MAX_SIZE													20480
#define MAX_DIR_LEN													100

#define MAX_K_LEN													128
#define MAX_PLAIN_LEN												10240
#define MAX_SERVER_RST_LEN											200
#define MAX_DATA_LEN												10240
#define MAX_LIST_LEN												10240
#define MAX_WRAPED_DATA_LEN											200
#define MAX_SM3_KDF_SECRET_LEN										256
#define MAX_SM3_KDF_KEY_LEN											256

#define MAX(a,b) ((a)>(b))?(a):(b)
#define MIN(a,b) ((a)<(b))?(a):(b)

/*send & response struct*/
typedef struct ts_send {
	uint32_t cmd;
	unsigned char data[MAX_SIZE];
} ts_send_t;

typedef struct ts_rsp {
	uint32_t ret;
	unsigned char data[MAX_SIZE];
} ts_rsp_t;

/*Initialize*/
typedef struct Initialize_send {
	char rootDir[MAX_DIR_LEN];
	char logDir[MAX_DIR_LEN];
} Initialize_send_t;

/*GenerateKeyPair*/
typedef struct GenerateKeyPair_rsp {
    int ID;
	BYTE pubKey[CUR25519_PUB_KEY_LENGTH];
} GenerateKeyPair_rsp_t;

/*GetPublickeyByID*/
typedef struct GetPublickeyByID_send {
    int ID;
} GetPublickeyByID_send_t;

typedef struct GetPublickeyByID_rsp {
	BYTE pubKey[CUR25519_PUB_KEY_LENGTH];
} GetPublickeyByID_rsp_t;

/*CurveMul*/
typedef struct CurveMul_send {
    int ID;
	BYTE point[CUR25519_PUB_KEY_LENGTH];
} CurveMul_send_t;

typedef struct CurveMul_rsp {
	BYTE rst[CUR25519_PUB_KEY_LENGTH];
} CurveMul_rsp_t;

/*Auth*/
typedef struct Auth_send {
	char pakg[512];
} Auth_send_t;
#endif