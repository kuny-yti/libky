
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_main.h
 * @brief    主入口定义
 *       1.定义了操作系统平台、使用的语言版本、体系架构
 *       2.各平台的主入口函数定义
 *       3.宏名称拼接定义
 *       4.堆申请和释放宏定义
 *       5.CPU字节序检测定义
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.2.2.1
 * @date     2011/04/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2011/04/02 | 1.0.0.1   | kunyang  | 创建文件定义平台宏
 * 2012/03/10 | 1.0.1.0   | kunyang  | 修改平台宏定义，添加入口函数定义
 * 2012/04/01 | 1.0.2.0   | kunyang  | 平台宏完善win32，linux，apple
 * 2012/04/03 | 1.0.2.1   | kunyang  | 修改入口函数实现
 * 2012/04/05 | 1.0.2.2   | kunyang  | 添加字节序检测
 * 2013/06/15 | 1.1.0.1   | kunyang  | 添加编译器语言宏
 * 2013/06/20 | 1.1.0.1   | kunyang  | 修改平台宏及判断方法
 * 2014/01/02 | 1.1.1.0   | kunyang  | 修改linux平台下头文件和入口函数实现
 * 2014/01/08 | 1.1.2.0   | kunyang  | 添加堆分配和释放宏定义
 * 2015/07/23 | 1.2.0.1   | kunyang  | 添加体系架构判断和宏定义
 * 2015/08/01 | 1.2.1.1   | kunyang  | 修改体系架构宏检测
 * 2016/09/20 | 1.2.2.1   | kunyang  | 修改编译器兼容信息
 * 2018/06/28 | 1.2.2.2   | kunyang  | 添加明确当前系统的特有宏
 */
#ifndef __KY_MAIN__
#define __KY_MAIN__

#define kyPlatformMask       0xff00000
#define kyArchMask           0x00ff000
#define kyArchBitMask        0x0000f00
#define kyArchVersionMask    0x00000ff

// desktop platform
#define kyPlatform_Win32     0x0100000 // Microsoft windows 32 bit
#define kyPlatform_Unix      0x0200000 // unix
#define kyPlatform_Linux     0x0400000 // linux 32/64 bit
#define kyPlatform_Apple     0x0800000 // apple 32/64 bit

// phone platform
#define kyPlatform_Phone     0x1100000 // Microsoft wine,phone
#define kyPlatform_Android   0x1400000 // android
#define kyPlatform_Ios       0x1800000 // ios

// arch bit
#define kyArchBit_32         0x0000100 // 32 bit system
#define kyArchBit_64         0x0000300 // 64 bit system
// arch
#define kyArch_X86           0x0001000 // x86
#define kyArch_IA64          0x0003000
#define kyArch_AMD64         0x0007000
#define kyArch_ARM           0x0008000
#define kyArch_PowerPC       0x0010000
#define kyArch_MIPS          0x0020000

// arch version
#define kyArch_ARMv8         0x000000f
#define kyArch_ARMv7         0x0000007
#define kyArch_ARMv6         0x0000003
#define kyArch_ARMv5         0x0000001
#define kyArch_X86v6         0x000000f
#define kyArch_X86v5         0x0000007
#define kyArch_X86v4         0x0000003
#define kyArch_X86v3         0x0000001

// compiler
#define kyCompiler_MSVC   0x20  // Microsoft Visual C/C++
#define kyCompiler_GNUC   0x21  // GNU gcc/g++ (Cygwin,MinGW)
#define kyCompiler_WINSCW 0x22  // Microsoft Visual C/C++
#define kyCompiler_GCCE   0x23  // GNU Compiler Collection for Embedded
#define kyCompiler_CLANG  0x24  // Apple clang

// language
#define kyLanguageC89 198901L // c89
#define kyLanguageC95 199409L // c95
#define kyLanguageC99 199901L // c99
#define kyLanguageC11 201112L // c11
#define kyLanguage98 200303L// c++98/c++03
#define kyLanguage11 201103L// c++11
#define kyLanguage14 201402L// c++14

// assembly
#define kyAssemblyAT_T  1// AT&T
#define kyAssemblyIntel 2// intel

#if defined(__ORDER_BIG_ENDIAN__)
#  define kyBigEndian __ORDER_BIG_ENDIAN__
#else
#  define kyBigEndian 4321
#endif

#if defined(__ORDER_LITTLE_ENDIAN__)
#  define kyLittleEndian __ORDER_LITTLE_ENDIAN__
#else
#  define kyLittleEndian 1234
#endif


