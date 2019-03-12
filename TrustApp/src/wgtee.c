
#include <string.h>
#include <stdlib.h>
#include "def.h"
#include "log.h"
#include "curve25519.h"
#include "file.h"
#include "file_helper.h"
#include "char2hex.h"
#include "int2char.h"
#include "qsee_prng.h"

/*region ###全局变量####*/
char ROOT_DIR[PATH_MAX] = "/persist";
char USER_DIR[PATH_MAX] = "/persist";
char USER_APP_DIR[PATH_MAX] = "";
char configfullpath[PATH_MAX] = "";
/*unsigned char secret[CURVE25519_KEY_SIZE];
unsigned char public[CURVE25519_KEY_SIZE];*/

void Init()
{

}

int Initialize(const char *rootDir, const char *logDir)
{
    return 0;
}

int GenerateKeyPair(char * pin, unsigned char *pubKey, int *id)
{
    unsigned char random[4];
    char filename[4]="";
    do{
        qsee_prng_getdata(random, 4);
        *id=(int)random;
        int2char(*id,filename);
        makePath(configfullpath,PATH_MAX,"%s/%s",USER_DIR,filename);
        LOGI("genkey_configfullpath:%s",configfullpath);
    }while(0 == isFileOrDirExist(configfullpath));
    LOGI("genkey_secret_id:%d",*id);
    LOGI("genkey_filename:%s",filename);
    //id=(int *)filename;
    unsigned char secret[CURVE25519_KEY_SIZE];
    unsigned char public[CURVE25519_KEY_SIZE];
    qsee_prng_getdata(secret, CURVE25519_KEY_SIZE);
    char hex[WG_KEY_LEN_HEX];
    key_to_hex(hex,secret);
    LOGI("genkey_creat_secret hex:%s",hex);
    
    if(0 != createFile(configfullpath, (unsigned char *)hex, WG_KEY_LEN_HEX-1))
        return -1;
    curve25519_generate_secret(secret);
    curve25519_generate_public(public, secret);
    key_to_hex(hex,public);
    LOGI("genkey_creat_public hex:%s",hex);
    if(0 != writeFile(configfullpath, WG_KEY_LEN_HEX-1, (unsigned char *)hex, WG_KEY_LEN_HEX-1))
        return -1;
    LOGI("genkey_pin:%s",pin);
    if(0 != writeFile(configfullpath, 2*(WG_KEY_LEN_HEX-1), (unsigned char *)pin, strlen(pin)))
        return -1;
    
    for(int i = 0; i < CUR25519_PUB_KEY_LENGTH; i++)
    {
        pubKey[i] = public[i];
    }
    return 0;
}

int GetPublickeyByID(unsigned char *pubKey, int id)
{
    char filename[4]="";
    int2char(id,filename);
    LOGI("getpubkey_secret_id:%d",id);
    LOGI("getpubkey_filename:%s",filename);
    makePath(configfullpath,PATH_MAX,"%s/%s",USER_DIR,filename);
    LOGI("getpubkey_configfullpath:%s",configfullpath);
    if(0 !=isFileOrDirExist(configfullpath)){
	LOGI("getpubkey_nofile:%s",configfullpath);
	return -1;
	} 
    char hex[WG_KEY_LEN_HEX];
    unsigned int len=WG_KEY_LEN_HEX-1;
    if(0 != readFile(configfullpath, WG_KEY_LEN_HEX-1,(unsigned char *)hex,&len))
        return -1;
    hex[WG_KEY_LEN_HEX-1]='\0';
    LOGI("getpubkey_get_public hex:%s",hex);
    unsigned char public[CURVE25519_KEY_SIZE];
    key_from_hex(public,hex);
    
    for(int i = 0; i < CUR25519_PUB_KEY_LENGTH; i++)
    {
        pubKey[i] = public[i];
    }
    return 0;
}

int CurveMul(char *pin, unsigned char *rst, unsigned char *point, int id)
{
    char filename[4]="";
    int2char(id,filename);
    LOGI("curmul__secret_id:%d",id);
    LOGI("curmul_filename:%s",filename);
    makePath(configfullpath,PATH_MAX,"%s/%s",USER_DIR,filename);
    LOGI("curmul_configfullpath:%s",configfullpath);
    if(0 !=isFileOrDirExist(configfullpath)){
	LOGI("curmul_nofile:%s",configfullpath);
	return -1;
	} 
    char rpin[strlen(pin)];
    unsigned int len=strlen(pin);
    if(0 != readFile(configfullpath, 2*(WG_KEY_LEN_HEX-1),(unsigned char *)rpin,&len))
        return -1;
    if(0 != strcmp(pin,rpin)){
	LOGI("curmul_pinerr:r_pin:%s,pin:%s",rpin,pin);
	return -1;
	} 
    char hex[WG_KEY_LEN_HEX];
    len=WG_KEY_LEN_HEX-1;
    if(0 != readFile(configfullpath,0,(unsigned char *)hex,&len))
        return -1;
    hex[WG_KEY_LEN_HEX-1]='\0';
    LOGI("curmul_get_secret hex:%s",hex);
    unsigned char secret[CURVE25519_KEY_SIZE];
    key_from_hex(secret,hex);
    bool ok = curve25519(rst, secret, point);
    if(ok) {
        return 0;
    } else {
        return 1;
    }
}
