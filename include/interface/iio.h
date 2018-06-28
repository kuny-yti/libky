
/**
 * Universal interface header
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     iio.h
 * @brief    所有io类型操作的抽象接口
 *       1.抽象了所有io的基本操作基础接口
 *       2.支持错误描述
 *       3.支持非阻塞操作
 *       4.支持异步操作
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.2.0.1
 * @date     2011/12/11
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2011/12/11 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/02/19 | 1.0.1.1   | kunyang  | 修改抽象的Io模式定义
 * 2014/10/07 | 1.0.2.1   | kunyang  | 加入io类型定义
 * 2015/02/23 | 1.1.0.1   | kunyang  | 重构类并加入reset功能
 * 2018/03/09 | 1.1.1.1   | kunyang  | 加入文件的随机性操作接口
 * 2018/04/10 | 1.2.0.1   | kunyang  | 加入非阻塞操作,异步操作
 */
#ifndef KY_IO_H
#define KY_IO_H

#include "tools/ky_url.h"
#include "ky_ptr.h"

//!
//! \brief The eIoTypes enum 当前Io的类型
//!
typedef enum IoType
{
    Io_TypeFile,    ///< 文件 io
    Io_TypeMemory,  ///< 内存 io
    Io_TypeArchive, ///< 归档 io
    Io_TypeTcp,     ///< 套接字 TCP io
    Io_TypeUdp,     ///< 套接字 UDP io
    Io_TypeHttp,    ///< 套接字 HTTP io
    Io_TypeFtp,     ///< 套接字 Ftp io
    Io_TypeSeral,   ///< 串口 io
    Io_TypeUsb,     ///< USB io
    Io_TypeMedia    ///< 媒体 io
}eIoTypes;
typedef int io_type_t;
//!
//! \brief The eIoModes enum 当前Io的操作模式
//!
typedef enum IoMode
{
    //! 必选
    Io_ReadOnly    = 0x01,  ///< 只读模式打开
    Io_WriteOnly   = 0x02, ///< 只写模式打开
    Io_ReadWrite   =       ///< 读写模式打开
                  Io_ReadOnly | Io_WriteOnly,

    Io_Append      = 0x04, ///< 附加模式打开, 默认覆盖
    Io_Text        = 0x08, ///< 文本模式打开, 默认二进制

    Io_NonBlocking = 0x10, ///< 非阻塞模式,默认阻塞模式
    Io_ASync       = 0x20, ///< 异步io模式,默认同步模式,异步下采用非阻塞实现

    ///! 异步
    Io_ASyncRead = Io_ASync | Io_ReadOnly,
    Io_ASyncWrite = Io_ASync | Io_WriteOnly,
    Io_ASyncReadWrite = Io_ASync | Io_ReadWrite,

    ///! 非阻塞
    Io_NonBlockingRead = Io_NonBlocking | Io_ReadOnly,
    Io_NonBlockingWrite = Io_NonBlocking | Io_WriteOnly,
    Io_NonBlockingReadWrite = Io_NonBlocking | Io_ReadWrite,

    ///! 文本
    Io_TextRead = Io_Text | Io_ReadOnly,
    Io_TextWrite = Io_Text | Io_WriteOnly,
    Io_TextReadWrite = Io_Text | Io_ReadWrite,

    Io_AppendWrite = Io_Append | Io_WriteOnly,
    Io_AppendReadWrite = Io_Append | Io_ReadWrite,

    Io_AppendTextWrite = Io_Append | Io_TextWrite,
    Io_AppendTextReadWrite = Io_Append | Io_TextReadWrite,

    Io_AppendASyncWrite = Io_Append | Io_ASyncWrite,
    Io_AppendASyncReadWrite = Io_Append | Io_ASyncReadWrite,

    Io_AppendNonBlockingWrite = Io_Append | Io_NonBlockingWrite,
    Io_AppendNonBlockingReadWrite = Io_Append | Io_NonBlockingReadWrite,

}eIoModes;
typedef int io_mode_t;
//!
//! \brief The eIoStatus enum 当前Io的操作状态
//!
typedef enum IoStatu
{
    Io_StateNot     = 0x00, ///< 无动作
    Io_StateRead    = 0x01, ///< 正在读取
    Io_StateWrite   = 0x02, ///< 正在写入
    Io_StateWait    = 0x04, ///< 等待系统完成
    Io_StatePrepare = 0x10, ///< 准备操作
    Io_StateFinish  = 0x20, ///< 系统完成

    Io_StateReadFinish = Io_StateRead | Io_StateFinish,
    Io_StateWriteFinish = Io_StateWrite | Io_StateFinish,
}eIoStatus;
typedef int io_statu_t;
//!
//! \brief The eIoErrors enum 当前Io的操作错误
//!
typedef enum IoError
{
    Io_ErrorNot = 0,           ///< 没有错误
    Io_ErrorNotReady = -1,     ///< 未就绪错误
    Io_ErrorStat = INT32_MIN,  ///< 状态操作错误
    Io_ErrorOpen,              ///< 打开错误
    Io_ErrorRead,              ///< 读取错误
    Io_ErrorWrite,             ///< 写入错误
    Io_ErrorClose,             ///< 关闭错误
    Io_ErrorNotOpen,           ///< 未打开错误
    Io_ErrorUnknown,           ///< 未知错误
}eIoErrors;
typedef int io_error_t;

///! 采用异步IO时，使用着需要管理缓冲，定义缓冲区大小
const static int kyASyncIoCacheSize = 512 *1024;

