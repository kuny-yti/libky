
/**
 * Universal interface header
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ircontext.h
 * @brief    窗口和渲染上下文接口
 *       1.支持GPU体系架构信息获取.
 *       2.支持OpenGL、OpenGLES、OpenVG、Vulkan作为渲染目标
 *       3.支持渲染属性设置
 *       4.支持GPU能力获取
 *       5.所有OpenGL渲染功能对象都在此统一创建
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.5.2.1
 * @date     2012/09/26
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/09/26 | 1.0.0.1   | kunyang  | 创建文件
 * 2012/09/29 | 1.1.0.1   | kunyang  | 加入颜色通道枚举来定义通道的位数
 * 2013/06/03 | 1.1.1.0   | kunyang  | 修改模板缓存定义
 * 2014/02/15 | 1.1.2.1   | kunyang  | 修改GPU信息获取接口
 * 2014/05/06 | 1.2.0.1   | kunyang  | 增加渲染目标和渲染客户端处理
 * 2015/06/11 | 1.2.1.1   | kunyang  | 修改渲染的目标定义加入Image渲染
 * 2016/01/22 | 1.3.0.1   | kunyang  | 加入渲染属性结构
 * 2016/04/12 | 1.3.1.1   | kunyang  | 调整渲染结构向前兼容属性
 * 2016/11/01 | 1.4.0.1   | kunyang  | 重构接口类，将原有渲染功能对象加入此类
 * 2017/04/26 | 1.4.1.1   | kunyang  | 根据渲染功能对象的修改，同时修改接口
 * 2017/06/07 | 1.5.0.1   | kunyang  | 加入GPU渲染能力获取
 * 2017/09/13 | 1.5.1.1   | kunyang  | 加入GPU能下限制获取
 * 2017/11/07 | 1.5.2.1   | kunyang  | 修改GPU能力获取对象接口
 */
#ifndef KY_RCONTEXT_H
#define KY_RCONTEXT_H

#include "ky_define.h"
#include "ky_ptr.h"
#include "maths/ky_vec4.h"
#include "ipackage.h"

#include "render/redef.h"
#include "render/itexture.h"
#include "render/ipixel_buffer.h"
#include "render/ivertex_buffer.h"
#include "render/ivertex_array.h"
#include "render/iindex_buffer.h"
#include "render/ishader_program.h"
#include "render/iframe_buffer.h"
#include "render/iuniform_buffer.h"
#include "render/irender_texture.h"


#define kyNotSupportLimit (-2)

#ifndef iNative
#define PPF ky_ptr(kyPackage inative)
typedef PPF pinative;
kyModUUID(native, 'n', 'a', 't', 'i', 'v', 'e', 's', ' ');
#define iNative
#undef PPF
#endif

#ifndef iRContext
#define PPF ky_ptr(kyPackage ircontext)
typedef PPF pircontext;
kyModUUID(context, 'c', 'o', 'n', 't', 'e', 'x', 't', 's');
#define iRContext
#undef PPF
#endif

#ifndef iGraph
#define PPF ky_ptr(kyPackage igraph)
typedef PPF pigraph;
kyModUUID(graph, 'g', 'r', 'a', 'p', 'h', 'i', 'c', 's');
#define iGraph
#undef PPF
#endif


/*!
 * @brief The eGPUVendors enum OpenGL供应商
 * @enum eGPUVendors
 */
typedef enum
{
    Vendor_Not,
    Vendor_Amd,
    Vendor_Ati = Vendor_Amd,
    Vendor_Nvidia,
    Vendor_Intel,
    Vendor_Qualcomm,
    Vendor_Arm,
    Vendor_Mesa,
    Vendor_Apple,
    Vendor_Mozilla,
    Vendor_Count
}eGPUVendors;


/*!
 * @brief The eGPUArchs enum GPU架构
 * @enum eGPUArchs
 */
typedef enum
{
  Arch_Unknown,
  Arch_Sandybridge,
  Arch_Sgx,
  Arch_Mali,
  Arch_Llvmpipe,
  Arch_Softpipe,
  Arch_SWRast
} eGPUArchs;

/*!
 * @brief The eOpenGLPackages enum OpenGL软件包
 * @enum eOpenGLPackages
 */
typedef enum
{
  Package_Unknown,
  Package_Mesa
} eOpenGLPackages;

/*!
 * @brief The eRenderPropertys enum 渲染属性
 * @enum eRenderPropertys
 */
