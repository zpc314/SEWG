#include <stdlib.h>
#include <android/log.h>
#include <stdio.h>
#include <string.h>
#include "tsca_comm.h"
#include "param.h"
#include "err.h"

const char *teeAppName = "sccore64";

ts_send_t mreq;
ts_rsp_t mrsp;

int32_t Core_Init();

int32_t Core_GenerateKeyPair(BYTE *pubkey, int32_t *ID);

int32_t Core_GetPublickeyByID(BYTE *pubkey, int32_t ID);

int32_t Core_CurveMul(BYTE *rst, BYTE *point, int32_t ID);

int32_t Core_Auth(char *pakg);