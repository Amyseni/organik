#include "Synthetik.h"

#include "Globals.h"
#include "Action.h"
#include "DefinitionHelpers/VariableHelper.h"


// RValue::operator Action*()
// {
// 	if (this->GetKind() == VALUE_ACTION) {
// 		return reinterpret_cast<Action*>(this);
// 	}
// 	return nullptr;
// }
// RValue::operator Action&()
// {
// 	if (this->GetKind() == VALUE_ACTION) {
// 		return *reinterpret_cast<Action*>(this);
// 	}
// 	throw std::bad_cast();
// }
#define ARRAY_OFFSET 0x6c
const char* RValue::GetKindName() const
{
	
	switch (this->m_Kind & MASK_KIND_RVALUE) {
	case 0:
		return "number";
	case 1:
		return "string";
	case 2:
		return "array";
	case 3:
		return "ptr";
	case 4:
		return "vec3";
	case 5:
		return "undefined";
	case 6:
		return "object";
	case 7:
		return "int32";
	case 8:
		return "vec4";
	case 9:
		return "vec44";
	case 0xa:
		return "int64";
	case 0xb:
		return "accessor";
	case 0xc:
		return "null";
	case 0xd:
		return "bool";
	case 0xe:
		return "iterator";
	default:
		return "unknown";
	}
	return KIND_NAME_RValue(this);
}

// const char* RValue::GetStringInner()
// {
// 	
//     return (((m_Kind & MASK_KIND_RVALUE) == VALUE_STRING) && m_String) ? *m_String : "";
// }

uint32_t RValue::GetKind(const RValue& rv)
{
	return rv.GetKind();
}

uint32_t RValue::GetKind() const
{
	switch (this->m_Kind & MASK_KIND_RVALUE) {
	case 0:
		return VALUE_REAL;
	case 1:
		return VALUE_STRING;
	case 2:
		return (m_Flags == 1) ? VALUE_VECTOR : VALUE_ARRAY;
	case 3:
		return VALUE_PTR;
	case 4:
		return VALUE_VEC3;
	case 5:
		return VALUE_UNDEFINED;
	case 6:
		return VALUE_OBJECT;
	case 7:
		return VALUE_INT32;
	case 8:
		return VALUE_VEC4;
	case 9:
		return VALUE_VEC44;
	case 0xa:
		return VALUE_INT64;
	case 0xb:
		if ((m_Flags) == 1)
			return VALUE_REF;
		else if ((m_Flags) == 2)
			return VALUE_FUNCTION;
		else
			return VALUE_ACCESSOR;	
	case 0xc:
		return VALUE_NULL;
	case 0xd:
		return VALUE_BOOL;
	case 0xe:
		return VALUE_ITERATOR;
	case ((VALUE_ARRAY << 16) | VALUE_FUNCTION):
		return VALUE_ACTIONARRAY;
	default:
		return VALUE_UNDEFINED;
	}
}
// EventMap* RValue::ToEventMap()
// {
// 	if (GetKind() == VALUE_OBJECT) {
// 		return *reinterpret_cast<EventMap**>(std::launder(&this->m_Pointer));
// 	}
// 	Error_Show_Action(
// 		GetLogger()->ParseFormatting("Expected MAP, got %s", GetKindName()).c_str(), 
// 		true, true
// 	);
// 	return nullptr;
// }
// EventMap* RValue::ToEventMap()
// {
// 	if (GetKind() == VALUE_OBJECT) {
// 		return *reinterpret_cast<EventMap**>(std::launder(&this->m_EventMap));
// 	}
// 	Error_Show_Action(
// 		GetLogger()->ParseFormatting("Expected OBJECT, got %s", GetKindName()).c_str(), 
// 		true, true
// 	);
// 	return nullptr;
// }

RValue::RValue(const char* Value)
{
	*this = RValue();
	YYCreateString(this, Value);
}