typedef enum
{
    Render_CoreProfile = 1 << 16,  ///< 使用不包含任何弃用功能
    Render_Debug = 1 << 17,        ///< 是否开启调试
    Render_SwapSync = 1 << 18,     ///< 是否开启交换同步
    Render_Stereo = 1 << 19,       ///< 是否开启立体
    Render_Alpha = 1 << 20,        ///< 是否开启Alpha
    Render_Compatible = 1 << 21,   ///< 是否使用向前兼容模式
    Render_Robustness = 1 << 22,   ///< 是否使用稳健性属性
    Render_RobustnessLose = 1 << 23 | Render_Robustness,
    Render_Release = 1 << 24,
    Render_ReleaseFlush = 1 << 25 | Render_Release
}eRenderPropertys;

/*!
 * @brief The eRenderClients enum 渲染的客户端
 * @enum eRenderClients
 */
typedef enum {
    Render_Default    = 0x0,
    Render_OpenGL     = 0x1,
    Render_OpenGLES   = 0x2,
    Render_OpenVG     = 0x4,
    Render_Vulkan     = 0x8,
}eRenderClients;

/*!
 * @brief The eRenderTargets enum 渲染的目标
 * @enum eRenderTargets
 */
typedef enum
{
    Render_Image,     // 渲染到图像中     (离屏渲染的一种)
    Render_Window,    // 渲染到系统窗口中
    Render_PBuffer,   // 渲染到PBufffer (离屏渲染的一种)
    Render_Affinity,  // 渲染组合 (Nvidia的一种多GPU模式)
    Render_OffScreen, // FBO实现 (离屏渲染的一种)
}eRenderTargets;

/*!
 * @brief The eSwapMethods enum 交换方法
 * @enum eSwapMethods
 */
typedef enum
{
    Swap_DefaultBuffer = 0,
    Swap_SingleBuffer,
    Swap_DoubleBuffer,
    Swap_TripleBuffer
}eSwapMethods;

/*!
 * @brief The eColorChannels enum 颜色通道位数
 * @enum eColorChannels
 */
typedef enum
{
    Channel_BitNot = 0,
    Channel_Bit8 = 8,
    Channel_Bit16 = 16,
    Channel_Bit24 = 24,
    Channel_Bit32 = 32,

    Channel_RedMask = 0xFF,
    Channel_GreenMask = 0xFF << 8,
    Channel_BlueMask = 0xFF << 16,
    Channel_AlphaMask = 0xFF << 24,

    Channel_Red8 = Channel_Bit8,
    Channel_Green8 = Channel_Bit8 << 8,
    Channel_Blue8 = Channel_Bit8 << 16,
    Channel_Alpha8 = Channel_Bit8 << 24,
    Channel_RGB8 = Channel_Red8 | Channel_Green8 | Channel_Blue8,
    Channel_RGBA8 = Channel_Red8 | Channel_Green8 | Channel_Blue8| Channel_Alpha8,

    Channel_Red16 = Channel_Bit16,
    Channel_Green16 = Channel_Bit16 << 8,
    Channel_Blue16 = Channel_Bit16 << 16,
    Channel_Alpha16 = Channel_Bit16 << 24,
    Channel_RGB16 = Channel_Red16 | Channel_Green16 | Channel_Blue16,
    Channel_RGBA16 = Channel_Red16 | Channel_Green16 | Channel_Blue16| Channel_Alpha16,

    Channel_Red32 = Channel_Bit32,
    Channel_Green32 = Channel_Bit32 << 8,
    Channel_Blue32 = Channel_Bit32 << 16,
    Channel_Alpha32 = Channel_Bit32 << 24,
    Channel_RGB32 = Channel_Red32 | Channel_Green32 | Channel_Blue32,
    Channel_RGBA32 = Channel_Red32 | Channel_Green32 | Channel_Blue32| Channel_Alpha32,
}eColorChannels;

#define kyRedChannel(rf) (rf & 0xFF)
#define kyGreenChannel(rf) (rf >> 8 & 0xFF)
#define kyBlueChannel(rf) (rf >> 16& 0xFF)
#define kyAlphaChannel(rf) (rf >> 24& 0xFF)

