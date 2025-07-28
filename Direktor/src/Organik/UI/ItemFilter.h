#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include <vector>
#include <string>
#include <regex>

template <typename T, typename Func>
requires (std::is_pointer_interconvertible_base_of_v<CInstance, T> ||
            std::is_pointer_interconvertible_base_of_v<YYObjectBase, T> ||
            std::is_pointer_interconvertible_base_of_v<CInstanceBase, T> ||
            std::is_pointer_interconvertible_base_of_v<CObjectGM, T>) &&
            (std::is_invocable_r_v<bool, Func, T>)
struct ItemFilter
{
    std::vector<int32_t> variableFilters;
    std::vector<std::string> textFilters;
    std::vector<Func> filterFunctions;
    enum {
        SortByName = 1<<0,
        SortByValue = 1<<1,
        SortByType = 1<<2,

        ASC = 1<<4,
        DESC = 1<<5
    } sortMode = SortByName;


    ItemFilter(const std::vector<T>& items, const std::string& filterText)
        : items(items), filterText(filterText) {}

    bool Matches(const T& item) const
    {
        if (filterText.empty())
            return true;

        // Use regex to match the filter text against the item's name
        std::regex filterRegex(filterText, std::regex_constants::icase);
        return std::regex_search(item->m_Name, filterRegex);
    }

    std::vector<T> Filter() const
    {
        std::vector<T> filteredItems;
        for (const auto& item : items)
        {
            if (Matches(item))
            {
                filteredItems.push_back(item);
            }
        }
        return filteredItems;
    }
}