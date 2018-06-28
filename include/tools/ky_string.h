
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
#ifndef ky_STRING
#define ky_STRING

#include "ky_define.h"
#include "ky_vector.h"
#include "ky_list.h"
#include "ky_array.h"
#include "ky_stream.h"
#include "ky_variant.h"

class ky_string;

typedef enum
{
    UTF_UnSet=0, // 不是UTF BOM
    UTF_Not,     // 不是UTF 编码
    UTF_8,       // UTF-8 本机 CPU 字节序
    UTF_16,      // UTF-16 本机 CPU 字节序
    UTF_16be,    // UTF-16 大端字节序
    UTF_16le,    // UTF-16 小端字节序
    UTF_32,      // UTF-32 本机 CPU 字节序
    UTF_32be,    // UTF-32 大端字节序
    UTF_32le     // UTF-32 小端字节序
}eUnicodeEncodings;

class ky_char
{
public:
    static ky_char from_latin1(char c);

public:
    ky_char();
    ky_char( const ky_char &);
    ky_char( char);
    ky_char( wchar_t);
    ky_char(uint16);

    ~ky_char();

    int digit() const ;
    char to_latin1() const;
    ky_char to_lower() const ;
    ky_char to_upper() const ;
    ky_char to_title_case() const ;
    ky_char to_case_folded() const ;

    // 作为字符
    operator char () const;
    operator wchar_t () const;
    operator uint16 ()const;
    operator uint16 &();

    uint16 unicode()const;
    uint16 &unicode() ;

    //! 判断是否是空子符'\0'
    bool is_valid()const;
    //! 判断是否是字母或数字
    bool is_letter_number() const;
    bool is_title_case() const;
    bool is_non_char() const ;
    bool is_high_surrogate() const ;
    bool is_low_surrogate() const ;
    bool is_surrogate() const ;
    //! 判断是否是十进制数字（'0' - '9'）
    bool is_digit() const;
    //! 判断是否是数字，包括正负号、小数点等
    bool is_number() const;
    //! 判断是否是小写字母
    bool is_lower() const;
    //! 判断是否是大写字母
    bool is_upper() const;
    //! 判断是否是字母
    bool is_letter() const;
    //! 判断是否是分隔符，包括空格等
    bool is_space() const;
    bool is_punct() const;
    //! 判断是否是可打印字符
    bool is_print() const;
    bool is_symbol() const;
    bool is_mark()const;
    bool is_unicode() const;

    ky_char& operator=(uint16);
    ky_char& operator=(char);
    ky_char& operator=(wchar_t);
    ky_char& operator=(const ky_char &);

    bool operator==(char) const;
    bool operator==(wchar_t)const ;
    bool operator==(const ky_char &)const ;

    bool operator!=(const ky_char&)const ;
    bool operator!=(wchar_t)const ;
    bool operator!=(char)const ;

    void lower();
    void upper();
    void reverse();
private:
    uint16  data;
};


typedef ky_list<ky_string> ky_string_list;
typedef ky_array<uint8 > ky_utf8;
typedef ky_array<uint16 > ky_utf16;
typedef ky_array<uint32 > ky_utf32;

typedef ky_utf16 ky_strbase;

class ky_string : public ky_strbase
{
public:
    ky_string();

    ky_string( const ky_string& );
    ky_string( const std::string& );
    ky_string( const std::wstring& );
    ky_string( const ky_char*, int /*length*/ );
    ky_string( const ky_strbase &array);
    ky_string( const char* );
    ky_string( const char*, int /*length*/ );
    ky_string( const wchar_t* src );
    ky_string( const wchar_t* src, int length );

    ~ky_string();

    std::string std_string()const;
    std::wstring std_wstring()const;

     //! 作为字符串
public:
    ky_char* data();
    const ky_char* data() const;

    ky_char& operator[](int);
    ky_char operator[](int) const;
    ky_char at(int) const;
    void at(int, ky_char);

    operator ky_char* () const;
    operator char* () const;
    operator wchar_t* () const;

public:
    //!字符串长度
    size_t length() const;
    int count() const;