bool RValue::ContainsValue(std::string_view _name) const
{
	if ((m_Kind & MASK_KIND_RVALUE) == VALUE_OBJECT) {
		YYObjectBase* pObj = reinterpret_cast<YYObjectBase*>(*(std::launder(&this->m_Pointer)));
		return (
			pObj->InternalReadYYVar(
				Code_Variable_FindAlloc_Slot_From_Name(
					pObj,
					const_cast<char*>(_name.data())
				)
			) != nullptr
		);
	}
	else if ((m_Kind & MASK_KIND_RVALUE) == VALUE_ARRAY) {
		RefDynamicArrayOfRValue* pObj = reinterpret_cast<RefDynamicArrayOfRValue*>(
			*(std::launder(&this->m_Pointer)));

		return pObj->m_Length > std::stoi(std::string(_name));
	}
	return false;
}
bool RValue::ContainsValue(int32_t idx) const
{
	if ((m_Kind & MASK_KIND_RVALUE) == VALUE_OBJECT) {
		YYObjectBase* pObj = reinterpret_cast<YYObjectBase*>(*(std::launder(&this->m_Pointer)));

		return (
			pObj->InternalReadYYVar(idx)
		) != nullptr;
	}
	else if ((m_Kind & MASK_KIND_RVALUE) == VALUE_ARRAY) {
		RefDynamicArrayOfRValue* pObj = *reinterpret_cast<RefDynamicArrayOfRValue* const*>(
			std::launder(&this->m_Pointer)
		);

		return pObj->m_Length > idx;
	}
	return false;
}

double RValue::ToDouble() const
{
	
	if (m_Kind == VALUE_REAL)
	{
		const double* d_Rv = reinterpret_cast<const double*>(std::launder(&this->m_Real));
		return *d_Rv;
	}
	return 0.0;
}

int32_t RValue::ToInt32() const
{
	

	if (m_Kind == VALUE_INT32 || m_Kind == VALUE_INT64 || m_Kind == VALUE_REAL)
	{
		const int* d_i32 = reinterpret_cast<const int*>(std::launder(&this->m_Real));
		return *d_i32 ? *d_i32 : static_cast<int32_t>(ToDouble());
	}
	return 0;
}

int64_t RValue::ToInt64() const
{
	if (m_Kind == VALUE_INT64 || m_Kind == VALUE_INT32 || m_Kind == VALUE_REAL)
	{
		const int64_t* d_i64 = reinterpret_cast<const int64_t*>(std::launder(&this->m_i64));
		return *d_i64;
	}
	return 0ll;
}

bool RValue::ToBoolean() const
{
	return (
		(*reinterpret_cast<const double*>(std::launder(&this->m_Real))) != 0) 
		|| ((*reinterpret_cast<const int*>(std::launder(&this->m_i32))) != 0);
}

YYObjectBase* RValue::ToObject() const
{
	return reinterpret_cast<YYObjectBase*>(*(std::launder(&this->m_Pointer)));
}

CInstance* RValue::ToInstance() const
{
	return reinterpret_cast<CInstance*>(*(std::launder(&this->m_Pointer)));
}

const char* RValue::ToCString() const
{
	return YYGetString(reinterpret_cast<const RValue*>(std::launder(&this->m_i32)), 0);
}
RValue::~RValue()
{
	if (!this) {
		return;
	}

	this->__Free();
}
std::string RValue::ToString() const
{
	return std::string(ToCString());
}

std::u8string RValue::ToUTF8String() const
{
	return reinterpret_cast<const char8_t*>(YYGetString(const_cast<RValue*>(this), 0));
}