#ifndef kyArchEndian
// returns endian-ness of current CPU
static inline int _ArchEndian_()
{
    // returns endian-ness of cpu.
    union {
        int i;
        unsigned char b[sizeof(int)];
    } u;
    u.i = 1;
    return (u.b[0] == 1) ? kyLittleEndian : kyBigEndian;
}
//__BYTE_ORDER__
#define kyArchEndian _ArchEndian_()
#endif

//! 系统平台
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define kyPlatform kyPlatform_Win32
#elif defined( __APPLE_CC__)
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || \
       __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define kyPlatform kyPlatform_Ios
#   else
#       define kyPlatform kyPlatform_Apple
#   endif
#elif defined(__ANDROID__) || defined(ANDROID)
#	define kyPlatform kyPlatform_Android
#elif defined(__linux__) || defined(__linux)
#   define kyPlatform kyPlatform_Linux
#else
#   define kyPlatform kyPlatform_Unix
#endif

//! 体系建构
#if defined(__x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__ia64) || defined(__ia64__)
#  define kyByteOrder kyLittleEndian
#   if defined(__amd64__) || defined(__x86_64__) || defined(__x86_64)
#       define kyArchitecture  (kyArchBit_64 | kyArch_AMD64)
#   elif defined(__ia64__) || defined(__ia64)
#       define kyArchitecture  (kyArchBit_64 | kyArch_IA64)
#   else
#       define kyArchitecture  (kyArchBit_64 | kyArch_X86)
#   endif

#elif  defined(__i386__) || defined(_M_IX86)
#  define kyByteOrder kyLittleEndian
#  if defined(_M_IX86)
#    define _X86_VERSION_   (_M_IX86/100)
#  elif defined(__i686__) || defined(__athlon__) || defined(__SSE__) || defined(__pentiumpro__)
#    define _X86_VERSION_     kyArch_X86v6
#  elif defined(__i586__) || defined(__k6__) || defined(__pentium__)
#    define _X86_VERSION_     kyArch_X86v5
#  elif defined(__i486__) || defined(__80486__)
#    define _X86_VERSION_     kyArch_X86v4
#  else
#    define _X86_VERSION_     kyArch_X86v3
#  endif
#   define kyArchitecture  (kyArchBit_32 | kyArch_X86 | _X86_VERSION_)

#elif defined(__powerpc64__)

#   define kyArchitecture  (kyArchBit_64 | kyArch_PowerPC)

#elif defined(__arm__) || defined(__TARGET_ARCH_ARM) || defined(_M_ARM) || defined(__aarch64__)

#  if defined(__ARM_ARCH) && __ARM_ARCH > 1
#    define _ARM_VERSION_NUM_ __ARM_ARCH
#  elif defined(__TARGET_ARCH_ARM) && __TARGET_ARCH_ARM > 1
#    define _ARM_VERSION_NUM_ __TARGET_ARCH_ARM
#  elif defined(_M_ARM) && _M_ARM > 1
#    define _ARM_VERSION_NUM_ _M_ARM
#  elif defined(__ARM64_ARCH_8__) || defined(__aarch64__)
#    define _ARM_VERSION_NUM_ 8
#  elif defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7R__) ||
        defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__) || defined(_ARM_ARCH_7)
#    define _ARM_VERSION_NUM_ 7
#  elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6T2__) ||
        defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6ZK__) ||
        defined(__ARM_ARCH_6M__)
#    define _ARM_VERSION_NUM_ 6
#  elif defined(__ARM_ARCH_5TEJ__) || defined(__ARM_ARCH_5TE__)
#    define _ARM_VERSION_NUM_ 5
#  else
#    define _ARM_VERSION_NUM_ 0
#  endif

#  if _ARM_VERSION_NUM_ >= 8
#    define _ARM_VERSION_ kyArch_ARMv8
#  elif _ARM_VERSION_NUM_ >= 7
#    define _ARM_VERSION_ kyArch_ARMv7
#  elif _ARM_VERSION_NUM_ >= 6
#    define _ARM_VERSION_ kyArch_ARMv6
#  elif _ARM_VERSION_NUM_ >= 5
#    define _ARM_VERSION_ kyArch_ARMv5
#  else
#    define __PRINT_MACRO(X) #X
#    define _PRINTF_MACRO(X) #X "=" __PRINT_MACRO(X)
#    pragma message(_PRINTF_MACRO(_ARM_VERSION_NUM_))
#    error "ARM architecture too old"
#    define _ARM_VERSION_ 0
#  endif

