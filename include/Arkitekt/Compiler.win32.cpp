/*
** This file has been pre-processed with DynASM.
** https://luajit.org/dynasm.html
** DynASM version 1.5.0, DynASM x86 version 1.5.0
** DO NOT EDIT! The original file is in "include/Arkitekt/Compiler.dcpp".
*/

#line 1 "include/Arkitekt/Compiler.dcpp"
#if false
//|.if 1
#endif
//|.endif

#include <Arkitekt/Shared.hpp>
#include <Arkitekt/Compiler.h>
#include <dynasm/dasm_proto.h>
#include <dynasm/dasm_x86.h>

//|.arch x86
#if DASM_VERSION != 10500
#error "Version mismatch between DynASM and included encoding engine"
#endif
#line 12 "include/Arkitekt/Compiler.dcpp"
//|.section code
#define DASM_SECTION_CODE	0
#define DASM_MAXSECTION		1
#line 13 "include/Arkitekt/Compiler.dcpp"
//|.globals lbl_
enum {
  lbl__func,
  lbl__MAX
};
#line 14 "include/Arkitekt/Compiler.dcpp"
//|.actionlist compiler_actions
static const unsigned char compiler_actions[54] = {
  254,0,250,15,248,10,85,137,229,255,82,255,80,255,81,83,86,87,255,252,255,
  181,233,255,81,255,252,255,21,237,255,95,94,91,89,255,88,255,90,255,137,252,
  236,93,255,194,236,255,195,255,3,37,237,255
};

#line 15 "include/Arkitekt/Compiler.dcpp"



using namespace Arkitekt;

void Arkitekt::Assembler::EmitHookWrapper(FunctionDefinition* forFunction, void* _targetHook)
{
	dasm_State** Dst = &this->dasm;
	tCallFlags _callSig = forFunction->GetCallFlags( );
	//|.code
	dasm_put(Dst, 0);
#line 25 "include/Arkitekt/Compiler.dcpp"
	//|.align 0x10            // align for x86
	//|->_func:               // begin function
	//| push ebp              // saving our previous stack frame
	//| mov ebp, esp
	dasm_put(Dst, 2);
#line 29 "include/Arkitekt/Compiler.dcpp"

	// save edx if it is not needed for our return value
	if ((_callSig & tCallFlags::_RETVOID) || not(_callSig & tCallFlags::_RETLONGLONG))
	{
		//| push edx
		dasm_put(Dst, 10);
#line 34 "include/Arkitekt/Compiler.dcpp"
	}
	if (_callSig & tCallFlags::_RETVOID) // save eax if it is not storing a return 
	{
		//| push eax
		dasm_put(Dst, 12);
#line 38 "include/Arkitekt/Compiler.dcpp"
	}
	//| push ecx              // commonly clobbered registers
	//| push ebx
	//| push esi
	//| push edi
	dasm_put(Dst, 14);
#line 43 "include/Arkitekt/Compiler.dcpp"

	// now the arguments are all out of order, so we push them to their correct locations
	if (forFunction->GetArgCount( ) > 0)
	{
		for (int32_t i = forFunction->GetArgCount( ) - 1;i >= 0;i--)
		{
			//| push aword [ebp + 8 + (4 * i)]
			dasm_put(Dst, 19, 8 + (4 * i));
#line 50 "include/Arkitekt/Compiler.dcpp"
		}
	}
	if ((_callSig & tCallFlags::_THISCALL))
	{
		//| push ecx
		dasm_put(Dst, 24);
#line 55 "include/Arkitekt/Compiler.dcpp"
	}

	//| call aword [_targetHook]     // call the hook (our hooks are all __stdcall so caller will cleanup the stack for us)
	dasm_put(Dst, 26, _targetHook);
#line 58 "include/Arkitekt/Compiler.dcpp"

	//| pop edi              // as above but in reverse
	//| pop esi
	//| pop ebx
	//| pop ecx
	dasm_put(Dst, 31);
#line 63 "include/Arkitekt/Compiler.dcpp"

	if ((_callSig & tCallFlags::_RETVOID))
	{
		//| pop eax
		dasm_put(Dst, 36);
#line 67 "include/Arkitekt/Compiler.dcpp"
	}

	if ((_callSig & tCallFlags::_RETVOID) || not(_callSig & tCallFlags::_RETLONGLONG))
	{
		//| pop edx
		dasm_put(Dst, 38);
#line 72 "include/Arkitekt/Compiler.dcpp"
	}

	//| mov esp, ebp
	//| pop ebp
	dasm_put(Dst, 40);
#line 76 "include/Arkitekt/Compiler.dcpp"

	if (not(_callSig & tCallFlags::_CDECL) && (forFunction->GetArgCount( ) > 0))
	{
		//| ret sizeof(uintptr_t) * forFunction->GetArgCount()
		dasm_put(Dst, 45, sizeof(uintptr_t) * forFunction->GetArgCount( ));
#line 80 "include/Arkitekt/Compiler.dcpp"
	}
	else
	{
		//| ret
		dasm_put(Dst, 48);
#line 84 "include/Arkitekt/Compiler.dcpp"
	}

}