#define kyOpenGLVersion(major, minor) (((major << 8) & 0xff00) | (minor & 0xff))
#define kyOpenGLMajor(ver) ((ver >> 8) & 0xff)
#define kyOpenGLMinor(ver) (ver & 0xff)
#define kyOpenGLVersionMask 0xffff
struct tRContextFormats
{
    // opengl
    kyPackage ircontext*share;      ///< 共享窗口
    eSwapMethods      swap;         ///< 支持双缓冲
    eRenderClients    client;       ///< 渲染客户端
    eRenderTargets    target;       ///< 渲染目标
    ivec4             wvp;          ///< 视口
    eColorChannels    color_bits;   ///< 窗口颜色位数
    eColorChannels    depth_bits;   ///< 深度缓存位数
    eColorChannels    stencil_bits; ///< 模板缓存位数
    eColorChannels    accum_bits;   ///< 累积缓存位数
    uint              samples:8;    ///< 采样位数
    int               property;     ///< 渲染属性
};
/*!
 * @brief The ircontext interface 显示窗口使用的渲染上下文
 * @interface ircontext
 * Implement
 */
struct ircontext : tRContextFormats
{
    virtual ~ircontext(){}

    inline tRContextFormats &format(){return *this;}
    inline const tRContextFormats &format()const{return *this;}
    inline ircontext & operator = (const tRContextFormats &rcc)
    {
        this->share = rcc.share;
        this->swap = rcc.swap;
        this->client = rcc.client;
        this->target = rcc.target;
        this->wvp = rcc.wvp;
        this->color_bits = rcc.color_bits;
        this->depth_bits = rcc.depth_bits;
        this->stencil_bits = rcc.stencil_bits;
        this->accum_bits = rcc.accum_bits;
        this->samples = rcc.samples;
        this->property = rcc.property;
        return *this;
    }

    struct gpu_capability
    {

        struct OpenGLLimit
        {
            struct UniformLimitGeneral
            {
                int component;     ///< 着色器的变量单个值存储的上限
                int component_out; ///< 着色器写入的每个组件上限
                int block;         ///< 着色器的块数的上限
                int vector;        ///< 着色器的4个向量存储的上限( = component / 4)
                int unit_tex_img;  ///< 着色器访问纹理贴图所支持的纹理图像单元上限;
                int image;         ///< 单个着色器阶段可用的图像上限;

                int atomic;        ///< 着色器访问的原子计数器上限;
                int atomic_buffer; ///< 着色器访问的原子计数器缓冲区的上限;
                int component_combined; ///< 统一块中所有变量的字节上限;
                int block_storage;      ///< 着色器可访问的有效着色器存储块上限;
            };

            struct UniformLimitVertex : UniformLimitGeneral
            {
                int attrib;        ///< 着色器可访问的4个组合通用顶点属性上限
            };
            struct UniformLimitTessControl : UniformLimitGeneral
            {
                int component_in;  ///< 着色器读取的每个组件上限
                int total;         ///< 着色器每个填补输出的组件上限;
                int gen_level;     ///< 支持图元生成的级别上限
            };
            struct UniformLimitTessEvaluation : UniformLimitGeneral
            {
                int component_in;  ///< 着色器读取的每个组件上限
            };
            struct UniformLimitGeometry : UniformLimitGeneral
            {
                int component_in;  ///< 着色器读取的每个组件上限
                int vector_out;    ///< 着色器写入的每个向量上限
                int invocation;    ///< 几何着色器调用计数上限
                int total;         ///< 着色器每个填补输出的组件上限
            };
            struct UniformLimitFragment : UniformLimitGeneral
            {
                int component_in;  ///< 着色器读取的每个组件上限
            };

