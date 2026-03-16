#pragma once
#include <inttypes.h>
#include <concepts>
#include <type_traits>

enum tCallFlags : uint32_t
{
    _CDECL = 1u << 0,
    _STDCALL = 1u << 1,
    _THISCALL = 1u << 2,
    _FASTCALL = 1u << 3,
    _UPRG_CALLEE = 1u << 4,
    _UPRG_CALLER = 1u << 5,
    _RETVOID = 1u << 8,
    _RETLONGLONG = 1u << 9,
    _DOESNOTRETN = 1u << 10,
    _PASSEDpSTRUCT = 1u << 16,
};

struct TCallFlags
{
    tCallFlags value;
    constexpr operator uint32_t( ) const noexcept
    {
        return (value);
    }
    constexpr operator tCallFlags( ) const noexcept
    {
        return (value);
    }

    template<uint32_t v>
    consteval TCallFlags( ) : value(static_cast<tCallFlags>(v)) { }

    template<tCallFlags v>
    consteval TCallFlags( ) : value(v) { }

    constexpr TCallFlags(uint32_t v) noexcept : value(static_cast<tCallFlags>(v)) { }
    constexpr TCallFlags(tCallFlags v) noexcept : value(v) { }
    constexpr TCallFlags& operator=(const tCallFlags& v) noexcept
    {
        this->value = v;
        return (*this);
    }
    constexpr TCallFlags& operator=(const uint32_t& v) noexcept
    {
        this->value = static_cast<tCallFlags>(v);
        return (*this);
    }
    constexpr TCallFlags& operator|(this TCallFlags& me, const tCallFlags& other) noexcept
    {
        me.value = static_cast<tCallFlags>(static_cast<uint32_t>(me.value) | static_cast<uint32_t>(other));
        return me;
    }
    constexpr const TCallFlags& operator|(this const TCallFlags& me, const tCallFlags& other) noexcept
    {
        return TCallFlags((static_cast<uint32_t>(me.value) | static_cast<uint32_t>(other)));
    }
    constexpr TCallFlags& operator|=(const tCallFlags& other) noexcept
    {
        this->value = static_cast<tCallFlags>(static_cast<uint32_t>(this->value) | static_cast<uint32_t>(other));
        return (*this);
    }
    constexpr TCallFlags& operator|=(const uint32_t& other) noexcept
    {
        this->value = static_cast<tCallFlags>(static_cast<uint32_t>(this->value) | other);
        return (*this);
    }
};
template<tCallFlags sig, uintptr_t Offset, typename Ret, typename FnType, typename... Args>
struct FnBinding
{
    using function_type = std::nullptr_t;
    using wrapper_type = Ret(__stdcall*)(Args...);
    wrapper_type* wrapper = nullptr;
    const std::string_view name;
    constexpr const uintptr_t offset = Offset;
    constexpr const tCallFlags GetCallFlags( )
    {
        tCallFlags flags = sig;
        flags |= std::is_void_t<Ret> ? tCallFlags::_RETVOID : 0u;
        flags |= (sizeof(Ret) == sizeof(long long)) ? tCallFlags::_RETLONGLONG : 0u;
        return auto(flags);
    };
    constexpr const size_t ArgCount = sizeof...(Args);
    static void* const ptr;
    operator wrapper_type& ()
    {
        if (!wrapper) wrapper = new std::function<wrapper_type>([&] (Args... args) -> Ret
        {
            return (*reinterpret_cast<function_type*>(std::launder(ptr)))(std::forward<Args>(args)...);
        });
        return *wrapper;
    }
    constexpr FnBinding(std::string_view const& _name) : name(_name)
    {

    }
};
template <tCallFlags sig, uintptr_t Offset, typename Ret, typename FnType, typename... Args>
void* const FnBinding<sig, Offset, Ret, FnType, Args...>::ptr = offset + GameModuleBase;

template<uintptr_t Offset, typename Ret, typename... Args>
struct FnBinding<tCallFlags::_CDECL, Offset, Ret, Ret(__cdecl*)(Args...), Args...>
{
    using function_type = Ret(__cdecl*)(Args...);
    using wrapper_type = Ret(__stdcall*)(Args...);
};

template<uintptr_t Offset, typename Ret, typename... Args>
struct FnBinding<tCallFlags::_STDCALL, Offset, Ret, Ret(__stdcall*)(Args...), Args...>
{
    using function_type = Ret(__stdcall*)(Args...);
    using wrapper_type = Ret(__stdcall*)(Args...);
};

template<uintptr_t Offset, typename Ret, typename ClassName, typename... Args>
struct FnBinding<tCallFlags::_THISCALL, Offset, Ret, Ret(__thiscall ClassName::*)(Args...), Args...>
{
    using function_type = Ret(__thiscall ClassName::*)(Args...);
    using wrapper_type = Ret(__thiscall ClassName::*)(Args...);
    operator wrapper_type& ()
    {
        return *(wrapper_type*) &ptr;
    }
};

/**
 * @brief it's official, I hate modern C++
 *
 * @tparam sig tCallFlags constant with ONLY a call signature flag set. Setting the ret type flags is undefined behavior
 * @tparam Ret return type as defined by std::invoke_result<Ret(*)(Args...), Args...>::type
 * @tparam Args list of argument types
 * @tparam Binding a type definition for this binding type. REQUIRES calling signature to be explicit on both function_type and wrapper_type
 */
template <typename FnType, tCallFlags sig, size_t Offset, typename Ret, typename... Args, template <typename, tCallFlags, size_t, typename, typename> typename Binding>
concept FunctionDefinition = requires(Binding<FnType, sig, Offset, Ret, Args...>&binding)
{
    typename Binding<sig, Offset, Ret, FnType, Args...>;
    typename Binding<sig, Offset, Ret, FnType, Args...>::function_type;
    typename Binding<sig, Offset, Ret, FnType, Args...>::wrapper_type;
    { Binding<sig, Offset, Ret, FnType, Args...>::ptr } -> std::same_as<void* const>;
    typename std::integral_constant<tCallFlags, Binding<sig, Offset, Ret, FnType, Args...>( ).GetCallFlags( )>;
    typename std::integral_constant<size_t, Binding<sig, Offset, Ret, FnType, Args...>( ).ArgCount>;
    { binding.GetHook((void*) 0x0u) } -> std::_Implicitly_convertible_to<typename Binding<sig, Offset, Ret, FnType, Args...>::wrapper_type>;
    { binding.GetSuper((void*) 0x0u) } -> std::_Implicitly_convertible_to<typename Binding<sig, Offset, Ret, FnType, Args...>::function_type>;
};
template <size_t Offset, typename Ret, typename... Args>
using CDeclBinding = FnBinding<_CDECL, Offset, Ret, Ret(__cdecl*)(Args...), Args...>;

template <size_t Offset, typename Ret, typename... Args>
using STDCallBinding = FnBinding<_STDCALL, Offset, Ret, Ret(__stdcall*)(Args...), Args...>;

template <size_t Offset, typename Ret, typename ClassName, typename... Args>
using MemberFnBinding = FnBinding<_THISCALL, Offset, Ret, Ret(__thiscall ClassName::*)(Args...), Args...>;

template <size_t Offset, typename Ret, typename... Args>
using FastCallBinding = FnBinding<_FASTCALL, Offset, Ret, Ret(__fastcall*)(Args...), Args...>;
