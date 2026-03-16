/*
** This file has been pre-processed with DynASM.
** https://luajit.org/dynasm.html
** DynASM version 1.5.0, DynASM x86 version 1.5.0
** DO NOT EDIT! The original file is in "/home/amyseni/organik/include/Arkitekt/Assembler.h".
*/

#line 1 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
#pragma once
#include <unordered_map>
#include <string>
#include <cstdint>
#include <concepts>
#include <functional>
#include <ranges>
#include <variant>
#include <string_view>
#include <fstream>
#include <GameMaker_Defs.h>
#include "detours.h"
#include "PALMemoryProtection.h"

#ifndef _DASM_INC_H_
#define _DASM_INC_H_ 1
#include <dynasm/dasm_proto.h>
#endif

//|.arch x86
#if DASM_VERSION != 10500
#error "Version mismatch between DynASM and included encoding engine"
#endif
#line 21 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
//|.section code
#define DASM_SECTION_CODE	0
#define DASM_MAXSECTION		1
#line 22 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
//|.globals lbl_
enum {
  lbl__func,
  lbl__MAX
};
#line 23 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
//|.actionlist compiler_actions
static const unsigned char compiler_actions[169] = {
  252,255,53,237,255,252,255,176,253,240,1,233,255,80,240,32,255,195,255,194,
  236,255,102,129,192,240,33,238,255,129,192,240,33,239,255,128,192,240,33,
  235,255,232,243,255,104,247,252,255,176,253,240,1,233,195,249,255,252,255,
  208,240,33,255,252,255,21,237,255,252,255,176,253,240,1,233,143,128,253,240,
  1,233,255,137,128,253,240,129,240,1,233,255,199,128,253,240,1,233,237,255,
  252,255,176,253,240,1,233,143,5,237,255,139,128,253,240,129,240,1,233,255,
  137,192,240,129,240,33,255,184,240,32,237,255,139,5,240,129,237,255,252,255,
  53,237,143,128,253,240,1,233,255,137,5,240,129,237,255,199,5,237,237,255,
  88,240,32,255,254,0,250,15,248,10,255
};

#line 24 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
struct Action;
namespace Arkitekt
{
    struct FnBlock;
    struct Assembler;
};



#define LIBARK_PLACEHOLDER {__NOP();}
#if LOGLEVEL >= 6
#define DO_LOG6(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG6S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG6(x, ...)
#define DO_LOG6S(x)
#endif
#if LOGLEVEL >= 5
#define DO_LOG5(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG5S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG5(x, ...)
#define DO_LOG5S(x)
#endif
#if LOGLEVEL >= 4
#define DO_LOG4(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG4S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG4(x, ...)
#define DO_LOG4S(x)
#endif
#if LOGLEVEL >= 3
#define DO_LOG3(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG3S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG3(x, ...)
#define DO_LOG3S(x)
#endif
#if LOGLEVEL >= 2
#define DO_LOG2(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG2S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG2(x, ...)
#define DO_LOG2S(x)
#endif
#if LOGLEVEL >= 1
#define DO_LOG1(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG1S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG1(x, ...)
#define DO_LOG1S(x)
#endif
#if LOGLEVEL >= 0
#define DO_LOG0(x, ...) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__  __VA_OPT__(,) __VA_ARGS__)
#define DO_LOG0S(x) \
GetLogger()->LogFormatted("%s:%s@%d -> " x, __FUNCTION__, __FILE__, __LINE__)
#else
#define DO_LOG0(x, ...)
#define DO_LOG0S(x)
#endif


using namespace Arkitekt;
using CHashMapHash=size_t;

using Arkitekt::FnBlock;
struct dasm_State;
struct Action;
struct Script;

constexpr inline static const uint32_t BITMASK_R8=1u<<4;
constexpr inline static const uint32_t BITMASK_R16=1u<<8;
constexpr inline static const uint32_t BITMASK_R32=1u<<16;
constexpr inline static const uint32_t BITMASK_XMM=1u<<24;
constexpr inline static const uint32_t BITMASK_SPC=1u<<31;
class _r8;
class _r16;
class _r32;
class xmm;

constexpr const uint32_t REGMASK=BITMASK_R8|BITMASK_R16|BITMASK_R32|BITMASK_XMM|BITMASK_SPC;

class register32;
template <typename T>
concept r32t=(
    std::_Is_any_of_v<T, _r8::__r8, _r16::__r16, _r32::__r32, xmm::__xmm>
    or (std::assignable_from<uint32_t&, const T&>)
    or (
        (std::assignable_from<int32_t&, const T&>)
        and (
            std::assignable_from<register32&, const T&>
            or  std::constructible_from<register32, const T&>
            or  std::same_as<register32, T>
            )
        )
    );
