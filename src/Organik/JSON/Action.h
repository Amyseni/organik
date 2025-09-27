#pragma once
#include "Synthetik.h"
#include "zhl_internal.h"
#include "Globals.h"
#define FLAGS_MAP (1<<0)
#define FLAGS_ACTION (1<<2)
#define FLAGS_
#define REF_OBJ_NEW 0x40000000
#define LIMIT_RECURSION_GETVAR 25

inline double GetLocalLuckMod()
{
    CInstance *inst = CInstance::FirstOrDefault([&](CInstance* inst) { return inst && inst->m_ObjectIndex == Organik::Objects::obj_localPlayer; });
    if (inst)
    {
        RValue* tmp = inst->InternalReadYYVar(VAR_HASH(mod_luck));
        double mod_luck = tmp ? parseRValueNumber<double>(tmp) : 0.0;
        return mod_luck;
    }
    return 0.0;
}
inline void ParseJSONRValue(RValue* jsonValue, RValue* out);
// std::unordered_map<YYObjectBase* const, bool>* PatchedEventsThisIter();
void ClearPatchCache();
std::unordered_map<CCode*, bool>* PatchedCodeThisIter();
#define OBJ_HASH(obj) (Organik::Objects::ObjIndexes[Organik::Objects::##obj])
#define OBJ_EVENT(obj, event) ((int64_t)(OBJ_HASH(obj)) << 32) | event
static std::unordered_map<int32_t, int64_t> eventHashes;
inline std::unordered_map<int32_t, std::pair<int32_t, int32_t>>* GetEventHashes()
{
    static std::unordered_map<int32_t, std::pair<int32_t, int32_t>> objectEventHashes;
    if (objectEventHashes.empty())
    {
        objectEventHashes = {
            {VAR_HASH(ev_create),            {EVENT(CREATE),     0}},
            {VAR_HASH(ev_step_main),         {EVENT(STEP),  SUBEV_STEP_NORMAL}  },
            {VAR_HASH(ev_step_begin),        {EVENT(STEP),  SUBEV_STEP_BEGIN}   },
            {VAR_HASH(ev_step_end),          {EVENT(STEP),  SUBEV_STEP_END}     },
            {VAR_HASH(ev_room_start),        {EVENT(OTHER), SUBEV_ROOM_START}   },
            {VAR_HASH(ev_room_end),          {EVENT(OTHER), SUBEV_ROOM_END}     },
            {VAR_HASH(ev_user_1),            {EVENT(OTHER), USER(1)}            },
            {VAR_HASH(ev_user_2),            {EVENT(OTHER), USER(2)}            },
            {VAR_HASH(ev_user_3),            {EVENT(OTHER), USER(3)}            },
            {VAR_HASH(ev_user_4),            {EVENT(OTHER), USER(4)}            },
            {VAR_HASH(ev_user_5),            {EVENT(OTHER), USER(5)}            },
            {VAR_HASH(ev_user_6),            {EVENT(OTHER), USER(6)}            },
            {VAR_HASH(ev_user_7),            {EVENT(OTHER), USER(7)}            },
            {VAR_HASH(ev_user_8),            {EVENT(OTHER), USER(8)}            },
            {VAR_HASH(ev_user_9),            {EVENT(OTHER), USER(9)}            },
            {VAR_HASH(ev_user_10),           {EVENT(OTHER), USER(10)}           },
            {VAR_HASH(ev_user_11),           {EVENT(OTHER), USER(11)}           },
            {VAR_HASH(ev_user_12),           {EVENT(OTHER), USER(12)}           },
            {VAR_HASH(ev_user_13),           {EVENT(OTHER), USER(13)}           },
            {VAR_HASH(ev_user_14),           {EVENT(OTHER), USER(14)}           },
            {VAR_HASH(ev_user_15),           {EVENT(OTHER), USER(15)}           },
            {VAR_HASH(ev_weap_syncstats),    {EVENT(OTHER), USER(1)}            },
            {VAR_HASH(ev_weap_pickup),       {EVENT(OTHER), USER(3)}            },
            {VAR_HASH(ev_weap_update_attach),{EVENT(OTHER), USER(4)}            },
            {VAR_HASH(ev_weap_set_variant),  {EVENT(OTHER), USER(5)}            },
            {VAR_HASH(ev_bullet_hit),        {EVENT(OTHER), USER(15)}           },
        };
    }
    return &objectEventHashes;
}
#define EVENTS_DO(THING) \
    THING(ev_step_main) \
    THING(ev_step_begin) \
    THING(ev_step_end) \
    THING(ev_room_start) \
    THING(ev_room_end) \
    THING(ev_user_1) \
    THING(ev_user_2) \
    THING(ev_user_3) \
    THING(ev_user_4) \
    THING(ev_user_5) \
    THING(ev_user_6) \
    THING(ev_user_7) \
    THING(ev_user_8) \
    THING(ev_user_9) \
    THING(ev_user_10) \
    THING(ev_user_11) \
    THING(ev_user_12) \
    THING(ev_user_13) \
    THING(ev_user_14) \
    THING(ev_user_15) \
    THING(ev_weap_syncstats) \
    THING(ev_weap_pickup) \
    THING(ev_weap_update_attach) \
    THING(ev_weap_set_variant) \
    THING(ev_bullet_hit) \
    
