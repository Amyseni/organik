#pragma once


#define YEXTERN 
#define YYCEXPORT 
#define VMExec void*
#define  EVENT_CREATE                    0
#define  EVENT_DESTROY                   1
#define  EVENT_ALARM                     2
#define  EVENT_STEP                      3
#define  EVENT_COLLISION                 4
#define  EVENT_KEYBOARD                  5
#define  EVENT_MOUSE                     6
#define  EVENT_OTHER                     7
#define  EVENT_DRAW                      8
#define  EVENT_KEYPRESS                  9
#define  EVENT_KEYRELEASE                10
#define  EVENT_TRIGGER	                 11
#define  EVENT_CLEAN_UP	                 12
#define  EVENT_GESTURE		             13
#define  EVENT_PRE_CREATE                14

#define  EVENT_STEP_NORMAL                0
#define  EVENT_STEP_BEGIN                 1
#define  EVENT_STEP_END                   2
#define  EVENT_OTHER_OUTSIDE              0
#define  EVENT_OTHER_BOUNDARY             1
#define  EVENT_OTHER_STARTGAME            2
#define  EVENT_OTHER_ENDGAME              3
#define  EVENT_OTHER_STARTROOM            4
#define  EVENT_OTHER_ENDROOM              5
#define  EVENT_OTHER_NOLIVES              6
#define  EVENT_OTHER_ANIMATIONEND         7
#define  EVENT_OTHER_ENDOFPATH            8
#define  EVENT_OTHER_NOHEALTH             9
#define  EVENT_OTHER_USER0			     10
#define  EVENT_OTHER_CLOSEBUTTON         30
#define  EVENT_OTHER_OUTSIDE_VIEW0       40
#define  EVENT_OTHER_BOUNDARY_VIEW0      50
#define  EVENT_OTHER_ANIMATION_UPDATE    58
#define  EVENT_OTHER_ANIMATION_EVENT     59

#define  EVENT_OTHER_WEB_IMAGE_LOAD      60
#define  EVENT_OTHER_WEB_SOUND_LOAD      61
#define  EVENT_OTHER_WEB_ASYNC           62

#define  EVENT_OTHER_DIALOG_ASYNC        63
#define  EVENT_OTHER_WEB_IAP             66
#define  EVENT_OTHER_WEB_CLOUD           67
#define  EVENT_OTHER_WEB_NETWORKING      68
#define  EVENT_OTHER_WEB_STEAM           69
#define  EVENT_OTHER_SOCIAL              70

#define  EVENT_OTHER_PUSH_NOTIFICATION   71

#define  EVENT_OTHER_ASYNC_SAVE_LOAD     72
#define  EVENT_OTHER_AUDIO_RECORDING     73
#define  EVENT_OTHER_AUDIO_PLAYBACK      74
#define  EVENT_OTHER_SYSTEM_EVENT        75


#define  EVENT_OTHER_MESSAGE_EVENT       76

#define  EVENT_DRAW_GUI                  64
#define  EVENT_DRAW_RESIZE               65

#define  EVENT_DRAW_BEGIN                72
#define  EVENT_DRAW_END                  73
#define  EVENT_DRAW_GUI_BEGIN            74
#define  EVENT_DRAW_GUI_END              75
#define  EVENT_DRAW_PRE                  76
#define  EVENT_DRAW_POST                 77

typedef unsigned char   undefined;
typedef unsigned char    byte;
typedef unsigned int    dword;
typedef void* ImageBaseOffset32;