enum class cleanup : uint32_t { caller=0u, callee=1u };
class register32
{
protected:
    uint32_t val;

public:
    friend class _r8;
    friend class _r16;
    friend class _r32;
    friend class xmm;
    explicit constexpr operator uint32_t()
    {
        return static_cast<uint32_t>(val);
    }
    explicit constexpr operator uint32_t() const
    {
        return static_cast<uint32_t>(val);
    }
    constexpr bool operator==(r32t auto other)
    {
        return val == uint32_t(other);
    }
    constexpr bool operator!=(r32t auto other)
    {
        return !(*this == uint32_t(other));
    }
    constexpr bool operator==(r32t auto other)const
    {
        return val == uint32_t(other);
    }
    constexpr bool operator!=(r32t auto other)const
    {
        return !(*this == uint32_t(other));
    }
    constexpr register32(r32t auto value) : val(static_cast<uint32_t>(value)) {}
    constexpr const auto& operator|(r32t auto other)
    {
        return register32(val | uint32_t(other));
    }
    constexpr const auto& operator^(r32t auto other)
    {
        return register32(val ^ uint32_t(other));
    }
    constexpr const auto& operator&(r32t auto other)
    {
        return register32(val & uint32_t(other));
    }
    constexpr const auto& operator|(const r32t auto& other)const
    {
        return register32(val | uint32_t(other));
    }
    constexpr const auto& operator^(const r32t auto& other)const
    {
        return register32(val ^ uint32_t(other));
    }
    constexpr const auto& operator&(const r32t auto& other)const
    {
        return register32(val & uint32_t(other));
    }

    constexpr register32& operator~()
    {
        val = ~GetValue();
        return *this;
    }
    const register32& operator~() const
    {
        return register32(~GetValue());
    }
    auto& operator|=(r32t auto other)
    {
        this->val = (this->val | uint32_t(other));
        return *this;
    }
    auto& operator^=(r32t auto other)
    {
        this->val = (this->val ^ uint32_t(other));
        return *this;
    }
    auto& operator&=(r32t auto other)
    {
        this->val = (this->val & uint32_t(other));
        return *this;
    }

    [[msvc::constexpr]] [[msvc::intrinsic]] register32() : val(0u) {}

    constexpr const uint32_t& GetRMask()
    {
        return uint32_t(this->val & REGMASK);
    }
    constexpr const uint32_t& ToRegID()
    {
        return uint32_t(this->val & 0b00001111);
    }
    constexpr const uint32_t& GetValue()
    {
        return uint32_t(val);
    }
    constexpr const uint32_t& GetRMask() const
    {
        return uint32_t(this->val & REGMASK);
    }
    constexpr const uint32_t& ToRegID() const
    {
        return uint32_t(this->val & 0b00001111);
    }
    constexpr const uint32_t& GetValue() const
    {
        return uint32_t(val);
    }
};
class _Ins {};
class _r8 : public register32
{
public:
    using register32::val;
    using register32::ToRegID;
    using register32::GetValue;
    using register32::operator&;
    using register32::operator|;
    using register32::operator^;
    enum __r8 : uint32_t
    {
        _al=(BITMASK_R8)|0,
        _cl=(BITMASK_R8)|1,
        _dl=(BITMASK_R8)|2,
        _bl=(BITMASK_R8)|3,
        _ah=(BITMASK_R8)|4,
        _ch=(BITMASK_R8)|5,
        _dh=(BITMASK_R8)|6,
        _bh=(BITMASK_R8)|7,
    };
    explicit constexpr _r8() : register32() {}
    explicit constexpr _r8(register32 other)
        : register32(other.GetValue()) {}
    explicit constexpr _r8(const std::_Has_class_or_enum_type auto& other)
        : register32(static_cast<uint32_t>(other)) {}

    [[msvc::intrinsic]] constexpr _r8(__r8 other) : register32(uint32_t(other)) {}

    [[msvc::intrinsic]] constexpr _r8& operator=(__r8 other)
    {
        this->val=uint32_t(other);
        return *this;
    }
    constexpr const uint32_t GetRMask() { return uint32_t(BITMASK_R8); }
    constexpr const uint32_t GetRMask() const { return uint32_t(BITMASK_R8); }
    constexpr operator uint32_t () { return static_cast<uint32_t>(this->val); }
    constexpr operator uint32_t () const { return static_cast<uint32_t>(this->val); }
};
class _r16 : public register32
{
public:
    using register32::val;
    using register32::ToRegID;
    using register32::GetValue;
    using register32::operator&;
    using register32::operator|;
    using register32::operator^;
    enum __r16 : uint32_t
    {
        _ax=(BITMASK_R16)|0,
        _cx=(BITMASK_R16)|1,
        _dx=(BITMASK_R16)|2,
        _bx=(BITMASK_R16)|3,
        _sp=(BITMASK_R16)|4,
        _bp=(BITMASK_R16)|5,
        _si=(BITMASK_R16)|6,
        _di=(BITMASK_R16)|7
    };
    explicit constexpr _r16() : register32() {}
    explicit constexpr _r16(register32 other)
        : register32(other.GetValue()) {}
    explicit constexpr _r16(const std::_Has_class_or_enum_type auto& other)
        : register32(static_cast<uint32_t>(other)) {}

    [[msvc::intrinsic]] constexpr _r16(__r16 other) : register32(uint32_t(other)) {}

    [[msvc::intrinsic]] constexpr _r16& operator=(__r16 other)
    {
        this->val=uint32_t(other);
        return *this;
    }
    constexpr const uint32_t GetRMask() const { return BITMASK_R16; }
    constexpr const uint32_t GetRMask() { return BITMASK_R16; }
    constexpr operator uint32_t () { return static_cast<uint32_t>(this->val); }
    constexpr operator uint32_t () const { return static_cast<uint32_t>(this->val); }
};
class _r32 : public register32
{
public:
    using register32::val;
    using register32::GetRMask;
    using register32::ToRegID;
    using register32::GetValue;
    using register32::operator&;
    using register32::operator|;
    using register32::operator^;