    //!是否为拉丁字符串
    bool is_latin1() const;
    //!是否为国际字符集
    bool is_unicode() const;
    //!是否为数字字符串
    bool is_number() const;

    //!包含指定字符串
public:
    bool contains(const char* )const;
    bool contains(const wchar_t* )const;
    bool contains(const ky_string &)const;
    bool contains(const std::string &)const;
    bool contains(const std::wstring &)const;

    //!字符串连接
public:
    ky_string& append( const char);
    ky_string& append( const wchar_t);
    ky_string& append(const ky_char &c);
    ky_string& append( const char*, int =-1);
    ky_string& append( const ky_char*, int );
    ky_string& append( const wchar_t*, int =-1);
    ky_string& append( const ky_string& );
    ky_string& append( const std::string& );
    ky_string& append( const std::wstring& );

    //!字符串插入
public:
    ky_string& insert(int pos,  const char*, int =-1);
    ky_string& insert(int pos,  const ky_char*, int );
    ky_string& insert(int pos,  const wchar_t*, int =-1);
    ky_string& insert(int pos,  const ky_string&);
    ky_string& insert(int pos,  const std::string&);
    ky_string& insert(int pos,  const std::wstring&);

    //!字符串删除
public:
    ky_string& remove(int pos, int len);
    ky_string& remove(const char*, int =-1);
    ky_string& remove(const ky_char*, int );
    ky_string& remove(const wchar_t*, int =-1);
    ky_string& remove(const ky_string &str);
    ky_string& remove(const std::string &str);
    ky_string& remove(const std::wstring &str);

    //!字符串替换
public:
    int replace( int pos, int len, const ky_string &token );
    int replace( const char* token1, const char* token2 );
    int replace( const wchar_t* token1, const wchar_t* token2 );
    int replace( const ky_string &token1, const ky_string &token2 );
    int replace( const std::string &token1, const std::string &token2 );
    int replace( const std::wstring &token1, const std::wstring &token2 );

    //!字符串提取，截断
public:
    //! count 负数时根据pos位置向前取，此时顺序不为逆
    ky_string extract( int pos, int count)const;
    //! 根据pos位置将后面的全部提取
    ky_string extract( int pos )const ;
    //! 从开头提取count个字符
    ky_string start( int count)const;
    //! 从结尾提取count个字符，此时不为逆
    ky_string end( int count) const ;
    //! 修饰字符串
    ky_string trimmed() const;

    //!字符串查找
public:
    int find(const char*) const;
    int find(const wchar_t*) const;
    int find(const ky_string&) const;
    int find(const std::string &) const;
    int find(const std::wstring &) const;

    //!字符串分割
public:
    ky_string_list split(const char*)const;
    ky_string_list split(const wchar_t*)const;
    ky_string_list split(const ky_string&)const;
    ky_string_list split(const std::string &)const;
    ky_string_list split(const std::wstring &)const;

    //!字符串比较
public:
   int compare(const char*)const;
   int compare(const wchar_t*)const;
   int compare(const ky_string&)const;
   int compare(const std::string &)const;
   int compare(const std::wstring &)const;

   //! 格式输入
public:
   ky_string& format( const char*, ...) ;
   ky_string& format( const wchar_t*, ...);

   static ky_string formats( const char*, ...) ;
   static ky_string formats( const wchar_t*, ...);

   //! ? and * 通配符
public:

   //! 重载 =
public:
   ky_string& operator=(const char*);
   ky_string& operator=(const wchar_t* );
   ky_string& operator=(const ky_string&);
   ky_string& operator=(const std::string &);
   ky_string& operator=(const std::wstring &);

   //! 重载 ==
public:
   bool operator==(const char*str)const {return compare(str) == 0;}
   bool operator==(const wchar_t* str)const {return compare(str) == 0;}
   bool operator==(const ky_string&str)const {return compare(str) == 0;}
   bool operator==(const std::string &str)const {return compare(str) == 0;}
   bool operator==(const std::wstring &str)const {return compare(str) == 0;}

