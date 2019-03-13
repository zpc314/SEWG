#ifndef SECURECORESERVICE_DEF_H
#define SECURECORESERVICE_DEF_H

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
//#include "cipher/def_sc.h"
//#include "qsee_heap.h"

#define PATH_MAX                       4096

#define DEFAULT_PIN                    "123456"

#define DEVICE_HANDLE                   0
#define MAX_MAC_LENGTH                  256
#define MAX_HASH_LENGTH                 128
#define MAX_CERT_LENGTH                 10240
#define MAX_RSA_SIGNATURE_LENGTH        1024

#define ECC_COORDINATE_PREFIX           0x04
#define ECC_MAX_XCOORDINATE_BITS_LEN    512
#define ECC_MAX_YCOORDINATE_BITS_LEN    512
#define ECC_MAX_MODULUS_BITS_LEN        512
#define ECC_MAX_XCOORDINATE_BYTES_LEN   32
#define ECC_MAX_YCOORDINATE_BYTES_LEN   32
#define ECC_MAX_MODULUS_BYTES_LEN       ECC_MAX_MODULUS_BITS_LEN/8

#define MAX_RSA_MODULUS_LEN             256
#define MAX_RSA_EXPONENT_LEN            4
#define RANDOM_LENGTH                   32

#define MAX_PUB_KEY_LENGTH              1024
#define MAX_PRI_KEY_LENGTH              1024
#define CUR25519_PUB_KEY_LENGTH         32
#define SM2_PUB_KEY_LENGTH              64
#define SM2_PUB_KEY_LENGTH_WITH_PREFIX  65
#define SM2_PUB_KEY_LENGTH_HALF         32
#define SM2_PRI_KEY_LENGTH              32
#define SM2_SIGN_RESULT_LENGTH          64
#define SM2_SIGN_RESULT_LENGTH_HALF     32
#define SM3_HASH_LENGTH                 32
#define MIN_ECC_CIPHER_BLOB_LENGTH      97
#define ECC_CIPHER_BLOB_X_POS           1
#define ECC_CIPHER_BLOB_Y_POS           33
#define ECC_CIPHER_BLOB_HASH_POS        65
#define ECC_CIPHER_BLOB_CIPHER_POS      97

#define SMALL_BUFFER_LENGTH             1024
#define MAX_NAME_LENGTH                 256
#define MAX_PIN_LENGTH                  64
#define MAX_FILE_NAME_LENGTH            256
#define MAX_USEID_LEN                   32
#define MAX_APPLICATION_COUNT           32
#define MAX_CONTAINER_COUNT             32
#define MAX_IV_LEN                      16
#define DEVAPI
#define MAX_SERVER_INFO_LENGTH          128

#define SIGN_CERT_FILE              "sign_cert.dat"
#define ENC_CERT_FILE               "enc_cert.dat"
#define CON_CONFIG_FILE             "config.dat"

#define DEFAULT_USER_NAME           "1234567812345678"
#define DEFAULT_USER_NAME_LENGTH    16

#define PUBKEY_PREFIX               0x04

#define SAFE_FREE(x)  if((x)!=NULL){free(x);x=NULL;}


#define ENCRYPT_SMS4_KEY_LENGTH    (MIN_ECC_CIPHER_BLOB_LENGTH+SMS4_KEY_LENGTH)
#define KEYID_LENGTH    32
#define MAX_PRI_KEY_CIPHER_LENGTH (MAX_PRI_KEY_LENGTH+SMS4_BLOCK_LENGTH)

#define MAX_APP_ID_LENGTH           20
#define MAX_APP_SECRET_LENGTH       64

#define CON_SIGN_KEY_BACKUP_LENGTH      (SM2_PUB_KEY_LENGTH+SM2_PRI_KEY_LENGTH+SM2_PRI_KEY_LENGTH+ENCRYPT_SMS4_KEY_LENGTH+SMS4_BLOCK_LENGTH)

#define PIN_INFO_LENGTH             64

#ifndef SM3_PARAM
#define SM3_PARAM

typedef struct _SM3_HASH_STATE {
    unsigned int H[8];
    unsigned char BB[64];

#ifdef WIN32
    unsigned __int64 u64Length;
#else
    unsigned long int u64Length;
#endif
} SM3_HASH_STATE;

typedef struct {
    unsigned long total[2];     /*!< number of bytes processed  */
    unsigned long state[8];     /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */

    unsigned char ipad[64];     /*!< HMAC: inner padding        */
    unsigned char opad[64];     /*!< HMAC: outer padding        */

} sm3_context;
#endif

#ifndef SM4_PARAM
#define SM4_PARAM

#define SMS4_KEY_LENGTH                (128/8)
#define SMS4_BLOCK_LENGTH            (128/8)
#define SMS4_ROUND                    32
typedef struct {
    int mode;
    uint32_t sk[32];
} sm4_context;
#endif

//#define malloc qsee_malloc
//#define free qsee_free

typedef unsigned char BYTE;
typedef int8_t CHAR;
typedef uint32_t ULONG;
typedef uint32_t DWORD;
typedef char *LPSTR;
typedef bool BOOL;
typedef uint32_t HANDLE;
typedef HANDLE DEVHANDLE;
typedef HANDLE HAPPLICATION;
typedef HANDLE HCONTAINER;

typedef struct Struct_FILEATTRIBUTE {
    char FileName[32];
    ULONG FileSize;
    ULONG ReadRights;
    ULONG WriteRights;
} FILEATTRIBUTE, *PFILEATTRIBUTE;

#endif