    enum __r32 : uint32_t
    {
        _eax=(BITMASK_R32)|0,
        _ecx=(BITMASK_R32)|1,
        _edx=(BITMASK_R32)|2,
        _ebx=(BITMASK_R32)|3,
        _esp=(BITMASK_R32)|4,
        _ebp=(BITMASK_R32)|5,
        _esi=(BITMASK_R32)|6,
        _edi=(BITMASK_R32)|7
    };
    explicit constexpr _r32() : register32() {}
    explicit constexpr _r32(register32 other)
        : register32(other.GetValue()) {}
    explicit constexpr _r32(const std::_Has_class_or_enum_type auto& other)
        : register32(static_cast<uint32_t>(other)) {}

    [[msvc::intrinsic]] constexpr _r32(__r32 other) : register32(uint32_t(other)) {}
    [[msvc::intrinsic]] constexpr _r32& operator=(__r32 other)
    {
        this->val=uint32_t(other);
        return *this;
    }
    constexpr const uint32_t GetRMask() { return BITMASK_R32; }
    constexpr const uint32_t GetRMask() const { return BITMASK_R32; }
    constexpr operator uint32_t () { return static_cast<uint32_t>(this->val); }
    constexpr operator uint32_t () const { return static_cast<uint32_t>(this->val); }
};
class xmm : public register32
{
public:
    using register32::val;
    using register32::GetRMask;
    using register32::ToRegID;
    using register32::GetValue;
    using register32::operator&;
    using register32::operator|;
    using register32::operator^;
    enum __xmm : uint32_t
    {
        _xmm0=(BITMASK_XMM)|0,
        _xmm1=(BITMASK_XMM)|1,
        _xmm2=(BITMASK_XMM)|2,
        _xmm3=(BITMASK_XMM)|3,
        _xmm4=(BITMASK_XMM)|4,
        _xmm5=(BITMASK_XMM)|5,
        _xmm6=(BITMASK_XMM)|6,
        _xmm7=(BITMASK_XMM)|7,
    };
    explicit constexpr xmm() : register32() {}
    explicit constexpr xmm(register32 other)
        : register32(other.val) {}
    explicit constexpr xmm(const std::_Has_class_or_enum_type auto& other)
        : register32(static_cast<uint32_t>(other)) {}

    [[msvc::intrinsic]] constexpr xmm(__xmm other) : register32(uint32_t(other)) {}

    [[msvc::intrinsic]] constexpr xmm& operator=(__xmm other)
    {
        this->val=uint32_t(other);
        return *this;
    }
    constexpr const uint32_t GetRMask() { return uint32_t(BITMASK_XMM); }
    constexpr const uint32_t GetRMask() const { return uint32_t(BITMASK_XMM); }
    constexpr operator uint32_t () { return static_cast<uint32_t>(this->val); }
    constexpr operator uint32_t () const { return static_cast<uint32_t>(this->val); }
};
constexpr _r8 ah=_r8::_al;
constexpr _r8 al=_r8::_cl;
constexpr _r8 bh=_r8::_dl;
constexpr _r8 bl=_r8::_bl;
constexpr _r8 ch=_r8::_ah;
constexpr _r8 cl=_r8::_ch;
constexpr _r8 dh=_r8::_dh;
constexpr _r8 dl=_r8::_bh;
constexpr _r16 ax=_r16::_ax;
constexpr _r16 bp=_r16::_bp;
constexpr _r16 bx=_r16::_bx;
constexpr _r16 cx=_r16::_cx;
constexpr _r16 di=_r16::_di;
constexpr _r16 dx=_r16::_dx;
constexpr _r16 si=_r16::_si;
constexpr _r16 sp=_r16::_sp;
constexpr _r32 eax=_r32::_eax;
constexpr _r32 ebp=_r32::_ebp;
constexpr _r32 ebx=_r32::_ebx;
constexpr _r32 ecx=_r32::_ecx;
constexpr _r32 edi=_r32::_edi;
constexpr _r32 edx=_r32::_edx;
constexpr _r32 esi=_r32::_esi;
constexpr _r32 esp=_r32::_esp;
constexpr xmm xmm0=xmm::_xmm0;
constexpr xmm xmm1=xmm::_xmm1;
constexpr xmm xmm2=xmm::_xmm2;
constexpr xmm xmm3=xmm::_xmm3;
constexpr xmm xmm4=xmm::_xmm4;
constexpr xmm xmm5=xmm::_xmm5;
constexpr xmm xmm6=xmm::_xmm6;
constexpr xmm xmm7=xmm::_xmm7;

struct Immediate32
{
    int32_t value;
    explicit Immediate32(std::signed_integral auto _val) : value(static_cast<int32_t>(_val))
    {}
    explicit operator uint32_t() const noexcept { return value; }
    operator int32_t() const noexcept { return value; }
};
struct ImmediateU32
{
    uint32_t value;
    explicit ImmediateU32(std::unsigned_integral auto _val) : value(static_cast<uint32_t>(_val))
    {}
    operator uint32_t() const noexcept { return value; }
    explicit operator int32_t() const noexcept { return value; }

};
template <typename T>
concept imm32 = (std::same_as<Immediate32, T>) || (std::same_as<ImmediateU32, T>) || (std::same_as<uint32_t, T>) || (std::same_as<int32_t, T>);