   //! 重载 !=
public:
   bool operator!=(const char*str)const {return !((*this) == str);}
   bool operator!=(const wchar_t* str)const{return !((*this) == str);}
   bool operator!=(const ky_string&str)const{return !((*this) == str);}
   bool operator!=(const std::string &str)const{return !((*this) == str);}
   bool operator!=(const std::wstring &str)const{return !((*this) == str);}

   //! 重载 +
public:
   ky_string operator+(const char) const;
   ky_string operator+(const wchar_t) const;
   ky_string operator+(const char*) const;
   ky_string operator+(const wchar_t*) const;
   ky_string operator+(const ky_string&) const;
   ky_string operator+(const std::string&) const;
   ky_string operator+(const std::wstring&) const;

   //! 重载 +=
public:
   const ky_string& operator+=(const char c){return append(c);}
   const ky_string& operator+=(const wchar_t c){return append(c);}
   const ky_string& operator+=(const char*str){return append(str);}
   const ky_string& operator+=(const wchar_t*str){return append(str);}
   const ky_string& operator+=(const ky_string&str){return append(str);}
   const ky_string& operator+=(const std::string&str){return append(str);}
   const ky_string& operator+=(const std::wstring&str){return append(str);}

public:
   bool operator < (const ky_string&str)const;
   bool operator > (const ky_string&str)const;
   //!转换字符串
public:
   char* to_latin1()const;
   wchar_t* to_wchar()const;
   ky_utf8 to_utf8()const;
   ky_utf16 to_utf16()const;
   ky_utf32 to_utf32()const;
   ky_byte to_byte()const;

   void upper();
   void lower();
   void reverse();
   //!
public:
   bool form_latin1(const char*);
   bool form_wchar(const wchar_t*);
   bool form_utf8(const uint8*, int );
   bool form_utf16(const uint16*, int );
   bool form_utf32(const uint32*, int );

   static ky_utf8 to_utf8s(uint16*, int) ;
   static ky_utf8 to_utf8s(uint32*, int) ;
   static ky_utf16 to_utf16s(uint8*, int) ;
   static ky_utf16 to_utf16s(uint32 *, int) ;
   static ky_utf32 to_utf32s(uint8*, int) ;
   static ky_utf32 to_utf32s(uint16*, int) ;

   //! 数字输入, radix 代表需要存储为什么进制, prec 代表精度
public:
   ky_string &form(bool, int radix = 10);
   ky_string &form(char, int radix = 10);
   ky_string &form(uchar, int radix = 10);
   ky_string &form(short, int radix = 10);
   ky_string &form(uint16, int radix = 10);
   ky_string &form(int, int radix = 10);
   ky_string &form(uint, int radix = 10);
   ky_string &form(long, int radix = 10);
   ky_string &form(ulong, int radix = 10);
   ky_string &form(longlong, int radix = 10);
   ky_string &form(ulonglong, int radix = 10);
   ky_string &form(float, int prec=10);
   ky_string &form(real, int prec=10);

   template<typename T>
   static ky_string number(T v, int radix = 10){ky_string str; str.form (v, radix);return str;}

   //! 数字输出 radix 代表存储的为什么进制
public:
   bool      to_bool(int radix = 10)const;
   char      to_char(int radix = 10)const;
   uchar     to_uchar(int radix = 10)const;
   short     to_short(int radix = 10)const;
   ushort    to_ushort(int radix = 10)const;
   int       to_int(int radix = 10)const;
   uint      to_uint(int radix = 10)const;
   long      to_long(int radix = 10)const;
   ulong     to_ulong(int radix = 10)const;
   longlong  to_longlong(int radix = 10)const;
   ulonglong to_ulonglong(int radix = 10)const;
   float     to_float()const;
   real      to_real()const;

private:
   ky_array<char> s_latin1;
   ky_array<wchar_t> s_unicode;
};


ky_streamb &operator << (ky_streamb &in, const ky_string &v);
ky_streamb &operator >> (ky_streamb &out, ky_string &v);
ky_streamt &operator << (ky_streamt &in, const ky_string &v);
ky_streamt &operator >> (ky_streamt &out, ky_string &v);
ky_variant &operator << (ky_variant &va, const ky_string &col);
ky_variant &operator >> (ky_variant &va, ky_string &col);

#endif // ky_STRING