void Arkitekt::Assembler::EmitSuperWrapper(FunctionDefinition auto* forFunction, void* _targetSuper)
{
	dasm_State** Dst = &this->dasm;
	auto _callSig = forFunction->GetCallFlags( );
	//|.code
	dasm_put(Dst, 0);
#line 93 "include/Arkitekt/Compiler.dcpp"
	//|.align 0x10
	//|->_func:
	//| push ebp
	//| mov ebp, esp
	dasm_put(Dst, 2);
#line 97 "include/Arkitekt/Compiler.dcpp"

	if ((_callSig & tCallFlags::_RETVOID) || !(_callSig & tCallFlags::_RETLONGLONG))
	{
		//| push edx
		dasm_put(Dst, 10);
#line 101 "include/Arkitekt/Compiler.dcpp"
	}
	if (_callSig & tCallFlags::_RETVOID)
	{
		//| push eax
		dasm_put(Dst, 12);
#line 105 "include/Arkitekt/Compiler.dcpp"
	}

	//| push ecx
	//| push ebx
	//| push esi
	//| push edi
	dasm_put(Dst, 14);
#line 111 "include/Arkitekt/Compiler.dcpp"

	if (forFunction->GetArgCount( ) > 0)
	{
		for (int32_t i = forFunction->GetArgCount( ) - 1;i >= 0;i--)
		{
			//| push aword [ebp + 8 + (4 * i)]
			dasm_put(Dst, 19, 8 + (4 * i));
#line 117 "include/Arkitekt/Compiler.dcpp"
		}
	}

	//| call aword [_targetSuper]
	dasm_put(Dst, 26, _targetSuper);
#line 121 "include/Arkitekt/Compiler.dcpp"

	if ((_callSig & tCallFlags::_CDECL))
	{
		//| add esp, [sizeof(uintptr_t) * forFunction->GetArgCount()]
		dasm_put(Dst, 50, sizeof(uintptr_t) * forFunction->GetArgCount( ));
#line 125 "include/Arkitekt/Compiler.dcpp"
	}

	//| pop edi
	//| pop esi
	//| pop ebx
	//| pop ecx
	dasm_put(Dst, 31);
#line 131 "include/Arkitekt/Compiler.dcpp"

	if ((_callSig & tCallFlags::_RETVOID))
	{
		//| pop eax
		dasm_put(Dst, 36);
#line 135 "include/Arkitekt/Compiler.dcpp"
	}
	if ((_callSig & tCallFlags::_RETVOID) || not(_callSig & tCallFlags::_RETLONGLONG))
	{
		//| pop edx
		dasm_put(Dst, 38);
#line 139 "include/Arkitekt/Compiler.dcpp"
	}
	//| mov esp, ebp
	//| pop ebp
	dasm_put(Dst, 40);
#line 142 "include/Arkitekt/Compiler.dcpp"
	if (not(_callSig & tCallFlags::_CDECL) && (forFunction->GetArgCount( ) > 0))
	{
		//| ret sizeof(uintptr_t) * forFunction->GetArgCount()
		dasm_put(Dst, 45, sizeof(uintptr_t) * forFunction->GetArgCount( ));
#line 145 "include/Arkitekt/Compiler.dcpp"
	}
	else
	{
		//| ret
		dasm_put(Dst, 48);
#line 149 "include/Arkitekt/Compiler.dcpp"
	}
}

FnBlock& FnBlock::Create(FnBlock* ptr, const std::string_view& name, void* _mem, size_t _sz, void* _fn, void**& labels)
{
	ptr = (FnBlock*) std::_Allocate<alignof(FnBlock)>(sizeof(FnBlock));
	*ptr = FnBlock(_mem, _sz, _fn, &labels);
	return *ptr;
}

Arkitekt::Assembler* Arkitekt::Assembler::Get( )
{
	static Arkitekt::Assembler C;
	if (!C._blocks.size( )) C.Init( );
	return &C;
}

FnBlock* Arkitekt::Assembler::FinalizeFunction(const std::string_view& name, void**& _labels)
{
	size_t _outSz;
	dasm_link(&this->dasm, &_outSz);
	const auto hash = CHashMapCalculateHash(name.data( ));
	void* buf;
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
	_blocks.emplace(std::pair(hash, (FnBlock*) std::_Allocate<alignof(FnBlock)>(sizeof(FnBlock))));
	return ::new(_blocks[hash]) FnBlock(buf, _outSz, _labels[lbl__func], &_labels);
}

void Arkitekt::Assembler::Begin(void*** _labels)
{
	void* labels[lbl__MAX];
	dasm_init(&this->dasm, DASM_MAXSECTION);
	dasm_setupglobal(&this->dasm, labels, lbl__MAX);
	*_labels = labels;
	dasm_setup(&this->dasm, compiler_actions);
}

void Arkitekt::Assembler::End( )
{
	dasm_free(&this->dasm);
}

//|.if 1
#if 0
//|.endif
#endif
