#ifndef TEESECURECORE_DEMO_H
#define TEESECURECORE_DEMO_H

#include "def.h"
#include "param.h"
#include "err.h"
#include "log.h"

#ifdef __cplusplus
extern "C" {
#endif
int sccore_start();

int sccore_shutdown();

int tsta_comm(void *req, void *rsp);

int set_pin_comm(char *pin, uint32_t length, void *req, void *rsp);

int get_pin_comm(char *pin, uint32_t length, void *req, void *rsp);

#ifdef __cplusplus
}
#endif

#endif

