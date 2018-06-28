
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_ptr.h
 * @brief    智能指针和对象申请宏实现
 *       1.支持对象申请宏参数展开
 *       2.支持引用计数机制为原子操作
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.1.1
 * @date     2012/07/27
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/07/27 | 1.0.0.1   | kunyang  | 创建文件实现对象申请宏
 * 2014/09/10 | 1.0.1.1   | kunyang  | 修改对象申请宏并添加对象参数展开机制
 */
#ifndef ky_PTR
#define ky_PTR
#include "ky_object.h"


#ifdef  kyHasAutoPtr
#define kyAutoPtr(type) ky_ptrref<type>
#define kyAutoPtrMalloc(type, ...) ky_ptrref<type> (kyNew (type(__VA_ARGS__)))
#define kyAutoPtrFree(ptr) ptr.reset()
#define kyAutoPtrNull(type) ky_ptrref<type>()
#else
#define kyAutoPtr(type) type *
#define kyAutoPtrMalloc(type, ...) kyNew (type(__VA_ARGS__))
#define kyAutoPtrFree(ptr) kyDelete (ptr)
#define kyAutoPtrNull(type) NULL
#endif

#define ky_ptr(type) kyAutoPtr(type)
#define ky_malloc(type, ...) kyAutoPtrMalloc(type, __VA_ARGS__)
#define ky_null(type) kyAutoPtrNull(type)
#define ky_free(ptr) kyAutoPtrFree(ptr)


#endif // ky_PTR

