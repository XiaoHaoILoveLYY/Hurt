#pragma once

/*
* Actor 小皓
* 注意：改文件内的函数请勿修改，暂不支持32bit指针加载
* aide、Android studio、vs都可以编译
* 仅供学习参考 切勿用作商业用途
* 使用该源码请勿用作非法用途，一切责任与原作者无关！
* e-mail: 1746006930@qq.com
* QQ群聊：772554723
*/
#include <jni.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include <thread>
/*
* 宏定义
*/
#define DWORD int
#define FLOAT float
#define WORD int16_t
#define DOUBLE double
#define BYTE char
#define QWORD long long
#define Addr long long
#define PointerHex std::vector<Addr>
#define Pointer std::string
#define PointerArray std::vector<Pointer>
#define PointerResult std::vector<Addr>
#define PI 3.1415926532

DWORD pid_ = -1;
DWORD handle_ = -1;
DWORD getPID(const char* Name);
DWORD getHandle(DWORD pid);
Addr getModuleBssAddr(const char* Name);
Addr getModuleAddr(const char* Name);
bool initialization(const char* Name);
DWORD getPID();
DWORD getHandle();
DWORD getAddrDword(Addr addr);
FLOAT getAddrFloat(Addr addr);
WORD getAddrWord(Addr addr);
BYTE getAddrByte(Addr addr);
DOUBLE getAddrDouble(Addr addr);
QWORD getAddrQword(Addr addr);
void setAddrDword(Addr addr, DWORD var);
void setAddrFloat(Addr addr, FLOAT var);
void setAddrWord(Addr addr, WORD var);
void setAddrByte(Addr addr, BYTE var);
void setAddrDouble(Addr addr, DOUBLE var);
void setAddrQword(Addr addr, QWORD var);
int HexStringToInt(const char* hex);
int HexStringToInt(std::string hex);
Addr getPointerAddr(Pointer pointer); // 获取一条指针结果
Addr getPointerAddr(PointerHex pointer);
PointerResult getPointerArrayResult(PointerArray pointerList);
char* jstringToChar(JNIEnv* env, jstring jstr);// java string 转 char
std::vector<std::string> split(std::string str,std::string separator);// 字符串解析
float rad(float a);

bool initialization(const char* Name){
    pid_ = getPID(Name);
    handle_ = getHandle(pid_);
}

Addr getModuleBssAddr(const char* Name){
    FILE *fp;
    int cnt = 0;
    Addr start;
    char tmp[256];
    fp = NULL;
    char line[1024];
    char fname[128];
    sprintf(fname, "/proc/%d/maps", pid_);
    fp = fopen(fname, "r");
    while (!feof(fp)){
        fgets(tmp, 256, fp);
        if (cnt == 1){
            if (strstr(tmp, "[anon:.bss]") != NULL){
                sscanf(tmp, "%lx-%*lx", &start);
                break;
            }else{
                cnt = 0;
            }
        }if (strstr(tmp, Name) != NULL){
            cnt = 1;
        }
    }
	return start;
}
Addr getModuleAddr(const char* Name){
    char filename[32];
    char cmdline[256];
    sprintf(filename, "/proc/%d/maps", pid_);
    FILE* fp = fopen(filename, "r");
    Addr revalue;
    if (fp){
        while (fgets(cmdline, 256, fp)){
            if (strstr(cmdline, Name) && strstr(cmdline, "r-xp")){
                char* str = strstr(cmdline, "-");
                if (str){
                    *str = '\0';
                    char num[32];
                    sprintf(num, "0x%s", cmdline);
                    revalue = strtoul(num, NULL, 0);
                    return revalue;
                }
            }
            memset(cmdline, 0, 256);
        }
        fclose(fp);
    }
    return revalue;
}

int HexStringToInt(const char* hex){
    return (strstr(hex,"0x") != nullptr) ? std::stoi(hex, 0, 16) : std::stoi(hex, 0, 10);
}

int HexStringToInt(std::string hex){
    return (strstr(hex.data(),"0x") != nullptr) ? std::stoi(hex, 0, 16) : std::stoi(hex, 0, 10);
}

PointerResult getPointerArrayResult(PointerArray pointerList){
    PointerResult result;
    for(Pointer pointer : pointerList){
        result.push_back(getPointerAddr(pointer));
    }
    sort(result.begin(), result.end());
    result.erase(unique(result.begin(), result.end()), result.end());
    return result;
}

