
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_define.h
 * @brief    基础定义
 *       1.定义了使用的类型.
 *       2.ky_ref 引用计数器，对象间的赋值及使用的基本管理
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.0.1
 * @date     2012/01/02
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/01/02 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/01/10 | 1.0.1.0   | kunyang  | 加入引用的可共享属性
 */
#ifndef ky_REGEX_H
#define ky_REGEX_H

#include "ky_vector.h"
#include "ky_string.h"


namespace impl {
struct regex;
}

class ky_regex
{
public:
    explicit ky_regex(const ky_string &re);
    ~ky_regex();

    ky_regex(const ky_regex &) = delete;
    void operator = (const ky_regex &) = delete;

    const ky_string& regex() const;

    int match(const ky_string &str) const;
    int search(const ky_string &str, ky_string_list *reustl = 0) const;
    int split(const ky_string &str, ky_string_list *reustl = 0)const;
private:
    impl::regex *impl;
};



#endif // ky_REGEX_H
