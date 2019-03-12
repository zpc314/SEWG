#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "def.h"
#include "qsee_sfs.h"
#include "qsee_fs.h"
#include "file_helper.h"
#include "log.h"

/*int cleanlist(sfs_file_entry** file_list, uint32_t size) {
	int i = 0;	
	LOGI("cleanlist start, size:%d", size);
	while(i < size) {
		qsee_sfs_clean_file_list(file_list[i]);	
		i++;
	}
	LOGI("cleanlist end, size:%d", size);
	return 0;
}*/

int path2file(const char *path, char *file) {
	int i = 0;
	LOGI("path2file start, path:%s, file:%s", path, file);
	if (path == NULL || file == NULL) {
		return EINVAL;
	}
	strcpy(file, path);
	/*while (file[i] != '\0') {
		if (file[i] == SPLIT_OLD) {
			file[i] = SPLIT_NEW;
		}
		i++;
	}*/
	LOGI("path2file end, path:%s, file:%s", path, file);
	return 0;
}

int getSubObject(const char *dir, const unsigned char type, const char* prefix,
		char** sub_object_list, uint32_t* sub_object_list_len) {

	sfs_file_entry* file_list;
	uint32_t file_list_len = 0;
	int dirlen;
	char *reg;
	int ret = 0;
	uint32_t i = 0;
	LOGI("getSubObject, dir:%s, type:%d, prefix:%s, sub_object_list_len:%d", dir, type, prefix, *sub_object_list_len);
	if (NULL == dir || type < 0 || type > 1 || NULL == sub_object_list || NULL == sub_object_list_len)
		return EINVAL;
	dirlen = strlen(dir);
	ret = qsee_sfs_get_file_list(&file_list, &file_list_len);
    LOGI("getSubObject, qsee_sfs_get_file_list end:%d",file_list_len);
	if (ret != 0) {
		/*ret = qsee_sfs_error();*/
		/*需要调用qsee_sfs_error吗？*/
		qsee_sfs_clean_file_list(file_list);
		/*todo:应该返回什么结果码*/
		return ENOENT;
	}

	if (prefix != NULL) {
		reg = (char *)malloc(dirlen + strlen(prefix) + 2);
	} else {
		reg = (char *)malloc(dirlen + 2);
	}
	ret = path2file(dir, reg);

	if (0 != ret) {
		SAFE_FREE(reg);
		qsee_sfs_clean_file_list(file_list);
		return ret;
	}
	reg[dirlen] = SPLIT_NEW;
	if (prefix != NULL) {
		strcpy(reg + dirlen + 1, prefix);
	} else {
		reg[dirlen+1] = '\0';
	}
	*sub_object_list_len = 0;
	i = 0;
    LOGI("getSubObject reg:%s", reg);
	while (i < file_list_len) {
		sfs_file_entry entry = file_list[i];
		char *name = entry.file_name;
		i++;
		if (name == strstr(name, reg)) {
			if(strlen(name) == strlen(dir) + 1) {
				continue;
			}
			char tmp[2];
			tmp[0] = SPLIT_NEW;
			tmp[1] = '\0';
			if (type == 1) {
				if ((name + strlen(name) - 1) == strstr(name + strlen(reg), (const char *)&tmp)) {
					*(name + strlen(name) - 1) = '\0';
					LOGI("getSubObject, name(dir):%s", name);
					strcpy(sub_object_list[*sub_object_list_len], name + strlen(dir) +1);
					(*sub_object_list_len)++;
				}
			} else {
				if (NULL == strstr(name + strlen(reg), (const char *)&tmp)) {
					LOGI("getSubObject, name(file):%s", name);
					strcpy(sub_object_list[*sub_object_list_len], name + strlen(dir) + 1);
					(*sub_object_list_len)++;
				}
			}
		}
	}
	SAFE_FREE(reg);
	qsee_sfs_clean_file_list(file_list);
	LOGI("getSubObject, dir:%s, type:%d, prefix:%s, sub_object_list_len:%d", dir, type, prefix, *sub_object_list_len);
	return 0;
}

