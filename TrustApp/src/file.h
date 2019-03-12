
#ifndef SECURECORESERVICE_FILE_H
#define SECURECORESERVICE_FILE_H
#ifdef __cplusplus
extern "C" {
#endif

/**
 * 获取目录下某种类型对象的名称
 * @param dir 目录
 * @param type 文件对象类型 0 文件；1 文件夹
 * @param list 文件列表字符串，若list为空，则length返回所需要的缓冲区长度）
 * @param length 文件对象列表的大小
 * @param prefix 文件名的前缀
 * @return 0 表示成功,非零表示出错
 */
int getDirSubObjectList(const char* dir, const unsigned char type, const char* prefix, char* list, unsigned int* length);
/**
 * 获取目录下某种类型对象的数量
 * @param dir 目录
 * @param type 文件对象类型 0 文件；1 文件夹
 * @param prefix 文件名的前缀
 * @return 0 表示成功,非零表示出错
 */
int getDirSubObjectCount(const char* dir, const unsigned char type, const char* prefix, int* count);

/**
 * 读取文件
 * @param fileName 文件名（完整路径）
 * @param offset 偏移
 * @param buffer 存储数据
 * @param length 输入：准备读取的长度，输出：实际读取的长度
 * @return 读取结果：0 表示成功，
 */
int readFile(const char *fileName, const int offset, unsigned char *buffer, unsigned int *length);
/**
 * 写文件
 * @param fileName 文件名
 * @param offset 偏移
 * @param buffer 待写入的数据
 * @param length 待写入数据的长度
 * @return 写入结果：0 表示成功
 */
int writeFile(const char *fileName, const int offset, unsigned char *buffer, int length);
/**
 * 判断文件或文件夹是否存在
 * @param path 文件夹路径
 * @return 0 存在
 */
int isFileOrDirExist(const char* path);
/**
 * 删除目录
 * @param dirPath 文件夹路径
 * @return 0 删除成功
 */
int deleteDirectory(const char* dirPath);

int deleteSubDirectory(const char* dir);
/**
 * 删除文件
 * @param filePath 文件路径
 * @return 0 删除成功
 */
int deleteFile(const char* filePath);
/**
 * 创建文件夹
 * @param path 文件夹路径
 * @return 0 创建成功
 */
int makeDir(const char* path);
/**
 * 创建文件
 * @param filePath 文件路径
 * @param buffer 写入的数据
 * @param length 写入数据的长度
 * @return  0 创建成功
 */
int createFile(const char* filePath, const unsigned char* buffer, const int length);
/**
 * 合成一个路径
 * @param path 完整路径
 * @param length 完整路径的最大长度
 * @param format 格式
 * @return 合成后的长度
 */
int makePath(char* path, const int length, const char* format, ...);

#ifdef __cplusplus
}
#endif

#endif
