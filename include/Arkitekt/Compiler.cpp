#if false
|.if 1
||#endif
|.endif
#include <Arkitekt/Shared.hpp>
#include <Arkitekt/Compiler.h>
#include <dynasm/dasm_proto.h>
#include <dynasm/dasm_x86.h>

|.arch x86
|.section code
|.globals lbl_
|.actionlist compiler_actions



using namespace Arkitekt;
void Arkitekt::Compiler::EmitCall(void* address)
{
    dasm_State** Dst = &this->dasm;
    | call dword [address]
}
void Arkitekt::Compiler::EmitJump(void* address)
{
    dasm_State** Dst = &this->dasm;
    | jmp dword [address]
}


void Arkitekt::Compiler::EmitHookPrologue(size_t _nArgs, bool _isVoid, bool _isCdecl)
{
    dasm_State** Dst = &this->dasm;
    |.code
    |.align 0x10
    |->_func:
    | push ebp
    | mov ebp, esp
    if (_isVoid)
    {
        | push edx
        | push eax
    }
    | push ecx
    | push ebx
    | push esi
    | push edi
    if (_nArgs > 0) 
    {
        for (int32_t i=_nArgs-1;i>=0;i--)
        {
            | push dword [ebp+8+(4*i)]
        }
    }
}

void Arkitekt::Compiler::EmitHookEpilogue(size_t _nArgs, bool _isVoid, bool _isCdecl)
{
    dasm_State** Dst = &this->dasm;
    if (_isCdecl && _nArgs > 0)
    {
        | add esp, 4*_nArgs
    }
    | pop edi
    | pop esi
    | pop ebx
    | pop ecx
    if (_isVoid)
    {
        | pop eax
        | pop edx
    }
    | mov esp, ebp
    | pop ebp
    if (!_isCdecl && (_nArgs > 0))
        | ret sizeof(uintptr_t) * _nArgs
    else
        | ret
}
void Arkitekt::Compiler::SaveStackFrame()
{
    dasm_State** Dst = &this->dasm;
    | push ebp
    | mov ebp, esp
}
void Arkitekt::Compiler::PopFrameAndRet(bool _isCdecl /* = false */, int _nArgs /* = 2 */)
{
    dasm_State** Dst = &this->dasm;
    | mov esp, ebp
    | pop ebp
    if (!_isCdecl && _nArgs > 0)
        | ret sizeof(uintptr_t) * _nArgs
    else
        | ret
}


// unused currently
void Arkitekt::Compiler::CommonPrologue(bool _isVoid /* = true */)
{
    dasm_State** Dst = &this->dasm;
    if (_isVoid)
    {
        | push eax
        | push edx
    }
    | push ecx
    | push ebx
    | push esi
    | push edi
}
void Arkitekt::Compiler::CommonEpilogue(bool _isVoid /* = true */)
{
    dasm_State** Dst = &this->dasm;
    | pop edi
    | pop esi
    | pop ebx
    | pop ecx
    if (_isVoid)
    {
        | pop edx
        | pop eax
    }
}

/* uint32_t Arkitekt::Compiler::DefineLabel(const std::string_view& _name)
{
    
} */

FnBlock* FnBlock::Add(const std::string_view& name, void* _mem, size_t _sz, void* _fn)
{
    auto hash=CHashMapCalculateHash(name.data());
    FnBlock* ptr = (FnBlock*) std::_Allocate<alignof(FnBlock)>(sizeof(FnBlock));
    *ptr = FnBlock(_mem, _sz, _fn);
    return ptr;
}

Arkitekt::Compiler* Arkitekt::Compiler::Get() {
    static Arkitekt::Compiler C;
    if (!C._blocks.size()) C.Init();
    return &C;
}

FnBlock* Arkitekt::Compiler::FinalizeFunction(const std::string_view& name, void**& _labels)
{
    const CHashMapHash _hash = CHashMapCalculateHash(name.data());
    size_t _outSz;
    dasm_link(&this->dasm, &_outSz);
    void *buf;
#ifdef _WIN32
    buf = VirtualAlloc(0, _outSz, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
#else
    buf = mmap(0, _outSz, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif
    dasm_encode(&this->dasm, buf);
#ifdef _WIN32
    { DWORD dwOld; VirtualProtect(buf, _outSz, PAGE_EXECUTE_READ, &dwOld); }
#else
    mprotect(buf, _outSz, PROT_READ | PROT_EXEC);
#endif
    return FnBlock::Add(name.data(), buf, _outSz, _labels[lbl__func]);
}
void Arkitekt::Compiler::Begin(void*** _labels)
{
    void *labels[lbl__MAX];
    dasm_init(&this->dasm, DASM_MAXSECTION);
    dasm_setupglobal(&this->dasm, labels, lbl__MAX);
    *_labels = labels;
    dasm_setup(&this->dasm, compiler_actions);
}

void Arkitekt::Compiler::End()
{
    dasm_free(&this->dasm);
}

bool Arkitekt::Compiler::Init()
{
    this->_blocks = std::unordered_map<CHashMapHash, FnBlock>();
    return true;
}
|.if 1
||#if 0
|.endif
#endif