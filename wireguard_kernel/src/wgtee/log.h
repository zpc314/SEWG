#ifndef LOG_H
#define LOG_H
//#include "qsee_log.h" 
//#define DEBUG_LOG

#include <android/log.h>
#define ANDROID_LOG
#define  LOG_TAG    "SC-JNI"

#ifdef  ANDROID_LOG

#define LOGI(...)  __android_log_print(ANDROID_LOG_INFO, "TEE logs", __VA_ARGS__)
#define LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,  "TEE logs", __VA_ARGS__)
#define LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,  "TEE logs", __VA_ARGS__)

#else

#ifdef  DEBUG_LOG

#define LOGI(...)  QSEE_LOG(QSEE_LOG_MSG_ERROR, __VA_ARGS__)
#define LOGE(...)  QSEE_LOG(QSEE_LOG_MSG_ERROR, __VA_ARGS__)
#define LOGD(...)  QSEE_LOG(QSEE_LOG_MSG_ERROR, __VA_ARGS__)

#else

#define  LOGI(...)
#define LOGE(...)
#define LOGD(...)

#endif

#endif

#endif