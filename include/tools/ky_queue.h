
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
#ifndef ky_QUEUE
#define ky_QUEUE
#include <queue>
#include "ky_list.h"

template <typename T>
class ky_queue
{
public:
    ky_queue() :sequence() {}
    ky_queue(const ky_queue &v) :sequence(v.sequence) {}
    ky_queue(const std::queue<T> &v) :sequence() {form(v);}

    std::queue<T> &std()
    {
        return sequence.to_std ();
    }
    void form (const std::queue<T> &v)
    {

    }

    T& front(){return sequence.front ();}
    const T& front()const{return sequence.front ();}
    T& back(){return sequence.back ();}
    const T& back()const{return sequence.back ();}

    void push(const T& val) {sequence.push_back (val);}
#if kyLanguage >= kyLanguage11
    void push(T && v){sequence.push_back (v);}
#endif

    void remove() {sequence.pop_front ();}
    void clear() {sequence.clear ();}
    T pop() {T out = sequence.front (); remove (); return out;}
    size_t size()const {return sequence.size ();}
    size_t count()const {return size();}

    bool empty()const {return sequence.empty ();}
    bool is_empty()const {return empty();}

protected:
    ky_list<T> sequence;
};

#endif // ky_QUEUE

