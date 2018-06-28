
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_fsys.h
 * @brief    文件系统访问类。
 *       1.可完成文件操作、目录操作、虚拟文件操作、文件内存映射、压缩文件操作、动态库操作.
 *       2.支持文件信息获取
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.2.2.1
 * @date     2012/02/01
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/02/01 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/02/02 | 1.0.0.2   | kunyang  | 实现windows下文件操作
 * 2012/04/18 | 1.0.1.0   | kunyang  | 实现文件信息获取
 * 2012/04/20 | 1.0.1.1   | kunyang  | 修改文件操作类兼容windiws和linux
 * 2012/05/02 | 1.0.2.0   | kunyang  | 实现动态库的操作
 * 2012/04/20 | 1.0.2.1   | kunyang  | 修改io操作基类并刷新基础实现
 * 2014/08/05 | 1.1.0.1   | kunyang  | 重新实现文件信息获取和io操作方法
 * 2014/11/13 | 1.2.0.1   | kunyang  | 将所有io操作方法封装为ky_fsys类中
 * 2015/04/10 | 1.2.1.1   | kunyang  | 将io操作类实现放入私有实现中
 * 2016/02/02 | 1.2.2.1   | kunyang  | 修改操作方法并加入归档文件的操作
 *
 */

#ifndef ky_fsys_H
#define ky_fsys_H
#include "interface/iio.h"
#include "tools/ky_datetime.h"

typedef enum
{
    Archive_Not,
    Archive_DeCompress,
    Archive_Compress,
    Archive_Create,
    Archive_Append,
    Archive_Add
}eArchiveModes;

typedef enum
{
    Type_File,
    Type_Dir,
    Type_Link
}eFileTypes;

typedef enum
{
    Permission_ReadOnly = 1,
    Permission_WriteOnly = 2,
    Permission_Excutable = 4,
    Permission_Hidden = 8,
    Permission_ReadWrite = Permission_ReadOnly | Permission_WriteOnly,
    Permission_ReadExcutable = Permission_ReadOnly | Permission_Excutable,
    Permission_WriteExcutable = Permission_WriteOnly | Permission_Excutable,
    Permission_ReadWriteExcutable = Permission_ReadOnly | Permission_WriteOnly |
                                    Permission_Excutable
}ePermissions;

/*!
 * @brief The eFileSystems enum
 * @enum eFileSystems
 */
typedef enum
{
    System_File,        ///< 普通文件
    System_VirtualFile, ///< 虚拟文件
    System_MemoryFile,  ///< 内存文件
    System_ArchiveFile, ///< 归档文件

    System_Dynamic,     ///< 动态库文件

    System_Directory,   ///< 目录

}eFileSystems;

typedef struct
{
    ky_string    ar_file;       ///< 归档文件名(含路径)
    size_t       ar_uncompress; ///< 归档解压后大小
    size_t       ar_compress;   ///< 归档压缩后大小
    ky_datetime  ar_modifydt;

    ///< 归档内选择的文件
    ky_string    sl_file;       ///< 归档内选择的文件名
    size_t       sl_uncompress; ///< 选择的文件解压后大小
    size_t       sl_compress;   ///< 选择的文件压缩后大小

}tArchiveInfos;

class ky_finfo
{
public:
    explicit ky_finfo(const ky_string &pathfile);
    virtual ~ky_finfo();

    //!
    //! \brief files
    //! \return
    //!
    ky_string files() const;
    //!
    //! \brief is_exists 检测文件或目录是否存在
    //! \return true 存在，否则不存在
    //!
    bool is_exists()const;
    //!
    //! \brief type 文件的类型，是文件还是目录或是符号链接
    //! \return
    //!
    eFileTypes type()const;
    //!
    //! \brief permission 文件的访问权限
    //! \return
    //!
    ePermissions permission()const;

    //!
    //! \brief drive 当前文件的驱动器
    //! \return
    //!
    ky_string drive()const;
    //!
    //! \brief path 当前文件的路径，不含驱动器
    //! \return
    //!
    ky_string path()const;
    //!
    //! \brief name 文件名称，包含扩展名
    //! \return
    //!
    ky_string name()const;
    //!
    //! \brief name_base 文件名称，不含扩展名
    //! \return
    //!
    ky_string name_base()const;
    //!
    //! \brief suffix 文件的扩展名
    //! \return
    //!
    ky_string suffix()const;
    ky_string suffixs()const;

