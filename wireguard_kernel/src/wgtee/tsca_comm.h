#ifndef TSCA_COMM_H_H
#define TSCA_COMM_H_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <sys/types.h>
//#include "demo.h"
/**
 * 启动TA
 * @param appname TA名字
 * @param buf_size 缓冲区长度（send+recv+200）
 * @return
 */
int32_t ts_start_app(const char *appname, uint32_t buf_size);

/**
 * 关闭应用
 * @return
 */
int32_t ts_shutdown_app(void);

/**
 * 发送命令
 * @param appname TA名字
 * @param send_buf 发送数据
 * @param send_size 发送数据长度
 * @param recv_buf 接受数据
 * @param recv_size 接受数据长度
 * @return
 */
int32_t ts_ca_send(const char *appname, void *send_buf, uint32_t send_size, void *recv_buf, uint32_t recv_size);

int32_t ts_ca_ui(const char *appname, uint32_t cmd, void *send_buf, uint32_t send_size, void *recv_buf, uint32_t recv_size);

#ifdef __cplusplus
}
#endif

#endif //TEESECURECORE_TSCA_COMM_H_H
