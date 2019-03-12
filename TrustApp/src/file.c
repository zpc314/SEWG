/**/
/* Created by Nick on 2017/3/16.*/
/**/

#include "file.h"
#include "qsee_sfs.h"
#include "qsee_fs.h"
/*#include <sys/stat.h>*/
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "def.h"
#include "log.h"
#include "file_helper.h"

int
getDirSubObjectList(const char *dir, const unsigned char type, const char *prefix, char *list,
                    unsigned int *length) {
	char *sub_object_list[MAX_FILE_NUM];
	uint32_t sub_object_list_len = 0;
	int ret = 0;
	int i = 0;
	int actualLen = 0, remainLen = *length;
	int prefixLength = (prefix == NULL ? 0 : strlen(prefix));
	LOGI("getDirSubObjectList start, dir: %s, type:%d, prefix:%s, list:%s, length:%d", dir, type, prefix, list, *length);
	if (NULL == dir || type < 0 || type > 1 || NULL == length)
		return EINVAL;
		
	while(i < MAX_FILE_NUM) {
		sub_object_list[i]  = (char*)malloc((uint32_t)SFS_MAX_FILENAME_SIZE + 1);
		i++;
	}
	ret = getSubObject(dir, type, prefix, sub_object_list, &sub_object_list_len);
        LOGI("getSubObject count :%d", sub_object_list_len);
        if (ret != 0) {
		array_free(sub_object_list, MAX_FILE_NUM);
		return ret;
	}
	if (NULL == list) {
		i = 0;
		while (i < sub_object_list_len) {
            		LOGI("sub_object_list[%d] :%s", i, sub_object_list[i]);
			*length = *length + (int)strlen(sub_object_list[i]) + 1 - prefixLength;
            		i++;
		}
		array_free(sub_object_list, MAX_FILE_NUM);
		return 0;
	}
	i = 0;
	while (i < sub_object_list_len) {
        	LOGI("prefix :%s", prefix);
		int len = (int)strlen(sub_object_list[i]) + 1 - prefixLength;
		if (remainLen < len) {
			ret = ENOMEM;
			break;
		}
        	LOGI("sub_object_list[%d] :%s", i, sub_object_list[i]);
		strcpy(list + actualLen, sub_object_list[i] + prefixLength);
		actualLen += len;
		remainLen -= len;
		i++;
	}
    	*length = actualLen;
    	LOGI("list :%s", list);
	array_free(sub_object_list, MAX_FILE_NUM);
	LOGI("getDirSubObjectList end,ret:%d, dir: %s, type:%d, prefix:%s, list:%s, length:%d",ret, dir, type, prefix, list, *length);
	return ret;
}

int
getDirSubObjectCount(const char *dir, const unsigned char type, const char *prefix, int *count) {
	char *sub_object_list[MAX_FILE_NUM];
	int ret = 0;
	int i = 0;
	LOGI("getDirSubObjectCount start, dir: %s, type:%d, prefix:%s, count:%d", dir, type, prefix, *count);
	if (NULL == dir || type < 0 || type > 1 || NULL == count)
		return EINVAL;
		
	while(i < MAX_FILE_NUM) {
		sub_object_list[i]  = (char*)malloc((uint32_t)SFS_MAX_FILENAME_SIZE + 1);
		i++;
	}
	ret = getSubObject(dir, type, prefix, sub_object_list, (uint32_t *)count);
	array_free(sub_object_list, MAX_FILE_NUM);
	LOGI("getDirSubObjectCount end, ret:%d, dir: %s, type:%d, prefix:%s, count:%d", ret, dir, type, prefix, *count);
	return ret;
}

int readFile(const char *fileName, const int offset, unsigned char *buffer, unsigned int *length) {
	char *qsee_filename;
	int fd = 0;
	int ret = 0;
	LOGI("readFile start, fileName: %s, offset:%d, buffer:%d, length:%d", fileName, offset, buffer, *length);
	if (NULL == fileName || offset < 0 || NULL == buffer || NULL == length || *length <= 0)
		return EINVAL;
	qsee_filename = (char *)malloc((uint32_t)strlen(fileName) + 1);
	path2file(fileName, qsee_filename);
	LOGI("readFile path2file end, filename:%s, qsee_filename:%s", fileName, qsee_filename);
	fd = qsee_sfs_open(qsee_filename, O_RDONLY);
	LOGI("readFile qsee_sfs_open end");
	if (fd == 0) {
		LOGI("readFile qsee_sfs_open failed, error:%d", qsee_sfs_error(fd));
		SAFE_FREE(qsee_filename);
		return ENOENT;
	}
	if (-1 == qsee_sfs_seek(fd, offset, SEEK_SET)) {
		LOGI("readFile qsee_sfs_seek failed, fd: %d, offset:%d, SEEK_SET:%d, error:%d", fd, offset, SEEK_SET, qsee_sfs_error(fd));
		SAFE_FREE(qsee_filename);
		return EIO;
	}
	LOGI("readFile qsee_sfs_seek end");
	*length = qsee_sfs_read(fd, (char *)buffer, *length);
	LOGI("readFile qsee_sfs_read end length: %d", *length);
	if (*length == -1) {
	    LOGI("readFile qsee_sfs_read FAILED, error:%d",qsee_sfs_error(fd));
		ret = EIO;
	}
	SAFE_FREE(qsee_filename);
	qsee_sfs_close(fd);
	LOGI("readFile end, ret:%d, fileName: %s, offset:%d, buffer:%d, length:%d",ret, fileName, offset, buffer, *length);
	return ret;
}

