#include <GameMaker_Defs.h>
#include "Synthetik.h"

    RValue::~RValue() {
		__localFree();
	} 

	void __localFree(void)
	{
		if (YYFree_valid_vkind(m_Kind)) {
			FREE_RValue__Pre(this);
		} 
	} 

	void __localCopy(RValue* _v) {
		COPY_RValue__Post(this, &_v);
	} 

	void __localCopy_nodest(RValue* _v) {
		COPY_RValue__Post_nodest(this, &_v);
	} 

	RValue::RValue() {
		m_Kind = VALUE_UNSET;
		innerVal.m_Pointer = NULL;
	} 

	RValue::RValue(RValue* _v) {
		__localCopy_nodest(_v);
	} 

	RValue::RValue(double _val) {
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _val;
	}

	RValue::RValue(bool _val) {
		m_Kind = VALUE_BOOL;
		innerVal.m_Real = _val ? 1 : 0;
	}

	RValue::RValue(float _val) {
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _val;
	}

	RValue::RValue(int _val) {
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _val;

	}

	RValue::RValue(long _val) {
		m_Kind = VALUE_INT32;
		innerVal.m_i32 = _val;

	}

	RValue::RValue(int _val, bool _f, bool _b) {
		m_Kind = VALUE_INT32;
		innerVal.m_i32 = _val;

	}

	RValue::RValue(long long _val) {
		m_Kind = VALUE_INT64;
		innerVal.m_i64 = _val;
	}

	RValue::RValue(const char* _pStr) {
		YYSetString(this, _pStr);
	}

	RValue::RValue(const char* _pStr, bool _dontCare) {

		YYConstString(this, _pStr);
	}

	RValue::RValue(YYObjectBase* _p) {
		m_Kind = VALUE_OBJECT;
		innerVal._objects.m_Object = _p;
	}

	RValue::RValue(PFUNC_YYGMLScript _pMethod, CInstance* _pSelf) {
		YYSetScriptRef(this, (PFUNC_YYGMLScript_Internal)_pMethod, (YYObjectBase*)_pSelf);
	}

	RValue::RValue(int a, bool _ref)
	{
		int index = a & 0x00ffffff;
		int refType = ((a >> 24) & 0xff);
		if (refType == 14) {
			refType = REFID_INSTANCE;
		} 
		else
			refType |= REFCAT_RESOURCE;
		m_Kind = VALUE_REF;
		innerVal.m_i64 = MAKE_REF(refType, index);
	}

	RValue RValue::operator-() {
		RValue ret;
		ret.m_Kind = m_Kind;
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL: m_Kind = VALUE_REAL;
		case VALUE_REAL: ret.innerVal.m_Real = -innerVal.m_Real; break;
		case VALUE_INT32: ret.innerVal.m_i32 = -innerVal.m_i32; break;
		case VALUE_INT64: ret.innerVal.m_i64 = -innerVal.m_i64; break;
		default: YYError("Invalid type for negate operation\n"); break;
		} 
		return ret;
	} 

	RValue* RValue::operator=(RValue* _v) {
		if (&_v != this) {
			DValue temp;
			memcpy(&temp, &_v, sizeof(RValue));
			__localFree();
			__localCopy(*(RValue*)&temp);
		} 
		return *this;
	} 

	RValue* RValue::operator=(const RValue* _pV) {
		if (_pV != this) {
			DValue temp;
			memcpy(&temp, _pV, sizeof(RValue));
			__localFree();
			__localCopy(*(RValue*)&temp);
		} 
		return *this;
	} 

	RValue* RValue::operator=(double _v) {
		__localFree();
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _v;
		return *this;
	} 

	RValue* RValue::operator=(float _v) {
		__localFree();
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _v;
		return *this;
	} 

	RValue* RValue::operator=(int _v) {
		__localFree();
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _v;
		return *this;
	} 

	RValue* RValue::operator=(long _v) {
		__localFree();
		m_Kind = VALUE_REAL;
		innerVal.m_Real = _v;
		return *this;
	} 

	RValue* RValue::operator=(long long _v) {
		__localFree();
		m_Kind = VALUE_INT64;
		innerVal.m_i64 = _v;
		return *this;
	} 

	RValue* RValue::operator=(const char* _pStr) {
		__localFree();
		YYCreateString(this, _pStr);
		return *this;
	} 

	RValue* RValue::operator=(bool _v) {
		__localFree();
		m_Kind = VALUE_BOOL;
		innerVal.m_Real = _v ? 1.0 : 0.0;
		return *this;
	} 

	RValue* RValue::operator=(YYObjectBase* _p) {
		__localFree();
		m_Kind = VALUE_OBJECT;
		innerVal._objects.m_Object = _p;
		YYObjectBase* pContainer = GetContextStackTop();
		DeterminePotentialRoot(pContainer, _p);
		return *this;
	} 

	RValue::operator bool() const {
		return BOOL_RValue(this);
	} 

	RValue::operator char*() const {
		return ((m_Kind & MASK_KIND_RVALUE) == VALUE_STRING) ? ((pRefString != NULL) ? (char*)pRefString->get() : NULL) : NULL;
	} 

	RValue::operator const char*() const {
		return ((m_Kind & MASK_KIND_RVALUE) == VALUE_STRING) ? ((pRefString != NULL) ? pRefString->get() : NULL) : NULL;
	} 

	RValue::operator double() const {
		return REAL_RValue(this);
	} 

	RValue::operator float() const {
		return (float)REAL_RValue(this);
	} 

	RValue::operator int() const {
		return INT32_RValue(this);
	} 

	RValue::operator unsigned int() const {
		return (unsigned int)INT32_RValue(this);
	} 

	RValue::operator long() const {
		return INT32_RValue(this);
	} 

	RValue::operator unsigned long() const {
		return (unsigned long)INT32_RValue(this);
	} 

	RValue::operator long long() const {
		return INT64_RValue(this);
	} 

	operator unsigned long long() const {
		return (unsigned long long)INT64_RValue(this);
	} 

	RValue* RValue::operator++() {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;

		case VALUE_REAL:
			++innerVal.m_Real;
			break;
		case VALUE_INT32:
			++innerVal.m_i32;
			break;
		case VALUE_INT64:
			++innerVal.m_i64;
			break;
		case VALUE_STRING:
		{
			double v = asReal();
			innerVal.m_Real = ++v;
			m_Kind = VALUE_REAL;
		}
		break;
		default:
			YYError("%lld ++ %lld", this->innerVal.m_i64, this->innerVal.m_i64);
			break;
		} 
		return *this;
	} 
	RValue RValue::operator++(int) {
		RValue tmp(*this);
		this->operator++();
		return tmp;
	} 

	RValue* RValue::operator--() {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
			--innerVal.m_Real;
			break;
		case VALUE_INT32:
			--innerVal.m_i32;
			break;
		case VALUE_INT64:
			--innerVal.m_i64;
			break;
		case VALUE_STRING:
		{
			double v = asReal();
			innerVal.m_Real = --v;
			m_Kind = VALUE_REAL;
		}
		break;
		default:
			YYError("%lld -- %lld", this->innerVal.m_i64, this->innerVal.m_i64);
			break;
		} 
		return *this;
	} 
	RValue RValue::operator--(int) {
		RValue tmp(*this);
		RValue::operator--();
		return tmp;
	} 

	RValue* RValue::operator+=(RValue* rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
			innerVal.m_Real += REAL_RValue(&rhs);
			break;
		case VALUE_INT32:
			if ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_REAL ) {
				innerVal.m_Real = (double)innerVal.m_i32 + rhs.innerVal.m_Real;
				m_Kind = VALUE_REAL;
			}
			else if((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_INT64)
			{
				innerVal.m_i64 = innerVal.m_i32 + rhs.innerVal.m_i64;
				m_Kind = VALUE_INT64;
			}
			else {
				innerVal.m_i32 += INT32_RValue(&rhs);
			} 
			break;
		case VALUE_INT64:
			if ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_REAL) {
				innerVal.m_Real = (double)innerVal.m_i64 + rhs.innerVal.m_Real;
				m_Kind = VALUE_REAL;
			}
			else {
				innerVal.m_i64 += INT64_RValue(&rhs);
			} 
			break;
		case VALUE_STRING:
		{
			if ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_STRING) {
				YYGML_AddString(*this, rhs);
			} 
			else {
				YYError("unable to add a number to string");
			} 
			break;
		} 
		default:
			YYError("%lld += %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
			break;
		} 
		return *this;
	} 
	RValue* RValue::operator+=(const double rhs) {
		switch (m_Kind) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
			innerVal.m_Real += rhs;
			break;
		case VALUE_INT32:
			innerVal.m_Real = (double)innerVal.m_i32 + rhs;
			m_Kind = VALUE_REAL;
			break;
		case VALUE_INT64:
			innerVal.m_Real = (double)innerVal.m_i64 + rhs;
			m_Kind = VALUE_REAL;
			break;
		case VALUE_STRING:

			YYError("unable to add a number to string");
			break;
		default:
		{
			RValue yyrhs(rhs);
			YYError("%lld += %lld", this->innerVal.m_i64, yyrhs.innerVal.m_i64);
		} 
		break;
		} 
		return *this;
	} 
	RValue* RValue::operator+=(const int rhs) {
		switch (m_Kind) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:

			innerVal.m_Real += rhs;
			break;
		case VALUE_INT32:
			innerVal.m_i32 += rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 += rhs;
			break;
		case VALUE_STRING:

			YYError("unable to add a number to string");
			break;
		default:
		{
			RValue yyrhs(rhs);
			YYError("%lld += %lld", this->innerVal.m_i64, yyrhs.innerVal.m_i64);
		} 
		break;
		} 
		return *this;
	} 
	RValue* RValue::operator+=(const long long rhs) {
		switch (m_Kind) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:

			innerVal.m_Real += rhs;
			break;
		case VALUE_INT32:
			innerVal.m_i32 += (int32)rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 += rhs;
			break;
		case VALUE_STRING:

			YYError("unable to add a number to string");
			break;
		default:
		{
			RValue yyrhs(rhs);
			YYError("%lld += %lld", this->innerVal.m_i64, yyrhs.innerVal.m_i64);
		} 
		break;
		} 
		return *this;
	} 
	RValue* RValue::operator+=(const long rhs) {
		switch (m_Kind) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:

			innerVal.m_Real += rhs;
			break;
		case VALUE_INT32:
			innerVal.m_i32 += (int32)rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 += (long)rhs;
			break;
		case VALUE_STRING:

			YYError("unable to add a number to string");
			break;
		default:
		{
			RValue yyrhs(rhs);
			YYError("%lld += %lld", this->innerVal.m_i64, yyrhs.innerVal.m_i64);
		} 
		break;
		} 
		return *this;
	} 
	RValue* RValue::operator+=(const char* rhs) {
		switch (m_Kind) {
		case VALUE_BOOL:
		case VALUE_INT32:
		case VALUE_INT64:
		case VALUE_REAL:
			*this += RValue(rhs);
			break;
		case VALUE_STRING:
		{

			const char* pFirst = (pRefString != NULL) ? pRefString->get() : NULL;
			char* pNew = (char*)YYGML_AddString(pFirst, rhs);
			YYCreateString(this, pNew);
			YYFree(pNew);
		} 
		break;
		default:
		{
			RValue yyrhs(rhs);
			YYError("%lld += %lld", this->innerVal.m_i64, yyrhs.innerVal.m_i64);
		} 
		break;
		} 
		return *this;
	} 
	friend RValue RValue::operator+(RValue* lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(RValue* lhs, const char* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(RValue* lhs, double rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(double lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(RValue* lhs, int rhs) {
		RValue tmp(lhs);
		tmp += RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator+(int lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(RValue* lhs, long long rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(long long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(RValue* lhs, float rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(float lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(RValue* lhs, long rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 
	friend RValue RValue::operator+(const char* lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp += rhs;
		return tmp;
	} 

	RValue* RValue::operator-=(RValue* rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
			innerVal.m_Real -= REAL_RValue(&rhs);
			break;
		case VALUE_INT32:
			if ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_REAL) {
				innerVal.m_Real = (double)innerVal.m_i32 - rhs.innerVal.m_Real;
				m_Kind = VALUE_REAL;
			}
			else if ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_INT64)
			{
				innerVal.m_i64 = innerVal.m_i32 - rhs.innerVal.m_i64;
				m_Kind = VALUE_INT64;
			}
			else {
				innerVal.m_i32 -= INT32_RValue(&rhs);
			} 
			break;
		case VALUE_INT64:
			if ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_REAL) {
				innerVal.m_Real = (double)innerVal.m_i64 - rhs.innerVal.m_Real;
				m_Kind = VALUE_REAL;
			}
			else {
				innerVal.m_i64 -= INT64_RValue(&rhs);
			} 
			break;
		case VALUE_STRING:
		{
			double db = REAL_RValue(this);
			m_Kind = VALUE_REAL;
			innerVal.m_Real = (db - REAL_RValue(&rhs));
		} 
		break;
		default:
			YYError("%lld -= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
			break;
		}
		return *this;
	} 
	friend RValue RValue::operator-(RValue* lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(RValue* lhs, double rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(double lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(RValue* lhs, int rhs) {
		RValue tmp(lhs);
		tmp -= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator-(int lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(RValue* lhs, long rhs) {
		RValue tmp(lhs);
		tmp -= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator-(long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(RValue* lhs, long long rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(long long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(RValue* lhs, float rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 
	friend RValue RValue::operator-(float lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp -= rhs;
		return tmp;
	} 

	RValue* RValue::operator/=(RValue* rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
			switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_PTR:
				YYError("%lld /= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
				break;
			default:
				innerVal.m_Real /= REAL_RValue(&rhs);
				break;
			} 
			break;
		case VALUE_INT32:
			switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				if (rhs.innerVal.m_i32 == 0) YYError("divide by zero");
				innerVal.m_i32 /= rhs.innerVal.m_i32;
				break;
			case VALUE_INT64:
				m_Kind = VALUE_INT64;
				if (rhs.innerVal.m_i64 == 0) YYError("divide by zero");
				innerVal.m_i64 /= rhs.innerVal.m_i64;
				break;
			default:
				m_Kind = VALUE_REAL;
				innerVal.m_Real = ((double)innerVal.m_i32 / REAL_RValue(&rhs));
				break;
			} 
			break;
		case VALUE_INT64:
			switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				if (rhs.innerVal.m_i32 == 0) YYError("divide by zero");
				innerVal.m_i64 /= rhs.innerVal.m_i32;
				break;
			case VALUE_INT64:
				if (rhs.innerVal.m_i64 == 0) YYError("divide by zero");
				innerVal.m_i64 /= rhs.innerVal.m_i64;
				break;
			case VALUE_PTR:
				YYError("%lld /= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
				break;
			default:
				m_Kind = VALUE_REAL;
				innerVal.m_Real = ((double)innerVal.m_i64 / REAL_RValue(&rhs));
				break;
			} 
			break;
		case VALUE_STRING:
		{
			double db = REAL_RValue(this);
			m_Kind = VALUE_REAL;
			innerVal.m_Real = (db / REAL_RValue(&rhs));
		} 
		break;
		default:
			YYError("%lld /= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
			break;
		}
		return *this;

	}
	friend RValue RValue::operator/(RValue* lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(RValue* lhs, double rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(double lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(RValue* lhs, int rhs) {
		RValue tmp(lhs);
		tmp /= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator/(int lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(RValue* lhs, long rhs) {
		RValue tmp(lhs);
		tmp /= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator/(long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(RValue* lhs, long long rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(long long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(RValue* lhs, float rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 
	friend RValue RValue::operator/(float lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp /= rhs;
		return tmp;
	} 

	RValue* RValue::operator*=(RValue* rhs) {
		if (is_number() && ((rhs.m_Kind & MASK_KIND_RVALUE) == VALUE_STRING)) {
			YYDuplicateMultiply(this, rhs);
		} 
		else {
			switch ((m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_BOOL:
				m_Kind = VALUE_REAL;
			case VALUE_REAL:
				innerVal.m_Real *= REAL_RValue(&rhs);
				break;
			case VALUE_INT32:
				switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
				case VALUE_INT32:
					innerVal.m_i32 *= rhs.innerVal.m_i32;
					break;
				case VALUE_INT64:
					m_Kind = VALUE_INT64;
					innerVal.m_i64 *= rhs.innerVal.m_i64;
					break;
				default:
					m_Kind = VALUE_REAL;
					innerVal.m_Real = ((double)innerVal.m_i32 * REAL_RValue(&rhs));
					break;
				} 
				break;
			case VALUE_INT64:
				switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
				case VALUE_INT32:
					innerVal.m_i64 *= rhs.innerVal.m_i32;
					break;
				case VALUE_INT64:
					innerVal.m_i64 *= rhs.innerVal.m_i64;
					break;

				default:
					m_Kind = VALUE_REAL;
					innerVal.m_Real = ((double)innerVal.m_i64 * REAL_RValue(&rhs));
					break;
				} 
				break;
			default:
				YYError("%lld *= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
				break;
			}
		} 
		return *this;
	}
	friend RValue RValue::operator*(RValue* lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(RValue* lhs, double rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(double lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(RValue* lhs, int rhs) {
		RValue tmp(lhs);
		tmp *= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator*(int lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(RValue* lhs, long rhs) {
		RValue tmp(lhs);
		tmp *= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator*(long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(RValue* lhs, long long rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(long long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(RValue* lhs, float rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 
	friend RValue RValue::operator*(float lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp *= rhs;
		return tmp;
	} 

	RValue* RValue::operator%=(RValue* rhs) {
		switch ((m_Kind&MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
			switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_PTR:
				YYError("%lld %= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
				break;
			default:
			{
				double db = REAL_RValue(&rhs);
				if (db == 0) {
					YYError("unable to mod by 0");
				} 
				innerVal.m_Real = fmod(innerVal.m_Real, db);
			} 
			break;
			} 
			break;
		case VALUE_INT32:
			switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				innerVal.m_i32 %= rhs.innerVal.m_i32;
				break;
			case VALUE_INT64:
				m_Kind = VALUE_INT64;
				innerVal.m_i64 %= rhs.innerVal.m_i64;
				break;
			case VALUE_PTR:
				YYError("%lld %= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
				break;
			default:
				m_Kind = VALUE_REAL;
				{
					double db = REAL_RValue(&rhs);
					if (db == 0) {
						YYError("unable to mod by 0");
					} 
					innerVal.m_Real = fmod((double)innerVal.m_i32, db);
				} 
				break;
			} 
			break;
		case VALUE_INT64:
			switch ((rhs.m_Kind & MASK_KIND_RVALUE)) {
			case VALUE_INT32:
				innerVal.m_i64 %= rhs.innerVal.m_i32;
				break;
			case VALUE_INT64:
				innerVal.m_i64 %= rhs.innerVal.m_i64;
				break;
			case VALUE_PTR:
				YYError("%lld %= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
				break;
			default:
				m_Kind = VALUE_REAL;
				{
					double db = REAL_RValue(&rhs);
					if (db == 0) {
						YYError("unable to mod by 0");
					} 
					innerVal.m_Real = fmod((double)innerVal.m_i64, db);
				} 
				break;
			} 
			break;
		case VALUE_STRING:
		{
			double db = REAL_RValue(&rhs);
			if (db == 0) {
				YYError("unable to mod by 0");
			} 
			double dbV = REAL_RValue(this);
			innerVal.m_Real = fmod(dbV, db);
			m_Kind = VALUE_REAL;
		} 
		break;
		default:
			YYError("%lld %= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
			break;
		} 
		return *this;
	}
	friend RValue RValue::operator%(RValue* lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(RValue* lhs, double rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(double lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(RValue* lhs, int rhs) {
		RValue tmp(lhs);
		tmp %= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator%(int lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(RValue* lhs, long rhs) {
		RValue tmp(lhs);
		tmp %= RValue(rhs, true, true);
		return tmp;
	} 
	friend RValue RValue::operator%(long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(RValue* lhs, long long rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(long long lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(RValue* lhs, float rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 
	friend RValue RValue::operator%(float lhs, RValue* rhs) {
		RValue tmp(lhs);
		tmp %= rhs;
		return tmp;
	} 

	RValue* RValue::operator[](const int _index) const
	{
		RValue* pV = NULL;
		if (((m_Kind & MASK_KIND_RVALUE) == VALUE_ARRAY) && (pRefArray != NULL)) {

			if ((_index >= 0) && (_index < pRefArray->len)) {
				pV = (RValue*)&pRefArray->pArray[_index];
			} 
			else {
				YYError("index out of bounds request %d maximum size is %d", _index, pRefArray->len);
			} 
		}

		else {
			YYError("trying to index variable that is not an array");
			pV = (RValue*)this;
		} 
		return *pV;
	}

	RValue* RValue::operator()(const int _index, bool _foo) const
	{
		RValue* ret = (*this)[_index];
		PushContextStack(pRefArray);
		return ret;
	}

	RValue* RValue::operator()(const int _index)
	{
		RValue* ret = *(RValue*)ARRAY_LVAL_RValue(this, _index);
		PushContextStack(pRefArray);
		return ret;
	}

	bool is_number() const
	{
		return (m_Kind == VALUE_REAL || m_Kind == VALUE_INT32 || m_Kind == VALUE_INT64 || m_Kind == VALUE_BOOL);
	}

	friend bool RValue::operator==(RValue* _v1, RValue* _v2) {
		return (YYCompareVal(_v1, _v2, g_GMLMathEpsilon, false) == 0);
	}
	friend bool RValue::operator==(RValue* _v1, double _v2) {
		return (_v1 == RValue(_v2));
	}
	friend bool RValue::operator==(double _v1, RValue* _v2) {
		return (RValue(_v1) == _v2);
	}
	friend bool RValue::operator==(RValue* _v1, float _v2) {
		return (_v1 == RValue(_v2));
	}
	friend bool RValue::operator==(float _v1, RValue* _v2) {
		return (RValue(_v1) == _v2);
	}
	friend bool RValue::operator==(RValue* _v1, int _v2) {
		return (_v1 == RValue(_v2));
	}
	friend bool RValue::operator==(int _v1, RValue* _v2) {
		return (RValue(_v1) == _v2);
	}
	friend bool RValue::operator==(RValue* _v1, long _v2) {
		return (_v1 == RValue(_v2));
	}
	friend bool RValue::operator==(long _v1, RValue* _v2) {
		return (RValue(_v1) == _v2);
	}
	friend bool RValue::operator==(RValue* _v1, long long _v2) {
		return (_v1 == RValue(_v2));
	}
	friend bool RValue::operator==(long long _v1, RValue* _v2) {
		return (RValue(_v1) == _v2);
	}
	friend bool RValue::operator==(bool _v1, RValue* _v2) {
		return (_v1 == (bool)_v2);
	}
	friend bool RValue::operator==(RValue* _v1, bool _v2) {
		return ((bool)_v1 == _v2);
	}
	friend bool RValue::operator==(RValue* _v1, YYObjectBase* _v2) {
		return (_v1 == RValue(_v2));
	}

	friend bool RValue::operator!=(RValue* _v1, RValue* _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(RValue* _v1, double _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(RValue* _v1, float _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(double _v1, RValue* _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(float _v1, RValue* _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(RValue* _v1, int _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(int _v1, RValue* _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(RValue* _v1, long _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(long _v1, RValue* _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(RValue* _v1, long long _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(long long _v1, RValue* _v2) {
		return !(_v1 == _v2);
	}
	friend bool RValue::operator!=(bool _v1, RValue* _v2) {
		return !(_v1 == (bool)_v2);
	}
	friend bool RValue::operator!=(RValue* _v1, bool _v2) {
		return !((bool)_v1 == _v2);
	}
	friend bool RValue::operator!=(RValue* _v1, YYObjectBase* _v2) {
		return !(_v1 == _v2);
	}

	friend bool RValue::operator<(RValue* _v1, RValue* _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans < 0);
	} 
	friend bool RValue::operator<(RValue* _v1, double _v2) {
		return (_v1 < RValue(_v2));
	} 
	friend bool RValue::operator<(RValue* _v1, float _v2) {
		return (_v1 < RValue(_v2));
	} 
	friend bool RValue::operator<(double _v1, RValue* _v2) {
		return (RValue(_v1) < _v2);
	} 
	friend bool RValue::operator<(float _v1, RValue* _v2) {
		return (RValue(_v1) < _v2);
	} 
	friend bool RValue::operator<(RValue* _v1, int _v2) {
		return (_v1 < RValue(_v2));
	} 
	friend bool RValue::operator<(int _v1, RValue* _v2) {
		return (RValue(_v1) < _v2);
	} 
	friend bool RValue::operator<(RValue* _v1, long _v2) {
		return (_v1 < RValue(_v2));
	} 
	friend bool RValue::operator<(long _v1, RValue* _v2) {
		return (RValue(_v1) < _v2);
	} 
	friend bool RValue::operator<(RValue* _v1, long long _v2) {
		return (_v1 < RValue(_v2));
	} 
	friend bool RValue::operator<(long long _v1, RValue* _v2) {
		return (RValue(_v1) < _v2);
	} 
	friend bool RValue::operator<(RValue* _v1, bool _v2) {
		return (_v1 < RValue(_v2));
	} 

	friend bool RValue::operator<=(RValue* _v1, RValue* _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans <= 0);
	} 
	friend bool RValue::operator<=(RValue* _v1, double _v2) {
		return (_v1 <= RValue(_v2));
	} 
	friend bool RValue::operator<=(RValue* _v1, float _v2) {
		return (_v1 <= RValue(_v2));
	} 
	friend bool RValue::operator<=(double _v1, RValue* _v2) {
		return (RValue(_v1) <= _v2);
	} 
	friend bool RValue::operator<=(float _v1, RValue* _v2) {
		return (RValue(_v1) <= _v2);
	} 
	friend bool RValue::operator<=(RValue* _v1, int _v2) {
		return (_v1 <= RValue(_v2));
	} 
	friend bool RValue::operator<=(int _v1, RValue* _v2) {
		return (RValue(_v1) <= _v2);
	} 
	friend bool RValue::operator<=(RValue* _v1, long _v2) {
		return (_v1 <= RValue(_v2));
	} 
	friend bool RValue::operator<=(long _v1, RValue* _v2) {
		return (RValue(_v1) <= _v2);
	} 
	friend bool RValue::operator<=(RValue* _v1, long long _v2) {
		return (_v1 <= RValue(_v2));
	} 
	friend bool RValue::operator<=(long long _v1, RValue* _v2) {
		return (RValue(_v1) <= _v2);
	} 
	friend bool RValue::operator<=(RValue* _v1, bool _v2) {
		return (_v1 <= RValue(_v2));
	} 

	friend bool RValue::operator>(RValue* _v1, RValue* _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans > 0);
	} 
	friend bool RValue::operator>(RValue* _v1, double _v2) {
		return (_v1 > RValue(_v2));
	} 
	friend bool RValue::operator>(RValue* _v1, float _v2) {
		return (_v1 > RValue(_v2));
	} 
	friend bool RValue::operator>(double _v1, RValue* _v2) {
		return (RValue(_v1) > _v2);
	} 
	friend bool RValue::operator>(float _v1, RValue* _v2) {
		return (RValue(_v1) > _v2);
	} 
	friend bool RValue::operator>(RValue* _v1, int _v2) {
		return (_v1 > RValue(_v2));
	} 
	friend bool RValue::operator>(int _v1, RValue* _v2) {
		return (RValue(_v1) > _v2);
	} 
	friend bool RValue::operator>(RValue* _v1, long _v2) {
		return (_v1 > RValue(_v2));
	} 
	friend bool RValue::operator>(long _v1, RValue* _v2) {
		return (RValue(_v1) > _v2);
	} 
	friend bool RValue::operator>(RValue* _v1, long long _v2) {
		return (_v1 > RValue(_v2));
	} 
	friend bool RValue::operator>(long long _v1, RValue* _v2) {
		return (RValue(_v1) > _v2);
	} 
	friend bool RValue::operator>(RValue* _v1, bool _v2) {
		return (_v1 > RValue(_v2));
	} 

	friend bool RValue::operator>=(RValue* _v1, RValue* _v2) {
		int ans = YYCompareVal(_v1, _v2, g_GMLMathEpsilon, true);
		return (ans == -2) ? false : (ans >= 0);
	} 
	friend bool RValue::operator>=(RValue* _v1, double _v2) {
		return (_v1 >= RValue(_v2));
	} 
	friend bool RValue::operator>=(RValue* _v1, float _v2) {
		return (_v1 >= RValue(_v2));
	} 
	friend bool RValue::operator>=(double _v1, RValue* _v2) {
		return (RValue(_v1) >= _v2);
	} 
	friend bool RValue::operator>=(float _v1, RValue* _v2) {
		return (RValue(_v1) >= _v2);
	} 
	friend bool RValue::operator>=(RValue* _v1, int _v2) {
		return (_v1 >= RValue(_v2));
	} 
	friend bool RValue::operator>=(int _v1, RValue* _v2) {
		return (RValue(_v1) >= _v2);
	} 
	friend bool RValue::operator>=(RValue* _v1, long _v2) {
		return (_v1 >= RValue(_v2));
	} 
	friend bool RValue::operator>=(long _v1, RValue* _v2) {
		return (RValue(_v1) >= _v2);
	} 
	friend bool RValue::operator>=(RValue* _v1, long long _v2) {
		return (_v1 >= RValue(_v2));
	} 
	friend bool RValue::operator>=(long long _v1, RValue* _v2) {
		return (RValue(_v1) >= _v2);
	} 
	friend bool RValue::operator>=(RValue* _v1, bool _v2) {
		return (_v1 >= RValue(_v2));
	} 

	RValue* RValue::operator|=(RValue* _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v |= INT64_RValue(&_rhs);
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 |= INT32_RValue(&_rhs);
			break;
		case VALUE_INT64:
			innerVal.m_i64 |= INT64_RValue(&_rhs);
			break;
		default:
			YYError("%lld |= %lld", this->innerVal.m_i64, _rhs.innerVal.m_i64);
			break;
		}
		return *this;
	}
	RValue* RValue::operator|=(double _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v |= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 |= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld |= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator|=(float _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v |= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 |= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld |= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator|=(long long _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v |= _rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 |= _rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld |= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator|=(int _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v |= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 |= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld |= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator|=(long _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v |= (long long)_rhs;
			innerVal.m_Real = (double)v;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 |= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 |= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld |= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}

	RValue* RValue::operator&=(RValue* _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v &= INT64_RValue(&_rhs);
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 &= INT32_RValue(&_rhs);
			break;
		case VALUE_INT64:
			innerVal.m_i64 &= INT64_RValue(&_rhs);
			break;
		default:
			YYError("%lld &= %lld", this->innerVal.m_i64, _rhs.innerVal.m_i64);
			break;
		}
		return *this;
	}
	RValue* RValue::operator&=(double _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v &= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 &= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld &= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator&=(float _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v &= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 &= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld &= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator&=(long long _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v &= _rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 &= _rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld &= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator&=(int _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v &= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 &= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld &= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator&=(long _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v &= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 &= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 &= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld &= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}

	RValue* RValue::operator^=(RValue* _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v ^= INT64_RValue(&_rhs);
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 ^= INT32_RValue(&_rhs);
			break;
		case VALUE_INT64:
			innerVal.m_i64 ^= INT64_RValue(&_rhs);
			break;
		default:
			YYError("%lld ^= %lld", this->innerVal.m_i64, _rhs.innerVal.m_i64);
			break;
		}
		return *this;
	}
	RValue* RValue::operator^=(double _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v ^= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 ^= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld ^= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator^=(float _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v ^= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 ^= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld ^= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator^=(long long _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v ^= _rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 ^= _rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld ^= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator^=(int _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v ^= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 ^= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld ^= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}
	RValue* RValue::operator^=(long _rhs) {
		switch ((m_Kind & MASK_KIND_RVALUE)) {
		case VALUE_BOOL:
			m_Kind = VALUE_REAL;
		case VALUE_REAL:
		{
			long long v = (long long)innerVal.m_Real;
			v ^= (long long)_rhs;
			innerVal.m_i64 = v;
			m_Kind = VALUE_INT64;
		} 
		break;
		case VALUE_INT32:
			innerVal.m_i32 ^= (int)_rhs;
			break;
		case VALUE_INT64:
			innerVal.m_i64 ^= (long long)_rhs;
			break;
		default:
		{
			RValue rhs(_rhs);
			YYError("%lld ^= %lld", this->innerVal.m_i64, rhs.innerVal.m_i64);
		} 
		break;
		}
		return *this;
	}