template <typename T>
concept is_register = requires(T reg)
{
    { reg.GetRMask() } -> std::convertible_to<uint32_t>;
    { reg.ToRegID() } -> std::convertible_to<uint32_t>;
    { reg.GetValue() } -> std::convertible_to<uint32_t>;
};

class RegisterIndex
{
public:
    constexpr static const uint32_t scale=sizeof(uintptr_t);
    const uint32_t baseOffset;
    const uint32_t index;
    const uint32_t totalOffset;
    const register32 _register;

    RegisterIndex(is_register auto _reg, uint32_t base, uint32_t index=0) : baseOffset(base), index(index),
        _register(_reg), totalOffset(base+(index*scale)) {}

    constexpr const uint32_t& GetRMask() noexcept
    {
        return uint32_t(this->_register & REGMASK);
    }
    constexpr const uint32_t& GetRMask() const noexcept
    {
        return uint32_t(this->_register & REGMASK);
    }
    constexpr const uint32_t& ToRegID() noexcept
    {
        return uint32_t(this->_register & 0b00001111);
    }
    constexpr const uint32_t& ToRegID() const noexcept
    {
        return uint32_t(this->_register & 0b00001111);
    }
    constexpr const uint32_t& GetValue() noexcept
    {
        return uint32_t(this->totalOffset);
    }
    constexpr const uint32_t& GetValue() const noexcept
    {
        return uint32_t(this->totalOffset);
    }

};


template <typename ptrType>
concept is_address=std::is_pointer_v<ptrType> || std::is_member_pointer_v<ptrType> || std::is_member_function_pointer_v<ptrType> || std::is_member_object_pointer_v<ptrType>;

template <typename TOper>
concept Operand = imm32<TOper> or is_address<TOper> or is_register<TOper> or std::convertible_to<TOper, void*> or std::is_function_v<TOper>;


template <typename TFN, typename... TOperands >
concept Instruction=requires(TFN fn, TOperands... operands)
{
    typename TFN;
    { TFN()(Arkitekt::Assembler::Get(), std::forward<TOperands>(operands)...) };
};


namespace Arkitekt
{
/// @brief struct for storing functions generated by DynASM, contains the pointer to the memory where the function is stored, the size of the memory allocation and a pointer to the function itself
    struct FnBlock
    {
/// @brief pointer to the memory where the function is stored
        void* m_Mem;
        /// @brief size of the memory allocation in bytes
        size_t m_Size;
        /// @brief pointer to the function itself, used to call the function
        void* m_Fn;
        void** m_Labels;
        constexpr FnBlock()=default;
        FnBlock(void* _mem, size_t _size, void* _fn, void*** labels) : m_Mem(_mem), m_Size(_size), m_Fn(_fn), m_Labels(*labels) {}

        static Arkitekt::FnBlock& Create(FnBlock* ptr, const std::string_view& name, void* _mem, size_t _sz, void* _fn, void**& labels);
    };

    /// @brief struct for generating functions at runtime using DynASM, contains the state of the assembler, a map of generated functions and frees those functions on destruction. Currently only used to generate assembly for function hooks, will also contain scripts created by users at runtime in the future.
    struct Assembler
    {
        friend struct Arkitekt::FnBlock;
    protected:
    /** @brief maximum number of local variables that currently have space allocated for them. If the number of local variables used in a function exceeds this number, more space will be allocated for them. This is necessary because we don't know how many local variables a function will use until we generate the code for it.
    **/
        unsigned long maxlocals;


        /** @brief identifier for the next local variable or label to be defined, incremented every time a new local variable or label is defined. Used by DynASM for its internal bookkeeping, incremented by the Assembler functions that define local variables or labels, and reset to 0 when the Assembler is destructed (which happens after each function is generated).
        **/
        unsigned long nextlocal;
    public:
        uint32_t StartFunction(const std::string_view& name);

        static Arkitekt::Assembler* Get()
        {
            static Arkitekt::Assembler C;
            if (!C._blocks.size()) C.Init();
            return &C;
        }

        /** @brief call this to initialize the assembler, must be called before any other function of the assembler is called, and exactly once per function. Returns a reference to itself if the initialization was successful, crashes the program otherwise (because we aren't chancing emitting assembly when the Assembler state is invalid).
        **/
        Arkitekt::Assembler& Init()
        {
            _blocks.size() || [&]() { this->_blocks=std::unordered_map<CHashMapHash, FnBlock>(); return true; }();
            return *this;
        }


        /** @brief pointer to the DynASM state, must be used at the top of any function that generates code using DynASM, declared as dasm_State** dst = &this->dasm because this is how DynASM's preprocessor expects it. Is allocated newly for each function generated (by calling Assembler::Begin) and freed after the function is finalized (by calling Assembler::End, because I didn't know any other way to do it, and DynASM is virtually undocumented).
         **/
        dasm_State* dasm;

