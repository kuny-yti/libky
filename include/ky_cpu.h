
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_cpu.h
 * @brief    关于CPU指令集和CPU能力测试类
 *       1.x86体系架构的指令集检测.
 *       2.CPU信息获取
 *       3.arm体系架构检测
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.0.1
 * @date     2012/01/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2016/07/02 | 1.0.0.1   | kunyang  | 创建文件，将ky_memory的关于CPU能力部分移到此类
 * 2016/11/10 | 1.0.1.0   | kunyang  | 修改类并加入cpu信息获取
 * 2017/04/04 | 1.1.0.1   | kunyang  | 加入arm体系架构检测
 */

#ifndef ky_CPU_H
#define ky_CPU_H
#include "ky_define.h"

typedef enum
{
#if ((kyArchitecture & kyArch_X86) == kyArch_X86)
    CPU_SSE = 1<<0,
    CPU_SSE2 = 1<<1,
    CPU_SSE3 = 1<<2,
    CPU_MMX = 1<<3,
    CPU_MMXEXT = 1<<4,
    CPU_SSSE3 = 1<<5,
    CPU_3DNOW = 1<<6,
    CPU_3DNOWEXT = 1<<7,
    CPU_SSE4 = 1 << 8,
    CPU_SSE42 = 1 << 9,
    CPU_SSE4a = 1 << 10,
    CPU_AVX = 1 << 11 ,
    CPU_AVX2 = 1 << 12,
    CPU_TSC = 1 << 13,
    CPU_SSE2FAST = 1 << 14,
    CPU_SSE2SLOW = 1 << 15,
    CPU_SSE3FAST = 1 << 16,
    CPU_SSE3SLOW = 1 << 17,
    CPU_SSSE3FAST = 1 << 18,
    CPU_SSSE3SLOW = 1 << 19,
    CPU_AVXFAST = 1 << 20 ,
    CPU_AVXSLOW = 1 << 21 ,
    CPU_XOP = 1 << 22 ,
    CPU_FMA3 = 1 << 23,
    CPU_FMA4 = 1 << 24,
    CPU_AESNI = 1 << 25,
    CPU_CMOV = 1 << 26,
    CPU_BMI1 = 1 << 27,
    CPU_BMI2 = 1 << 28,
    CPU_ATOM = 1 << 29,
#elif ((kyArchitecture & kyArch_ARM) == kyArch_ARM)
    CPU_ARMV5TE = 1 << 0,
    CPU_ARMV6 = 1 << 1,
    CPU_ARMV6T2 = 1 << 2,
    CPU_VFP = 1 << 3,
    CPU_VFPV3 = 1 << 4,
    CPU_NEON = 1 << 5,
    CPU_SETEND = 1 << 6,
    CPU_VFPVM = 1 << 7,
    CPU_ARMV8 = 1 << 8,
#elif ((kyArchitecture & kyArch_MIPS) == kyArch_MIPS)

#elif (( kyArchitecture & kyArch_PowerPC) == kyArch_PowerPC)

#endif
    CPU_Unknown = ~(-1)
}eCPUCapables;

class cpu_capable
{
    friend class ky_cpu;
private:
    cpu_capable();
    ~cpu_capable();

#if ((kyArchitecture & kyArch_X86) == kyArch_X86)
    bool have_cpuid();
    void cpuid(uint ax, uint *p);
#endif

    char vendor[16];
    char name[64];
    int  family;
    int  model;
    int  cache;
    uint capable;
};

class ky_cpu
{
public:
    ky_cpu();
    ~ky_cpu();

    bool has(eCPUCapables cap);
    int count();

    int family()const{return capable->family;}
    int model()const{return capable->model;}
    int cache()const{return capable->cache;}
    char* vendor()const{return capable->vendor;}
    char* name()const{return capable->name;}

private:
    cpu_capable *capable;
};


#endif // ky_CPU_H
