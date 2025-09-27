#pragma once
#include "Synthetik.h"
#include <string>
#include <vector>
#include "imgui/imgui.h"
#include "imgui/imgui_stdlib.h"
void ChatAddHooks();
namespace Organik
{
    struct Message 
    {
        uint32_t color;
        std::string name;
        std::string text;
        Message(uint32_t color, const std::string& name, const std::string& text)
            : color(color), name(name), text(text) {}

    };
    class ChatTab : public UIElement
    {
    protected:
        std::mutex selfLock;
        std::mutex messagesLock;
        bool hasUnread = false;

        // we protect these constructors to ensure only subclasses can instantiate
        ChatTab() = default;
        ChatTab(int32_t instanceID, const std::string& name) : instanceID(instanceID), name(name) {};

    public:
        ChatTab(const ChatTab& other) = delete;
        ChatTab& operator=(const ChatTab& other) = delete;
        ChatTab(ChatTab&& other) noexcept;
        ChatTab&& operator=(ChatTab&& other) noexcept;
        virtual ~ChatTab() = default;

        virtual void Draw(bool& out_mousedOver, bool* p_open) = 0;
        virtual void Send(int32_t icon, const std::string& name, const std::string& message) = 0;
        virtual void Receive(int32_t icon, const std::string& name, const std::string& message) = 0;
        void Clear();

        virtual void GetName(char* buf) = 0;
        virtual inline std::string& GetInputBuffer() { return inputBuffer; }

        virtual inline int32_t GetHash() = 0;

        inline bool& GetUnreadFlag() { return hasUnread; }
        inline void SetUnreadFlag(bool setFlag) { hasUnread = setFlag; }
        inline const int32_t GetInstanceID() const { return instanceID; }
        inline std::unique_lock<std::mutex> GetLock() { return std::move(std::unique_lock<std::mutex>(selfLock)); }
    protected:
        std::vector<Message*> messages;
        
        
    private:
        int32_t instanceID;
        std::string name;
        std::string inputBuffer;
    };
}