#   if defined(__aarch64__)
#      define kyArchitecture  (kyArchBit_64 | kyArch_ARM | _ARM_VERSION_)
#   else
#      define kyArchitecture  (kyArchBit_32 | kyArch_ARM | _ARM_VERSION_)
#   endif

#  if defined(__ARMEL__)
#    define kyByteOrder kyLittleEndian
#  elif defined(__ARMEB__)
#    define kyByteOrder kyBigEndian
#  endif

#else

#   pragma error "No architecture. Abort! "

#endif

//! 编译器
#if defined( __GCCE__ )
#   define kyCompiler kyCompiler_GCCE
#   define kyCompilerVersion _MSC_VER
#   define kyAssemblyMode kyAssemblyAT_T
#elif defined( __WINSCW__ )
#   define kyCompiler kyCompiler_WINSCW
#   define kyCompilerVersion _MSC_VER
#   define kyAssemblyMode kyAssemblyIntel
#elif defined( _MSC_VER )
#   define kyCompiler kyCompiler_MSVC
#   define kyCompilerVersion _MSC_VER
#   define kyAssemblyMode kyAssemblyIntel
#elif defined( __clang__ )
#   define kyCompiler kyCompiler_CLANG
#   define kyCompilerVersion (((__clang_major__)*100) +(__clang_minor__*10) +__clang_patchlevel__)
#   define kyAssemblyMode kyAssemblyAT_T
#elif defined( __GNUC__ )
#   define kyCompiler kyCompiler_GNUC
#   define kyCompilerVersion (((__GNUC__)*100) +(__GNUC_MINOR__*10) +__GNUC_PATCHLEVEL__)
#   define kyAssemblyMode kyAssemblyAT_T
#else
#   pragma error "No compiler. Abort! "
#endif

//! 定义明确当前系统的特有宏
#define kyPlatformEqual(PF) (kyPlatform == kyPlatform_##PF)
#define kyArchitectureTure(AB) ((kyArchitecture & kyArch_##AB) == kyArch_##AB)
#define kyArchBitTure(AB) ((kyArchitecture & kyArchBit_##AB) == kyArchBit_##AB)
#define kyLanguageEqual(LV) (kyLanguage == kyLanguage_##LV)
#define kyLanguageCEqual(LV) (kyLanguageC == kyLanguageC_##LV)
#define kyCompilerEqual(CC) (kyCompiler == kyCompiler_##CC)

#define kyArchIs32Bit() (kyArchBitTure(32))
#define kyArchIs64Bit() (kyArchBitTure(64))
#define kyArchIsARM() (kyArchitectureTure(ARM))
#define kyArchIsX86() (kyArchitectureTure(X86))
#define kyArchIsAMD64() (kyArchitectureTure(AMD64))
#define kyArchIsIA64() (kyArchitectureTure(IA64))
#define kyPlatformIsWin32() (kyPlatformEqual(Win32))
#define kyPlatformIsLinux() (kyPlatformEqual(Linux))
#define kyPlatformIsAndroid() (kyPlatformEqual(Android))
#define kyPlatformIsIos() (kyPlatformEqual(Ios))
#define kyPlatformIsApple() (kyPlatformEqual(Apple))
#define kyPlatformIsUnix() (kyPlatformEqual(Unix))
#define kyCompilerIsGNUC() (kyCompilerEqual(GNUC))
#define kyCompilerIsCLANG() (kyCompilerEqual(CLANG))
#define kyCompilerIsGCCE() (kyCompilerEqual(GCCE))
#define kyCompilerIsWINSCW() (kyCompilerEqual(WINSCW))
#define kyCompilerIsMSVC() (kyCompilerEqual(MSVC))


//! C语言版本
#if (kyPlatformIsUnix() || kyPlatformIsLinux())

#   if __USE_ISOC11
#    define kyLanguageC kyLanguageC11
#   elif __USE_ISOC99
#    define kyLanguageC kyLanguageC99
#   elif __USE_ISOC95
#    define kyLanguageC kyLanguageC95
#   else
#    define kyLanguageC kyLanguageC89
#   endif

#elif defined(__STDC_VERSION__)
#   if __STDC_VERSION__ >= kyLanguageC11
#     define kyLanguageC kyLanguageC11
#   elif __STDC_VERSION__ >= kyLanguageC99
#     define kyLanguageC kyLanguageC99
#   elif __STDC_VERSION__ >= kyLanguageC95
#     define kyLanguageC kyLanguageC95
#   else
#    define kyLanguageC kyLanguageC89
#   endif