            struct TextureLimit
            {
                int size_unit;
                int size_3d;      ///< 3D纹理维度上限
                int Layer_array;  ///< Array 数组纹理的层数上限
                int size_cube;    ///< Cube 立方体纹理维度上限
                int size_rect;    ///< Rectangle 矩形纹理维度上限
                int size_render;  ///< 渲染缓冲区的支持宽高尺寸上限
                int size_buffer;  ///< 纹理缓冲区对象所允许最大纹理数上限
                int lod_bias;     ///< 纹理细节偏差的最大绝对值
                int size;         ///< 1D,2D纹理的尺寸上限
                int num_compress; ///< 压缩纹理格式的数量
            };
            struct UniformLimitTransformFeedback
            {
                int component_interleaved; ///< 在交错模式中单个缓冲区组件数上限
                int attrib; ///< 在变换反馈中可以捕获的独立属性或输出数量上限
                int component; ///< 使用变换反馈写入缓冲区对象数上限
            };
            struct DebugLimit
            {
                int messages;///< 存储在调试消息日志中的消息数上限
                int length;///< 调试消息字符串的最大长度
                int stack;   ///< 调试消息组堆栈的深度上限
                int label;///< 可以分配给对象的标签字符串的最大长度
            };
            struct FramebufferLimit
            {
                int attach;  ///< 颜色缓冲区的附着点上限
                int sample_color;    ///<  多重采样纹理中所有颜色格式的采样上限
                int sample_depth;   ///< 多重采样深度或深度模板纹理中的采样上限
                int draw;           ///< 写入片段着色器同时输出的上限
                int draw_dual;      ///< 使用双源混合时的主动绘图缓冲区的上限
                int height;        ///< 没有附件的帧缓冲区的高度上限
                int layer;         ///< 没有附件的帧缓冲区最大层数
                int sample; ///< 没有附件的帧缓冲区中的最大采样数。
                int width;   ///< 没有附件的帧缓冲区的最大宽度。
                int sample_int; ///< 以整数格式多采样缓冲区支持的最大采样数。
            };
            struct ProgramLimit
            {
                int size_atomic;   ///<  原子计数器缓冲区的基本单位尺寸上限
                int resource;    ///< 激活的图像单元于片段输出资源的上限
                int storage; ///< 程序可访问的着色器存储块的上限
                int sample; ///<  与图像单元绑定的纹理允许采样的级别上限
                int unit;   ///< 图像的加载，存储和原子操作单元上限
                int offset_tex_max; ///< 纹理查找中允许的最大纹理像素偏移量
                int offset_tex_min; ///< 纹理查找中允许的最大纹理像素偏移量
                int size_storage; ///<  着色器存储块的基本单元的最大尺寸
                int size_uniform;///< 统一块基本单元的最大尺寸
                int location; ///< 明确分配统一位置的最大数量
                int varying; ///< varying 类型的变量四组件分量数上限
                int offset_relative; ///< 可能添加到顶点绑定偏移量的最大偏移量
                int attrib; ///<  可能绑定的顶点缓冲区的最大数量
                int stride ; ///< 最大顶点属性步幅
            };
            struct VertexArrayLimit
            {
                int index; ///< 在将通用顶点属性传输到GL期间可以指定的最大索引
                int indice; ///< 推荐的最大顶点数组索引数
                int vertice; ///< 推荐的最大顶点数组顶点数
            };

            UniformLimitVertex vertex_max;
            UniformLimitFragment fragment_max;
            UniformLimitTessControl tess_control_max;
            UniformLimitTessEvaluation tess_evaluation_max;
            UniformLimitGeometry geometry_max;
            TextureLimit  texture_max;
            UniformLimitTransformFeedback transform_feedback_max;

            DebugLimit debug_max;
            FramebufferLimit framebuffer_max;
            ProgramLimit program_max;
            VertexArrayLimit vertex_array_max;

            int storage_binding_max;///< 缓冲区绑定数量上限
            int atomic_binding_max;///< 缓冲区绑定数量上限
            int uniform_binding_max; ///< 缓冲区绑定数量上限
            int size_atomic_buffer_max;          ///<
            int size_atomic_max;
            int size_unit_max;
            int size_uniform_block_max;
            int sample_max;    ///< 支持所有非整数格式的采样上限
            int size_vp_max[2]; ///< 视口最大支持的宽度和高度
            int  count_vp_max;  ///< 支持的同时视口的最大数量。
            int patch;         ///< 着色器填补图元的顶点上限
            int stream;        ///< 着色器可以输出的顶点流上限
            int subpix_bit;    ///< 估计子像素分辨率的位数

        };

        OpenGLLimit limit;

        //! OpenGL 版本
        ky_string version;
        //! 供应商
        ky_string vendor;
        //! 渲染设备
        ky_string renderer;
        //! GlSL版本
        ky_string version_glsl;
        //! 供应商
        virtual eGPUVendors vendors()const = 0;
        //! 软件包
        virtual eOpenGLPackages package() const = 0;
        //! 体系架构
        virtual eGPUArchs arch()const = 0;
        virtual bool is_support(const ky_string &ext)const = 0;

    }*gpucap;

    //!
    //! \brief initialize 根据本地窗口初始化上下文
    //! \param native
    //! \return 返回true则设置成功，否则不成功
    //!
    virtual bool initialize(const pinative &native) = 0;
    virtual void uninitialize() = 0;

