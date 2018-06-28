
/**
 * Universal interface header
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     inative.h
 * @brief    本地系统窗口接口
 *       1.支持多窗口渲染
 *       2.支持多窗口共享
 *       3.支持窗口分离到线程渲染
 *       4.支持窗口在主进程渲染
 *       5.支持事件触发渲染
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.4.1.1
 * @date     2012/09/26
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/09/26 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/09/27 | 1.0.1.0   | kunyang  | 修改事件回调函数
 * 2014/03/03 | 1.0.2.0   | kunyang  | 修改和GL初始化时上下文丢失产生的错误
 * 2015/02/15 | 1.1.0.1   | kunyang  | 增加系统事件的分类并实现窗口支持事件功能
 * 2015/05/06 | 1.1.1.1   | kunyang  | 修改事件定义
 * 2015/06/11 | 1.2.0.1   | kunyang  | 增加窗口样式定义和实现
 * 2016/01/22 | 1.2.1.1   | kunyang  | 修改窗口调整不分发窗口调整事件
 * 2016/04/12 | 1.2.1.2   | kunyang  | 修改调整窗口尺寸方法
 * 2016/11/01 | 1.3.0.1   | kunyang  | 修改剪切板支持，增加渲染、事件的信号对象
 * 2017/04/26 | 1.3.1.1   | kunyang  | 修改事件发送和渲染同时触发死循环
 * 2017/06/07 | 1.3.2.1   | kunyang  | 修改默认开启剪切板
 * 2017/09/13 | 1.4.0.1   | kunyang  | 修改事件的处理机制并添加光标接口
 * 2017/09/29 | 1.4.1.1   | kunyang  | 修改exec执行后在多窗口中无法轮循事件
 */
#ifndef KY_NATIVE_H
#define KY_NATIVE_H

#include "ky_ptr.h"
#include "ipackage.h"
#include "ievent.h"
#include "icursor.h"
#include "maths/ky_vec4.h"

#ifndef iNative
#define PPF ky_ptr(kyPackage inative)
typedef PPF pinative;
kyModUUID(native, 'n', 'a', 't', 'i', 'v', 'e', 's', ' ');
#define iNative
#undef PPF
#endif

#ifndef iRContext
#define PPF ky_ptr(kyPackage ircontext)
typedef PPF pircontext;
kyModUUID(context, 'c', 'o', 'n', 't', 'e', 'x', 't', 's');
#define iRContext
#undef PPF
#endif

#ifndef iGraph
kyPackage igraph;
#define PPF ky_ptr(igraph)
typedef PPF pigraph;
kyModUUID(graph, 'g', 'r', 'a', 'p', 'h', 'i', 'c', 's');
#define iGraph
#undef PPF
#endif


/*!
 * @brief The eNativeEvents enum 窗口的事件
 * @enum eNativeEvents
 */
typedef enum
{
    Native_Input = iWindowEventMacro(0),      //!< 输入
    Native_Initialize = iWindowEventMacro(1), //!< 初始化
    Native_Paint = iWindowEventMacro(2),      //!< 绘制
    Native_Resize = iWindowEventMacro(4),     //!< 窗口尺寸更新
    Native_Exit = iWindowEventMacro(8),       //!< 退出

    Native_EventNot
}eNativeEvents;

/*!
 * @brief The eNativeStyles enum 窗口的装饰样式
 * @enum eNativeStyles
 */
typedef enum
{
    Native_Rimless = 1 << 3,             //!< 窗口无边框
    Native_FullScreen = 1 << 4,          //!< 窗口全屏
    Native_Resizable = 1 << 5,           //!< 窗口可调整
    Native_AlwaysTop = 1 << 6,           //!< 窗口置顶
    Native_MultiThread = 1 << 7,         //!< 将窗口分离到线程中运行，事件触发机制,默认为主进程运行
    Native_AlwaysRender = 1 << 8,        //!< 窗口一直渲染，不等待事件触发机制
    Native_HideCursor = 1 << 9,
    Native_StyleNot
}eNativeStyles;

/*!
 * @brief The eNativeEvents enum 窗口可使用的事件
 * @enum eNativeEvents
 */
typedef enum
{
    Native_InputOnly  = 1 ,      //!< 支持窗口事件,其他事件都不能用
    Native_Mouse      = 1 << 1,  //!< 支持鼠标事件
    Native_Keybord    = 1 << 2,  //!< 支持键盘事件
    Native_DragDrop   = 1 << 3,  //!< 支持拖放事件
    Native_Clipboard  = 1 << 4,  //!< 支持剪切板事件
    Native_MouseTrack = 1 << 5,  //!< 支持鼠标跟踪
    Native_Cursor     = 1 << 6,  //!< 支持系统光标
    Native_InputMethod= 1 << 7 | Native_Keybord,  //!< 支持键盘输入法事件
    Native_InputOut   = 1 << 8 | Native_Clipboard | Native_DragDrop |
                                 Native_Keybord | Native_Mouse | Native_MouseTrack |
                                 Native_Cursor,
    Native_SupportEventNot
}eNativeSupportEvents;