int isPathExist(const char *path) {
	sfs_file_entry* file_list;
	uint32_t file_list_len = 0;
	int dirlen;
	char *reg;
	int ret = 0;
	uint32_t i = 0;
	LOGI("isPathExist start, path:%s", path);
	if (NULL == path)
		return EINVAL;
	dirlen = strlen(path);
	ret = qsee_sfs_get_file_list(&file_list, &file_list_len);
	if (ret != 0 || file_list_len == 0) {
		qsee_sfs_clean_file_list(file_list);
		/*todo:应该返回什么结果码*/
		return ENOENT;
	}
	reg = (char *)malloc(dirlen + 1);
	ret = path2file(path, reg);
	/*printf("%s\n", reg);*/
	if (0 != ret) {
		SAFE_FREE(reg);
		qsee_sfs_clean_file_list(file_list);
		return ret;
	}
	LOGI("isPathExist, reg:%s", reg);
	i = 0;
    ret = ENOENT;
	while (i < file_list_len) {
		sfs_file_entry entry = file_list[i];
		char *name = entry.file_name;
		LOGI("isPathExist, name:%s", name);
		i++;
        char chr = *(name+strlen(reg));
		if (name == strstr(name, reg) && (chr == '\0' || chr == SPLIT_NEW)) {
			ret =  0;
			break;
		}
	}
	SAFE_FREE(reg);
	qsee_sfs_clean_file_list(file_list);
	LOGI("isPathExist end, ret:%d, path:%s", ret, path);
	return ret;
}

int deleteDir(const char *dir) {
	sfs_file_entry* file_list;
	uint32_t file_list_len = 0;
	int dirlen;
	char *reg;
	int ret = 0;
	uint32_t i = 0;
	LOGI("deleteDir start, dir:%s", dir);
	if (NULL == dir)
		return EINVAL;
	dirlen = strlen(dir);
	ret = qsee_sfs_get_file_list(&file_list, &file_list_len);
	LOGI("deleteDir, qsee_sfs_get_file_list end, ret:%d, file_list_len:%d", ret, file_list_len);
	if (ret != 0) {
		/*ret = qsee_sfs_error();*/
		/*需要调用qsee_sfs_error吗？*/
		qsee_sfs_clean_file_list(file_list);
		/*todo:应该返回什么结果码*/
		return ENOENT;
	}
	reg = (char *)malloc(dirlen + 2);
	ret = path2file(dir, reg);
	LOGI("deleteDir, path2file end, reg:%s", reg);
	if (0 != ret) {
		SAFE_FREE(reg);
		qsee_sfs_clean_file_list(file_list);
		return ret;
	}
	reg[dirlen] = SPLIT_NEW;
	reg[dirlen+1] = '\0';
	i = 0;
	while (i < file_list_len) {
		sfs_file_entry entry = file_list[i];
		char *name = entry.file_name;
		LOGI("deleteDir, name:%s", name);
		if (name == strstr(name, reg)) {
			ret = qsee_sfs_rm(name);
			if (ret != 0) {
				ret = ENOENT;
				break;
			}
		}
		i++;
	}
	SAFE_FREE(reg);
	qsee_sfs_clean_file_list(file_list);
	LOGI("deleteDir end, ret:%d, dir:%s", ret, dir);
	return ret;
}

int deleteSubDir(const char *dir) {
	sfs_file_entry* file_list;
	uint32_t file_list_len = 0;
	int dirlen;
	char *reg;
	int ret = 0;
	uint32_t i = 0;

	if (NULL == dir)
		return EINVAL;
	dirlen = strlen(dir);
	ret = qsee_sfs_get_file_list(&file_list, &file_list_len);
	if (ret != 0) {
		qsee_sfs_clean_file_list(file_list);
		/*todo:应该返回什么结果码*/
		return ENOENT;
	}
	reg = (char *)malloc(dirlen + 2);
	ret = path2file(dir, reg);
	if (0 != ret) {
		SAFE_FREE(reg);
		qsee_sfs_clean_file_list(file_list);
		return ret;
	}
	reg[dirlen] = SPLIT_NEW;
	reg[dirlen+1] = '\0';
	i = 0;
	while (i < file_list_len) {
		sfs_file_entry entry = file_list[i];
		char *name = entry.file_name;
		i++;
		if (name == strstr(name, reg)) {
			if(strlen(name) == strlen(dir) + 1) {
				continue;
			}
			char tmp[2];
			tmp[0] = SPLIT_NEW;
			tmp[1] = '\0';
			if (NULL != strstr(name + strlen(reg), (const char *)&tmp)) {
				ret = qsee_sfs_rm(name);
				if (ret != 0) {
					ret = ENOENT;
					break;
				}
			}
		}
	}
	SAFE_FREE(reg);
	qsee_sfs_clean_file_list(file_list);
	return ret;
}

void array_free(char **pointer, int length) {
	int i = 0;
	while(i < length) {
		SAFE_FREE(pointer[i])
		i++;
	}
}
