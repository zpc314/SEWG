#include "wgtee.h"

int32_t Core_Init()
{
    int32_t rst = ts_start_app(teeAppName,sizeof(ts_send_t)+sizeof(ts_rsp_t)+200);
    return rst;
}

int32_t Core_GenerateKeyPair(BYTE *pubkey, int32_t *ID)
{
    mreq.cmd = GenerateKeyPair_CMD;
    //*pubkey = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_ui(teeAppName, SET_PIN_UI, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        if (SAR_OK == (ret = mrsp.ret)) {
            GenerateKeyPair_rsp_t *rsp = (GenerateKeyPair_rsp_t *) (mrsp.data);
            *ID = rsp->ID;
            memcpy(pubkey, rsp->pubKey, CUR25519_PUB_KEY_LENGTH);
            ret = SAR_OK;
        }
    }
    //ts_shutdown_app();
    return ret;
}

int32_t Core_GetPublickeyByID(BYTE *pubkey, int32_t ID)
{
    //ts_start_app(teeAppName,sizeof(ts_send_t)+sizeof(ts_rsp_t)+200);
    GetPublickeyByID_send_t *send = (GetPublickeyByID_send_t *) (mreq.data);
    mreq.cmd = GetPublickeyByID_CMD;
    send->ID = ID;
    //*pubkey = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_send(teeAppName, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        if (SAR_OK == (ret = mrsp.ret)) {
            GetPublickeyByID_rsp_t *rsp = (GetPublickeyByID_rsp_t *) (mrsp.data);
            memcpy(pubkey, rsp->pubKey, CUR25519_PUB_KEY_LENGTH);
            //ret = SAR_OK;
        }
    }
    //ts_shutdown_app();
    return ret;
}

int32_t Core_CurveMul(BYTE *rst, BYTE *point, int32_t ID)
{
    //ts_start_app(teeAppName,sizeof(ts_send_t)+sizeof(ts_rsp_t)+200);
    CurveMul_send_t *send = (CurveMul_send_t *) (mreq.data);
    mreq.cmd = CurveMul_CMD;
    send->ID = ID;
    memcpy(send->point, point, CUR25519_PUB_KEY_LENGTH);
    uint8_t *mp = (uint8_t *)point;
    //for(int i = 0; i < CUR25519_PUB_KEY_LENGTH; i++) {
      //  __android_log_print(ANDROID_LOG_ERROR, "Core_CurveMul point", "point[%d]: %x", i, mp[i]);
    //}

    //*rst = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_ui(teeAppName, INPUT_PIN_UI, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        if (SAR_OK == (ret = mrsp.ret)) {
            CurveMul_rsp_t *rsp = (CurveMul_rsp_t *) (mrsp.data);
            memcpy(rst, rsp->rst, CUR25519_PUB_KEY_LENGTH);
            ret = SAR_OK;
        }
    }
    //if (0 == ts_ca_send(teeAppName, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
    //        if (SAR_OK == (ret = mrsp.ret)) {
    //            CurveMul_rsp_t *rsp = (CurveMul_rsp_t *) (mrsp.data);
    //            memcpy(*rst, rsp->rst, CUR25519_PUB_KEY_LENGTH);
    //            ret = SAR_OK;
    //        }
    //    }
    //ts_shutdown_app();
    return ret;
}

int32_t Core_Auth(char *pakg)
{
	CurveMul_send_t *send = (Auth_send_t *) (mreq.data);
    mreq.cmd = Auth_CMD;
    memcpy(send->pakg, pakg, 512);
    //for(int i = 0; i < CUR25519_PUB_KEY_LENGTH; i++) {
      //  __android_log_print(ANDROID_LOG_ERROR, "Core_CurveMul point", "point[%d]: %x", i, mp[i]);
    //}

    //*rst = (BYTE *)malloc(CUR25519_PUB_KEY_LENGTH);
    //memset(*pubkey, 1, CUR25519_PUB_KEY_LENGTH);
    int32_t ret = SAR_TEECOMMERR;
    mrsp.ret = SAR_TEECOMMERR;

    if (0 == ts_ca_ui(teeAppName, AUTH_UI, &mreq, sizeof(ts_send_t), &mrsp, sizeof(ts_rsp_t))) {
        return ret;
    }
	return ret;
}