#include "param.h"
#define SPLIT_OLD		'/'
#define SPLIT_NEW		'/'
#define MAX_FILE_NUM	1024

/*将securecore中的路径转换为tee里对应的文件名*/
int path2file(const char *path, char *file);

/*获取目录下所有子元素*/
int getSubObject(const char *dir, const unsigned char type, const char* prefix,
	char** sub_object_list, uint32_t* sub_object_list_len);

/*判断文件或文件夹是否存在*/
int isPathExist(const char *path);

/*删除文件夹*/
int deleteDir(const char *dir);

/*删除所有子文件夹*/
int deleteSubDir(const char *dir);

/*释放指针数组*/
void array_free(char **pointer, int length);