typedef long long       longlong;
typedef unsigned char    uchar;
typedef unsigned char    uint8;
typedef unsigned int    uint32;
typedef unsigned int    uint;
typedef unsigned long    ulong;
typedef unsigned long long    ulonglong;
typedef unsigned char    undefined1;
typedef unsigned short    undefined2;
typedef unsigned int    undefined4;
typedef unsigned long long    undefined8;
typedef unsigned short    ushort;
typedef unsigned short    wchar16;
typedef unsigned int    wchar32;
typedef int             int32_t;
typedef int             int32;
typedef long long       int64_t;
typedef long long       int64;
typedef unsigned short    word;
    struct CBackGM;
    struct CCode;
    struct CEvent;
    template <typename TKey, typename TValue, int TInitialSize>
    struct CHashMap;
    struct CInstance;
    struct CInstanceBase;
    struct CLayer;
    struct CLayerEffectInfo;
    struct CLayerElementBase;
    struct CLayerInstanceElement;
    struct CLayerSpriteElement;
    struct CObjectGM;
    struct CPhysicsDataGM;
    struct CPhysicsObject;
    struct CPhysicsWorld;
    struct CRoom;
    struct CScript;
    struct CScriptRef;
    struct CSkeletonInstance;
    struct CViewGM;
    struct CWeakRef;
    struct DLL_RFunction;
    enum eBuffer_Format : int32_t;
    struct GCObjectContainer;
    struct IBuffer;
    struct RTile;
    struct RToken;
    struct RValue;
    struct RValue;
    struct RVariableRoutine;
    struct YYGMLFuncs;
    struct YYObjectBase;
    struct YYRECT;
    struct YYRoom;
    struct YYRoomInstances;
    struct YYRoomTiles;
    struct YYRunnerInterface;

#define yymax(a,b)            (((a) > (b)) ? (a) : (b))
#define yymin(a,b)            (((a) < (b)) ? (a) : (b))
#define	DEG(_ang)			((Pi/180.0f)*(_ang))
#define	RAD(_ang)			((180.0f/Pi)*(_ang))
#define	Ord(c)			    ((Byte)(c))
#define	Chr(c)			    ((char)(c))
#define Round(x)            (yylrint(x))
#define Ceil(x)		        (int)ceil(x)
#define Floor(x)	        (int)floor(x)
#define Sqr(x)		        ((x)*(x))

#ifndef UTEXT
#define UTEXT(x) ((const unsigned char*)(x))
#endif // UTEXT
typedef bool (*FNGetVariable)(CInstance* self, int index, RValue *val);
typedef bool (*FNSetVariable)(CInstance* self, int ind, RValue *val);

#ifndef NULL_INDEX
#define NULL_INDEX INT_MIN
#endif // NULL_INDEX

typedef void* HYYMUTEX;
typedef void* HSPRITEASYNC;
struct HTTP_REQ_CONTEXT;
typedef int (*PFUNC_async)(HTTP_REQ_CONTEXT* _pContext, void* _pPayload, int* _pMap);
typedef void (*PFUNC_cleanup)(HTTP_REQ_CONTEXT* _pContext);
typedef void (*PFUNC_process)(HTTP_REQ_CONTEXT* _pContext);

typedef void (*TSetRunnerInterface)(const YYRunnerInterface* pRunnerInterface, size_t _functions_size);
typedef void (*TYYBuiltin)(RValue* Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg);
typedef void (*TRoutine)(RValue& _result, CInstance* _self, CInstance* _other, int _argc, RValue _args[]);

template <typename T>
struct CArrayStructure {

};
struct SLLVMVars{

};
template <typename T>
concept CIntegerCompatible = requires(T Param)
{
    requires std::is_convertible_v<T, int64_t>;
    requires std::is_integral_v<T>;
    requires !std::is_same_v<bool, T>;
    requires !std::is_pointer_v<T>;
};
template <typename T>
concept CGameMakerObject = requires(T Param)
{
    requires (std::is_pointer_interconvertible_base_of_v<CInstance, T> 
            || std::is_pointer_interconvertible_base_of_v<YYObjectBase, T> 
            || std::is_pointer_interconvertible_base_of_v<CInstanceBase, T> 
            || std::is_pointer_interconvertible_base_of_v<CObjectGM, T>)
            || (std::is_same_v<std::remove_pointer_t<T>, RValue>);
};
template <typename T>
concept NumberCompatible = requires(T Param)
{   
    requires std::_Is_any_of_v<T, 
        int, long, long long, float, double, 
        short, byte, char, unsigned char, 
        unsigned int, unsigned long, 
        unsigned long long, unsigned short>;
};
typedef void(*PFUNC_YYGML)(CInstance* Self,CInstance* Other);