std::vector<RValue*> RValue::ToRefVector()
{
    if ((this->m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY)
        return {};

    std::vector<RValue*> result;
    
	RefDynamicArrayOfRValue* arrayRef = 
		*reinterpret_cast<RefDynamicArrayOfRValue* const*>(std::launder(&this->m_RefArray));
	
	if (!arrayRef || !arrayRef->m_Array) return {};
	
	int len = arrayRef->m_Length;
	
	for (int i = 0; i < len; i++) {
		result.push_back(&(arrayRef->m_Array[i]));
	}

    return result;
}

int32_t RValue::GetArrayLength() const
{
	// Log("RValue::GetArrayLength called for RValue at %p", this);
	// Log("RValue::GetArrayLength: m_RefArray->length=%d", this->m_RefArray->length);
	if ((m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY) {
		Error_Show_Action(
			GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_ARRAY, got %s", GetKindName()).c_str(), 
			true, true
		);
		return 0;
	}
	int len = (*reinterpret_cast<RefDynamicArrayOfRValue* const*>(std::launder(&this->m_RefArray)))->m_Length;
	return len;
}

int32_t RValue::GetArrayLength()
{
	if ((m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY) {
		Error_Show_Action(
			GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_ARRAY, got %s", GetKindName()).c_str(), 
			true, true
		);
		return 0;
	}
	int len = (*reinterpret_cast<RefDynamicArrayOfRValue* const*>(std::launder(&this->m_RefArray)))->m_Length;
	return len;
	// DoBuiltinRef(&gml_array_length, length, {*this});
	// if (length.m_Kind == VALUE_INT32) {
	// 	return length.ToInt32();
	// } else {
	// 	Error_Show_Action(
	// 		const_cast<char*>(GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_INT32, got %s. Value %lld", length.GetKindName(), 
	// 			length.ToInt64()).c_str()),
	// 		true, true
	// 	);
	// }
	// Log("RValue::GetArrayLength (the \"correct way\"): %d", length.ToInt32);
	// return len;
}

RValue* RValue::ToArray()
{
	return &(*reinterpret_cast<RValue**>(
		(&reinterpret_cast<char*>(*std::launder(&this->m_Pointer))[ARRAY_OFFSET])
	)[0]);
}

void* RValue::ToPointer() const
{
	// Log("RValue::ToPointer called for RValue at %p", this);
	// Log("RValue::ToPointer: m_Pointer=%p", this->m_Pointer);
	const PVOID* d_Ptr = reinterpret_cast<const PVOID*>(std::launder(&this->m_Pointer));
	return *d_Ptr;
}

RValue& RValue::push_back(const RValue& value)
{
	if ((m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY) {
		Error_Show_Action(
			GetLogger()->ParseFormatting("RValue::push_back: Expected VALUE_ARRAY, got %s", GetKindName()).c_str(), 
			true, true
		);
		return *this;
	}
	RefDynamicArrayOfRValue* arrayRef = 
		*reinterpret_cast<RefDynamicArrayOfRValue* const*>(std::launder(&this->m_RefArray));

	arrayRef->m_Length += 1;
	YYArrayResize((void**) &arrayRef->m_Array, (arrayRef->m_Length) << 4, __FILE__, 0x4a7);

	COPY_RValue(
		&arrayRef->m_Array[arrayRef->m_Length - 1],
		&value
	);
	return (*this)[(size_t) GetArrayLength() - 1];
}

std::vector<RValue> RValue::ToVector()
{
	if ((this->m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY)
		return {};

	std::vector<RValue> result;
	RefDynamicArrayOfRValue* arrayRef = 
		*reinterpret_cast<RefDynamicArrayOfRValue* const*>(std::launder(&this->m_RefArray));


	if (!arrayRef || !arrayRef->m_Array) return {};

	int len = GetArrayLength();

	for (int i = 0; i < len; i++) {
		result.push_back((arrayRef->m_Array[i]));
	}

    return result;
}

RValue::RValue()
{
	
	*std::launder(&this->m_Real) = 0;
	this->m_Flags = 0;
	this->m_Kind = VALUE_UNDEFINED;
}



RValue::RValue(const std::vector<RValue>& Values)
{
	
	// Initialize to undefined
	*reinterpret_cast<RValue*>(std::launder(this)) = RValue();

	std::vector<double> dummy_array(Values.size(), 0.0);

	// Initialize this RValue as an array
	YYCreateArray(
		this,
		static_cast<int>(dummy_array.size()),
		dummy_array.data()
	);

	// Use direct object manipulation to set the actual values
	for (size_t index = 0; index < Values.size(); index++)
	{

		RValue* member_value = &(*reinterpret_cast<RValue**>(
			&reinterpret_cast<char*>(*std::launder(&this->m_Pointer))[ARRAY_OFFSET]
			)[index]);


		*member_value = std::data(Values)[index];

	}
}

RValue::RValue(std::string_view Value)
{
	

	// Initialize it to just empty stuff
	FREE_RValue(this);
	// We can ignore this, because if it fails, we're just initialized to UNSET

	YYCreateString(this, Value.data());
	this->m_Kind = VALUE_STRING;
}

RValue::RValue(bool Value)
{
	
	FREE_RValue(this);

	*reinterpret_cast<int64_t*>(std::launder(&this->m_i64)) = static_cast<int64_t>(Value);
	this->m_Kind = VALUE_BOOL;
	this->m_Flags = 0;
}

RValue::RValue(const RValue& Other)
{
	
	FREE_RValue(this);
	

	COPY_RValue(
		this,
		&Other
	);

	if (this->m_Kind == VALUE_UNDEFINED || this->m_Kind == VALUE_NULL || this->m_Kind == VALUE_UNSET) {
		this->m_Kind = VALUE_REAL;
		this->m_Real = 0.0;
		this->m_Flags = 0;
	}
}


RValue& RValue::operator=(const std::vector<RValue>& Values)
{
	FREE_RValue(this);
	this->m_Kind = VALUE_ARRAY;
	this->m_Flags = 0;
	std::vector<double> dummy_array(Values.size(), 0.0);

	YYCreateArray(
		this,
		static_cast<int>(Values.size()),
		dummy_array.data()
	);
	for (size_t index = 0; index < Values.size(); index++)
	{
		COPY_RValue(
			&(*this)[index],
			&Values[index]
		);
	}
	return *this;

}
RValue& RValue::back()
{
	if ((this->m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY)
		Error_Show_Action(
			GetLogger()->ParseFormatting("RValue::back: Expected VALUE_ARRAY, got %s with flag %d", GetKindName(), m_Flags).c_str(), 
			true, true
		);
	return (*this)[GetArrayLength() - 1];
}
// RValue& RValue::operator=(Action* actionPointer)
// {
// Log("RValue::operator= called for RValue at %p", this);
// Log("RValue::operator=: Other.m_Kind=%d", Other.m_Kind);
// 	FREE_RValue(this);
// 	Log("setting value of %p to %s: %lld", this, GetKindName(), Other.ToInt64());
// 	memcpy(this, actionPointer, sizeof(RValue));
// 	this->m_Kind = static_cast<RValueType>(VALUE_ACTION);
// 	return *this;
// }

RValue& RValue::operator=(const RValue& Other)
{
	FREE_RValue(this);

	COPY_RValue(
		this,
		&Other
	);
	if (this->m_Kind == VALUE_UNDEFINED || this->m_Kind == VALUE_NULL || this->m_Kind == VALUE_UNSET) {
		this->m_Kind = VALUE_REAL;
		this->m_Real = 0.0;
		this->m_Flags = 0;
	}
	return *this;
}
// RValue& RValue::operator=(RValue&& Other) noexcept
// {
// 	FREE_RValue(this);

// 	COPY_RValue(
// 		this,
// 		&Other
// 	);
// 	if (this->m_Kind == VALUE_UNDEFINED || this->m_Kind == VALUE_NULL || this->m_Kind == VALUE_UNSET) {
// 		this->m_Kind = VALUE_REAL;
// 		this->m_Real = 0.0;
// 		this->m_Flags = 0;
// 	}
// 	return *this;
// }

RValue::RValue(const std::map<std::string, RValue>& Values)
{
	*this = RValue();

	StructCreate(
		this
	);
	this->m_Kind = VALUE_OBJECT;
	this->m_Flags = 0;
	YYObjectBase* pObj = reinterpret_cast<YYObjectBase*>(*(std::launder(&this->m_Pointer)));
	for (auto [key, value] : Values)
	{
		*pObj->InternalGetYYVarRef(
			Code_Variable_FindAlloc_Slot_From_Name(
				pObj,
				key.c_str()
			)
		) = value;
	}
}
RValue::RValue(const std::unordered_map<int32_t, RValue>& Values)
{
	*this = RValue();

	StructCreate(
		this
	);
	this->m_Kind = VALUE_OBJECT;
	this->m_Flags = 0;
	YYObjectBase* pObj = reinterpret_cast<YYObjectBase*>(*(std::launder(&this->m_Pointer)));

	for (auto [key, value] : Values)
	{
		*pObj->InternalGetYYVarRef(
			key
		) = value;
	}
}

RValue& RValue::operator[](size_t Index)
{
// 	Log("RValue::operator[] called for RValue at %p, Index=%zu", this, Index);
	
	if (!((this->m_Kind & MASK_KIND_RVALUE) == VALUE_ARRAY))
		Error_Show_Action(
			GetLogger()->ParseFormatting("RValue::operator[]: Expected VALUE_ARRAY, got %s with flag %d", GetKindName(), m_Flags).c_str(), 
			true, true
		);

	RefDynamicArrayOfRValue* arrayRef = 
            *reinterpret_cast<RefDynamicArrayOfRValue* const*>(std::launder(&this->m_RefArray));
        
	if (!arrayRef || !arrayRef->m_Array)
		Error_Show_Action(
			"RValue::operator[]: Array is null", 
			true, true
		);

	if (Index < 0 || Index >= static_cast<size_t>(arrayRef->m_Length))
		Error_Show_Action(
			GetLogger()->ParseFormatting("RValue::operator[]: Index %zu out of bounds for array of length %d", Index, arrayRef->m_Length).c_str(), 
			true, true
		);

	return arrayRef->m_Array[Index];
}
RValue& RValue::operator[](RValue Index)
{
// 	Log("RValue::operator[] called for RValue at %p, Index=%d", this, idx);
	size_t idx = parseRValueNumber<size_t>(Index);
	switch(Index.m_Kind & MASK_KIND_RVALUE) {
		case VALUE_ARRAY:
			
			return ((*this)[(idx)]);
		case VALUE_OBJECT:
			
			return *this->ToObject()->InternalGetYYVarRef(int32_t(idx));
		default:
			Error_Show_Action(
				GetLogger()->ParseFormatting("RValue::operator[]: Expected VALUE_ARRAY or VALUE_OBJECT, got %s with flag %d", GetKindName(), m_Flags).c_str(), 
				true, true
			);
			break;
	}
	return *this;
}
// RValue RValue::operator[](RValue Index) const
// {
// 	int32_t idx = parseRValueNumber<int32_t>(Index);
// // 	Log("RValue::operator[] called for RValue at %p, Index=%d", this, idx);

// 	switch(m_Kind & MASK_KIND_RVALUE) {
// 		case VALUE_ARRAY:
// 			return (*this)[static_cast<size_t>(idx)];
// 		case VALUE_OBJECT:
// 			return *this->ToObject()->InternalGetYYVarRef(idx);
// 		default:
// 			Error_Show_Action(
// 				GetLogger()->ParseFormatting("RValue::operator[]: Expected VALUE_ARRAY or VALUE_OBJECT, got %s with flag %d", GetKindName(), m_Flags).c_str(), 
// 				true, true
// 			);
// 			break;
// 	}
// 	return *this;
// }
RValue& ReadStructValue(RValue* p_Struct, RValue index)
{
	int32_t hash = 0;
	if (index.GetKind() == VALUE_STRING) {
		hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, index.ToCString());
	} else if (index.GetKind() == VALUE_INT32 || index.GetKind() == VALUE_REAL) {
		hash = parseRValueNumber<int32_t>(index);
	} else {
		Error_Show_Action(
			GetLogger()->ParseFormatting("ReadStructValue: Expected string or int32 for member name, got %s", index.GetKindName()).c_str(), 
			true, true
		);
		return *p_Struct;
	}
	if ((p_Struct->m_Kind & MASK_KIND_RVALUE) != VALUE_OBJECT) {
		Error_Show_Action(
			GetLogger()->ParseFormatting("ReadStructValue: Expected VALUE_OBJECT, got %s", p_Struct->GetKindName()).c_str(), 
			true, true
		);
		return *p_Struct;
	}
	YYObjectBase* pObj = reinterpret_cast<YYObjectBase*>(*(std::launder(&p_Struct->m_Pointer)));

	RValue* pVal = pObj->InternalReadYYVar(hash);
	if (!pVal) {
		Error_Show_Action(
			GetLogger()->ParseFormatting("ReadStructValue: Object does not contain member with hash 0x%X", hash).c_str(), 
			true, true
		);
		return *p_Struct;
	}
	return *pVal;
}
void RValue::__Free()
{
	if (this == nullptr) return;
	FREE_RValue(reinterpret_cast<RValue*>(std::launder(this)));
	this->m_Kind = VALUE_UNDEFINED;
}