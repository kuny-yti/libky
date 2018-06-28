
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
#ifndef ky_STACK
#define ky_STACK
#include <stack>
#include "ky_list.h"

template <typename T>
class ky_stack
{
public:
    ky_stack(): sequence() {}
    ky_stack(const ky_stack<T> &s): sequence(s.sequence) {}
    ky_stack(const std::stack<T> &s):sequence() {form(s);}

    std::stack<T> &to_std(){return sequence.to_std ();}
    void form (const std::stack<T> &rhs)
    {

    }

    T &top(){return sequence.back ();}
    const T &top() const{return sequence.back ();}

    void push(const T& val){sequence.push_back (val);}
#if kyLanguage >= kyLanguage11
    void push(T && v){sequence.push_back (v);}
#endif
    T pop(){T out = sequence.back (); remove ();return out;}
    void remove(){sequence.pop_back();}
    void swap(ky_stack& s){ky_swap (sequence, s.sequence);}
    friend void ky_swap (ky_stack &a, ky_stack &b){a.swap (b);}

    size_t size() const{return sequence.size ();}
    size_t count() const {return size();}

    bool empty() const {return sequence.empty ();}
    bool is_empty() const{return empty ();}
protected:
    ky_list <T> sequence;
};

#endif // ky_STACK

