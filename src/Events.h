#pragma once

#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl_private.h"
#include "zhl.h"
#include "Globals.h"
#include <shared_mutex>

/*
EVENT_CREATE      =     0
EVENT_DESTROY     =     1
EVENT_ALARM       =     2
EVENT_STEP        =     3
EVENT_COLLISION   =     4
EVENT_KEYBOARD    =     5
EVENT_MOUSE       =     6
EVENT_OTHER       =     7
EVENT_DRAW        =     8
EVENT_KEYPRESS    =     9
EVENT_KEYRELEASE  =    10
EVENT_CLEAN_UP	  =    12
EVENT_PRE_CREATE  =    14
*/


enum EventDetailType : int32_t
{
    EV_ENM,
    EV_WEAPON,
    EV_PLAYER
};
inline bool volatile ExecuteItRunning(CCode* code)
{
    static std::unordered_map<int32_t, bool> isExecuteItRunningMap;
    return isExecuteItRunningMap.contains((int32_t const) code->m_CodeIndex);
}
inline int32_t EventCodeFromName(std::string const& name)
{
    switch(name[0] & ~32)
    {
        case 'C': 
            switch (name[1] & ~32)
            {
                case 'R': return EVENT_CREATE; 
                case 'O': return EVENT_COLLISION;
                case 'L': return EVENT_CLEAN_UP;
            }
            break;
        case 'D': 
            if ((name[1] & (~32)) == 'E') return EVENT_DESTROY;
            else if ((name[1] & (~32)) == 'R') return EVENT_DRAW;
        case 'A': return EVENT_ALARM;
        case 'S': return EVENT_STEP;
        case 'M': return EVENT_MOUSE;
        case 'K': 
            switch (name[1] & ~32) 
            {
                case 'B': return EVENT_KEYBOARD;
                case 'P': return EVENT_KEYPRESS;
                case 'R': return EVENT_KEYRELEASE;
            }
        case 'O': return EVENT_OTHER;
        case 'P': return EVENT_PRE_CREATE;
    }
    return -1;
}

//? for generating event names for user display. Is intended to just return SOMETHING so we don't crash.
inline std::string eventTypeName(int32_t eventCode)
{
    switch(eventCode)
    {
        case 0: return "Create";
        case 1: return "Destroy";
        case 2: return "Alarm";
        case 3: return "Step";
        case 4: return "Collision";
        case 5: return "Keyboard";
        case 6: return "Mouse";
        case 7: return "Other";
        case 8: return "Draw";
        case 9: return "Key Press";
        case 10: return "Key Release";
        case 12: return "Cleanup";
        case 14: return "Pre-Create";
        default: return "Unknown";
    }
}

//? for generating event names for user display. Is intended to just return SOMETHING so we don't crash.
inline std::string eventSpecificName(int32_t eventCode, int32_t subCode, EventDetailType detailType = EV_WEAPON)
{
    char subCodeBuf[] = { static_cast<char>(subCode & 0xFF), '\0' };
    switch(eventCode)
    {
        case 2: return "Alarm " + std::to_string(subCode);
        case 3:
        {
            switch(subCode)
            {
                case 0: return "Step - Begin";
                case 1: return "Step - Normal";
                case 2: return "Step - End";
                default: return "Step " + std::to_string(subCode);
            }
        }
        case 5: return "Key " + std::string(subCodeBuf) + " Press";
        case 6:
        {
            switch(subCode)
            {
                case 0: return "Mouse Left Button";
                case 1: return "Mouse Right Button";
                case 2: return "Mouse Middle Button";
                case 3: return "Mouse Wheel Up";
                case 4: return "Mouse Wheel Down";
                default: return "Mouse Button " + std::to_string(subCode);
            }
        }
        case 7:
        {
            switch(subCode)
            {
                case 4: return "Other - Room Start";
                case 5: return "Other - Room End";
                case 10: return "Other - User 1";
                case 11: return "Other - User 1";
                case 12: return "Other - User 2";
                case 13: return "Other - User 3";
                case 14: return "Other - Apply Variant (User 4)";
                case 15: return "Other - User 5";
                case 16: return "Other - User 6";
                case 17: return "Other - User 7";
                case 18: return "Other - User 8";
                case 19: return "Other - User 9";
                case 20: return "Other - User 10";
                case 21: return "Other - User 11";
                case 22: return "Other - User 12 (Network)";
                case 23: return "Other - User 13";
                case 24: return "Other - User 14";
                case 25: return "Other - User 15";
                default: return "Other " + std::to_string(subCode);
            }
        }
        case 9: return "Key " + std::string(subCodeBuf) + " Press";
        case 10: return "Key " + std::string(subCodeBuf) + " Release";
        default: return eventTypeName(eventCode) + " (" + std::to_string(subCode) + ")";
    }
}