Addr getPointerAddr(Pointer pointer){
    Addr result = -1;
    std::vector<std::string> pointerArray = split(pointer, " -> ");
    if(strstr(pointerArray[0].data(),"lib") != nullptr && strstr(pointerArray[0].data(),".so") != nullptr) {
        result = (strstr(pointerArray[0].data(),":bss") != nullptr) ? getModuleBssAddr(split(pointerArray[0], ":")[0].data())
             : getModuleAddr(pointerArray[0].data());
    }else {
        result = HexStringToInt(pointerArray[0]);
    }
    for(int i = 1; i < pointerArray.size() - 1; i++){
        result += HexStringToInt(pointerArray[i]);
        result = getAddrQword(result);
    }
    return result + HexStringToInt(pointerArray[pointerArray.size() - 1]);
}

Addr getPointerAddr(Addr base, PointerHex pointer){
    Addr result = base;
    for(int i = 0; i < pointer.size() - 1; i++){
        result += pointer[i];
        result = getAddrQword(result);
    }
    return result+pointer[pointer.size() - 1];
}

std::vector<std::string> split(std::string str,std::string separator){
    std::vector<std::string> result;
    int cutAt;
    while((cutAt = str.find_first_of(separator))!=str.npos){
        if(cutAt>0){
            result.push_back(str.substr(0,cutAt));
        }
        str=str.substr(cutAt+1);
    }
    if(str.length()>0){
        result.push_back(str);
    }
    return result;
}

DWORD getPID(const char* Name){
    int id = -1;
    DIR *dir;
    FILE *fp;
    char filename[32];
    char cmdline[256];
    struct dirent *entry;
    dir = opendir("/proc");
    while ((entry = readdir(dir)) != NULL){
        id = atoi(entry->d_name);
        if (id != 0){
            sprintf(filename, "/proc/%d/cmdline", id);
            fp = fopen(filename, "r");
            if (fp){
                fgets(cmdline, sizeof(cmdline), fp);
                fclose(fp);
                if (strcmp(Name, cmdline) == 0){
                    return id;
                }
            }
        }
    }
    closedir(dir);
	return -1;
}

DWORD getHandle(DWORD pid){
    DWORD handle;
    char lj[64];
    sprintf(lj, "/proc/%d/mem", pid);
    handle=open(lj, O_RDWR);
    lseek(handle_, 0, SEEK_SET);
    return handle;
}

DWORD getAddrDword(Addr addr){
    DWORD var;
    pread64(handle_, &var, 4, addr);
    return var;
}
FLOAT getAddrFloat(Addr addr){
    FLOAT var;
    pread64(handle_, &var, 4, addr);
    return var;
}
WORD getAddrWord(Addr addr){
    WORD var;
    pread64(handle_, &var, 2, addr);
    return var;
}
BYTE getAddrByte(Addr addr){
    BYTE var;
    pread64(handle_, &var, 1, addr);
    return var;
}
DOUBLE getAddrDouble(Addr addr){
    DOUBLE var;
    pread64(handle_, &var, 8, addr);
    return var;
}
QWORD getAddrQword(Addr addr){
    QWORD var;
    pread64(handle_, &var, 8, addr);
    return var;
}

void setAddrDword(Addr addr, DWORD var){
    pwrite64(handle_, &var, 4, addr);
}

void setAddrFloat(Addr addr, FLOAT var){
    pwrite64(handle_, &var, 4, addr);
}

void setAddrWord(Addr addr, WORD var){
    pwrite64(handle_, &var, 2, addr);
}

void setAddrByte(Addr addr, BYTE var){
    pwrite64(handle_, &var, 1, addr);
}

void setAddrDouble(Addr addr, DOUBLE var){
    pwrite64(handle_, &var, 8, addr);
}

void setAddrQword(Addr addr, QWORD var){
    pwrite64(handle_, &var, 8, addr);
}

char* jstringToChar(JNIEnv* env, jstring jstr){
    char* rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("UTF-8");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray)env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte* ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0){
        rtn = (char*)malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    return rtn;
}


DWORD getPID(){
    return pid_;
}

DWORD getHandle(){
    return handle_;
}

float rad(float a) {
    return a / 180.0f * PI;
}
