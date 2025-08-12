#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "Logging.h"
#include "DefinitionHelpers/VariableHelper.h"
#include "DefinitionHelpers/BuiltinHelper.h"

#define ARRAY_OFFSET 0x6c
const char* RValue::GetKindName() const
{
	Organik::GetLogger()->LogFormatted("RValue::GetKindName called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::GetKindName: m_Kind=%d", this->m_Kind);
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
			if (*((int*)this + 0x58) == 3) {
				return "method";
			} else {
				return "struct";
			}
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

RValue::RValue(const char* Value)
{
	Organik::GetLogger()->LogFormatted("RValue::RValue(const char* Value) called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: Value=%s", Value);
	
    *this = RValue();
// 	Organik::GetLogger()->LogFormatted("%s", Value);
    YYCreateString(this, const_cast<char*>(Value));
}

bool RValue::ContainsValue (std::string_view _name) const
{
	Organik::GetLogger()->LogFormatted("RValue::ContainsValue called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ContainsValue: _name=%s", _name.data());
	
    if ((m_Kind & MASK_KIND_RVALUE) == VALUE_OBJECT) {
        return (
			this->ToObject())
			->InternalReadYYVar(
				Code_Variable_FindAlloc_Slot_From_Name(
					(YYObjectBase*)this, 
					const_cast<char*>(_name.data()
				)
			)
		) != nullptr;
    }
// 	Organik::GetLogger()->LogFormatted("_name: %s", (_name.size() ?  _name.data() : "empty string"));
    return false;
}

double RValue::ToDouble() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToDouble called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToDouble: m_Real=%f", this->m_Real);
	
	return REAL_RValue(this);
}

int32_t RValue::ToInt32() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToInt32 called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToInt32: m_i32=%d", this->m_i32);
	
	return INT32_RValue(this);
}

int64_t RValue::ToInt64() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToInt64 called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToInt64: m_i64=%lld", this->m_i64);
	
	return INT64_RValue(this);
}

bool RValue::ToBoolean() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToBoolean called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToBoolean: m_Kind=%d", this->m_Kind);
	
	return BOOL_RValue(this);
}

YYObjectBase* RValue::ToObject() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToObject called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToObject: m_Pointer=%p", this->m_Pointer);
	
	return ToPointer<YYObjectBase*>();
}

CInstance* RValue::ToInstance() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToInstance called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToInstance: m_Pointer=%p", this->m_Pointer);
	
	return ToPointer<CInstance*>();
}

const char* RValue::ToCString() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToCString called for RValue at %p", this);

	
	return YYGetString(const_cast<RValue*>(this), 0);
}
RValue::~RValue()
{
	Organik::GetLogger()->LogFormatted("RValue::~RValue called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::~RValue: m_Kind=%d", this->m_Kind);
	
	if (!this) {
		return;
	}

	this->__Free();
}
std::string RValue::ToString() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToString called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToString: this->ToCString=%s", this->ToCString());
	
	return std::string(ToCString());
}

std::u8string RValue::ToUTF8String() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToUTF8String called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToUTF8String: this->ToCString=%s", this->ToCString());
	
	return reinterpret_cast<const char8_t*>(YYGetString(const_cast<RValue*>(this), 0));
}

std::vector<RValue> RValue::ToVector() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToVector called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToVector: m_Kind=%d", this->m_Kind);
    if (m_Kind != VALUE_ARRAY)
        return {};

    std::vector<RValue> result;
	int len = GetArrayLength();
    result.reserve(len);

    for (int i = 0; i < len; i++)
    {

		// danger bad no
        result.push_back(std::move(RValue(m_RefArray->m_Array[i])));
    }

    return result;
}

std::vector<RValue*> RValue::ToRefVector()
{
	Organik::GetLogger()->LogFormatted("RValue::ToRefVector called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToRefVector: m_Kind=%d", this->m_Kind);
    if (m_Kind != VALUE_ARRAY)
        return {};

    std::vector<RValue*> result;
	int len = GetArrayLength();
    result.reserve(len);

    for (int i = 0; i < len; i++)
    {

		// danger bad no
		// hi archie, try not to think about it too much okay?
        result.push_back(&m_RefArray->m_Array[i]);
    }

    return result;
}

