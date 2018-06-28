
/**
 * Basic tool library
 * Copyright (C) 2014 kunyang kunyang.yk@gmail.com
 *
 * @file     ky_event.h
 * @brief    继承ievent后完成鼠标、键盘、窗口的基本定义
 *       1.宏定义占位宽度和占位标志.
 *       2.定义了键盘事件、鼠标事件、窗口事件等
 *       3.时间戳及剪切板信息
 *
 * @author   kunyang
 * @email    kunyang.yk@gmail.com
 * @version  1.0.3.1
 * @date     2012/04/01
 * @license  GNU General Public License (GPL)
 *
 * Change History :
 *    Date    |  Version  |  Author  |   Description
 * 2012/04/02 | 1.0.0.1   | kunyang  | 创建文件定义键盘事件
 * 2012/05/10 | 1.0.1.0   | kunyang  | 鼠标事件的定义
 * 2012/05/11 | 1.0.2.0   | kunyang  | 将事件枚举修改为宏定义占位标志
 * 2014/02/22 | 1.0.3.0   | kunyang  | 修改键码和宏字段
 * 2016/08/20 | 1.0.3.1   | kunyang  | 事件基础上修改时间戳和剪切板字符串
 *
 */
#ifndef KY_EVENT_H
#define KY_EVENT_H
#include "interface/ievent.h"
#include "tools/ky_string.h"
//                       ievent    state modifier mouse other  keybord
// 23~31 Bit已经被占用 [1111 1111 1][111 1][111 1][111 11][1][1 1111 1111]

/* 状态事件占用的段定义 */
#define kyState_Field 18   // 占位19 ~ 22
/* 状态事件位宽 */
#define kyState_Wide 0x0f
/* 状态事件定义宏 */
#define kyState_EventDef(val) (val << kyState_Field)
/* 状态事件掩码段定义 */
#define kyState_MaskField (kyState_Wide << kyState_Field)

/* 修饰按键事件占用的段定义 */
#define kyModifier_Field 14// 占位15~18
/* 修饰按键事件位宽 */
#define kyModifier_Wide 0x0f
/* 修饰按键事件定义宏 */
#define kyModifier_EventDef(val) (val << kyModifier_Field)
/* 修饰按键事件掩码段定义 */
#define kyModifier_MaskField (kyModifier_Wide << kyModifier_Field)

/* 鼠标按键事件占用的段定义 */
#define kyMouse_Field 9  // 占位10~14
/* 鼠标按键事件位宽 */
#define kyMouse_Wide 0x1f
/* 鼠标按键事件定义宏 */
#define kyMouse_EventDef(val) (iMouseEventMacro(val << kyMouse_Field))
/* 鼠标按键事件掩码段定义 */
#define kyMouse_MaskField (kyMouse_Wide << kyMouse_Field)

/* 键盘按键事件占用的段定义 */
#define kyKeybord_Field 0 // 占位0~9
/* 键盘按键事件位宽 */
#define kyKeybord_Wide 0x1ff
/* 键盘按键事件定义宏 */
#define kyKeybord_EventDef(val) (iKeybordEventMacro(val << kyKeybord_Field))
/* 键盘按键事件掩码段定义 */
#define kyKeybord_MaskField (kyKeybord_Wide << kyKeybord_Field)

/* 其他按键事件占用的段定义 */
#define kyOtherFlags 8  // 占位9
#define kyFlagsWide 0x01
/* 其他按键事件掩码段定义 */
#define kyOtherMaskField (kyFlagsWide << kyOtherFlags)


/*!
 * @brief The eEventStates enum
 * @enum eEventStates
 */
typedef enum
{
    Event_StateNot = kyState_EventDef(0),

    Event_StateIm = kyState_EventDef(1 << 0),        ///< 输入法
    Event_StateDown = kyState_EventDef(1 << 1),      ///< 按下
    Event_StateUp = kyState_EventDef(1 << 2),        ///< 弹起
    Event_StateImDown = Event_StateIm | Event_StateDown,
    Event_StateImUp = Event_StateIm | Event_StateUp,
    Event_StateMove= kyState_EventDef(1 << 3)        ///< 移动

}eEventStates;
/*!
 * @brief The eEventMouses enum
 * @enum eEventMouses
 */
typedef enum
{
    Event_MouseNot = kyMouse_EventDef(0),

    Event_MouseLeft = kyMouse_EventDef(1 << 0),  ///左键
    Event_MouseRight = kyMouse_EventDef(1 << 1),   ///右键
    Event_MouseMiddle = kyMouse_EventDef(1 << 2),  ///中键
    Event_MouseWheel = kyMouse_EventDef(1 << 3),    ///滚轮
    Event_MouseDouble = kyMouse_EventDef(1 << 4)
}eEventMouses;
/*!
 * @brief The eEventModifiers enum
 * @enum eEventModifiers
 */