        void Begin(void*** _labels);
        uint32_t stackOffset;
        std::unordered_map<CHashMapHash, uint32_t> LabelMapCurrent;
        FnBlock* FinalizeFunction(const std::string_view& name, void**& _labels);
        template<typename FnType>
        uint32_t GetFunctionDynamicLabel(const std::string_view& name)
        {
            auto hash = CHashMapCalculateHash(name.data()) ^ CHashMapCalculateHash(typeid(FnType).name());
            LabelMapCurrent[hash] = nextlocal++;
            if (nextlocal >= maxlocals)
                dasm_growpc(&this->dasm, std::bit_floor(maxlocals*=1.6));
            return LabelMapCurrent[hash];
        }
        uint32_t GetNextDynamicLabel(const std::string_view& name)
        {
            auto hash = CHashMapCalculateHash(name.data());
            LabelMapCurrent[hash] = nextlocal++;
            if (nextlocal >= maxlocals)
                dasm_growpc(&this->dasm, std::bit_floor(maxlocals*=1.6));
            return LabelMapCurrent[hash];
        }
        uint32_t GetOrAllocDynamicLabel(const std::string_view& name)
        {
            auto hash = CHashMapCalculateHash(name.data());
            if (LabelMapCurrent.find(hash) != LabelMapCurrent.end())
                return LabelMapCurrent[hash];
            return GetNextDynamicLabel(name.data());
        }
    protected:


         /** @brief  map of FnBlocks generated by this assembler, indexed by the hash (murmurhash) of the function name. Used to free the generated functions on destruction of the assembler, and to look up functions by name to call from user scripts.
         **/
        std::unordered_map<uint32_t, FnBlock> _blocks;

        /** @brief map of script names to their hashes, used to look up function names by their hash (for debugging purposes within user scripts, not used by the Assembler itself).
        **/
        std::unordered_map<uint32_t, std::string_view> m_scriptNames;

        /** @brief currently unused. Will map local variable allocated storage to script hashes (for all local variables per script) in the future.
        **/
        std::unordered_map<uint32_t, YYObjectBase*> m_Locals;

        /** @brief constructor and destructor, the constructor initializes the member variables to their default values, and the destructor frees all generated functions and resets the member variables. The destructor must only be called on program exit. Calling it during runtime would cause all generated functions to become invalid (as they would be freed) and would crash as soon as any of those functions is called. (including any function hooks). This is protected for a reason.
        **/
        Assembler() : _blocks{}, m_Locals{}, dasm(nullptr), nextlocal(0), maxlocals(8) {};
        ~Assembler()=default;
        ;
    public:
        // FnBlock* FinalizeFunction(const std::string_view& name, void**& _labels);
        void End();
    };
}

struct push : public _Ins
{
    void operator()(Arkitekt::Assembler* assembler, Operand auto toPush)
    {
        dasm_State** Dst=&assembler->dasm;
        if constexpr (imm32<decltype(toPush)>)
        {
            // immediate pop
            //|push aword[toPush.value]
            dasm_put(Dst, 0, toPush.value);
#line 634 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                assembler->stackOffset += sizeof(uintptr_t);
        }
        else if constexpr (std::same_as<std::remove_all_extents_t<decltype(toPush)>, RegisterIndex>)
        {
            uint32_t value=toPush.GetValue();
            //|push aword[Rd(toPush.ToRegID())+value]
            dasm_put(Dst, 5, (toPush.ToRegID()), value);
#line 640 "/home/amyseni/organik/include/Arkitekt/Assembler.h"

                assembler->stackOffset += sizeof(uintptr_t);
        }
        else if constexpr (is_register<decltype(toPush)>)
        {
            switch (toPush.GetValue()&REGMASK)
            {
            case BITMASK_R32: // push r[0-7] (eax-esi)
                //|push Rd(toPush.ToRegID())
                dasm_put(Dst, 13, (toPush.ToRegID()));
#line 649 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                    assembler->stackOffset += sizeof(uint32_t);
                break;
            case BITMASK_XMM:// invalid in 32bit mode
            case BITMASK_R16:// invalid in 32bit mode
            case BITMASK_R8: // invalid in 32bit mode
                throw new std::bad_function_call;
                break;
            }
        }
    }
    constexpr  push() {}
};

struct ret : public _Ins
{
    void operator() (Arkitekt::Assembler* assembler)
    {
        dasm_State** Dst = &assembler->dasm;
        //|ret
        dasm_put(Dst, 17);
#line 668 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
    }
    void operator() (Arkitekt::Assembler* assembler, imm32 auto oper)
    {
        dasm_State** Dst = &assembler->dasm;
        //|ret uint32_t(oper)
        dasm_put(Dst, 19, uint32_t(oper));
#line 673 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
    }
    constexpr ret() {}
};

