#pragma once
#include "Synthetik.h"
#include "zhl_internal.h"
#include <string>
#include <vector>
#include <memory>
#include "UIManager.h"

namespace Organik
{

    class ChatBox : public UIElement
    {
        private:
            std::mutex tabsListLock;
        public:
            ChatBox() ;
            ChatBox(const std::string& name);
            ~ChatBox();

            static bool g_ShowGlobalChat;
            static bool g_CloseGlobalChat;
            static bool g_GrabInput;
            void Draw(bool& out_mousedOver, bool* p_open, const std::string &title = "") override;

            #ifndef ORGANIK_CHAT_TABS
            #define ORGANIK_CHAT_TABS        
            template <typename T>
            requires std::is_base_of_v<ChatTab, T>
            void AddTab(T* tab)
            {
                if (!tab) return;
                int32_t hash = tab->GetHash();
                std::lock_guard<std::mutex> lock(tabsListLock);
                auto it = tabs.find(hash);
                if (it != tabs.end())
                {
                    // Tab with this hash already exists
                    return;
                } 
                tabs.insert({hash, std::unique_ptr<T>(tab)});
            }
            template <typename T>
            requires std::is_base_of_v<ChatTab, T>
            void RemoveTab(T* tab)
            {
                if (!tab) return;
                int32_t hash = tab->GetHash();
                std::lock_guard<std::mutex> lock(tabsListLock);
                tabs.erase(hash);
            }

            template <typename T>
            requires std::is_base_of_v<ChatTab, T>
            _NODISCARD_LOCK std::pair<std::unique_lock<std::mutex>, T*> GetTab(int32_t hash = 0) {
                std::lock_guard<std::mutex> lock(tabsListLock);
                int32_t searchHash = hash ? hash : typeid(T).hash_code();
                auto it = tabs.find(searchHash);

                if (it != tabs.end()) {
                    T* tab = dynamic_cast<T*>(it->second.get());
                    if (tab) {
                        return
                        {
                            std::move(tab->GetLock()),
                            tab
                        };
                    }
                }
                
                static std::mutex noReturn;
                std::unique_lock<std::mutex> noLock(noReturn);
                
                return std::make_pair(std::move(noLock), (T*) nullptr);
            }
            #endif // ORGANIK_CHAT_TABS
                
            static ChatBox* GetInstance()
            {
                static ChatBox* chatInstance = nullptr;
                if (!chatInstance)
                {
                    chatInstance = UIManager::GetInstance()->GetElement<ChatBox>(true, "Chat Box");
                }
                return chatInstance;
            }
        
    private:
        std::string name;
        std::unordered_map<int32_t, std::unique_ptr<ChatTab>> tabs;
        int activeTabIndex = 0;
    };
    

}