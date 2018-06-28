
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
#ifndef PARSER_H
#define PARSER_H
#include "ky_string.h"
#include "maths/ky_maths.h"
#include "maths/ky_quaternion.h"
#include "ky_color.h"
#include "interface/iio.h"
#include "ky_ptr.h"
#include "ky_map.h"
#include "ky_uuid.h"

 static const char* const aParserExtHeader = "+";
 static const char* const aParserSplitBegin = "[";
 static const char* const aParserSplitValue = ",";
 static const char* const aParserSplitEnd = "]";

 typedef enum
 {
     Parser_Bool,
     Parser_Char,
     Parser_Integer,
     Parser_Float,
     Parser_String,
     Parser_Vec2,
     Parser_Vec3,
     Parser_Vec4,
     Parser_Mat2,
     Parser_Mat3,
     Parser_Mat4,
     Parser_Quate,
     Parser_Color,
     Parser_Uuid,

     Parser_Count
 }eParserDataTypes;

 static const struct tParserObjectTypes{
    eParserDataTypes type;
    const char *text;
}
aParserObject[Parser_Count] =
{
   {Parser_Bool,     "Bool"},
   {Parser_Char,     "Char"},
   {Parser_Integer,  "Integer"},
   {Parser_Float,    "Float"},
   {Parser_String,   ""},
   {Parser_Vec2,     "Vec2"},
   {Parser_Vec3,     "Vec3"},
   {Parser_Vec4,     "Vec4"},
   {Parser_Mat2,     "Mat2"},
   {Parser_Mat3,     "Mat3"},
   {Parser_Mat4,     "Mat4"},
   {Parser_Quate,    "Quate"},
   {Parser_Color,    "Color"},
   {Parser_Uuid,     "Uuid"}
};

#define PP ky_ptr(kyPackage iparser)
typedef PP piparser;
#undef PP

 /*!
  * @brief The eParserTypes enum 解析器需要解析的类型
  * @enum eParserTypes
  */