typedef enum
{
    Event_ModifierNot = kyModifier_EventDef(0),

    Event_ModifierShift = kyModifier_EventDef(1 << 0),
    Event_ModifierControl = kyModifier_EventDef(1 << 1),
    Event_ModifierAlt = kyModifier_EventDef(1 << 2),
    Event_ModifierMeta = kyModifier_EventDef(1 << 3),
    Event_ModifierSuper = Event_ModifierMeta
}eEventModifiers;

//键盘键码
typedef enum
{
    Event_KeyNot = 0,

    Event_KeySpace = 0x20,   // "Space" 空格键
    Event_KeyExclam = 0x21,  // "!" 感叹号
    Event_KeyQuoteDbl = 0x22,// """ 双引号
    Event_KeyNumberSign=0x23,// "#" 井号
    Event_KeyDollar = 0x24,  // "$" 美元号
    Event_KeyPercent = 0x25, // "%" 百分号
    Event_KeyAmpersand =0x26,// "&" 和号
    Event_KeyApostrophe=0x27,// "'" 单引号-闭单引号
    Event_KeyParenLeft =0x28,// "(" 左小括号
    Event_KeyParenRight=0x29,// ")" 右小括号
    Event_KeyAsterisk = 0x2a,// "*" 星号
    Event_KeyMultiply = Event_KeyAsterisk,
    Event_KeyPlus = 0x2b,    // "+" 加号
    Event_KeyAdd = Event_KeyPlus,
    Event_KeyComma = 0x2c,   // "," 逗号
    Event_KeyMinus = 0x2d,   // "-" 减号
    Event_KeySubtract = Event_KeyMinus,
    Event_KeyPeriod = 0x2e,  // "." 句号
    Event_KeyDecimal = Event_KeyPeriod,
    Event_KeySlash = 0x2f,   // "/" 斜杠
    Event_KeyDivide = Event_KeySlash,

    Event_Key0 = 0x30,        // 数字键
    Event_Key1 = 0x31,
    Event_Key2 = 0x32,
    Event_Key3 = 0x33,
    Event_Key4 = 0x34,
    Event_Key5 = 0x35,
    Event_Key6 = 0x36,
    Event_Key7 = 0x37,
    Event_Key8 = 0x38,
    Event_Key9 = 0x39,
    Event_KeyColon = 0x3a,     // ":" 冒号
    Event_KeySemicolon = 0x3b, // ";" 分号
    Event_KeyLess = 0x3c,      // "<" 小于号
    Event_KeyEqual = 0x3d,     // "=" 等号
    Event_KeyGreater = 0x3e,   // ">" 大于号
    Event_KeyQuestion = 0x3f,  // "?" 问号
    Event_KeyAt = 0x40,        // "@" 电子邮件符号
    Event_KeyA = 0x41,         // 字母键
    Event_KeyB = 0x42,
    Event_KeyC = 0x43,
    Event_KeyD = 0x44,
    Event_KeyE = 0x45,
    Event_KeyF = 0x46,
    Event_KeyG = 0x47,
    Event_KeyH = 0x48,
    Event_KeyI = 0x49,
    Event_KeyJ = 0x4a,
    Event_KeyK = 0x4b,
    Event_KeyL = 0x4c,
    Event_KeyM = 0x4d,
    Event_KeyN = 0x4e,
    Event_KeyO = 0x4f,
    Event_KeyP = 0x50,
    Event_KeyQ = 0x51,
    Event_KeyR = 0x52,
    Event_KeyS = 0x53,
    Event_KeyT = 0x54,
    Event_KeyU = 0x55,
    Event_KeyV = 0x56,
    Event_KeyW = 0x57,
    Event_KeyX = 0x58,
    Event_KeyY = 0x59,
    Event_KeyZ = 0x5a,
    Event_KeyBracketsLeft = 0x5b, // "[" 左方括号
    Event_KeyBackslash = 0x5c,    // "\" 反斜杠
    Event_KeyBracketsRight = 0x5d,// "]" 右方括号
    Event_KeyCircum = 0x5e,       // "^" 脱字符 AsciiCircum
    Event_KeyUnderscore = 0x5f,   // "_" 下划线
    Event_KeyGraveAccent = 0x60,  // "`" 键盘左上角那个点-开单引号

    Event_Keya = 0x61,         // 字母键
    Event_Keyb = 0x62,
    Event_Keyc = 0x63,
    Event_Keyd = 0x64,
    Event_Keye = 0x65,
    Event_Keyf = 0x66,
    Event_Keyg = 0x67,
    Event_Keyh = 0x68,
    Event_Keyi = 0x69,
    Event_Keyj = 0x6a,
    Event_Keyk = 0x6b,
    Event_Keyl = 0x6c,
    Event_Keym = 0x6d,
    Event_Keyn = 0x6e,
    Event_Keyo = 0x6f,
    Event_Keyp = 0x70,
    Event_Keyq = 0x71,
    Event_Keyr = 0x72,
    Event_Keys = 0x73,
    Event_Keyt = 0x74,
    Event_Keyu = 0x75,
    Event_Keyv = 0x76,
    Event_Keyw = 0x77,
    Event_Keyx = 0x78,
    Event_Keyy = 0x79,
    Event_Keyz = 0x7a,

    Event_KeyBraceLeft = 0x7b,    // "{" 左花括号
    Event_KeyBar = 0x7c,          // "|" 竖线
    Event_KeyBraceRight = 0x7d,   // "}" 右花括号
    Event_KeyTilde = 0x7e,        // "~" 波浪号

    Event_KeyF1 = 0x80,                // 功能键
    Event_KeyF2 = 0x81,
    Event_KeyF3 ,
    Event_KeyF4 ,
    Event_KeyF5 ,
    Event_KeyF6,
    Event_KeyF7,
    Event_KeyF8,
    Event_KeyF9 ,
    Event_KeyF10,
    Event_KeyF11 = 0x8a,
    Event_KeyF12 = 0x8b,

    Event_KeyEscape = 0x8c,       // "Esc" Esc键
    Event_KeyTab  = 0x8d,         // "Tap" Tap键
    Event_KeyBacktab = 0x8e,      // "Backtab" 退格键
    Event_KeyBackspace = 0x8f,    // "Backspace" 退格键
    Event_KeyReturn = 0x90,       // Event_KeyEnter
    Event_KeyEnter = Event_KeyReturn,// "Enter" 回车键
    Event_KeyInsert = 0x91,       // "Insert" 插入键
    Event_KeyDelete = 0x92,       // "Delete" 删除键
    Event_KeyPause = 0x93,        // "Pause/Break" Pause/Break键
    Event_KeyPrint = 0x94,        // "PrintScreen" 截屏键
    Event_KeySysReq = 0x95,       // Event_KeyPrint
    Event_KeyClear = 0x96,        // 清除键
    Event_KeyHome  = 0x97,        // "Home" 起始键
    Event_KeyEnd = 0x98,          // "End" 结尾键
    Event_KeyLeft  = 0x99,        // "←" 左箭头
    Event_KeyUp = 0x9a,           // "↑" 上箭头
    Event_KeyRight = 0x9b,        // "→" 右箭头
    Event_KeyDown = 0x9c,         // "↓" 下箭头
    Event_KeyPageUp  = 0x9d,      // "PageUp" 向上翻页键
    Event_KeyPageDown = 0x9e,     // "PageDown" 向下翻页键
    Event_KeyShift = 0x9f,        // "Shift" 上档转换键
    Event_KeyShiftLeft = 0x100,    // "Shift" 左上档转换键
    Event_KeyShiftRight = 0x101,   // "Shift" 右上档转换键
    Event_KeyControl = 0x102,      // "Ctrl" 键
    Event_KeyControlLeft = 0x103,  // "Ctrl" 左键
    Event_KeyControlRight = 0x104, // "Ctrl" 右键
    Event_KeyMeta = 0x105,
    Event_KeyMetaLeft = 0x106,
    Event_KeyMetaRight = 0x107,
    Event_KeySuper = 0x108,
    Event_KeySuperLeft = 0x109,
    Event_KeySuperRight = 0x10a,
    Event_KeyAlt = 0x10b,          // "Alt" 交替换档键
    Event_KeyAltLeft = 0x10c,      // "Alt" 左交替换档键
    Event_KeyAltRight = 0x10d,     // "Alt" 右交替换档键
    Event_KeyCapsLock = 0x10e,     // "CapsLock" 大小写锁定键
    Event_KeyNumLock  = 0x10f,     // "NumLock" 小键盘锁定键
    Event_KeyScrollLock  = 0x110,  // "ScrollLock" 滚动锁定键

    Event_KeyMenu  = 0x111,        //  菜单键
    Event_KeyHelp = 0x112,         //  帮助键

    Event_KeyKP = 0x140,           // 小键盘要或上此值
    Event_KeyFN = 0x1ff            // "FN" 扩展键
}eEventKeys;

