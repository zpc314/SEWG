#include "demo.h"
#include <stdlib.h>
#include "wgtee.h"
//#include "alg.h"
//#include "tool.h"

int sccore_start() 
{
	/*Finalize();*/
	LOGI("sccore_start start");
	Init();
	LOGI("sccore_start end");
	return 0;
}

int sccore_shutdown() 
{
	//Finalize();
	LOGI("ta shutdown");
	return 0;
}

int tsta_comm(void *ts_req, void *ts_rsp)
{
    ts_send_t *mreq = (ts_send_t *)ts_req;
    ts_rsp_t *mrsp = (ts_rsp_t *)ts_rsp;
    uint32_t cmd_id = mreq->cmd;
    LOGI("CMD:%d received", cmd_id);
    switch (cmd_id) {
        case Initialize_CMD:
        {
            Initialize_send_t *send = (Initialize_send_t *)(mreq->data);
            mrsp->ret = Initialize(
                    send->rootDir,
                    send->logDir
            );
            break;
        }
        case GenerateKeyPair_CMD:
        {
           /* GenerateKeyPair_rsp_t *rsp = (GenerateKeyPair_rsp_t *)(mrsp->data);
            mrsp->ret = GenerateKeyPair(
                    rsp->pubKey,
                    &(rsp->ID)
            );*/
            break;
        }
        case GetPublickeyByID_CMD:
        {
            GetPublickeyByID_send_t *send = (GetPublickeyByID_send_t *)(mreq->data);
            GetPublickeyByID_rsp_t *rsp = (GetPublickeyByID_rsp_t *)(mrsp->data);
            mrsp->ret = GetPublickeyByID(
                    rsp->pubKey,
                    send->ID
            );
            break;
        }
        case CurveMul_CMD:
        {
           /* CurveMul_send_t *send = (CurveMul_send_t *)(mreq->data);
            CurveMul_rsp_t *rsp = (CurveMul_rsp_t *)(mrsp->data);
            mrsp->ret = CurveMul(
                    rsp->rst,
                    send->point, 
                    send->ID
            );*/
            break;
        }
        default:
            break;
    }
    LOGI("CMD:%d finished", cmd_id);
    return 0;
}

int set_pin_comm(char *pin, uint32_t length, void *req, void *rsp) {
    ts_send_t *mreq = (ts_send_t *)req;
    ts_rsp_t *mrsp = (ts_rsp_t *)rsp;
    uint32_t cmd_id = mreq->cmd;
    LOGI("set_pin_comm CMD:%d received, pin = %s,length = %d", cmd_id, pin, length);
    pin[length] = '\0';
    switch (cmd_id) {
        case GenerateKeyPair_CMD:
        {
            GenerateKeyPair_rsp_t *rsp = (GenerateKeyPair_rsp_t *)(mrsp->data);
            mrsp->ret = GenerateKeyPair(
                    pin,
                    rsp->pubKey,
                    &(rsp->ID)
            );
            break;
        }
        default:
            break;
    }
    LOGI("set_pin_comm CMD:%d finisned", cmd_id);
    return 0;
}

int get_pin_comm(char *pin, uint32_t length, void *req, void *rsp){
    ts_send_t *mreq = (ts_send_t *)req;
    ts_rsp_t *mrsp = (ts_rsp_t *)rsp;
    uint32_t cmd_id = mreq->cmd;
    LOGI("get_pin_comm CMD:%d received, pin = %s,length = %d", cmd_id, pin, length);
    pin[length] = '\0';
    switch (cmd_id) {
        case CurveMul_CMD:
        {
            CurveMul_send_t *send = (CurveMul_send_t *)(mreq->data);
            CurveMul_rsp_t *rsp = (CurveMul_rsp_t *)(mrsp->data);
            mrsp->ret = CurveMul(
                    pin,
                    rsp->rst,
                    send->point,
                    send->ID
            );
            break;
        }
        default:
            break;
    }
    LOGI("get_pin_comm CMD:%d finisned", cmd_id);
    return 0;
}
