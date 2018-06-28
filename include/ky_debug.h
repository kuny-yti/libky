
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_debug.h
 * @brief    调试信息和日志打印基本类
 *       1.调试信息的输出
 *       2.全局日志功能打印
 *       3.加入日志和调试信息过滤配置
 *       4.统一所有程序的断言
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.2.0
 * @date     2012/04/01
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/04/01 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/04/05 | 1.0.1.0   | kunyang  | 修改调试输出为标准输出
 * 2014/03/10 | 1.0.2.0   | kunyang  | 加入日志打印方式
 * 2014/04/06 | 1.1.0.1   | kunyang  | 修改日志模式为统一方式实现
 * 2015/06/06 | 1.1.2.0   | kunyang  | 修改日志打印函数并加入日志等级过滤
 *
 */
#ifndef KY_DEBUG_H
#define KY_DEBUG_H
#include "ky_define.h"
#include "tools/ky_stream.h"
#include "tools/ky_string.h"

#if defined(kyHasLog) || defined(kyHasDebug)
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>

typedef enum log_level {
    Log_Fatal = 0,   ///< 致命
    Log_Alert = 1,   ///< 警报
    Log_Critical = 2,///< 危急
    Log_Error = 3,   ///< 错误
    Log_Warning = 4, ///< 警告
    Log_Notice = 5,  ///< 通知
    Log_Info = 6,    ///< 信息
    Log_Debug = 7,   ///< 调试
    Log_Count
}eLogLevels;
namespace impl {struct log_core;}
class ky_debug
{
public:
    struct filter
    {
        char file[128];
        int line;
        char func[128];
        char subs[128];
    };
    struct config{int level[Log_Count];};

public:
    ky_debug(const char* fn = NULL, bool clr = true);
    ~ky_debug();

    // config set
    void set(const config &cfg);

    // filter operate
    int append(const filter &flt);
    void remove(int h);
    void clear();

    static bool relog(const char* fn = NULL, bool clr = true);

public:
    __attribute__((format(printf, 7, 8)))
    static void formats(const char *file, int line, const char *func,
                        const config *config,
                        const char *subs, eLogLevels level, const char *format,
                        ...);

protected:
    __attribute__((format(printf, 7, 0)))
    static void submit(const char *file, int line, const char *func,
                       const ky_debug::config *config,
                       const char *subs, eLogLevels level,
                       const char *format, va_list args);

private:
    static impl::log_core *impl;
    friend struct impl::log_core;
public:
    static config cfg;
};

#define kyLogConfigAll(debug, info, notice, warning, error, critical, alert, fatal) \
    { (fatal), (alert),(critical),(error),(warning),(notice),(info),(debug), }
#define kyLogConfigDebug(debug) \
    kyLogConfigAll((debug), 2, 2, 2, 2, 2, 2, 2)
#define kyLogConfigInfo(debug, info) \
    kyLogConfigAll((debug), (info), 2, 2, 2, 2, 2, 2)
#define kyLogConfigWarning(debug, info, notice, warning) \
    kyLogConfigAll((debug), (info), (notice), (warning), 2, 2, 2, 2)

#define kyLogSubSystem NULL
#define kyLogDefaultBase __FILE__, __LINE__, __func__
#define kyLogDefaultCfg kyLogDefaultBase, &ky_debug::cfg
#define kyLogDefault kyLogDefaultCfg, kyLogSubSystem

#define ky_log_printf(level, format, ...) \
    ky_debug::formats(kyLogDefault, (level), (format), ##__VA_ARGS__)

#ifdef kyHasDebug
    #define log_debug(format, ...) \
        ky_log_printf(Log_Debug, (format), ##__VA_ARGS__)
#else
static inline __attribute__((format(printf, 2, 3)))
void ky_log_dummyf(unsigned int , const char *, ...){}
#define log_debug(format, ...) \
    ky_log_dummyf(Log_Debug, (format), ##__VA_ARGS__)
#endif

#define log_info(format, ...) \
    ky_log_printf(Log_Info, (format), ##__VA_ARGS__)
#define log_notice(format, ...) \
    ky_log_printf(Log_Notice, (format), ##__VA_ARGS__)
#define log_warning(format, ...) \
    ky_log_printf(Log_Warning, (format), ##__VA_ARGS__)
#define log_error(format, ...) \
    ky_log_printf(Log_Error, (format), ##__VA_ARGS__)
#define log_critical(format, ...) \
    ky_log_printf(Log_Critical, (format), ##__VA_ARGS__)
#define log_alert(format, ...) \
    ky_log_printf(Log_Alert, (format), ##__VA_ARGS__)
#define log_fatal(format, ...) \
    ky_log_printf(Log_Fatal, (format), ##__VA_ARGS__)

#else // no logs
struct ky_debug{};
#define log_info(format, ...)
#define log_notice(format, ...)
#define log_warning(format, ...)
#define log_error(format, ...)
#define log_critical(format, ...)
#define log_alert(format, ...)
#define log_fatal(format, ...)

#endif

#define log_dbg log_debug
#define log_warn log_warning
#define log_err log_error
#define log_crit log_critical

#ifdef kyHasAssert
#ifdef kyASSERT
#undef kyASSERT
#endif
#    define kyASSERT(val, str) \
           if (!!(val)) {log_fatal("assert[%s]", str);abort ();}
#else
#  define kyASSERT(val, str)
#endif


#endif