    //!
    //! \brief version 渲染器 gl 版本
    //! \return
    //!
    ky_string version()const{return capability().version;}
    //!
    //! \brief vendor 渲染器供应商
    //! \return
    //!
    ky_string vendor()const{return capability().vendor;}
    //!
    //! \brief renderer 渲染器设备
    //! \return
    //!
    ky_string renderer()const{return capability().renderer;}
    //!
    //! \brief version_shader 着色器版本
    //! \return
    //!
    ky_string version_shader()const{return capability().version_glsl;}
    //!
    //! \brief is_support 渲染器是否支持指定的功能
    //! \param ext
    //! \return
    //!
    bool is_support(const ky_string &ext) {return capability().is_support(ext);}

    virtual bool is_stereo()const = 0;
    virtual bool is_sRGB()const = 0;

    virtual const gpu_capability &capability() const {return *gpucap;}

    //! 对象
    //!
    //! \brief texture 生成一个纹理对象
    //! \return
    //!
    virtual pitexture texture() = 0;
    //!
    //! \brief fbo 生成一个帧缓冲对象
    //! \return
    //!
    virtual pifbo frame_buffer() = 0;

    //!
    //! \brief pbo 生成一个像素缓冲对象
    //! \return
    //!
    //virtual pipbo pixel_buffer() = 0;

    //!
    //! \brief shader 生成一个着色器对象
    //! \return
    //!
    virtual pishader shader() = 0;
    //!
    //! \brief program 生成一个着色器程序对象
    //! \return
    //!
    virtual piprogram program() = 0;
    //!
    //! \brief vbo 生成一个顶点缓冲对象
    //! \return
    //!
    virtual pivbo vertex_buffer() = 0;
    //!
    //! \brief vao 生成一个顶点数组对象
    //! \return
    //!
    virtual pivao vertex_array() = 0;
    //!
    //! \brief ibo 生成一个索引缓冲对象
    //! \return
    //!
    virtual piibo index_buffer() = 0;

    //!
    //! \brief ubo 生成一个统一缓冲对象
    //! \return
    //!
    virtual piubo uniform_buffer() =0;

    virtual pirtt render_texture() =0;

    //!
    //! \brief viewport 设置渲染器视口
    //! \param
    //!
    virtual void viewport(const ivec4 &) = 0;
    virtual ivec4 viewport () const = 0;

    //!
    //! \brief viewport 设置渲染器视口裁剪
    //! \param
    //!
    virtual void scissor(const ivec4 &) = 0;

    //!
    //! \brief enable 开启和关闭指定功能
    //! \param
    //!
    virtual void enable(eGLModes mod) = 0;
    virtual void disable(eGLModes mod) = 0;
    virtual bool is_enable(eGLModes mod)const = 0;

    //!
    //! \brief depth_mode 设置深度模式
    //! \param
    //!
    virtual void depth_mode(eDepthModes mode) = 0;

    //!
    //! \brief blending_mode 设置混合模式
    //! \param
    //!
    virtual void blending_mode(eBlendingModes mode) = 0;

    //!
    //! \brief mask_color 设置遮罩
    //! \param r, g, b, a
    //!
    virtual void mask_color(bool r, bool g, bool b, bool a) = 0;
    virtual void mask_depth(bool depth) =0;

    //!
    //! \brief stencil_func 设置模板
    //! \param func
    //! \param ref
    //!
    virtual void stencil_func(eStencilFuncs func, int ref=0) = 0;
    virtual void stencil_op(eStencilOps op) = 0;

    //!
    //! \brief cull_mode 设置多边形剔除
    //! \param mode
    //!
    virtual void cull_mode(eCullModes mode) = 0;
    //!
    //! \brief front_face 前后台绘制方式
    //! \param mode
    //!
    virtual void front_face(eFaceModes mode) = 0;

    //!
    //! \brief clear 清理指定缓冲
    //! \param
    //!
    virtual void clear(int buffer)= 0;
    //!
    //! \brief background 设置背景颜色
    //! \param
    //!
    virtual void background(const ky_color & color)= 0;

    //!
    //! \brief make_current 取得上下文关联
    //!
    virtual void make_current() = 0;
    virtual void done_current() = 0;
    //!
    //! \brief swap_buffers 交换前后缓冲区
    //!
    virtual void swap_buffers() = 0;
    virtual void swap_interval(int interval) = 0;

    //!
    //! \brief hd
    //!
    virtual intptr hd() = 0;
};


#endif // IRCONTEXT_H