#define EVENTS_OTHER_DO(THING) \
    THING(ev_step_main, STEP, STEP, NORMAL) \
    THING(ev_step_begin, STEP, STEP, BEGIN) \
    THING(ev_step_end, OTHER, STEP, END) \
    THING(ev_room_start, OTHER, ROOM , START) \
    THING(ev_room_end, OTHER, ROOM , END) \
    THING(ev_user_1, OTHER, USER, 1) \
    THING(ev_user_2, OTHER, USER, 2) \
    THING(ev_user_3, OTHER, USER, 3) \
    THING(ev_user_4, OTHER, USER, 4) \
    THING(ev_user_5, OTHER, USER, 5) \
    THING(ev_user_6, OTHER, USER, 6) \
    THING(ev_user_7, OTHER, USER, 7) \
    THING(ev_user_8, OTHER, USER, 8) \
    THING(ev_user_9, OTHER, USER, 9) \
    THING(ev_user_10, OTHER, USER, 10) \
    THING(ev_user_11, OTHER, USER, 11) \
    THING(ev_user_12, OTHER, USER, 12) \
    THING(ev_user_13, OTHER, USER, 13) \
    THING(ev_user_14, OTHER, USER, 14) \
    THING(ev_user_15, OTHER, USER, 15) \
    THING(ev_weap_syncstats, OTHER, USER, 1) \
    THING(ev_weap_pickup, OTHER, USER, 3) \
    THING(ev_weap_update_attach, OTHER, USER, 4) \
    THING(ev_weap_set_variant, OTHER, USER, 5) \
    THING(ev_bullet_hit15, OTHER, USER, 15) \

#define MAKE_REVERSEMAP(THING, EVT, SUBTYPE, SUB) {(EVENT(EVT) << 16) | (SUBTYPE(SUB) & 0xFFFF), #THING},

static std::unordered_map<int32_t, std::string> eventReverseMap = {
    EVENTS_OTHER_DO(MAKE_REVERSEMAP)
};
static const char* const eventNames[] = {
    EVENTS_DO(MAKE_STRINGS)
};
struct Action;
struct ActionLinks
{
    Action* m_Next;
    Action* m_Prev;


    ActionLinks() ;
    ActionLinks(Action* _head, Action* _next) ;
};
using TDoAction = bool(__fastcall*)(RValue* in, RValue* out);

struct Action : public CCode
{
    virtual ~Action() = default;
    static int32_t CreateCount;
    void Free()
    {
        this->~Action();
    }
    friend struct CCode;
    inline Action() : CCode(
        /*m_Watch */ false, /*m_Kind*/ 1, /*m_Compiled*/ 1, 
        /*m_Name*/ "ORGANIK ACTION", /*m_Functions*/ new YYGMLFuncs(), 
        /*m_Str*/ "", /*m_CodeIndex*/ (69420 + (++CreateCount)),
        /*m_LocalsCount*/ 1, /*m_ArgsCount*/ 0, /*m_Flags*/ 0, 
        /*m_Prototype*/ nullptr, /*m_Offset*/ 0, /*m_Token*/ RToken(), 
        /*m_Value*/ RValue(), /*m_VmInstance*/ nullptr, /*m_VmDebugInfo*/ nullptr,
        /*m_Code*/ "", /*m_Next*/ nullptr) 
    {
        this->m_Watch = false;
        this->m_Kind = 1;
        this->m_Compiled = 1;
        this->m_Name = "ORGANIK ACTION";
        this->m_Functions->m_CodeFunction = nullptr;
        this->m_Str = "";
        this->m_CodeIndex = 69420 + (++CreateCount);
        this->m_LocalsCount = 0;
        this->m_ArgsCount = 0;
        this->m_Flags = 0;
        this->m_Prototype = nullptr;
        this->m_Offset = 0;
        this->m_Token = RToken();
        this->m_Value = RValue();
        StructCreate(&(this->m_Value));
        StructCreate(this->m_Value.ToObject()->InternalGetYYVarRef(VAR_HASH(variables)));
        StructCreate(this->m_Value.ToObject()->InternalGetYYVarRef(VAR_HASH(parameters)));
        YYCreateString(this->m_Value.ToObject()->InternalGetYYVarRef(VAR_HASH(variables))->ToObject()
            ->InternalGetYYVarRef(VAR_HASH(debug)), "test localvars on fake events hack");
        *(this->m_Value.ToObject()->InternalGetYYVarRef(VAR_HASH(parameters))->ToObject()
            ->InternalGetYYVarRef(VAR_HASH(self))) = -2;
        *(this->m_Value.ToObject()->InternalGetYYVarRef(VAR_HASH(parameters))->ToObject()
            ->InternalGetYYVarRef(VAR_HASH(other))) = -2;
        
        this->m_VmInstance = nullptr;
        this->m_VmDebugInfo = nullptr;
    }

