#pragma once


/* 
 * Basically, this file is 98% credit to YYToolkit/Archie, 2% me disassembling/reversing yoyo.lib and synthetik.exe
*/
#define FLAG_MAP 1<<0
#define FLAG_TRIGGER 1<<1
#define FLAG_ACTION 1<<2
#define YEXTERN 
#define YYCEXPORT 
#define VMExec void*
#define EVENT_HASH(EV,SUBEV) ((int32_t)(EV) << 16 | (int32_t)(SUBEV & 0xFFFF))
#define EVENT_CREATE                     0
#define EVENT_DESTROY                    1
#define EVENT_ALARM                      2
#define EVENT_STEP                       3
#define EVENT_COLLISION                  4
#define EVENT_KEYBOARD                   5
#define EVENT_MOUSE                      6
#define EVENT_OTHER                      7
#define EVENT_DRAW                       8
#define EVENT_KEYPRESS                   9
#define EVENT_KEYRELEASE                10
#define EVENT_CLEAN_UP	                12
#define EVENT_PRE_CREATE                14

#define SUBEV_STEP_NORMAL                0
#define SUBEV_STEP_BEGIN                 1
#define SUBEV_STEP_END                   2
#define SUBEV_ROOM_START                 4
#define SUBEV_ROOM_END                   5
#define SUBEV_USER0                     10
#define SUBEV_USER1                     11
#define SUBEV_USER2                     12
#define SUBEV_USER3                     13
#define SUBEV_USER4                     14
#define SUBEV_USER5                     15
#define SUBEV_USER6                     16
#define SUBEV_USER7                     17
#define SUBEV_USER8                     18
#define SUBEV_USER9                     19
#define SUBEV_USER10                    20
#define SUBEV_USER11                    21
#define SUBEV_ALARM0                     0
#define SUBEV_ALARM1                     1
#define SUBEV_ALARM2                     2
#define SUBEV_ALARM3                     3
#define SUBEV_ALARM4                     4
#define SUBEV_ALARM5                     5
#define SUBEV_ALARM6                     6
#define SUBEV_ALARM7                     7
#define SUBEV_ALARM8                     8
#define SUBEV_ALARM9                     9
#define SUBEV_ALARM10                   10
#define SUBEV_ALARM11                   11
#define SUBEV_USER12                    22
#define SUBEV_USER13                    23
#define SUBEV_USER14                    24
#define SUBEV_USER15                    25
#ifndef CONCATENATE
#define CONCATENATE(s1, s2) s1##s2
#define EXPAND_THEN_CONCATENATE(s1, s2) CONCATENATE(s1, s2)
#endif
#define USER(EVT)                       EXPAND_THEN_CONCATENATE(SUBEV_USER, EVT)
#define STEP(EVT)                       EXPAND_THEN_CONCATENATE(SUBEV_STEP_, EVT)
#define ROOM(EVT)                       EXPAND_THEN_CONCATENATE(SUBEV_ROOM_, EVT)
#define EVENT(EVT)                      EXPAND_THEN_CONCATENATE(EVENT_, EVT)
#define SUBEVENT(EVT)                   EXPAND_THEN_CONCATENATE(SUBEV_, EVT)
#define MAKECODE(EVT, SUBEVT)           ((int32_t)(EVT & 0xFFFF) << 16 | (int32_t)(SUBEVT & 0xFFFF))
#define STEP_MAIN                       MAKECODE(EVENT(STEP), SUBEVENT(STEP_NORMAL))
#define STEP_BEGIN                      MAKECODE(EVENT(STEP), SUBEVENT(STEP_BEGIN))
#define STEP_END                        MAKECODE(EVENT(STEP), SUBEVENT(STEP_END))
#define ROOM_START                      MAKECODE(EVENT(STEP), SUBEVENT(ROOM_START))
#define ROOM_END                        MAKECODE(EVENT(STEP), SUBEVENT(ROOM_END))
#define UPDATE                          MAKECODE(EVENT(OTHER), USER(0))
#define PACKETRCV                       MAKECODE(EVENT(OTHER), USER(12))
#define ENEMY_STATE0                    MAKECODE(EVENT(OTHER), USER(0))
#define WEAP_SYNCSTATS                  MAKECODE(EVENT(OTHER), USER(1))
#define SHOT_HIT                        MAKECODE(EVENT(OTHER), USER(15))
#define WEAP_PICKUP                     MAKECODE(EVENT(OTHER), USER(3))
#define WEAP_UPDATE_ATTACH              MAKECODE(EVENT(OTHER), USER(4))
#define WEAP_SET_VARIANT                MAKECODE(EVENT(OTHER), USER(5))
#define DESTROY                         MAKECODE(EVENT(DESTROY), 0)
#define SUBEV_APPEXIT                   30
#define SUBEV_POSITION                  40