using PFUNC_RAW = void(*)();

typedef RValue* (*PFUNC_YYGMLScript)(CInstance* Self,CInstance* Other,RValue* Result,int ArgumentCount,RValue* Arguments[]);

using PFN_YYObjectBaseAdd = void(__thiscall*)(YYObjectBase* This, const char* Name, RValue* Value, int Flags);

typedef int (*FNGetOwnProperty)(struct YYObjectBase *, struct RValue *, char *);

typedef int (*FNDeleteProperty)(struct YYObjectBase *, struct RValue *, char *, bool);

typedef int (*FNDefineOwnProperty)(struct YYObjectBase *, char *, struct RValue *, bool);

enum CmColor : unsigned char
{
    CM_BLACK = 0,
    CM_BLUE,
    CM_GREEN,
    CM_AQUA,
    CM_RED,
    CM_PURPLE,
    CM_YELLOW,
    CM_WHITE,
    CM_GRAY,
    CM_LIGHTBLUE,
    CM_LIGHTGREEN,
    CM_LIGHTAQUA,
    CM_LIGHTRED,
    CM_LIGHTPURPLE,
    CM_LIGHTYELLOW,
    CM_BRIGHTWHITE
};
enum InstanceKeywords : int32_t
{
    VAR_SELF = -1,
    VAR_OTHER = -2,
    VAR_ALL = -3,
    VAR_NOONE = -4,
    VAR_GLOBAL = -5,
    VAR_BUILTIN = -6,
    VAR_LOCAL = -7,
    VAR_STACKTOP = -9,
    VAR_ARGUMENT = -15,
};
enum ELayerElementType
{
    eLayerElementType_Undefined = 0,
    eLayerElementType_Background,
    eLayerElementType_Instance,
    eLayerElementType_OldTilemap,
    eLayerElementType_Sprite,
    eLayerElementType_Tilemap,
    eLayerElementType_ParticleSystem,
    eLayerElementType_Tile,
    eLayerElementType_Sequence,
};
enum YYObjectKind {
    OBJECT_KIND_YYOBJECTBASE=0,
    OBJECT_KIND_CINSTANCE=1,
    OBJECT_KIND_ACCESSOR=2,
    OBJECT_KIND_SCRIPTREF=3,
    OBJECT_KIND_PROPERTY=4,
    OBJECT_KIND_ARRAY=5,
    OBJECT_KIND_WEAKREF=6,
    OBJECT_KIND_CONTAINER=7,
    OBJECT_KIND_SEQUENCE=8,
    OBJECT_KIND_SEQUENCEINSTANCE=9,
    OBJECT_KIND_SEQUENCETRACK=10,
    OBJECT_KIND_SEQUENCECURVE=11,
    OBJECT_KIND_SEQUENCECURVECHANNEL=12,
    OBJECT_KIND_SEQUENCECURVEPOINT=13,
    OBJECT_KIND_SEQUENCEKEYFRAMESTORE=14,
    OBJECT_KIND_SEQUENCEKEYFRAME=15,
    OBJECT_KIND_SEQUENCEKEYFRAMEDATA=16,
    OBJECT_KIND_SEQUENCEEVALTREE=17,
    OBJECT_KIND_SEQUENCEEVALNODE=18,
    OBJECT_KIND_SEQUENCEEVENT=19,
    OBJECT_KIND_NINESLICE=20,
    OBJECT_KIND_FILTERHOST=21,
    OBJECT_KIND_EFFECTINSTANCE=22,
    OBJECT_KIND_SKELETON_SKIN=23,
    OBJECT_KIND_AUDIOBUS=24,
    OBJECT_KIND_AUDIOEFFECT=25,
    OBJECT_KIND_MAX=26
};
#define MASK_KIND_RVALUE 0x0ffffff
enum RValueType : uint32_t
{
    VALUE_REAL = 0,				// Real value
    VALUE_STRING = 1,			// String value
    VALUE_ARRAY = 2,			// Array value
    VALUE_PTR = 3,				// Ptr value
    VALUE_VEC3 = 4,				// Vec3 (x,y,z) value (within the RValue)
    VALUE_UNDEFINED = 5,		// Undefined value
    VALUE_OBJECT = 6,			// YYObjectBase* value 
    VALUE_INT32 = 7,			// Int32 value
    VALUE_VEC4 = 8,				// Vec4 (x,y,z,w) value (allocated from pool)
    VALUE_VEC44 = 9,			// Vec44 (matrix) value (allocated from pool)
    VALUE_INT64 = 10,			// Int64 value
    VALUE_ACCESSOR = 11,		// Actually an accessor
    VALUE_NULL = 12,			// JS Null
    VALUE_BOOL = 13,			// Bool value
    VALUE_ITERATOR = 14,		// JS For-in Iterator
    VALUE_REF = 15,				// Reference value
    VALUE_UNSET = 0x0ffffff		// Unset value (never initialized)
};
enum eVM_Type
{
    eVMT_Double = 0,
	eVMT_Float, 
	eVMT_Int,
	eVMT_Long,
	eVMT_Bool,
	eVMT_Variable,
	eVMT_String,
	eVMT_GMDebug_StringPatch,	
	eVMT_Delete,
	eVMT_Undefined,
	eVMT_PtrType,
	eVMT_Error = 0xf,			
};
enum EventTriggers : uint32_t
{
    EVENT_OBJECT_CALL = 1,	// The event represents a Code_Execute() call.
    EVENT_FRAME = 2,		// The event represents an IDXGISwapChain::Present() call.
    EVENT_RESIZE = 3,		// The event represents an IDXGISwapChain::ResizeBuffers() call.
    EVENT_UNUSED = 4,		// This value is unused.
    EVENT_WNDPROC = 5		// The event represents a WndProc() call.
};
enum eBuffer_Type : int32_t
{
    eBuffer_None = 0x0,
    eBuffer_U8 = 0x1,
    eBuffer_S8 = 0x2,
    eBuffer_U16 = 0x3,
    eBuffer_S16 = 0x4,
    eBuffer_U32 = 0x5,
    eBuffer_S32 = 0x6,
    eBuffer_F16 = 0x7,
    eBuffer_F32 = 0x8,
    eBuffer_D64 = 0x9,
    eBuffer_Bool = 0xA,
    eBuffer_String = 0xB,
    eBuffer_U64 = 0xC,
    eBuffer_Text = 0xD,
};
enum eBuffer_Format {
    eBuffer_Fixed = 0,
    eBuffer_Grow = 1,
    eBuffer_Wrap = 2,
    eBuffer_Fast = 3,
    eBuffer_VBuffer = 4,
    eBuffer_Network = 5,
};
enum eBuffer_Seek : int32_t
{
    eBuffer_Start = 0x0,
    eBuffer_Relative = 0x1,
    eBuffer_End = 0x2,
};

union FunctionUnion
{
    PFUNC_YYGMLScript m_ScriptFunction;
    PFUNC_YYGML m_CodeFunction;
    PFUNC_RAW m_RawFunction;
};
union Objects {
    struct YYObjectBase* m_Object;
    struct CInstance* m_Instance;
};
union RValueValue {
    int m_i32;
    __int64 m_i64;
    double m_Real;
    Objects _objects;
    void* m_Pointer;
};
union ScriptIndex
{
    char* m_Script;
    int m_CompiledIndex;
};
union LayerElementFlink
{
    CLayerInstanceElement* m_InstanceFlink;
    CLayerSpriteElement* m_SpriteFlink;
    CLayerElementBase* m_Flink;
};
union LayerElementBlink
{
    CLayerInstanceElement* m_InstanceBlink;
    CLayerSpriteElement* m_SpriteBlink;
    CLayerElementBase* m_Blink;
};