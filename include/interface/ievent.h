
/**
 * Universal interface header
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ievent.h
 * @brief    事件基础接口
 *       1.触控板、触摸屏、键盘、鼠标、操纵杆的输入事件定义.
 *       2.系统窗口、系统分发、系统剪切板、系统拖拽的系统事件定义.
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.3.0.1
 * @date     2013/05/11
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2013/05/11 | 1.0.0.1   | kunyang  | 创建文件
 * 2013/06/19 | 1.0.1.0   | kunyang  | 键盘、鼠标、系统窗口事件定义
 * 2014/04/06 | 1.1.0.1   | kunyang  | 重构类将其分为实现部分和基础宏部分
 * 2014/09/22 | 1.1.1.0   | kunyang  | 增加系统事件分发和系统拖拽事件
 * 2016/11/20 | 1.2.0.1   | kunyang  | 修改占位标志，并增加操纵杆定义
 * 2017/03/10 | 1.3.0.1   | kunyang  | 增加触摸板和触控板定义，并修改字段宏
 */
#ifndef IEVENT_H
#define IEVENT_H

#include "ky_define.h"

/* 标志位宽 */
#define iFlagsWide 0x01
/* 事件定义宏 */
#define iEventDefine(val, field) (val << field)
/* 事件定义获取宏 */
#define iEventGetDef(val, field) (val >> field)
/* 系统事件标志位定义 */
#define iSystemFlags 31
/* 系统事件掩码段定义 */
#define iSystemMaskField (iFlagsWide << iSystemFlags)
/* 键盘事件标志位定义 */
#define iKeybordFlags 30
/* 键盘事件掩码段定义 */
#define iKeybordMaskField (iFlagsWide << iKeybordFlags)
/* 鼠标事件标志位定义 */
#define iMouseFlags 29
/* 鼠标事件掩码段定义 */
#define iMouseMaskField (iFlagsWide << iMouseFlags)
/* 窗口事件标志位定义 */
#define iWindowFlags 28
/* 窗口事件掩码段定义 */
#define iWindowMaskField (iFlagsWide << iWindowFlags)
/* 触摸屏事件标志位定义 */
#define iTouchScreenFlags 27
/* 触摸屏事件掩码段定义 */
#define iTouchScreenMaskField (iFlagsWide << iTouchScreenFlags)
/* 触控板事件标志位定义 */
#define iTouchpadFlags 26
/* 触控板事件掩码段定义 */
#define iTouchpadMaskField (iFlagsWide << iTouchpadFlags)
/* 操纵杆事件标志位定义 */
#define iJoystickFlags 25
/* 操纵杆事件掩码段定义 */
#define iJoystickMaskField (iFlagsWide << iJoystickFlags)
/* 剪切板事件标志位定义 */
#define iClipboardFlags 24
/* 剪切板事件掩码段定义 */
#define iClipboardMaskField (iFlagsWide << iClipboardFlags)
/* 拖放事件标志位定义 */
#define iDragDropFlags 23
/* 拖放事件掩码段定义 */
#define iDragDropMaskField (iFlagsWide << iDragDropFlags)

typedef enum
{
    Event_System = iEventDefine(1 , iSystemFlags),
    Event_Keybord = iEventDefine(1, iKeybordFlags),
    Event_Mouse = iEventDefine(1, iMouseFlags),
    Event_Window = iEventDefine(1, iWindowFlags),
    Event_TouchScreen = iEventDefine(1, iTouchScreenFlags),
    Event_Touchpad = iEventDefine(1, iTouchpadFlags),
    Event_Joystick = iEventDefine(1, iJoystickFlags),
    Event_Clipboard = iEventDefine(1, iClipboardFlags),
    Event_DragDrop = iEventDefine(1, iDragDropFlags)
}eEvents;

/* 系统事件定义宏 */
#define iSystemEventMacro(val) (Event_System | val)
/* 键盘事件定义宏 */
#define iKeybordEventMacro(val) (Event_Keybord | val)
/* 鼠标事件定义宏 */
#define iMouseEventMacro(val) (Event_Mouse | val)
/* 窗口事件定义宏 */
#define iWindowEventMacro(val) (Event_Window | val)
/* 触摸屏事件定义宏 */
#define iTouchScreenEventMacro(val) (Event_TouchScreen | val)
/* 触控板事件定义宏 */
#define iTouchpadEventMacro(val) (Event_Touchpad | val)
/* 操纵杆事件定义宏 */
#define iJoystickEventMacro(val) (Event_Joystick | val)
/* 剪切板事件定义宏 */
#define iClipboardEventMacro(val) (Event_Clipboard | val)
/* 拖放事件定义宏 */
#define iDragDropEventMacro(val) (Event_DragDrop | val)

/* 是否有evt事件 */
#define iEventIs(code, evt) (code & evt)
/* 是否匹配evt事件 */
#define iEventMatch(code, evt) (iEventIs(code, evt) == evt)

kyPackage ievent
{
    explicit ievent(int v=0):_code(v){}

    virtual ~ievent(){}

    int code()const{return _code;}

    int64 &timestamp() {return _tstamp;}
    int64 timestamp()const {return _tstamp;}

    bool is_system()const{return iEventIs(code(), Event_System);}
    bool is_keybord()const{return iEventIs(code(), Event_Keybord);}
    bool is_mouse()const{return iEventIs(code(), Event_Mouse);}
    bool is_window()const{return iEventIs(code(), Event_Window);}
    bool is_touchscreen()const{return iEventIs(code(), Event_TouchScreen);}
    bool is_touchpad()const{return iEventIs(code(), Event_Touchpad);}
    bool is_joystick()const{return iEventIs(code(), Event_Joystick);}
    bool is_clipboard()const{return iEventIs(code(), Event_Clipboard);}
    bool is_dragdrop()const{return iEventIs(code(), Event_DragDrop);}

    int _code;
    int64 _tstamp;
};

#endif // IEVENT_H