    inline void Append(Action* prev, Action* next)
    {
        if (prev && next)
        {
            prev->m_Next = next;
        }
    }
    inline static void Create(PFUNC_YYGML pfn, Action** out, const RValue& params = RValue(false))
    {
        if (!pfn) Error_Show_Action("Tried to create Action with null function pointer!", false, true);
        *out = new Action();
        (*out)->SetEventFunction(pfn);
        
    }
    inline static Action* Create(PFUNC_YYGML pfn, const RValue& params = RValue(false))
    {
        if (!pfn) Error_Show_Action("Tried to create Action with null function pointer!", false, true);
        Action* action = new Action();
        action->SetEventFunction(pfn);
        if (params.GetKind() == VALUE_OBJECT)
        {
            *(action->GetParams()) = params.ToObject();
        }
        return action;
    }
    Action(const Action& Other) = delete;
    Action& operator=(const Action& Other) = delete;
    Action(Action&& Other) noexcept = delete;
    Action& operator=(Action&& Other) noexcept = delete;

    inline Action* GetNext() 
    {
        return reinterpret_cast<Action*>(&this->m_Next);
    };
    inline void SetNext(Action* next)
    {
        *reinterpret_cast<Action**>(&this->m_Next) = next;
    }
    inline RValue* GetParams(){
        if (this->m_Value.GetKind() != VALUE_OBJECT) return nullptr;
        if (!this->m_Value.ToObject()) return nullptr;
        return this->m_Prototype->InternalGetYYVarRef(VAR_HASH(parameters));
    }
    inline RValue* GetLocalVariables(){
        if (this->m_Value.GetKind() != VALUE_OBJECT) return nullptr;
        if (!this->m_Value.ToObject()) return nullptr;
        return this->m_Prototype->InternalGetYYVarRef(VAR_HASH(variables));
    }
    inline PFUNC_YYGML GetEventFunction()
    {
        return *std::launder(&this->m_Functions->m_CodeFunction);
    }
    inline PFUNC_YYGML SetEventFunction(PFUNC_YYGML func)
    {
        return *std::launder(&this->m_Functions->m_CodeFunction) = func;
    }
    inline PFUNC_YYGMLScript GetScriptFunction()
    {
        return *std::launder(&this->m_Functions->m_ScriptFunction);
    }
    inline PFUNC_YYGMLScript SetScriptFunction(PFUNC_YYGMLScript func)
    {
        return *std::launder(&this->m_Functions->m_ScriptFunction) = func;
    }
    
    void InsertIntoEventMap(CHashMap<int64_t, CEvent*, 3>*& eventMap, int32_t eventCode, int32_t eventSubCode, int32_t objectOwner)
    {
        if (!eventMap) return;
        auto hash = CalculateEventHash(eventCode, eventSubCode);
        eventMap->InsertOrAssign(hash, new CEvent(this, objectOwner));
    }
    operator RValue() {
        RValue ret = RValue();
        *std::launder(&ret.m_Pointer) = reinterpret_cast<void*>(this);
        ret.m_Kind = VALUE_ACTION;
        return ret;
    }
};
Action **CurrentActionGlobal();
YYObjectBase* volatile* CurrentContextGlobal();
bool ParseActions(YYObjectBase* jsonValue, int32_t objectIndex, int32_t eventCode, int32_t eventSubCode, bool after);

std::unordered_map<int32_t, PFN_ACTIONHANDLER>* GetActionHandlers();
void RegisterActionTypeHandler(int32_t actionType, PFN_ACTIONHANDLER handler);
inline int32_t GetEventHash(std::pair<int32_t, int32_t> eventLookup)
{
    return CalculateEventHash(eventLookup.first, eventLookup.second);
}
bool doApplyVariables(CInstance* self, CInstance* other, YYObjectBase* context);
bool doCallScript(CInstance* self, CInstance* other, YYObjectBase* context);
bool doCallBuiltin(CInstance* self, CInstance* other, YYObjectBase* context);
bool doRequiresVars(CInstance* self, CInstance* other, YYObjectBase* context);
bool doGetFrom(CInstance* self, CInstance* other, YYObjectBase* context);
bool doRequiresChance(CInstance* self, CInstance* other, YYObjectBase* context);