struct add : public _Ins
{
    void operator() (Arkitekt::Assembler* assembler, is_register auto target, imm32 auto value)
    {
        dasm_State** Dst = &assembler->dasm;
        switch (target.GetValue()&REGMASK)
        {
        case BITMASK_R16:
            //|add Rw(target.ToRegID()), (ushort)uint32_t(value)
            dasm_put(Dst, 22, (target.ToRegID()), (ushort)uint32_t(value));
#line 686 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                break;
        case BITMASK_R32:
            //|add Rd(target.ToRegID()), uint32_t(value)
            dasm_put(Dst, 29, (target.ToRegID()), uint32_t(value));
#line 689 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                break;
        case BITMASK_R8:
            //|add Rb(target.ToRegID()), (unsigned char)uint32_t(value)
            dasm_put(Dst, 35, (target.ToRegID()), (unsigned char)uint32_t(value));
#line 692 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                break;
        case BITMASK_XMM:// invalid in 32bit mode(?)
            throw new std::bad_function_call;
            break;
        }
    }
    constexpr add() {}
};
struct call : public _Ins
{
    void operator() (Arkitekt::Assembler* assembler, Operand auto address)
    {
        dasm_State** Dst = &assembler->dasm;
        if constexpr (imm32<decltype(address)> || is_address<decltype(address)>)
        {
            //|call aword & (*(void**)&address)
            dasm_put(Dst, 41, (ptrdiff_t)( (*(void**)&address)));
#line 708 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        }
        else if constexpr (std::same_as<std::remove_all_extents_t<decltype(address)>, RegisterIndex>)
        {
            // ?? look mom I'm ROP hacking
            auto counter = assembler->GetNextDynamicLabel(("ret@"+std::to_string(address.GetValue())).c_str());
            //|push =>counter
                //|push dword[Rd(address.ToRegID())+address.GetValue()]
                //|ret
                //|=>counter:
                dasm_put(Dst, 44, counter, (address.ToRegID()), address.GetValue(), counter);
#line 717 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        }
        else if constexpr (is_register<decltype(address)>)
        {
            //|call Rd(address.ToRegID())
            dasm_put(Dst, 56, (address.ToRegID()));
#line 721 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        }
    }
    constexpr  call() {}
};
struct ind_call : public _Ins
{
    void operator() (Arkitekt::Assembler* assembler, Operand auto address)
    {
        dasm_State** Dst = &assembler->dasm;
        if constexpr (imm32<decltype(address)> || is_address<decltype(address)>)
        {
            //|call dword[uint32_t(address)]
            dasm_put(Dst, 62, uint32_t(address));
#line 733 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        }
        else if constexpr (std::same_as<std::remove_all_extents_t<decltype(address)>, RegisterIndex>)
        {
            // ?? archie would not be pleased if he saw this (but I stole the idea from him XD)
            std::string label = "ret@"+std::to_string((uint32_t)address.GetValue());
            auto counter = assembler->GetNextDynamicLabel(label.c_str());
            //|push =>counter
                //|push dword[Rd(address.ToRegID())+address.GetValue()]
                //|ret
                //|=>counter:
                dasm_put(Dst, 44, counter, (address.ToRegID()), address.GetValue(), counter);
#line 743 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        }
        else if constexpr (is_register<decltype(address)>)
        {
            //|call Rd(address.ToRegID())
            dasm_put(Dst, 56, (address.ToRegID()));
#line 747 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        }
    }
    constexpr  ind_call() {}
};
struct mov : public _Ins
{
    void operator()(Arkitekt::Assembler* assembler, Operand auto lhs, Operand auto rhs)
    {
        dasm_State** Dst = &assembler->dasm;
        if constexpr (std::same_as<decltype(lhs), RegisterIndex>)
        {
            if constexpr (std::same_as<std::remove_all_extents_t<decltype(rhs)>, RegisterIndex>)
            {
                //| push aword[Rd(rhs.ToRegID())+rhs.GetValue()]
                    //| pop  aword[Rd(lhs.ToRegID())+lhs.GetValue()]
                    dasm_put(Dst, 67, (rhs.ToRegID()), rhs.GetValue(), (lhs.ToRegID()), lhs.GetValue());
#line 762 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (is_register<decltype(rhs)>)
            {
                //| mov aword[Rd(lhs.ToRegID())+lhs.GetValue()], Rd(rhs.ToRegID())
                dasm_put(Dst, 81, (rhs.ToRegID()), (lhs.ToRegID()), lhs.GetValue());
#line 766 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (imm32<decltype(rhs)>)
            {
                //| mov aword[Rd(lhs.ToRegID())+lhs.GetValue()], rhs.GetValue()
                dasm_put(Dst, 90, (lhs.ToRegID()), lhs.GetValue(), rhs.GetValue());
#line 770 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (is_address<decltype(rhs)>)
            {
                //| push aword[Rd(lhs.ToRegID())+lhs.GetValue()]
                    //| pop aword[uint32_t(rhs)]
                    dasm_put(Dst, 98, (lhs.ToRegID()), lhs.GetValue(), uint32_t(rhs));
#line 775 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
        }
        else if constexpr (is_register<decltype(lhs)>)
        {
            if constexpr (std::same_as<std::remove_all_extents_t<decltype(rhs)>, RegisterIndex>)
            {
                //| mov Rd(lhs.ToRegID()), aword[Rd(rhs.ToRegID())+rhs.GetValue()]
                dasm_put(Dst, 109, (lhs.ToRegID()), (rhs.ToRegID()), rhs.GetValue());
#line 782 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (is_register<decltype(rhs)>)
            {
                //| mov Rd(lhs.ToRegID()), Rd(rhs.ToRegID())
                dasm_put(Dst, 118, (rhs.ToRegID()), (lhs.ToRegID()));
#line 786 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (imm32<decltype(rhs)>)
            {
                //| mov Rd(lhs.ToRegID()), (uint32_t)rhs
                dasm_put(Dst, 125, (lhs.ToRegID()), (uint32_t)rhs);
#line 790 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (is_address<decltype(rhs)>)
            {
                //| mov Rd(lhs.ToRegID()), aword[uint32_t(rhs)]
                dasm_put(Dst, 130, (lhs.ToRegID()), uint32_t(rhs));
#line 794 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
        }
        else if constexpr (imm32<decltype(lhs)> || is_address<decltype(lhs)>)
        {
            if constexpr (std::same_as<std::remove_all_extents_t<decltype(rhs)>, RegisterIndex>)
            {
                //| push aword[uint32_t(lhs)]
                    //| pop aword[Rd(rhs.ToRegID())+rhs.GetValue()]
                    dasm_put(Dst, 136, uint32_t(lhs), (rhs.ToRegID()), rhs.GetValue());
#line 802 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (is_register<decltype(rhs)>)
            {
                //| mov dword[uint32_t(lhs)], Rd(rhs.ToRegID())
                dasm_put(Dst, 147, (rhs.ToRegID()), uint32_t(lhs));
#line 806 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
            else if constexpr (imm32<decltype(rhs)>)
            {
                //| mov dword[uint32_t(lhs)], uint32_t(rhs)
                dasm_put(Dst, 153, uint32_t(lhs), uint32_t(rhs));
#line 810 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            }
        }
    }
    constexpr  mov() {}
};
struct pop : public _Ins
{
    void operator()(Arkitekt::Assembler* assembler, Operand auto toPop)
    {
        dasm_State** Dst=&assembler->dasm;
        if constexpr (imm32<decltype(toPop)> == true)
        {
            // immediate pop
            //|pop aword[toPop.value]
            dasm_put(Dst, 105, toPop.value);
#line 824 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                assembler->stackOffset -= sizeof(uintptr_t);
        }
        else if constexpr (std::same_as<std::remove_all_extents_t<decltype(toPop)>, RegisterIndex>)
        {
            if (0<toPop.GetValue())
                //|pop aword[Rd(toPop.ToRegID())+toPop.GetValue()]
                dasm_put(Dst, 74, (toPop.ToRegID()), toPop.GetValue());
#line 830 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
            else
                //|pop aword[Rd(toPop.ToRegID())-toPop.GetValue()]
                dasm_put(Dst, 74, (toPop.ToRegID()), -toPop.GetValue());
#line 832 "/home/amyseni/organik/include/Arkitekt/Assembler.h"

                assembler->stackOffset -= sizeof(uintptr_t);
        }
        else if constexpr (is_register<decltype(toPop)>)
        {
            switch (toPop.GetValue()&REGMASK)
            {
            case BITMASK_R32: // push r[0-7] (eax-esi)
                //|pop Rd(toPop.ToRegID())
                dasm_put(Dst, 158, (toPop.ToRegID()));
#line 841 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                    assembler->stackOffset -= sizeof(uint32_t);
                break;
            case BITMASK_XMM:// invalid in 32bit mode
            case BITMASK_R16:// invalid in 32bit mode
            case BITMASK_R8: // invalid in 32bit mode
                throw new std::bad_function_call;
                break;
            }
        }
        else if constexpr (imm32<decltype(toPop)> || is_address<decltype(toPop)>)
        {
            //|pop dword[uint32_t(toPop)]
            dasm_put(Dst, 105, uint32_t(toPop));
#line 853 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
                assembler->stackOffset -= sizeof(uint32_t);
        }
    }
    constexpr  pop() {}
};

template <typename T>
constexpr size_t GetStackSize()
{
    if constexpr (std::is_reference_v<T>)
        return sizeof(T*);
    return sizeof(T);
}

template <typename FnType> struct TotalStackSize;

template <typename Ret, typename... Args>
struct TotalStackSize<Ret(__cdecl*)(Args...)>
{
    constexpr const static size_t argC = sizeof...(Args);
    constexpr const static size_t size = sizeof...(Args) * sizeof(uintptr_t);// (GetStackSize<Args>() + ...);
};
template <typename Ret, typename... Args>
struct TotalStackSize<Ret(__stdcall*)(Args...)>
{
    constexpr const static size_t argC = sizeof...(Args);
    constexpr const static size_t size = sizeof...(Args) * sizeof(uintptr_t); // (GetStackSize<Args>() + ...);
};
template <typename Ret, typename classname, typename... Args>
struct TotalStackSize<Ret(__thiscall classname::*)(Args...)>
{
    constexpr const static size_t argC = sizeof...(Args);
    constexpr const static size_t size = sizeof...(Args) * sizeof(uintptr_t); // (GetStackSize<Args>() + ...);
};


inline void Assembler::Begin(void*** _labels)
{
    void* __labels[lbl__MAX];
    dasm_init(&this->dasm, DASM_MAXSECTION);
    dasm_setupglobal(&this->dasm, __labels, lbl__MAX);
    *_labels = __labels;
    dasm_setup(&this->dasm, compiler_actions);
    dasm_State** Dst = &(this->dasm);

    //|.code
    dasm_put(Dst, 162);
#line 899 "/home/amyseni/organik/include/Arkitekt/Assembler.h"
        //|.align 0x10
        //|->_func:
        dasm_put(Dst, 164);
#line 901 "/home/amyseni/organik/include/Arkitekt/Assembler.h"

    this->stackOffset = sizeof(uintptr_t);
}

inline FnBlock* Assembler::FinalizeFunction(const std::string_view& name, void**& _labels)
{
    size_t _outSz;
    dasm_link(&this->dasm, &_outSz);
    auto hash=CHashMapCalculateHash(name.data());
    void* buf;
#ifdef _WIN32
    buf=VirtualAlloc(0, _outSz, MEM_RESERVE|MEM_COMMIT, PAGE_READWRITE);
#else
    buf=mmap(0, _outSz, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
#endif
    dasm_encode(&this->dasm, buf);
#ifdef _WIN32
    [&]() { DWORD dwOld; VirtualProtect(buf, _outSz, PAGE_EXECUTE_READ, &dwOld); }();
#else
    mprotect(buf, _outSz, PROT_READ|PROT_EXEC);
#endif
    ::new (&_blocks[hash]) FnBlock(buf, _outSz, _labels[lbl__func], &_labels);
    return (&_blocks[hash]);
}

inline void Arkitekt::Assembler::End()
{
    dasm_free(&this->dasm);
}

template <typename FnType>
struct IsVoidOr32Bit;

template <typename Ret, typename... Args>
struct IsVoidOr32Bit<Ret(__cdecl*)(Args...)>
{
    constexpr const static bool value = []()
    {
        if constexpr (std::is_void_v<Ret> == true)
            return true;
        else if constexpr (not std::is_void_v<Ret>)
            return (sizeof(std::declval<Ret>()) <= 4u);
    }();
};
template <typename Ret, typename... Args>
struct IsVoidOr32Bit<Ret(__stdcall*)(Args...)>
{
    constexpr const static bool value = []()
    {
        if constexpr (std::is_void_v<Ret> == true)
            return true;
        else if constexpr (not std::is_void_v<Ret>)
            return (sizeof(std::declval<Ret>()) <= 4u);
    }();
};
template <typename Ret, typename classname, typename... Args>
struct IsVoidOr32Bit<Ret(__thiscall classname::*)(Args...)>
{
    constexpr const static bool value = []()
    {
        if constexpr (std::is_void_v<Ret> == true)
            return true;
        else if constexpr (not std::is_void_v<Ret>)
            return (sizeof(std::declval<Ret>()) <= 4u);
    }();
};
template <typename FnType>
struct IsVoid;

template <typename Ret, typename... Args>
struct IsVoid<Ret(__cdecl*)(Args...)>
{
    constexpr const static bool value = (std::is_void_v<Ret> == true);
};
template <typename Ret, typename... Args>
struct IsVoid<Ret(__stdcall*)(Args...)>
{
    constexpr const static bool value = (std::is_void_v<Ret> == true);
};
template <typename Ret, typename classname, typename... Args>
struct IsVoid<Ret(__thiscall classname::*)(Args...)>
{
    constexpr const static bool value = (std::is_void_v<Ret> == true);
};
using Arkitekt::FnBlock;
template<typename TFn, bool NeedsCallerCleanup = true>
static FnBlock* CreateWrapper(const std::string_view& name, void* inPtrToWrap)
{
    Assembler& a = Assembler::Get()->Init();
    if (!inPtrToWrap)
        throw new std::bad_weak_ptr; // need a strong pointer that goes to the gym and shit


    void** labels;
    a.Begin(&labels);

    push()(&a, ebp);
    mov()(&a, ebp, esp);

    constexpr size_t argc = TotalStackSize<TFn>::argC;
    constexpr size_t fullStackSize = TotalStackSize<TFn>::size;

    if constexpr (IsVoidOr32Bit<TFn>::value)
        push()(&a, edx);

    if constexpr (IsVoid<TFn>::value)
        push()(&a, eax);

    push()(&a, ecx);
    push()(&a, ebx);
    push()(&a, esi);
    push()(&a, edi);

    if constexpr (argc > 0)
    {
        for (int32_t i = argc-1; i>=0; --i)
            push()(&a, RegisterIndex(ebp, 8, i)); // effectively, push [ebp+8+(4i)]
    }

    //TODO: handle non-MSVC compilers passing 'this' on the stack, vs. in ECX (only matters when our compiler differs from the target's compiler)
    call()(&a, inPtrToWrap);

    if constexpr (argc > 0 && NeedsCallerCleanup)
        add()(&a, esp, ImmediateU32(fullStackSize));

    pop()(&a, edi);
    pop()(&a, esi);
    pop()(&a, ebx);
    pop()(&a, ecx);

    if constexpr (IsVoid<TFn>::value)
        pop()(&a, eax);

    if constexpr (IsVoidOr32Bit<TFn>::value)
        pop()(&a, edx);

    mov()(&a, esp, ebp);
    pop()(&a, ebp);

    if constexpr ((not NeedsCallerCleanup) and argc > 0)
        ret()(&a, ImmediateU32(fullStackSize));
    else
        ret()(&a);

    auto* fn = a.FinalizeFunction(name.data(), labels);
    Log("%s %p", name.data(), fn);
    a.End();
    return fn;
}

template<typename TFn, bool NeedsCallerCleanup = true>
static FnBlock* CreateWrapper(const std::string_view& name, TFn inPtrToWrap)
{
    void* ptr;
    memcpy_s(&ptr, 4u, &inPtrToWrap, 4u);
    return CreateWrapper<TFn, NeedsCallerCleanup>(name.data(), ptr);
}
template<typename TFn, bool NeedsCallerCleanup = true>
static FnBlock* CreateWrapper(const std::string_view& name, size_t offset, const std::string_view& moduleName)
{

    auto* ptr = ((void*)(offset+ (uintptr_t)GetModuleHandleA(moduleName.data())));
    return CreateWrapper<TFn, NeedsCallerCleanup>(name.data(), ptr);
}


template <typename TFn>
static TFn DefineFunction(const std::string_view& name, size_t offset, const std::string_view& moduleName)
{
    void* ret = (void*)(offset+(uintptr_t)GetModuleHandleA(moduleName.data()));
    TFn trueRet = *(TFn* const)((const_cast<void** const>(&ret)));
    Log("%s - %p", name.data(), trueRet);
    return TFn(trueRet);
}