//需要有此OtherFlags标志指定
typedef enum
{
    //浏览器键码
    Event_KeyBack = 0x01,       //  Browser Back key
    Event_KeyForward = 0x02,    //  Browser Forward key
    Event_KeyRefresh = 0x03,    //  Browser Refresh key
    Event_KeyStop = 0x04,       //  Browser Stop key
    Event_KeySearch = 0x05,     //  Browser Search key
    Event_KeyFavorites = 0x06,  //  Browser Favorites key
    Event_KeyHomePage = 0x07,   //  Browser Start and Home key

    //多媒体键码
    Event_KeyVolumeMute = 0x08, //  Volume Mute key
    Event_KeyVolumeDown = 0x09, //  Volume Down key
    Event_KeyVolumeUp = 0x0A,   //  Volume Up key
    Event_KeyMediaNext = 0x0B,  //  Next Track key
    Event_KeyMediaPrev = 0x0C,  // Previous Track key
    Event_KeyMediaStop = 0x0D,  //  Stop Media key
    Event_KeyMediaPlay = 0x0E,  //  Play/Pause Media key
    Event_KeyMediaPause = 0x0F,
    Event_KeyMediaRecord = 0x10,
    Event_KeyMediaTogglePlayPause = 0x12,
    Event_KeyAudioForward = 0x13,
    Event_KeyAudioRewind = 0x14,
    Event_KeyChannelDown = 0x15,
    Event_KeyChannelUp = 0x16,
    Event_KeyMicMute = 0x17,
    Event_KeyMicVolumeDown = 0x18,
    Event_KeyMicVolumeUp = 0x19,

    Event_KeyUndo = 0x1A,
    Event_KeyRedo = 0x1B,
    Event_KeyCopy = 0x1C,
    Event_KeyCut = 0x1D,
    Event_KeyPaste = 0x1E,

    Event_KeyZoom = 0x1F,
}eEventKeyOthers;

