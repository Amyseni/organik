#include "Synthetik.h"
#include "zhl_internal.h"
#include "zhl.h"
#include "Logging.h"
#include "DefinitionHelpers/VariableHelper.h"
#include "DefinitionHelpers/BuiltinHelper.h"

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
	
    *this = RValue();
// 	Organik::GetLogger()->LogFormatted("%s", Value);
    YYCreateString(this, const_cast<char*>(Value));
}

bool RValue::ContainsValue (std::string_view _name) const
{
	
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
	
	return REAL_RValue(this);
}

int32_t RValue::ToInt32() const
{
	
	return INT32_RValue(this);
}

int64_t RValue::ToInt64() const
{
	
	return INT64_RValue(this);
}

bool RValue::ToBoolean() const
{
	
	return BOOL_RValue(this);
}

YYObjectBase* RValue::ToObject() const
{
	
	return ToPointer<YYObjectBase*>();
}

CInstance* RValue::ToInstance() const
{
	
	return ToPointer<CInstance*>();
}

const char* RValue::ToCString() const
{
	return YYGetString(const_cast<RValue*>(this), 0);
}
RValue::~RValue()
{
	if (!this) {
		return;
	}
	if (this->m_Kind == VALUE_UNDEFINED) {
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

std::vector<RValue> RValue::ToVector() const
{
    if (m_Kind != VALUE_ARRAY)
        return {};

    std::vector<RValue> result;
	int len = GetArrayLength();
    result.reserve(len);

    for (int i = 0; i < len; i++)
    {

		// danger bad no
        result.push_back((*reinterpret_cast<RValue**>(
			&reinterpret_cast<char*>(this->m_Pointer)[ARRAY_OFFSET]
		)[i]));
    }

    return result;
}
std::vector<RValue*> RValue::ToRefVector()
{
    if (m_Kind != VALUE_ARRAY)
        return {};

    std::vector<RValue*> result;
	int len = GetArrayLength();
    result.reserve(len);

    for (int i = 0; i < len; i++)
    {

		// danger bad no
        result.push_back(&(*reinterpret_cast<RValue**>(
			&reinterpret_cast<char*>(this->m_Pointer)[ARRAY_OFFSET]
		)[i]));
    }

    return result;
}

int32_t RValue::GetArrayLength() const
{
	int len = *(int *)((&this->m_i32) + 0x7c);
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength: %d", len);
	RValue length;
	if (m_Kind != VALUE_ARRAY) {
		return 0;
	}
	DoBuiltinRef(&gml_array_length, length, {*this});
	if (length.m_Kind == VALUE_INT32) {
		return length.m_i32;
	} else {
		Error_Show_Action(
			const_cast<char*>(GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_INT32, got %s. Value %lld", length.GetKindName(), 
				length.m_i64).c_str()),
			true, true
		);
	}
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength (the \"correct way\"): %d", length.m_i32);
	return len;
}

int32_t RValue::GetArrayLength()
{
	int len = *(int *)(&((*this).m_i32) + 0x7c);
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength: %d", len);
	RValue length;
	if (m_Kind != VALUE_ARRAY) {
		return 0;
	}
	DoBuiltinRef(&gml_array_length, length, {*this});
	if (length.m_Kind == VALUE_INT32) {
		return length.m_i32;
	} else {
		Error_Show_Action(
			const_cast<char*>(GetLogger()->ParseFormatting("RValue::GetArrayLength: Expected VALUE_INT32, got %s. Value %lld", length.GetKindName(), 
				length.m_i64).c_str()),
			true, true
		);
	}
	Organik::GetLogger()->LogFormatted("RValue::GetArrayLength (the \"correct way\"): %d", length.m_i32);
	return len;
}

RValue* RValue::ToArray()
{
	
	return &(*reinterpret_cast<RValue**>(
		&reinterpret_cast<char*>(this->m_Pointer)[ARRAY_OFFSET]
	)[0]);
}

void* RValue::ToPointer() const
{
	
	return PTR_RValue(this);
}

RValue::RValue()
{
	this->m_Real = 0;
	this->m_Flags = 0;
	this->m_Kind = VALUE_UNDEFINED;
}

// RValue::~RValue()
// {
//     this->__Free();
// }

RValue::RValue(	const std::vector<RValue>& Values)
{
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
	
	// Initialize it to just empty stuff
	*this = RValue();

	// We can ignore this, because if it fails, we're just initialized to UNSET
	
	YYCreateString(this, const_cast<char*>(Value.data()));
}

RValue::RValue(bool Value)
{
	
	this->m_Real = static_cast<double>(Value);
	this->m_Flags = 0;
	this->m_Kind = VALUE_BOOL;
}

RValue::RValue(const RValue& Other)
{
	*this = RValue();
// 	Organik::GetLogger()->LogFormatted("setting value of %p to %s: %lld", this, GetKindName(), Other.m_i64);

	COPY_RValue(
		this,
		const_cast<RValue*>(&Other)
	);
}

RValue& RValue::operator=(const RValue& Other)
{
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
	// Initialize this RValue to unset.
	*this = RValue();
	

	StructCreate(
		this
	);

	for (auto [key, value] : Values)
	{
		// "value" gets copied by StructAddRValue.
		
		StructAddRValue(
			this,
			key.c_str(),
			&value
		);
	}
}

RValue& RValue::operator[](	IN size_t Index	)
{
	
	return *this->ToRefVector().at(Index); 
}

RValue::operator bool()
{
	
	return this->ToBoolean();
}

RValue::operator double()
{
	
	return this->ToDouble();
}

RValue::operator std::string()
{
	
	return this->ToString();
}

RValue::operator std::u8string()
{
	
	return this->ToUTF8String();
}

RValue::operator int32_t()
{
	
	return this->ToInt32();
}

RValue::operator int64_t()
{
	
	return this->ToInt64();
}

void RValue::__Free()
{
	if (!this) return;
	if (this->m_Kind == VALUE_UNDEFINED) return;
	
// 	Organik::GetLogger()->LogFormatted("%s:%d -- Freeing RValue of kind %s at %p", __FILE__, __LINE__, this->GetKindName(), this);
	FREE_RValue(this);

	this->m_i64 = 0;
	this->m_Flags = 0;
	this->m_Kind = VALUE_UNDEFINED;
}