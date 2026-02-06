#pragma once
#include <unordered_map>
#include <string>
#include <cstdint>
#include <string_view>
#include <dynasm/dasm_proto.h>
struct Action;
namespace Arkitekt {
    struct FnBlock;    
    using ::Action;
    struct ExecContext;
    struct Compiler;
    struct MemoryManager;
    constexpr std::hash<void*>* get_hasher();
    struct Blob;
    struct MemBlob;
};
using namespace Arkitekt;



using CHashMapHash = size_t;
struct dasm_State;




#ifdef ARK_GML
enum class GMLVariableType {

};
enum class GMLComparisonTypes {

};

// borrowed from UTMT/Underanalyzer tysm

#define    GMLOP_Conv    0x07u      
#define    GMLOP_Mul     0x08u      
#define    GMLOP_Div     0x09u      
#define    GMLOP_Rem     0x0Au      
#define    GMLOP_Mod     0x0Bu      
#define    GMLOP_Add     0x0Cu      
#define    GMLOP_Sub     0x0Du      
#define    GMLOP_And     0x0Eu      
#define    GMLOP_Or      0x0Fu      
#define    GMLOP_Xor     0x10u      
#define    GMLOP_Neg     0x11u      
#define    GMLOP_Not     0x12u      
#define    GMLOP_Shl     0x13u      
#define    GMLOP_Shr     0x14u      
#define    GMLOP_Cmp     0x15u       // the third byte of the DoubleTypeInstruction holds the comparison type
#define    GMLOP_Set     0x45u      
#define    GMLOP_Dup     0x86u      
#define    GMLOP_Ret     0x9Cu      
#define    GMLOP_Exit    0x9Du      
#define    GMLOP_Pop     0x9Eu      
#define    GMLOP_Br      0xB6u      
#define    GMLOP_Brt     0xB7u      
#define    GMLOP_Brf     0xB8u      
#define    GMLOP_PushEnv 0xBAu      
#define    GMLOP_PopEnv  0xBBu      
#define    GMLOP_PushCst 0xC0u       // push constant
#define    GMLOP_PushLoc 0xC1u       // push local variable (probably)
#define    GMLOP_PushGlb 0xC2u       // push global variable
#define    GMLOP_PushVar 0xC3u       // push other variable
#define    GMLOP_PushI16 0x84u       // push int16
                                     // NOTE: the push type is preserved from the previous version, so the program can check for any of these values
                                     //       instead of every kind separate and then checking the actual type
#define    GMLOP_Call    0xD9u      
#define    GMLOP_Break   0xFFu      
#endif

struct Action;

template <typename T> CHashMapHash CHashMapCalculateHash(T value)
{
    return std::hash<T>()(value);
}
namespace Arkitekt {
    struct FnBlock {
        void *m_Mem;
        size_t m_Size;
        void *m_Fn;
        constexpr FnBlock() = default;
        constexpr FnBlock(void* _mem, size_t _size, void* _fn): m_Mem(_mem), m_Size(_size), m_Fn(_fn) {}

        static Arkitekt::FnBlock* Add(const std::string_view& _name, void* _mem, size_t _size, void* _fn);
    };

    /* struct Action {
        enum class ErrorType {
            READ_NULL_REF=0
            // That's probablly the only error that ever happens, should be fine here, right?
        };
        virtual bool* IsFinalized() noexcept = 0;
        virtual Action** const GetNodeOwner() noexcept = 0;
        virtual void Draw() = 0;
        virtual void Emit(Arkitekt::Compiler* c) = 0;
        virtual void* ErrorHandler(const ErrorType& eT) {
            return nullptr;
        }
    }; */
    
    struct Compiler {
        friend struct Arkitekt::FnBlock;
    public:
        static Arkitekt::Compiler* Get();
    protected:
        dasm_State* dasm;
        unsigned long maxlocals;
        unsigned long nextlocal;
        bool Init();
        std::unordered_map<uint32_t, FnBlock> _blocks;
        std::unordered_map<uint32_t, std::string_view> m_scriptNames;
        std::unordered_map<uint32_t, void*> m_Locals;
        Compiler(): _blocks{}, m_Locals{}, dasm(nullptr), nextlocal(0), maxlocals(8) {};
        ~Compiler() = default;
    public:
        FnBlock* FinalizeFunction(const std::string_view& name, void**& _labels);
        void Begin(void*** _labels);
        void End();
        void EmitHookPrologue(size_t _argc, bool isVoid, bool isCdecl);
        void EmitHookEpilogue(size_t _argc, bool isVoid, bool isCdecl);
        void EmitCall(void* address);
        void EmitJump(void* address);
        void SaveStackFrame();
        void PopFrameAndRet(bool _isCdecl = false, int _nArgs = 2);
        void CommonPrologue(bool _isEventHook = true);
        void CommonEpilogue(bool _isEventHook = true);
        uint32_t DefineLabel(const std::string_view& _name);
        void GetInstance(const std::string_view& _name);
    };
}

