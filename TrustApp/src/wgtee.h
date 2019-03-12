#ifndef WGTEE_H
#define WGTEE_H

#include "def.h"
#include "log.h"
#include "param.h"
#include <string.h>

void Init();

int Initialize(const char *rootDir, const char *logDir);

int GenerateKeyPair(char *pin, unsigned char *pubKey, int *id);

int GetPublickeyByID(unsigned char *pubKey, int id);

int CurveMul(char *pin, unsigned char *rst, unsigned char *point, int id);
#endif