typedef enum
{
    Parser_Text,
    Parser_Binary
}eParserTypes;

 /*!
  * @brief The tParserNodes struct 解析器获取的数据
  * @struct tParserNodes
  */
 typedef struct _tParserDatas_
 {
     friend class parser_node;

 public:
     ~_tParserDatas_(){}
     _tParserDatas_(){type = Parser_Count;}
     _tParserDatas_(const _tParserDatas_ &pd)
     {
         type = pd.type;
         data = pd.data;
     }
     _tParserDatas_ (bool d){
         data.append(uint8(d));
         type = Parser_Bool;
     }
     _tParserDatas_ (char d){
         data.append(uint8(d));
         type = Parser_Char;
     }
     _tParserDatas_ (longlong d){
         data.append((uint8*)&d, sizeof(longlong));
         type = Parser_Integer;
     }
     _tParserDatas_ (real d){
         data.append((uint8*)&d, sizeof(real));
         type = Parser_Float;
     }
     _tParserDatas_ (const ky_string &d){
         data.append((uint8*)d.data(), d.bytecount());
         type = Parser_String;
     }
     _tParserDatas_ (const vec2 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Vec2;
     }
     _tParserDatas_ (const vec3 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Vec3;
     }
     _tParserDatas_ (const vec4 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Vec4;
     }
     _tParserDatas_ (const mat2 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Mat2;
     }
     _tParserDatas_ (const mat3 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Mat3;
     }
     _tParserDatas_ (const mat4 &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Mat4;
     }
     _tParserDatas_ (const quat &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Quate;
     }

     _tParserDatas_ (const ky_color &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Color;
     }
     _tParserDatas_ (const ky_uuid &d){
         data.append((uint8*)&d, sizeof(d));
         type = Parser_Uuid;
     }

     _tParserDatas_ &operator = (const _tParserDatas_ &pd)
     {
         data = pd.data;
         type = pd.type;
         return *this;
     }

     template <typename T>
     T get()
     {
         if (type == Parser_String)
         {
             ky_string v = ky_string((ky_char*)data.data(), data.size()/sizeof(ushort));
             return *((T*)(&v));
         }
         else if (data.size() == sizeof(T))
             return *((T*)data.data());
         return T();
     }
     template <typename T>
     T get()const
     {
         if (type == Parser_String)
         {
             ky_string v = ky_string((ky_char*)data.data(), data.size()/sizeof(ushort));
             return *((T*)(&v));
         }
         else if (data.size() == sizeof(T))
             return *((T*)data.data());
         return T();
     }

     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const bool &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, bool &val)
     {
         val = pd.get<bool>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const char &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, char &val)
     {
         val = pd.get<char>();
         return pd;
     }

     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const longlong &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, longlong &val)
     {
         val = pd.get<longlong>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const real &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, real &val)
     {
         val = pd.get<real>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const ky_string &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, ky_string &val)
     {
         val = pd.get<ky_string>();
         return pd;
     }

     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const vec2 &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, vec2 &val)
     {
         val = pd.get<vec2>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const vec3 &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, vec3 &val)
     {
         val = pd.get<vec3>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const vec4 &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, vec4 &val)
     {
         val = pd.get<vec4>();
         return pd;
     }

     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const mat2 &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, mat2 &val)
     {
         val = pd.get<mat2>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const mat3 &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, mat3 &val)
     {
         val = pd.get<mat3>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const mat4 &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, mat4 &val)
     {
         val = pd.get<mat4>();
         return pd;
     }

     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const quat &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, quat &val)
     {
         val = pd.get<quat>();
         return pd;
     }
     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const ky_color &val)
     {
         pd = _tParserDatas_(val);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, ky_color &val)
     {
         val = pd.get<ky_color>();
         return pd;
     }

     friend _tParserDatas_ &operator << (_tParserDatas_&pd, const ky_uuid &uid)
     {
         pd = _tParserDatas_(uid);
         return pd;
     }
     friend _tParserDatas_ &operator >> (_tParserDatas_&pd, ky_uuid &uid)
     {
         uid = pd.get<ky_uuid>();
         return pd;
     }

 private:
     eParserDataTypes type;
     ky_array<uint8> data;
 }tParserDatas;

 /*!
  * @brief The tParserNodes struct 解析器获取的节点
  * @struct tParserNodes
  */
 typedef struct _tParserNodes_ :public ky_map<ky_string, struct _tParserRoots_>
 {

     _tParserNodes_():
     ky_map<ky_string, struct _tParserRoots_>()
     {}
     ~_tParserNodes_()
     {
         ky_map<ky_string, struct _tParserRoots_>::clear ();
     }
     typedef struct _NodeValue_ eValueTypes;
     bool op_begin()const;
     struct _NodeValue_ op_next(bool &ret)const;
 private:
     ky_map<ky_string, struct _tParserRoots_>::iterator op_value;
 }tParserNodes;

 /*!
  * @brief The tParserMemers struct 解析器获取的成员的值
  * @struct tParserMemers
  */
 typedef struct _MemerVaule_ :public ky_map<ky_string, tParserDatas>::iterator
 {

     _MemerVaule_(){}
     ~_MemerVaule_(){}
     _MemerVaule_(const ky_map<ky_string, tParserDatas>::iterator &it):
         ky_map<ky_string, tParserDatas>::iterator(it)
     {
     }
     tParserDatas data()const
     {
         return (*this).value();
     }
     tParserDatas &data()
     {
         return (*this).value();
     }
 }tMemerValues;
 /*!
  * @brief The tParserMemers struct 解析器获取的成员
  * @struct tParserMemers
  */
 typedef struct _tParserMemers_ :public ky_map<ky_string, tParserDatas>
 {

     _tParserMemers_():
         ky_map<ky_string, tParserDatas>()
     {}
     ~_tParserMemers_()
     {
         ky_map<ky_string, tParserDatas>::clear ();
     }
     typedef tMemerValues eValueTypes;
     bool op_begin()const
     {
         _tParserMemers_ *fack = (_tParserMemers_ *)this;
         fack->op_value = fack->begin();
         return fack->op_value != fack->end();
     }
     tMemerValues op_next(bool &ret)const
     {
         _tParserMemers_ *fack = (_tParserMemers_ *)this;
         if (fack->op_value == fack->end())
             ret = false;
         else
         {
             ret = true;
             return tMemerValues(fack->op_value++);
         }
         return fack->end();
     }

 private:
     ky_map<ky_string, tParserDatas>::iterator op_value;
 }tParserMemers;

 /*!
  * @brief The tParserRoots struct 解析器获取的跟
  * @struct tParserRoots
  */
 typedef struct _tParserRoots_ : public tParserNodes
 {
     friend class parser_node;

 public:
     _tParserRoots_():
         tParserNodes()
     {
         tParserNodes::clear ();
         memers.clear ();
     }
     tParserNodes &node()
     {
         return (*this);
     }
     const _tParserNodes_ &node()const
     {
         return (*this);
     }
     tParserMemers &memer()
     {
         return memers;
     }
     const tParserMemers &memer()const
     {
         return memers;
     }

     bool is_empty()const
     {
         return empty () & memers.empty ();
     }

 private:
     tParserMemers memers;
 }tParserRoots;

 /*!
  * @brief The tNodeValues struct 节点内部的值
  * @struct tNodeValues
  */
 typedef struct _NodeValue_ :public ky_map<ky_string, struct _tParserRoots_>::iterator
 {

     _NodeValue_(){}
     ~_NodeValue_(){}

     _NodeValue_(const ky_map<ky_string, struct _tParserRoots_>::iterator &it):
         ky_map<ky_string, struct _tParserRoots_>::iterator(it)
     {
     }
     tParserRoots data()const
     {
         return (*this).value();
     }
     tParserRoots &data()
     {
         return (*this).value();
     }

 }tNodeValues;

 /*!
  * @brief The iparser class 解析器接口
  * @class iparser
  * 1.解析器解析后的组成:
  *     |-> tParserRoots(根) --------------------------> tParserMemers(成员) --------------> tMemerValues(成员值)
  *     |     |--> tParserNodes(节点) --> tNodeValues(节点值)    |--> tParserDatas(数据)            |--> tParserDatas(数据)
  *     |<--------------|<--------------------|
  *
  *
  */