    //!
    //! \brief size 文件大小
    //! \return
    //!
    size_t size()const;
    //!
    //! \brief access 最后一次访问时间
    //! \return
    //!
    ky_datetime access() const;
    //!
    //! \brief modify 最后一次修改时间
    //! \return
    //!
    ky_datetime modify() const;
    //!
    //! \brief change 最后一次属性改变时间
    //! \return
    //!
    ky_datetime change() const;

private:
    class impl_file *impl;
    ky_string file;

    ky_string drive_;
    ky_string path_;
    ky_string fname_;
    ky_string fext_;
};

class ky_fsys
{
    // Directory
public:
    //!
    //! \brief scan 返回指定路径下包含的文件
    //! \param path 扫描路径
    //! \param filter 后缀过滤
    //! \return
    //!
    static ky_string_list scan(const ky_string &path, const ky_string &filter = ky_string());
    //!
    //! \brief is_directory 返回是否为目录
    //! \param path
    //! \return
    //!
    static bool is_directory(const ky_string &path);
    //!
    //! \brief current 当前所在的目录
    //! \param size
    //! \return
    //!
    static ky_string current(size_t size = 4096);
    //!
    //! \brief change 改变目录
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool change(const ky_string & path);

    // file Directory
public:
    //!
    //! \brief remove 删除指定文件,目录
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool remove(const ky_string& path, eFileSystems fs = System_File);
    //!
    //! \brief create 创建指定文件,目录
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool create(const ky_string& path, eFileSystems fs = System_File);
    //!
    //! \brief rename 重命名指定文件
    //! \param path_s
    //! \param path_d
    //! \return true 成功，否则失败
    //!
    static bool rename(const ky_string& path_s, const ky_string& path_d, eFileSystems fs = System_File);

    //!
    //! \brief clean 清空指定文件内容
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool clean(const ky_string& path);
    //!
    //! \brief exist 指定文件是否存在
    //! \param path
    //! \return true 成功，否则失败
    //!
    static bool exist(const ky_string& path);
    //!
    //! \brief slash 路径中斜杠的处理
    //! \param path
    //! \return
    //!
    static ky_string slash(const ky_string &path);

    // File
public:
    explicit ky_fsys(const piio &io = piio());
    ~ky_fsys();
    //!
    //! \brief access 最后一次访问时间
    //! \return
    //!
    ky_datetime access()const;
    //!
    //! \brief modify 最后一次修改时间
    //! \return
    //!
    ky_datetime modify()const;
    //!
    //! \brief change 最后一次属性改变时间
    //! \return
    //!
    ky_datetime change() const;

    // Dynamic
public:
    //!
    //! \brief symbol 文件内的符号地址
    //! \param name 符号名
    //! \return
    //!
    void *symbol(const ky_string &name);
    //!
    //! \brief suffix 文件使用的后缀
    //! \return
    //!
    static ky_string suffix() ;

    // Archive
public:
    //!
    //! \brief uncompress 从归档中解压指定文件
    //! \param io 文件io
    //! \param fn 文件名
    //! \return   文件内容
    //!
    ky_byte uncompress(const ky_string &fn, piio *io = 0);
    //!
    //! \brief compress 根据模式压缩文件到归档中
    //! \param io  文件io
    //! \param fn  文件名
    //! \param dat 文件内容
    //! \return
    //!
    bool compress(const ky_string &fn, const ky_byte &dat, piio *io = 0);
    //!
    //! \brief compress 根据模式压缩文件到归档中
    //! \param io 文件io
    //! \param path 文件路径或目录路径
    //! \return
    //!
    bool compress(const ky_string &path, piio *io = 0);
    /*
    bool passwd(const piio &io, const ky_string &passwd);
    bool crc(const piio &io, int32 crc);*/
public:
    static piio create(eFileSystems fs = System_File, int mode = Archive_DeCompress) ;

private:
    piio ope_io;
};

#endif
