#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <unordered_map>

HOOK_METHOD(CRoom, DeleteInstance, (CInstance* self, bool removeGlobal) -> void)
{
    if (self == nullptr)
    {
        std::cerr << "Attempted to delete a null instance." << std::endl;
        return;
    }
    if (!GetActiveInstances().contains(self->m_ID))
    {
        std::cerr << "Tried to remove " << self->m_ID << " but it is not in the instances list." << std::endl;
        super(self, removeGlobal);
        return;
    }
    auto it = GetActiveInstances().find(self->m_ID);
    if (it != GetActiveInstances().end())
    {
        GetActiveInstances().erase(it);
        std::cout << "Instance with ID " << self->m_ID << " removed from instance viewer list." << std::endl;
    }
    // Call the original method to handle the rest of the deletion logic
    super(self, removeGlobal);
}


//re-created from the disassembly, in an attempt to trace the logic as it runs
HOOK_METHOD(CRoom, UpdateActive, (void) -> void)
{
    std::cout << "Updating active instances in the room:" << std::endl;
    CInstance* cur = this->m_Deactive.m_First;
    CInstance* other = nullptr;
    CInstance* theOneSheSaidNotToWorryAbout = nullptr;
    while (true)
    {
        do
        {
            other = cur;
            if (other == nullptr)
            {
                cur = this->m_Active.m_First;
                while (other = cur, other != nullptr) // I didn't even know this was legal syntax
                {
                    cur = other->m_Next;
                    if (other->m_InstanceFlags >> 1 & 1)
                    {
                        if (other->m_Prev == nullptr) {
                            this->m_Active.m_First = other->m_Next;
                        } else {
                            other->m_Prev->m_Next = other->m_Next;
                        }
                        if (other->m_Next == nullptr) {
                            this->m_Active.m_Last = other->m_Prev;
                        } else {
                            other->m_Next->m_Prev = other->m_Prev;
                        }
                        other->m_Next = nullptr;
                        other->m_Prev = nullptr;
                        this->m_Active.m_Count--;
                        this->m_Deactive.m_Count++;
                        theOneSheSaidNotToWorryAbout = this->m_Deactive.m_Last;
                        if (theOneSheSaidNotToWorryAbout == nullptr)
                        {
                            this->m_Deactive.m_First = other;
                            this->m_Deactive.m_Last = other;
                            other->m_Prev = nullptr;
                        } else {
                            theOneSheSaidNotToWorryAbout->m_Next = other;
                            other->m_Prev = this->m_Deactive.m_Last;
                            this->m_Deactive.m_Last = other;
                        }
                        other->m_Next = nullptr;
                    }
                }
            }
        } while ((cur->m_InstanceFlags >> 1 & 1) != 0);
    }
}

// I'm pretty sure this is for collision detection? Since it references the sprites.
HOOK_GLOBAL(MarkInstancesAsDirty, (int spriteIndex) -> void)
{
    // This function marks all instances of a given sprite as dirty
    for (auto& [id, instance] : GetActiveInstances())
    {
        if (instance->m_SpriteIndex == spriteIndex)
        {
            instance->m_InstanceFlags |= 0x1; // Assuming 0x1 is the dirty flag
            std::cout << "Marked instance with ID " << id << " as dirty." << std::endl;
        }
    }
}