struct tNativeSettings
{
    // window
    //inative      *    parent;      ///< 父窗口
    ivec2             minisize;    ///< 窗口可调整最小尺寸
    ivec2             maxisize;    ///< 窗口可调整最大尺寸
    vec2              minaspect;   ///< 窗口调整的最小比例
    vec2              maxaspect;   ///< 窗口调整的最大比例
    ky_string         title;       ///< 窗口的标题
    int               style;       ///< 窗口的装饰样式
    int               support;     ///< 窗口可使用的事件
};

/*!
 * @brief The inative interface Screen the show native window 显示设备屏幕的显示窗口
 * @interface inative
 * Implement
 *
 * 窗口支持：
 * 1.支持多窗口渲染
 * 2.支持多窗口共享
 * 3.支持窗口分离到线程渲染
 * 4.支持窗口在主进程渲染
 * 5.支持事件触发渲染
 * 窗口的机制：
 * 1.没有设置线程渲染也没有设置总是渲染
 *     主进程等待事件到来，当有事件时则分发到各自窗口内进行处理并渲染，之后等待下一次的事件到来。
 * 2.没有设置线程渲染则设置了总是渲染
 *     主进程轮循查找事件，当有事件则分发到各自窗口内进行处理。当有无事件时各自窗口都会渲染。
 * 3.设置了线程渲染没有设置总是渲染
 *     主进程等待事件到来，当事件到来则分发到各自窗口的事件队列中，有窗口线程处理事件，线程处理完事件并渲染，
 *   当没有事件时则线程进入等待，主进程等待下一次事件到来并通知线程进行处理。
 * 4.设置了线程渲染并设置了总是渲染
 *     主进程等待事件的到来，当有事件时则分发到各自窗口的队列中，线程处理事件后并一直渲染。

 */
struct inative : tNativeSettings, public ky_object /*, isurface*/
{
    kyObject(inative)
    explicit inative(ky_object *parent = 0):ky_object(parent){}
    virtual ~inative(){}

    inline tNativeSettings &setting(){return *this;}
    inline const tNativeSettings &setting()const{return *this;}
    inline inative & operator = (const tNativeSettings &nc)
    {
        //this->parent = nc.parent;
        this->minisize = nc.minisize;
        this->maxisize = nc.maxisize;
        this->minaspect = nc.minaspect;
        this->maxaspect = nc.maxaspect;
        this->title = nc.title;
        this->style = nc.style;
        this->support = nc.support;
        return *this;
    }

    //!
    //! \brief render 在系统窗口内的绘制
    //!
    Signal<void ()>              render;
    Signal<void (const isize&)>  resize;
    Signal<void (const ievent*, const intptr)> event;

    //!
    //! \brief update
    //!
    virtual void update() = 0;
    //!
    //! \brief exec 执行本地事件派遣,若不是在线程内渲染调用无效
    //! \param ret_code 返回退出码
    //!
    virtual void exec(int & ret_code) = 0;

    //!
    //! \brief setup 设置系统窗口
    //! \param rc 渲染上下文
    //! \return 返回true则设置成功，否则不成功
    //!
    virtual bool setup (const pircontext &rc) = 0;
    //!
    //! \brief config 设置本地配置
    //! \param cfg
    //!
    virtual void config(intptr cfg) = 0;
    //!
    //! \brief clipboard 设置本地剪切板内容
    //! \param txt
    //! \return
    //!
    virtual bool clipboard(const ky_string &txt) = 0;

    //!
    //! \brief exit 退出事件派遣
    //! \return
    //!
    virtual void exit() = 0;
    //!
    //! \brief capture 将窗口截图
    //! \return
    //!
    virtual ky_image capture() const= 0;

    //!
    //! \brief root 返回窗口的父句柄 [linux display, window root]
    //! \return
    //!
    virtual intptr root() const = 0;
    //!
    //! \brief hd 返回窗口的句柄
    //! \return
    //!
    virtual intptr hd() const = 0;

    //!
    //! \brief cursor 返回本地光標
    //! \return
    //!
    virtual picursor cursor()const = 0;

    //!
    //! \brief clipboard 返回本地是否支持剪切板
    //! \return
    //!
    virtual bool has_clipboard(){return support & Native_Clipboard;}
};


#endif // INATIVE_H