#define SUBEV_WEB_IMAGE_LOAD            60
#define SUBEV_WEB_SOUND_LOAD            61
#define SUBEV_WEB_ASYNC                 62

#define SUBEV_DIALOG_ASYNC              63
#define SUBEV_WEB_NETWORKING            68
#define SUBEV_WEB_STEAM                 69

#define SUBEV_DRAW_GUI                  64
#define SUBEV_DRAW_RESIZE               65

#define SUBEV_DRAW_BEGIN                72
#define SUBEV_DRAW_END                  73
#define SUBEV_DRAW_GUI_BEGIN            74
#define SUBEV_DRAW_GUI_END              75
#define SUBEV_DRAW_PRE                  76
#define SUBEV_DRAW_POST                 77
#define DRAW_GUI                        SUBEV_DRAW_GUI
#define DRAW_RESIZE                     SUBEV_DRAW_RESIZE
#define DRAW_BEGIN                      SUBEV_DRAW_BEGIN
#define DRAW_END                        SUBEV_DRAW_END
#define DRAW_GUI_BEGIN                  SUBEV_DRAW_GUI_BEGIN
#define DRAW_GUI_END                    SUBEV_DRAW_GUI_END
#define DRAW_PRE                        SUBEV_DRAW_PRE
#define DRAW_POST                       SUBEV_DRAW_POST

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

#define	DEG(_ang)			((Pi/180.0f)*(_ang))
#define	RAD(_ang)			((180.0f/Pi)*(_ang))
#define	ORD(c)			    ((Byte)(c))
#define	CHR(c)			    ((char)(c))

#ifndef UTEXT
#define UTEXT(x) ((const unsigned char*)(x))
#endif // UTEXT
typedef bool (*FNGetVariable)(CInstance* self, int index, RValue *val);
typedef bool (*FNSetVariable)(CInstance* self, int ind, RValue *val);

#ifndef NULL_INDEX
#define NULL_INDEX INT_MIN
#endif // NULL_INDEX
#ifndef HASH_DELETED
#define HASH_DELETED 0x80000000
#endif // HASH_DELETED
#ifndef HASH_NULL
#define HASH_NULL 0
#endif // HASH_NULL

#define VARIANT_DESC_MAX_LENGTH 512



typedef void* HYYMUTEX;
typedef void* HSPRITEASYNC;
struct HTTP_REQ_CONTEXT;
typedef int (*PFUNC_async)(HTTP_REQ_CONTEXT* _pContext, void* _pPayload, int* _pMap);
typedef void (*PFUNC_cleanup)(HTTP_REQ_CONTEXT* _pContext);
typedef void (*PFUNC_process)(HTTP_REQ_CONTEXT* _pContext);

typedef void (*TSetRunnerInterface)(const YYRunnerInterface* pRunnerInterface, size_t _functions_size);
typedef void (*TYYBuiltin)(RValue* Result, CInstance* selfinst, CInstance* otherinst, int argc, RValue* arg);
typedef void (*TRoutine)(RValue& _result, CInstance* _self, CInstance* _other, int _argc, RValue _args[]);
_NODISCARD_LOCK std::pair<std::unique_lock<std::mutex>, std::unordered_map<int32_t, CInstance*>> GetActiveInstances();
template <typename T>
requires std::is_invocable_r_v<bool, T, CInstance*>
static CInstance* GetFirstInstanceOrDefault(T predicate, CInstance* defaultInstance = nullptr)
{
    for (auto& [id, instance] : GetActiveInstances())
    {
        if (predicate(instance))
        {
            return instance; // Return the first match
        }
    }
    return defaultInstance;
}

