#ifndef PARAM_H
#define PARAM_H

#include <stdint.h>
#include "def.h"

#define SET_PIN_UI													9
#define INPUT_PIN_UI												10
//#define SIGN_UI														11

#define Initialize_CMD												1
#define GenerateKeyPair_CMD                                         2
#define GetPublickeyByID_CMD                                        3
#define CurveMul_CMD												4
/*
#define CheckCallingAppDirectory_CMD								2
#define GenerateSignKeyLocalPart_CMD								3
#define GenerateSignKeyWithRandom_CMD								4
#define VerifySignKeyPIN_CMD										5
#define ChangeSignKeyPIN_CMD										6
#define ChangeSignKeyPINAndRandomWithServerCooperation_CMD			7
#define ChangeSignKeyPINUpdateWithServerCooperation_CMD				8
#define ResetSignKeyPINAndRandom_CMD								9
#define Finalize_CMD												10
#define CheckDataEncryptionKeyStatus_CMD							11
#define SM2CoDecryptInitialize_CMD									12
#define SM2CoDecryptIntegrate_CMD									13
#define SM2CoSignInitialize_CMD										14
#define SM2CoSignIntegrate_CMD										15
#define RestoreDataEncryptionKey_CMD								16
#define RestoreDataEncryptionKeyInitializeWithServerCooperation_CMD	17
#define RestoreDataEncryptionKeyIntegrateWithServerCooperation_CMD	18
#define EraseDataEncryptionKey_CMD									19
#define GetSignKeyID_CMD											20
#define SetServerAuthInfo_CMD										21
#define GetServerAuthInfo_CMD										22
#define BackupSignKeyData_CMD										23
#define RestoreSignKeyData_CMD										24
#define SKF_CreateApplication_CMD									25
#define SKF_EnumApplication_CMD										26
#define SKF_DeleteApplication_CMD									27
#define SKF_OpenApplication_CMD										28
#define SKF_CloseApplication_CMD									29
#define SKF_CreateFile_CMD											30
#define SKF_DeleteFile_CMD											31
#define SKF_EnumFiles_CMD											32
#define SKF_GetFilelnfo_CMD											33
#define SKF_WriteFile_CMD											34
#define SKF_ReadFile_CMD											35
#define SKF_CreateContainer_CMD										36
#define SKF_DeleteContainer_CMD										37
#define SKF_EnumContainer_CMD										38
#define SKF_OpenContainer_CMD										39
#define SKF_CloseContainer_CMD										40
#define SKF_GetContainerType_CMD									41
#define SKF_ImportCertificate_CMD									42
#define SKF_ExportCertificate_CMD									43
#define SKF_GenRandom_CMD											44
#define SKF_GenECCKeyPair_CMD										45
#define SKF_ImportECCKeyPair_CMD									46
#define SKF_ECCSignData_CMD											47
#define SKF_ECCHashAndSignData_CMD									48
#define SKF_ECCVerify_CMD											49
#define SKF_ECCHashAndVerify_CMD									50
#define SKF_ECCDecrypt_CMD											51
#define SKF_ECCDecryptBySignKeyPair_CMD								52
#define SKF_ECCExportSessionKey_CMD									53
#define SKF_ExtECCEncrypt_CMD										54
#define SKF_GenerateAgreementDataWithECC_CMD						55
#define SKF_GenerateAgreementDataAndKeyWithECC_CMD					56
#define SKF_GenerateKeyWithECC_CMD									57
#define SKF_ExportPublicKey_CMD										58
#define SKF_CheckKeyPairExistence_CMD								59
#define SKF_ImportSessionKey_CMD									60
#define SKF_EncryptInit_CMD											61
#define SKF_Encrypt_CMD												62
#define SKF_EncryptUpdate_CMD										63
#define SKF_EncryptFinal_CMD										64
#define SKF_DecryptInit_CMD											65
#define SKF_Decrypt_CMD												66
#define SKF_DecryptUpdate_CMD										67
#define SKF_DecryptFinal_CMD										68
#define SKF_DigestInit_CMD											69
#define SKF_Digest_CMD												70
#define SKF_DigestUpdate_CMD										71
#define SKF_DigestFinal_CMD											72
#define SKF_MacInit_CMD												73
#define SKF_Mac_CMD													74
#define SKF_MacUpdate_CMD											75
#define SKF_MacFinal_CMD											76
#define SKF_CloseHandle_CMD											77
#define SM2EncryptMessage_CMD										78
#define SM2DecryptMessage_CMD										79
#define generateECCKeyPair_CMD										80
#define Sm3KDF_CMD													81
*/
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
#endif