#ifndef IIo
#define IIO ky_ptr(kyPackage iio)
typedef IIO piio;
#define IIo
#undef IIO
#endif

/*!
 * @brief The iio class 统一io操作接口
 * @class iio
 * 1.当指定非阻塞时，当io未就绪时则返回错误。
 * 2.当指定异步时，当io就绪后会调用就绪信号
 */
kyPackage iio
{
    virtual ~iio(){}

    //!
    //! \brief io_ready 采用异步模式时，当IO处理完成后此信号响应
    //!
    Signal<void (eIoStatus status, const ky_byte &dat)> io_ready;

    //!
    //! \brief is_async 是否为异步IO
    //! \return
    //!
    bool is_async()const{return io_mode & Io_ASync;}

    //!
    //! \brief is_nonblocking 是否为非阻塞
    //! \return
    //!
    bool is_nonblocking()const{return io_mode & Io_NonBlocking;}

    //!
    //! \brief is_ready 是否操作就绪
    //! \return
    //!
    bool is_ready()const{return (io_statu & Io_StateFinish) || (io_statu == Io_StateNot);}

    //!
    //! \brief is_open 当前io是否打开
    //! \return
    //!
    bool is_open() const{return io_open;}

    //!
    //! \brief mode 返回当前操作的模式
    //! \return
    //!
    io_mode_t mode()const{return io_mode;}
    //!
    //! \brief type 返回当前操作的模式类型
    //! \return
    //!
    io_type_t type()const{return io_type;}

    //!
    //! \brief error 返回当前错误代码
    //! \return
    //!
    io_error_t error()const{return io_error;}

    //!
    //! \brief path 返回当前操作的io路径-包括io文件名
    //! \return
    //!
    ky_string path() const{return io_url.path ();}

    //!
    //! \brief url 返回当前操作的完整url路径
    //! \return
    //!
    const ky_url& url() const {return io_url;}

    //!
    //! \brief available 当前打开的io剩余字节数
    //! \return
    //!
    size_t available()const{return io_size - io_pos;}

    //!
    //! \brief size 当前打开的io大小
    //! \return
    //!
    size_t size()const{return io_size;}

    //!
    //! \brief is_eof 是结束
    //! \return
    //!
    bool is_eof()const{return at_end ();}

    //!
    //! \brief at_end 是否遇到结束标志
    //! \return
    //!
    virtual bool at_end()const { return io_eof; }

    //!
    //! \brief tell 当前打开的io位置
    //! \return
    //!
    virtual size_t tell()const{return io_pos;}

    //!
    //! \brief errors 返回当前错误描述
    //! \return
    //!
    virtual ky_string errors()const = 0;
    //!
    //! \brief open 打开io
    //! \param mode
    //! \return
    //!
    virtual bool open(io_mode_t mode = Io_ReadWrite) = 0;
    //!
    //! \brief close 关闭io
    //!
    virtual void close() = 0;

    //!
    //! \brief seek 移动当前打开的io位置
    //! \param pos
    //! \return
    //!
    virtual bool seek(size_t pos)=0;
    //!
    //! \brief read 读取io数据
    //! \param buf 存放地址
    //! \param len 存放大小
    //! \param offset 当前位置的偏移
    //! \return 返回读取的实际大小
    //!
    virtual size_t read(void *buf, size_t len)const = 0;
    //!
    //! \brief write 写入io数据
    //! \param buf 写入数据的地址
    //! \param len 写入的长度
    //! \param offset 当前位置的偏移
    //! \return 返回实际写入的长度
    //!
    virtual size_t write(void *buf, size_t len)const = 0;
    //!
    //! \brief open URL格式打开io
    //! \param url
    //! \param mode
    //! \return
    //!
    virtual bool open(const ky_url &, io_mode_t  = Io_ReadWrite) {return false;}
    //!
    //! \brief open 字符串格式打开io
    //! \param name
    //! \param mode
    //! \return
    //!
    virtual bool open(const ky_string &name, io_mode_t mode = Io_ReadWrite)
    {
        io_url = ky_url(name);
        return open (mode);
    }
    //!
    //! \brief cancel 取消io操作,对非阻塞或异步有效
    //! \return
    //!
    virtual bool cancel() {return false;}

    //!
    //! \brief read 提高随机性读取io数据
    //! \param buf 存放地址
    //! \param len 存放大小
    //! \param offset 当前位置的偏移
    //! \return 返回读取的实际大小
    //!
    virtual size_t read(void *, size_t, int64)const{return -1;}
    //!
    //! \brief write 提高随机性写入io数据
    //! \param buf 写入数据的地址
    //! \param len 写入的长度
    //! \param offset 当前位置的偏移
    //! \return 返回实际写入的长度
    //!
    virtual size_t write(void *, size_t, int64)const{return -1;}

    virtual size_t read(ky_byte &buf) const
    {
        return read(buf.data (), buf.bytecount ());
    }
    virtual size_t write(const ky_byte &buf) const
    {
        return write((void*)buf.data (), buf.bytecount ());
    }

    virtual void reset()
    {
        io_open = false;
        io_eof = false;
        io_error = Io_ErrorNot;
        io_mode = Io_ReadWrite;
        io_type = Io_TypeFile;
        io_statu = Io_StateNot;
        io_size = 0;
        io_pos = 0;
    }

    bool       io_open;
    bool       io_eof;
    io_error_t io_error;
    io_mode_t  io_mode;
    io_type_t  io_type;
    io_statu_t io_statu;
    size_t     io_size;
    size_t     io_pos;
    ky_url     io_url;
};

#endif
