
/**
 * Universal interface header
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     icursor.h
 * @brief    系统光标、鼠标接口
 *       1.实现基本的光标功能
 *       2.光标指针位置及显示功能
 *       3.支持自定义光标指针图标
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.0.1
 * @date     2014/07/18
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2014/07/18 | 1.0.0.1   | kunyang  | 创建文件
 */
#ifndef KY_CURSOR_H
#define KY_CURSOR_H
#include "maths/ky_vec2.h"
#include "tools/ky_image.h"
#include "ky_ptr.h"

typedef enum
{
    Cursor_IBeam = 0xE900,
    Cursor_Arrow = 0xE901,
    Cursor_Cross = 0xE902,
    Cursor_Hand = 0xE903,
    Cursor_HResize = 0xE904,
    Cursor_VResize = 0xE905,
    Cursor_Move = 0xE906,
    Cursor_Question = 0xE907,
    Cursor_Resize = 0xE908
}eCursorTypes;

#ifndef ICursor
#define ICUR ky_ptr(kyPackage icursor)
typedef ICUR picursor;
#define ICursor
#undef ICUR
#endif

kyPackage icursor
{
    eCursorTypes type;
    bool is_hide;

    icursor &operator = (const ipoint &pos){set(pos); return *this;}
    const ipoint &operator ()(){return get ();}
    const ipoint &pos() const {return get ();}

    virtual const ipoint &get() const = 0;
    virtual void set(const ipoint &pos) = 0;
    virtual void set(const ky_image &img) = 0;
    virtual void set(const eCursorTypes &t) {type = t;}
    virtual void hide(bool s = true) {is_hide = s;}
    virtual void show(bool s = true) {is_hide = !s;}
};

#endif // ICURSOR_H
