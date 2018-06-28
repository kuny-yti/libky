
/**
 * Universal interface header
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ipackage.h
 * @brief    软件包和扩展模块接口
 *       1.使用包的概念设计所有扩展功能以及插件机制。
 *       2.一个包可支持多个扩展模块，通过uuid来进行创建并根据id使用对象
 *       3.实现模块中可插入用户界面接口功能
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.1.0.1
 * @date     2014/09/07
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2014/09/07 | 1.0.0.1   | kunyang  | 创建文件
 * 2016/04/10 | 1.0.1.0   | kunyang  | 修改包类设计，将信息部分剥离出一个信息类
 * 2017/06/21 | 1.1.0.1   | kunyang  | 增加扩展模块接口，修改一个包中支持多个扩展模块
 */
#ifndef KY_PACKAGE_H
#define KY_PACKAGE_H

#include "ky_define.h"
#include "ky_ptr.h"
#include "tools/ky_string.h"
#include "tools/ky_vector.h"
#include "tools/ky_typeinfo.h"
#include "tools/ky_map.h"
#include "tools/ky_uuid.h"

//! 用于包的启用于卸载
typedef intptr (*package_begin)(void);
typedef void (*package_end)(void);

//! package_info
//! 包信息类
//! 包括：
//! 1.包id(此id是用包的指针地址)
//! 2.包名称
//! 3.包版本
//! 4.包开发商
//! 5.包版权
//! 6.包的许可证
//! 7.包的描述
//! 8.包的依赖(依赖那个包，写上包的名称，有多个可用";"分开,有多依赖则需要在前)
kyPackage package_info
{
    intptr pkg_ids;

    virtual ~package_info(){}
    virtual ky_string name()const {return ky_typeinfo<package_info>();}
    virtual intptr id() const{return pkg_ids;}
    virtual ky_string version() const = 0;
    virtual ky_string vendor() const  = 0;
    virtual ky_string copyright()  const = 0;
    virtual ky_string license() const  = 0;
    virtual ky_string description() const = 0;
    virtual ky_string dependencies() const= 0;

    ky_string dump()const
    {
        ky_string info("Package Info:\n");
        info += "    name: ";
        info += name();
        info += "\n";

        info += "    version: ";
        info += version();
        info += "\n";

        info += "    vendor: ";
        info += vendor();
        info += "\n";

        info += "    copyright: ";
        info += copyright();
        info += "\n";

        info += "    license: ";
        info += license();
        info += "\n";

        info += "    description: ";
        info += description();
        info += "\n";

        info += "    dependencies: ";
        info += dependencies();
        info += "\n";
        return info;
    }
};

#ifndef IModule
#define IMD ky_ptr(kyPackage imodule)
typedef IMD pimodule;
#define IModule
#undef IMD
#endif

//! ipackage
//! 包基类
//! 包括：
//! 1.包的安装
//! 2.包的卸载
//! 3.包的类型
kyPackage ipackage :public package_info
{
    virtual ~ipackage() {}
    /**
      * @brief install 安装
      * @return 默认安装的模块id(此id为对象指针地址)
      */
    virtual intptr install() = 0;
    /**
      * @brief uninstall 卸载
      * @return
      */
    virtual void uninstall() = 0;

   /**
     * @brief has 是否支持uuid模块
     * @return
     */
    virtual bool has(const ky_uuid &uuid) = 0;
   /**
     * @brief create 根据支持的uuid模块创建一个模块
     * @return
     */
    virtual pimodule create(const ky_uuid &uuid) = 0;

    virtual ky_string name() const{return ky_typeinfo<ipackage>();}
};

#ifndef IPackage
#define IPK ky_ptr(ipackage)
typedef IPK pipackage;
#define IPackage
#undef IPK
#endif

//! 扩展后包括基本信息和可视化UI接口定义
kyPackage imodule
{
    intptr  mod_ids;
    virtual ~imodule(){}

    virtual intptr    id()const{return mod_ids;}
    virtual ky_uuid   uuid()const {return ky_uuid();}
    virtual ky_string name() const{return ky_typeinfo<imodule>();}
};

///< MVSS
#define kyModProduct ('M' << 8 | 'V')
///< Yantu kunyang
#define kyModVendor ('Y' << 24 | 'T' << 16 | 'K' << 8 | 'Y')
///< 1.0.0.1
const ushort kyModVersion = (1 << 12 | 0 << 8 | 0 << 4 | 1);
#define kyModUUID(type, d1, d2, d3, d4, d5, d6, d7, d8) \
    const ky_uuid uuid_##type = {kyModVendor, kyModProduct, kyModVersion, {d1, d2, d3, d4, d5, d6, d7, d8}}

#define kyPackageBegin(type) \
    static pipackage _pkg_##type##_ = pipackage(); \
    kyExternC intptr kyExport PackageBegin(void)\
    {\
        if (_pkg_##type##_.get() == 0) \
        { \
            ky_ptr(type) tptr = ky_malloc (type);\
            tptr->pkg_ids = (intptr)tptr.get(); \
            _pkg_##type##_ = tptr.as<type>();\
            app()->install(tptr); \
        } \
        return (intptr)_pkg_##type##_->pkg_ids; \
    }

#define kyPackageEnd(type) \
    kyExternC  void kyExport PackageEnd(void)\
    {\
        app()->uninstall(_pkg_##type##_); \
        _pkg_##type##_ = pipackage(); \
    }

#define kyPackageFunc(type) \
    kyPackageBegin(type)\
    kyPackageEnd(type)

#define kyPackageBeginFunc "PackageBegin"
#define kyPackageEndFunc "PackageEnd"

#endif

