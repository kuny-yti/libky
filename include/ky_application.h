
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_application.h
 * @brief    应用程序的核心类
 *       1.单例模式存在.
 *       2.管理所有安装包以及安装包内的模块扩展创建
 *       3.类内会存储应用程序启动时命令行的所有参数
 *       4.应用程序的组织信息、程序信息获取和设置
 *       5.全局日志和调试信息的设置
 *       6.返回应用程序当前的进程id
 *       7.全局事件的派遣以及事件的过滤设置
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.0.1
 * @date     2013/03/04
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2013/03/04 | 1.0.0.1   | kunyang  | 创建文件，并解析程序参数
 * 2013/04/10 | 1.0.1.0   | kunyang  | 加入组织信息和程序信息
 * 2013/08/07 | 1.0.2.0   | kunyang  | 加入日志和调试信息
 * 2014/06/21 | 1.0.5.0   | kunyang  | 修改日志支持多线程及配置方法
 * 2015/08/20 | 1.0.8.0   | kunyang  | 加入全局事件派遣及修改日志终端打印方法
 * 2015/10/10 | 1.1.0.1   | kunyang  | 修改全局事件派遣加入事件派遣的过滤
 */
#ifndef ky_APPLICATION_H
#define ky_APPLICATION_H

#include "ky_utils.h"
#include "ky_object.h"
#include "ky_debug.h"
#include "tools/ky_string.h"
#include "interface/ipackage.h"
#include "interface/ievent.h"

#define app() ky_application::instance ()

const char * const pkg_subdir = "package";

namespace impl {
struct application;
}
class ky_application : public ky_object, public ky_singleton<ky_application>
{
private:
    friend class ky_singleton<ky_application>;
    ky_application();

public:
    virtual ~ky_application();

    void enter(int argc, char **argv);
    //!
    //! \brief exit 退出程序
    //! \param code 退出返回代码
    //!
    void exit(_in int code = 0);

    //!
    //! \brief quit 安全退出
    //!
    void quit();
    //!
    //! \brief run 执行应用程序
    //! \return
    //!
    int run()const;

    //!
    //! \brief parameters 返回所有程序的参数(未经处理)
    //! \return
    //!
    ky_string_list parameters();
    //!
    //! \brief parameter 根据key返回一个参数
    //! \param key
    //! \return
    //!
    ky_string parameter (_in ky_string _ref key);

    // 组织信息
    //!
    //! \brief org_name 返回组织的名称
    //! \return
    //!
    ky_string org_name() const;
    //!
    //! \brief org_info 返回组织的相关信息
    //! \return
    //!
    ky_string org_info() const;
    //!
    //! \brief org_name 设置组织名称
    //! \param on
    //!
    void org_name (_in ky_string _ref on);
    //!
    //! \brief org_info 设置组织信息
    //! \param oi
    //!
    void org_info (_in ky_string _ref oi);

    // 程序信息
    //!
    //! \brief app_name 返回程序的名称
    //! \return
    //!
    ky_string app_name()const;
    //!
    //! \brief app_version 返回程序的版本
    //! \return
    //!
    ky_string app_version()const;
    //!
    //! \brief app_path 返回程序所在的相对路径
    //! \return
    //!
    ky_string app_path() const;
    //!
    //! \brief app_paths  返回程序所在的绝对路径
    //! \return
    //!
    ky_string app_paths() const;
    //!
    //! \brief pid 返回程序的进程id
    //! \return
    //!
    process_id pid() const;

    //!
    //! \brief log_path 返回程序日志路径
    //! \return
    //!
    ky_string log_path()const;
    //!
    //! \brief log_paths 返回程序日志绝对路径
    //! \return
    //!
    ky_string log_paths()const;
    //!
    //! \brief log_name 返回程序日志名称
    //! \return
    //!
    ky_string log_name()const;

    //!
    //! \brief log_path 设置程序日志路径
    //! \param lp
    //!
    void log_path (_in ky_string _ref lp);
    //!
    //! \brief log_name 设置程序日志名称
    //! \param lp
    //!
    void log_name (_in ky_string _ref ln);
    //!
    //! \brief log_name 设置程序日志的配置
    //! \param cfg
    //!
    void log_config(const ky_debug::config &cfg);

    //!
    //! \brief pkg_path 设置安装包路径
    //! \param pp
    //!
    void pkg_path (_in ky_string _ref pp);
    //!
    //! \brief pkg_path  返回安装包所在的路径
    //! \return
    //!
    ky_string pkg_path ()_fix;

    //!
    //! \brief create 创建扩展模块
    //! \param id
    //! \return
    //!
    pimodule create(_in ky_uuid _ref id )_fix;
    //!
    //! \brief install  安装包加载后将对象装入系统
    //! \return
    //!
    void install(_in pipackage _ref ii);
    //!
    //! \brief install  根据安装包id卸载安装包
    //! \return
    //!
    void uninstall(_in pipackage _ref ii);

public:
    //!
    //! \brief dispatch  调度系统的总事件接口
    //! \param evt
    //! \param hd
    //!
    void evt_dispatch(const ievent *evt, const intptr hd);
    //!
    //! \brief evt_append 添加需要调度的事件
    //! \param hd 事件的句柄
    //!
    void evt_append(const intptr hd);
    //!
    //! \brief evt_remove 删除需要调度的事件
    //! \param hd
    //!
    void evt_remove(const intptr hd);

    //!
    //! \brief exec
    //! \param e
    //!
    Signal<void(int &code)> exec;
    //!
    //! \brief posting  寄送一个事件信号
    //!                 需要接收到系统的事件则连接此信号
    //! \param e
    //! \param hd
    //!
    Signal<void(const ievent *e, const intptr hd)> posting;
    //!
    //! \brief filter  过滤事件信号。
    //! \param e    需要匹配的事件。
    //! \param then 返回true则过滤掉此事件
    //!
    Signal<void(const ievent *e, bool &then)> filter;

private:
    impl::application * impl;
};

#endif