#elif defined (__STDC__)
#  define kyLanguageC kyLanguageC89
#else
#  pragma error "Compiler not support C language. Abort! "
#endif

//! C++语言版本
#ifdef __cplusplus
#  include <cstddef>

#  if __cplusplus >= kyLanguage14
#    define kyLanguage kyLanguage14
#  elif __cplusplus >= kyLanguage11
#    define kyLanguage kyLanguage11
#  elif __cplusplus >= kyLanguage98
#    define kyLanguage kyLanguage98
#else
#  pragma error "Compiler support C++ version unknown. Abort! "
#endif

#else
#  pragma error "Compiler not support C++ language. Abort! "
#endif // end C++


// Windows Settings
#if kyPlatformIsWin32()
#	if defined( kyStaticLIB )
#   	define kyExport
#   elif defined( kyExportDll )
#       if kyCompilerIsGNUC()
#           if kyCompilerVersion >= (4*100)
#               define kyExport __attribute__ ((visibility("default")))
#           else
#               define kyExport __attribute__((dllexport))
#           endif
#       elif kyCompilerIsMSVC()
#           define kyExport __declspec( dllexport )
#       endif
#   else
#       if kyCompilerIsGNUC()
#           if kyCompilerVersion >= (4*100)
#               define kyExport
#           else
#               define kyExport __attribute__((dllimport))
#           endif
#       else
#           if kyCompilerIsMSVC()
#               define kyExport __declspec( dllimport )
#           endif
#       endif
#	endif

#elif kyPlatformIsUnix() || kyPlatformIsLinux() || \
      kyPlatformIsApple() || kyPlatformIsIos() ||  \
      kyPlatformIsAndroid()

#   if kyCompilerIsGNUC()
#       if kyCompilerVersion >= (4*100)
#           define kyExport  __attribute__ ((visibility("default")))
#       else
#           define kyExport
#       endif
#   elif kyCompilerIsCLANG()
#       define kyExport  __attribute__ ((visibility("default")))
#   endif

#endif

#ifndef kyExternC
#define kyExternC extern "C"
#endif

#ifndef kyNew
#define kyNew(t) new t
#endif

#ifndef kyDelete
#define kyDelete(oj) delete oj
#endif

#ifndef kyMalloc
#define kyMalloc(size) malloc(size)
#endif

#ifndef kyRealloc
#define kyRealloc(ptr, size) realloc(ptr, size)
#endif

#ifndef kyFree
#define kyFree(oj) free(oj)
#endif

#ifndef kyASSERT
#include <assert.h>
#define kyASSERT(cond, a) (assert(cond))
#endif

#if kyAssemblyMode == kyAssemblyAT_T
#ifndef kyInlineASM
#define kyInlineASM __asm__ __volatile__
#endif
#else
#ifndef kyInlineASM
#define kyInlineASM asm volatile
#endif
#endif

#define kyStrCat(S1, S2) S1 ## S2
#define kyStrCat2(S1, S2) kyStrCat(S1, S2)

// 定义平台的主入口函数
#if kyPlatformIsWin32()
// 解决windows系统下出现此宏版本太低
#  if _WIN32_WINNT < 0x0600
#    undef _WIN32_WINNT
#    define _WIN32_WINNT 0x0600
#  endif
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#define ky_main \
     ky_main(int argc, char **argv);\
static inline char *wideToMulti(int codePage, const wchar_t *aw) \
{\
    const int required = WideCharToMultiByte(codePage, 0, aw, -1, NULL, 0, NULL, NULL);\
    char *result = new char[required];\
    WideCharToMultiByte(codePage, 0, aw, -1, result, required, NULL, NULL);\
    return result;\
}\
extern "C" int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR , int )\
{\
    int argc;\
    wchar_t **argvW = CommandLineToArgvW(GetCommandLineW(), &argc);\
    if (!argvW)\
        return -1;\
    char **argv = kyNew (char *[argc]);\
    for (int i = 0; i < argc; ++i)\
        argv[i] = wideToMulti(CP_ACP, argvW[i]);\
    LocalFree(argvW);\
    const int exitCode = ky_main(argc, argv);\
    for (int i = 0; i < argc; ++i)\
        kyDelete ([] argv[i]);\
    kyDelete ([] argv);\
    return exitCode;\
} \
    int ky_main

#elif kyPlatformIsUnix() || kyPlatformIsLinux()

#include <stdio.h>
#include <stdlib.h>
#include <features.h>
#include <pthread.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/time.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <net/if.h>

#define ky_main  main

#elif kyPlatform == kyPlatform_Apple

#endif


#endif // KY_PLATFORM

