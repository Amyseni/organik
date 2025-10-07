#pragma once    
#include <string>
#include <unordered_map>
#include "Globals.h"

#define DISPLAYFLAGS_DO(thing) \
    thing(DISPLAY_FLATNUM, 1, _flat) \
    thing(DISPLAY_PERCENT, 2, _percent) \
    thing(DISPLAY_MULTIPLIER, 3, _multiplier) \
    thing(DISPLAY_ROTATION, 4, _degrees) \
    thing(DISPLAY_TIME, 5, _time) \
    thing(DISPLAY_HEX, 6, _hex) \
    \
    thing(TYPE_INT, 1<<8, _int) \
    thing(TYPE_REAL, 2<<8, _real) \
    thing(TYPE_STRING, 3<<8, _text) \
    thing(TYPE_BOOL, 4<<8, _boolean) \
    thing(TYPE_COLOR, 5<<8, _color) \
    \
    thing(TYPE_GOOD_POSITIVE, 1<<16, _positive) \
    thing(TYPE_GOOD_NEGATIVE, 2<<16, _negative) \
    thing(TYPE_SPECIAL, 3<<16, _special) \
    \
    thing(TYPE_ARRAY, 1u << 31, _array) \



#define MAKE_FLAGSENUM(x,y,z) x = y,
#define MAKE_MAPSTRING(x,y,z) {Code_Variable_FindAlloc_Slot_From_Name(nullptr, #z), VariableDef::x},

struct VariableDef
{
    enum DisplayFlags : uint32_t
    {
        DISPLAYFLAGS_DO(MAKE_FLAGSENUM)
    } m_DisplayFlags;
    int32_t m_ID = 0;
    std::string m_Name;
    std::string m_Description;
    const std::string GetRepresentation(const RValue& value) const;
    void DrawDisplayValueInTable(RValue* value);
    void DrawEditorInPopup(RValue* value);

};

const std::unordered_map<uint32_t, int32_t>* gsGetDisplayTypeMap(bool _const = true);
std::unordered_map<uint32_t, int32_t>& gsGetDisplayTypeMap(int32_t _const );

struct VariableDefHelper {
    bool m_isInitialized = false;
    std::unordered_map<int32_t, VariableDef> m_VariableDefs;
    std::filesystem::path m_DefFilePath;
        static inline const int GetDisplayFormatCount()
    {
        return 6;
    }
    static inline const int* const GetDisplayFormatValues()
    {
        static int values[] = {
            VariableDef::DISPLAY_FLATNUM,
            VariableDef::DISPLAY_PERCENT,
            VariableDef::DISPLAY_MULTIPLIER,
            VariableDef::DISPLAY_ROTATION,
            VariableDef::DISPLAY_TIME,
            VariableDef::DISPLAY_HEX
        };
        return values;
    }
    static inline const int GetDisplayTypeCount()
    {
        return 5;
    }
    static inline const int* const GetDisplayTypeValues()
    {
        static int values[] = {
            VariableDef::TYPE_INT,
            VariableDef::TYPE_REAL,
            VariableDef::TYPE_STRING,
            VariableDef::TYPE_BOOL,
            VariableDef::TYPE_COLOR
        };
        return values;
    }
    static inline const int GetDisplaySpecialCount()
    {
        return 3;
    }
    static inline const int* const GetDisplaySpecialValues()
    {
        static int values[] = {
            VariableDef::TYPE_GOOD_POSITIVE,
            VariableDef::TYPE_GOOD_NEGATIVE,
            VariableDef::TYPE_SPECIAL
        };
        return values;
    }
    static inline const char* const* GetPositiveNegativeListForDisplay()
    {
        static const char* const list[] = {
            "Buffs are Positive",
            "Buffs are Negative",
            "Neutral/Special"
        };
        return list;
    }
    static inline const char* const* GetFormatListForDisplay()
    {
        static const char* const list[] = {
            "Flat (Add/Subtract)",
            "Percent (+/- %)",
            "Multiplier (x)",
            "Angle/Rotation (°)",
            "Time (s)",
            "Hexadecimal (0x)"
        };
        return list;
    }
    static inline const char* const* GetTypeListForDisplay()
    {
        static const char* const list[] = {
            "Integer (Whole Number e.g. 69)",
            "Real (Has Decimal e.g. 4.20)",
            "Text/String (For display e.g. 'Twin Link')",
            "True/False (Checkboxes)",
            "Color (3 Values Red,Green,Blue)"
        };
        return list;
    }
    static VariableDefHelper* Get();
    std::unordered_map<int32_t, VariableDef>* const GetVariableDefinitions();
    const std::unordered_map<int32_t, VariableDef>* const GetVariableDefinitions() const;

    void SetPath(const std::filesystem::path& path);
    bool Load();
    bool Init();
    bool Save();
    inline bool IsLoaded() {
        return m_isInitialized;
    }
    
    const int32_t GetDisplayFlagsForVar(int32_t varID) const;
    const int32_t GetDisplayFlagsForVar(const char* varName) const;
    const int32_t GetVarIDForName(const char* varName) const;
    const VariableDef& GetVarDefForID(int32_t varID);

    inline const int32_t GetDisplayFlagsForVar(const std::string& varName) const { return
        GetDisplayFlagsForVar(varName.c_str());
    }
    inline const int32_t GetVarIDForName(const std::string& varName) const { return
        GetVarIDForName(varName.c_str());
    }
    

    inline const VariableDef& GetVarDefForName(const char* varName) { return
        GetVarDefForID(GetVarIDForName(varName));
    }

    inline const VariableDef& GetVarDefForName(const std::string& varName) { return
        GetVarDefForID(GetVarIDForName(varName));
    }
private:
    VariableDefHelper();
    ~VariableDefHelper() = default;
};