class ky_event : public ievent
{
public:
    ky_event(): ievent(), _detle(0), _pos_x(0), _pos_y(0),_text() {}

    //! 默认构造赋值函数
    ky_event(_in ky_event &e): ievent(e.code ()), _detle(e.wheel()),
        _pos_x(e.x()), _pos_y(e.y()),_text(e.text ())
    {timestamp () = e.timestamp();}
    ky_event &operator = (_in ky_event &e)
    {
        this->_code = e._code;
        this->_tstamp = e._tstamp;
        this->_detle = e._detle;
        this->_pos_x = e._pos_x;
        this->_pos_y = e._pos_y;
        this->_text = e._text;
        return *this;
    }

    //! 构建窗口事件
    explicit ky_event(int evt): ievent(evt),_detle(0), _pos_x(0), _pos_y(0),_text() {}

    //! 构建键盘事件
    explicit ky_event(int key, int modif, size_t ts):
        ievent (kyKeybord_EventDef(key & kyKeybord_Wide) | (key & ~kyKeybord_Wide) | modif),
        _detle(0), _pos_x(0), _pos_y(0), _text() {timestamp () = ts;}

    //! 构建鼠标事件
    explicit ky_event(int mouse, int m_x,int m_y, int modif, size_t ts):
        ievent ((mouse) | modif),
        _detle(0), _pos_x(m_x), _pos_y(m_y), _text() {timestamp () = ts;}

    //! 构建鼠标滚轮事件
    explicit ky_event(int mouse, int m_w, int modif, size_t ts):
        ievent ((mouse) | modif),
        _detle(m_w), _pos_x(0), _pos_y(0), _text() {timestamp () = ts;}

    //! 构建剪切板,拖放事件
    explicit ky_event(const ky_string &txt, int evt, size_t ts):
        ievent (evt == Event_DragDrop ? Event_DragDrop : Event_Clipboard),
        _detle(0), _pos_x(0), _pos_y(0), _text(txt) {timestamp () = ts;}

    int &wheel() {return _detle;}
    float &x() {return _pos_x;}
    float &y() {return _pos_y;}

    int wheel()const { return _detle;}
    float x()const {return _pos_x;}
    float y()const {return _pos_y;}
    int button()const {return (code() & kyMouse_MaskField) | (code() & kyState_MaskField);}
    int key()const {return (code() & kyKeybord_MaskField) | (code() & kyState_MaskField);}
    int modifier()const {return code() & kyModifier_MaskField;}

    ky_string &text(){return _text;}
    const ky_string &text()const{return _text;}

    bool operator == (const ky_event &Rhs);
    bool operator != (const ky_event &Rhs)
    {
        return !((*this) == Rhs);
    }

    ky_string dump()const;

protected:
    int   _detle;   ///< 滚轮值
    float _pos_x;   ///< 鼠标位置
    float _pos_y;
    ky_string _text;///< 拖放或剪切板
};

#endif // KY_EVENT_H