int32_t RValue::GetArrayLength() const
{
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength: m_RefArray->length=%d", this->m_RefArray->length);
	int len = this->m_RefArray->length;
	RValue length;
	if ((m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY) {
		Error_Show_Action(
			const_cast<char*>(GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_ARRAY, got %s", GetKindName()).c_str()),
			true, true
		);
		return 0;
	}
	return len;
}

int32_t RValue::GetArrayLength()
{
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength: m_RefArray->length=%d", this->m_RefArray->length);
	int len = this->m_RefArray->length;
	RValue length;
	if ((m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY) {
		Error_Show_Action(
			const_cast<char*>(GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_ARRAY, got %s", GetKindName()).c_str()),
			true, true
		);
		return 0;
	}
	// DoBuiltinRef(&gml_array_length, length, {*this});
	// if (length.m_Kind == VALUE_INT32) {
	// 	return length.m_i32;
	// } else {
	// 	Error_Show_Action(
	// 		const_cast<char*>(GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_INT32, got %s. Value %lld", length.GetKindName(), 
	// 			length.m_i64).c_str()),
	// 		true, true
	// 	);
	// }
	// Organik::GetLogger()->LogFormatted("RValue::GetArrayLength (the \"correct way\"): %d", length.m_i32);
	return len;
}

RValue* RValue::ToArray()
{
	Organik::GetLogger()->LogFormatted("RValue::ToArray called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToArray: m_Pointer=%p", this->m_Pointer);
	Error_Show_Action(
		const_cast<char*>("RValue::ToArray called. Heap corruption imminent. Peace out."),
		true, true
	);
	return &(*reinterpret_cast<RValue**>(
		&reinterpret_cast<char*>(this->m_Pointer)[ARRAY_OFFSET]
	)[0]);
}

void* RValue::ToPointer() const
{
	Organik::GetLogger()->LogFormatted("RValue::ToPointer called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::ToPointer: m_Pointer=%p", this->m_Pointer);
	
	return PTR_RValue(this);
}

RValue::RValue()
{
	Organik::GetLogger()->LogFormatted("RValue::RValue() called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: m_Kind=%d", this->m_Kind);
	this->m_Real = 0;
	this->m_Flags = 0;
	this->m_Kind = VALUE_UNDEFINED;
}

// RValue::~RValue()
// {
//     this->__Free();
// }

RValue::RValue(const std::vector<RValue>& Values)
{
	Organik::GetLogger()->LogFormatted("RValue::RValue(const std::vector<RValue>& Values) called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: Values.size()=%zu", Values.size());
	// Initialize to undefined
	*this = RValue();
    
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
			&reinterpret_cast<char*>(this->m_Pointer)[ARRAY_OFFSET]
		)[index]);
		

		*member_value = std::data(Values)[index];
		
	}
}

RValue::RValue(std::string_view Value)
{
	Organik::GetLogger()->LogFormatted("RValue::RValue(std::string_view Value) called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: Value=%s", Value.data());
	
	// Initialize it to just empty stuff
	*this = RValue();

	// We can ignore this, because if it fails, we're just initialized to UNSET
	
	YYCreateString(this, const_cast<char*>(Value.data()));
}

RValue::RValue(bool Value)
{
	Organik::GetLogger()->LogFormatted("RValue::RValue(bool Value) called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: Value=%d", Value);
	
	this->m_Real = static_cast<double>(Value);
	this->m_Flags = 0;
	this->m_Kind = VALUE_BOOL;
}

RValue::RValue(const RValue& Other)
{
	Organik::GetLogger()->LogFormatted("RValue::RValue(const RValue& Other) called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: Other.m_Kind=%d", Other.m_Kind);
	*this = RValue();
// 	Organik::GetLogger()->LogFormatted("setting value of %p to %s: %lld", this, GetKindName(), Other.m_i64);

	COPY_RValue(
		this,
		const_cast<RValue*>(&Other)
	);
}

RValue& RValue::operator=(const RValue& Other)
{
	Organik::GetLogger()->LogFormatted("RValue::operator= called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::operator=: Other.m_Kind=%d", Other.m_Kind);
	FREE_RValue(this);
// 	Organik::GetLogger()->LogFormatted("setting value of %p to %s: %lld", this, GetKindName(), Other.m_i64);

	COPY_RValue(
		this,
		const_cast<RValue*>(&Other)
	);

	return *this;
}

RValue::RValue(const std::map<std::string, RValue>& Values)
{
	Organik::GetLogger()->LogFormatted("RValue::RValue(const std::map<std::string, RValue>& Values) called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::RValue: Values.size()=%zu", Values.size());
	// Initialize this RValue to unset.
	*this = RValue();
	

	Organik::GetLogger()->LogFormatted("StructCreate called for RValue at %p", this);
	StructCreate(
		this
	);

	Organik::GetLogger()->LogFormatted("Iterating over Values");
	for (auto [key, value] : Values)
	{
		Organik::GetLogger()->LogFormatted("StructAddRValue called with key=%s, value=%p", key.c_str(), &value);
		StructAddRValue(
			this,
			key.c_str(),
			&value
		);
	}
}

RValue& RValue::operator[](	IN size_t Index	)
{
	Organik::GetLogger()->LogFormatted("RValue::operator[] called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::operator[]: Index=%zu", Index);
	
	return *this->ToRefVector().at(Index); 
}

RValue::operator bool()
{
	Organik::GetLogger()->LogFormatted("RValue::operator bool() called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::operator bool: m_Kind=%d", this->m_Kind);
	
	return this->ToBoolean();
}

RValue::operator double()
{
	Organik::GetLogger()->LogFormatted("RValue::operator double() called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::operator double: m_Real=%f", this->m_Real);
	
	return this->ToDouble();
}

RValue::operator std::string()
{
	Organik::GetLogger()->LogFormatted("RValue::operator std::string() called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::operator std::string: this->ToCString=%s", this->ToCString());

	return this->ToCString();
}

void RValue::__Free()
{
	Organik::GetLogger()->LogFormatted("RValue::__Free called for RValue at %p", this);
	Organik::GetLogger()->LogFormatted("RValue::__Free: m_Kind=%d", this->m_Kind);
	Organik::GetLogger()->LogFormatted("__Free called for RValue at %p", this);
	if (!this) return;
	Organik::GetLogger()->LogFormatted("__Free: Checking if RValue is undefined");
	//if (this->m_Kind == VALUE_UNDEFINED) return;
	Organik::GetLogger()->LogFormatted("__Free: Freeing RValue of kind %s at %p", this->GetKindName(), this);
	FREE_RValue(this);
	Organik::GetLogger()->LogFormatted("__Free: Freed");
}