int writeFile(const char *fileName, const int offset, unsigned char *buffer, int length) {
	char *qsee_filename;
	int fd = 0;
	int ret = 0;
	LOGI("writeFile start, fileName: %s, offset:%d, buffer:%d, length:%d", fileName, offset, buffer, length);
	if (NULL == fileName || offset < 0 || NULL == buffer || length <= 0)
		return EINVAL;
	qsee_filename = (char *)malloc((uint32_t)strlen(fileName) + 1);
	path2file(fileName, qsee_filename);
	LOGI("writeFile path2file end ,filename:%s, qsee_filename:%s", fileName, qsee_filename);
	fd = qsee_sfs_open(qsee_filename, O_RDWR | O_CREAT);
	if (fd == 0) {
		SAFE_FREE(qsee_filename);
		return ENOENT;
	}
	LOGI("writeFile qsee_sfs_open end");
	if (-1 == qsee_sfs_seek(fd, offset, SEEK_SET)) {
		SAFE_FREE(qsee_filename);
		return EIO;
	}
	LOGI("writeFile qsee_sfs_seek end");
	ret = qsee_sfs_write(fd, (const char *)buffer, length);
	LOGI("writeFile qsee_sfs_write end ret = %d", ret);
	if (ret != length) {
		ret = EIO;
	} else {
	    ret = 0;
	}
	SAFE_FREE(qsee_filename);
	qsee_sfs_close(fd);
	LOGI("writeFile end, ret:%d, fileName: %s, offset:%d, buffer:%d, length:%d", ret, fileName, offset, buffer, length);
    return ret;
}

int isFileOrDirExist(const char *path) {
	LOGI("isFileOrDirExist start, path: %s", path);
	return isPathExist(path);
}

int deleteDirectory(const char *dir) {
	LOGI("deleteDirectory start, dir: %s", dir);
    return deleteDir(dir);
}

int deleteSubDirectory(const char* dir) {
	LOGI("deleteSubDirectory start, dir: %s", dir);
    return deleteSubDir(dir);
}

int deleteFile(const char *filePath) {
	char *qsee_filename;
	int ret = 0;
	LOGI("deleteFile start, filePath: %s", filePath);
	qsee_filename = (char *)malloc((uint32_t)strlen(filePath) + 1);
	path2file(filePath, qsee_filename);
	LOGI("deleteFile path2file end, filePath: %s, qsee_filename:%s", filePath, qsee_filename);
	ret = qsee_sfs_rm(qsee_filename);
	if (ret != 0) {
		ret = ENOENT;
	}
	SAFE_FREE(qsee_filename);
	LOGI("deleteFile end, filePath: %s", filePath);
    return ret;
}

int makeDir(const char *path) {
	char *qsee_filename;
	int ret = 0;
	int fd = 0;
	LOGI("makeDir start, path: %s", path);
	if (NULL == path) {
		return EINVAL;
	}
	qsee_filename = (char *)malloc((uint32_t)strlen(path) + 2);
	path2file(path, qsee_filename);
	LOGI("makeDir path2file end, path: %s, qsee_filename:%s", path, qsee_filename);
	qsee_filename[strlen(path)] = SPLIT_NEW;
	qsee_filename[strlen(path) + 1] = '\0';
	fd = qsee_sfs_open(qsee_filename, O_CREAT | O_RDWR);
	LOGI("makeDir qsee_sfs_open end");
	if (fd == 0) {
		ret = ENOENT;
	}
	SAFE_FREE(qsee_filename);
	qsee_sfs_close(fd);
	LOGI("makeDir end, path: %s", path);
    return ret;
}

int createFile(const char *filePath, const unsigned char* buffer, const int length) {
	char *qsee_filename;
	int pathLen;
	int fd;
	int i = 0;
	int ret = 0;
	LOGI("createFile start, filePath: %s, buffer:%d, length:%d", filePath, buffer, length);
	while(i < length) {
		LOGI("buffer[%d]: %d", i, buffer[i]);
		i++;
	}
	if (NULL == filePath)
        	return EINVAL;
	pathLen = strlen(filePath);
	qsee_filename = (char *)malloc(pathLen + 1);
	strcpy(qsee_filename, filePath);
	i = pathLen-1;
	while (i >= 0) {
		if (qsee_filename[i] == SPLIT_OLD) {
			qsee_filename[i] = '\0';
			break;
		}
		i--;
	}
	LOGI("createFile dirsplit end, the dir:", qsee_filename);
	if (0 != isPathExist(qsee_filename)) {
		SAFE_FREE(qsee_filename);
		return ENOENT;
	}
	path2file(filePath, qsee_filename);
    LOGI("createFile path2file end, qsee_filename: %s", qsee_filename);
	fd = qsee_sfs_open(qsee_filename, O_RDWR | O_CREAT | O_TRUNC);
	LOGI("createFile qsee_sfs_open end");
	if (fd == 0) {
		SAFE_FREE(qsee_filename);
		return ENOENT;
	}
	if (NULL != buffer && length>0) {
		ret = qsee_sfs_write(fd, (const char *)buffer, length);
		if (ret != length) {
			ret = EIO;
		} else {
		    ret = 0;    
		}
	}
	SAFE_FREE(qsee_filename);
	qsee_sfs_close(fd);
	LOGI("createFile start, ret:%d, filePath: %s, buffer:%d, length:%d", ret, filePath, buffer, length);
	return ret;
}

int makePath(char *path, const int length, const char *format, ...) {
	va_list ap;
	int ret;
	LOGI("makePath start");
    va_start(ap, format);
    /*vsnprintf在c89中不支持*/
    ret = vsnprintf(path, length, format, ap);
    /*ret = vsprintf(path, format, ap);*/
    va_end(ap);
    if (ret < 0)
        return errno;
	LOGI("makePath, path:%s", path);
    return 0;
}