kyPackage iparser : public iio
{

    virtual ~iparser(){}

    virtual bool open(const ky_string &name, int mode = Io_ReadOnly)= 0;
    virtual bool open(int mode = Io_ReadOnly)= 0;
    virtual bool open(char *mem_data, size_t mem_len) = 0;
    virtual void close() = 0;

    virtual tParserRoots read(bool *ret = NULL) = 0;
    virtual bool write(const tParserRoots &root) = 0;

    virtual ky_string dump(const tParserRoots &root) = 0;

protected:
    virtual size_t read(void *, size_t)const{return -1;}
    virtual size_t read(ky_byte &) const{return -1;}
    virtual size_t write(void *, size_t)const{return -1;}
    virtual size_t write(const ky_byte &) const{return -1;}
    virtual bool seek(size_t){return false;}

public:
    static piparser create(eParserTypes type = Parser_Text) ;
};


#define foreach_t(val_type, val, lva) val_type val;\
    for (bool bf = lva.op_begin(); val = lva.op_next(bf), bf == true;)

inline bool tParserNodes::op_begin() const
{
    tParserNodes *fack = (tParserNodes *)this;
    fack->op_value = fack->begin();
    return fack->op_value != fack->end();
}
inline struct _NodeValue_ tParserNodes::op_next(bool &ret)const
{
    tParserNodes *fack = (tParserNodes *)this;
    if (fack->op_value == fack->end())
        ret = false;
    else
    {
        ret = true;
        return tNodeValues(fack->op_value++);
    }
    return fack->end();
}



#endif // PARSER_H