template <typename T>
requires std::is_invocable_r_v<bool, T, CInstance*>
static std::vector<CInstance*> GetInstancesWhere(T predicate)
{
    std::vector<CInstance*> result;
    for (auto& [id, instance] : GetActiveInstances())
    {
        if (predicate(instance))
        {
            result.push_back(instance);
        }
    }
    return result;
}

template <typename T>
struct CArrayStructure {
    char padding[0x8];

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
{std::string_view("a").at(0);
    requires std::_Is_any_of_v<T, 
        int, long, long long, float, double, 
        short, char, unsigned char, 
        unsigned int, unsigned long, 
        unsigned long long, unsigned short>;
};

typedef void(*PFUNC_YYGML)(CInstance* Self,CInstance* Other);

using PFUNC_RAW = void(*)();

using PFUNC_YYGMLScript = RValue & (__cdecl*)(
		CInstance* Self,
		CInstance* Other,
		RValue* Result,
		int ArgumentCount,
		RValue* Arguments[]
);

typedef int64_t* PQWORD;
#define HIDWORD(ll) ((DWORD)((*((PQWORD)(ll)) >> 32) & 0xffffffff))
#define PTR_HIDWORD(p_LL) ((PDWORD)&(((char*)(p_LL))[0]))
#define LODWORD(ll) ((DWORD)((*((PQWORD)(ll)) & 0xffffffff)))
#define PTR_LODWORD(p_LL) ((PDWORD)&(((char*)(p_LL) + 4)[0]))

using PFN_YYObjectBaseAdd = void(__thiscall*)(YYObjectBase* This, const char* Name, RValue* Value, int Flags);

typedef int (*FNGetOwnProperty)(struct YYObjectBase *, struct RValue *, char *);

typedef void (*FNDeleteOwnProperty)(struct YYObjectBase *obj, struct RValue *result, const char *name, bool unk);

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
struct RefDynamicArrayOfRValue;
struct ListCInstance {
	
};
// typedef CHashMapElement<int, RValue*> CHashMapElementRValuePtr;
#define MASK_KIND_RVALUE 0x0ffffff
enum RValueType : uint32_t
{
    VALUE_REAL = 0U,			
    VALUE_STRING = 1U,		
    VALUE_ARRAY = 2U,		
    VALUE_PTR = 3U,			
    VALUE_VEC3 = 4U,			
    VALUE_UNDEFINED = 5U,	
    VALUE_OBJECT = 6U,		
    VALUE_INT32 = 7U,		
    VALUE_VEC4 = 8U,			
    VALUE_VEC44 = 9U,		
    VALUE_INT64 = 10U,		
    VALUE_ACCESSOR = 11U,	
    VALUE_NULL = 12U,		
    VALUE_BOOL = 13U,		
    VALUE_ITERATOR = 14U,    
    VALUE_REF = 15U,
    VALUE_MAP = 16U,
    VALUE_VECTOR = 17U,
    VALUE_FUNCTION = 18U,
    VALUE_TRIGGER = 19U,
    VALUE_ACTION = 20U,
    VALUE_PARAMS = 21U,
    VALUE_VARREF = 22U,
    VALUE_ACTIONARRAY = (uint32_t) ((VALUE_ARRAY << 16) | VALUE_FUNCTION),
    VALUE_UNSET = 0x0ffffff 
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
#define VAR_HASH(var) Organik::Variables::Hashes[Organik::Variables::##var]

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
    
typedef bool(*PFN_ACTIONHANDLER)(CInstance* self, CInstance* other, YYObjectBase* context);

