#pragma once

#pragma warning( disable : 4722 )

#ifdef LIBZHL_EXPORTS
#define LIBZHL_API __declspec(dllexport)
#else
#define LIBZHL_API __declspec(dllimport)
#endif
#define LIBZHL_INTERFACE __declspec(novtable)
__declspec(noreturn) inline void __cdecl __NOP() {}
#define LIBZHL_PLACEHOLDER {__NOP();}


#define _WINSOCKAPI_
#include <Windows.h>
#include "zhl.h"
#include <d3d11.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <mutex>
#include <string>
#include <map>
#include <cstdarg>
#include <unordered_map>
#include <functional>
#include <GameMaker_Defs.h>
#include <random>
#include "Logging.h"
#include "zhl_internal.h"

#ifdef GetObject
#undef GetObject
#endif
#ifdef GetObjectW
#undef GetObjectW
#endif
#ifdef GetObjectA
#undef GetObjectA
#endif
struct SWithIterator;
struct CBackGM;
struct CCode;
struct CEvent;
template <typename TKey, typename TValue, int TInitialSize>
struct CHashMap;
struct CInstance;
struct CInstanceBase;
struct CLayer;
struct CLayerEffectInfo;
struct CLayerElementBase;
struct CLayerInstanceElement;
struct CLayerSpriteElement;
struct CObjectGM;
struct CPhysicsDataGM;
struct CPhysicsObject;
struct CPhysicsWorld;
struct CRoom;
struct CScript;
struct CScriptRef;
struct CSkeletonInstance;
struct CViewGM;
struct CWeakRef;
struct DLL_RFunction;
enum eBuffer_Format : int32_t;
struct GCObjectContainer;
struct IBuffer;
struct RTile;
struct RToken;
struct RValue;
struct RVariableRoutine;
struct YYGMLFuncs;
struct YYObjectBase;
struct YYRECT;
struct YYRoom;
struct YYRoomInstances;
struct YYRoomTiles;
struct YYRunnerInterface;

typedef void (*MFunction)(YYRunnerInterface* Interface, size_t Size);
using namespace Organik;
struct vec3
{
	float x, y, z;
};


struct vec4

{
	float x, y, z, w;
};

struct matrix44
{
	vec4 m[4];
};

using PFN_FindAllocSlot = int(*)(YYObjectBase* Object, const char* Name);

struct RefRValueArray;
        
struct CInstance;

struct RValue;
struct DynamicArrayOfRValue
{
    int m_Length;
    RValue* m_Array;
};
template <typename T>
struct LinkedList
{
	T* m_First;
	T* m_Last;
	int32_t m_Count;
	int32_t m_DeleteType;
};
template <typename T>
struct OLinkedList
{
	T* m_First;
	T* m_Last;
	int32_t m_Count;
};
template <typename T>
struct SLinkedList
{
	typedef struct Node
	{
		Node* m_Next;
		Node* m_Prev;
		T* m_Object;
	} Node;

	Node* m_First;
	Node* m_Last;
	int m_Count;
};
void Error_Show_Action(bool mustCrash, bool manualError, const char* fmt, ...);
struct YYLink
{
	YYLink *p_next;
	YYLink *m_Last;
	PVOID *list;
};
    
using CHashMapHash = uint32_t;

template <typename TKey, typename TValue, int TInitialMask>
struct CHashMap
{
    struct Element
    {
        TValue m_Value;
        TKey m_Key;
        CHashMapHash m_Hash;
    };

private:
	// Typed functions for calculating hashes
	static CHashMapHash CHashMapCalculateHash(
		int Key
	)
	{
		return (Key * -0x61c8864f + 1) & INT_MAX;
	}

	static CHashMapHash CHashMapCalculateHash(
		YYObjectBase* Key
	)
	{
		return ((static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(Key)) >> 6) * 7 + 1) & INT_MAX;
	}

	static CHashMapHash CHashMapCalculateHash(
		void* Key
	)
	{
		return ((static_cast<unsigned long long>(reinterpret_cast<uintptr_t>(Key)) >> 8) + 1) & INT_MAX;
	};

	static CHashMapHash CHashMapCalculateHash(
		const char* Key
	)
	{
		// https://github.com/jwerle/murmurhash.c - Licensed under MIT
		size_t len = strlen(Key);
		uint32_t c1 = 0xcc9e2d51;
		uint32_t c2 = 0x1b873593;
		uint32_t r1 = 15;
		uint32_t r2 = 13;
		uint32_t m = 5;
		uint32_t n = 0xe6546b64;
		uint32_t h = 0;
		uint32_t k = 0;
		uint8_t* d = (uint8_t*)Key; // 32 bit extract from 'key'
		const uint32_t* chunks = NULL;
		const uint8_t* tail = NULL; // tail - last 8 bytes
		int i = 0;
		int l = len / 4; // chunk length

		chunks = (const uint32_t*)(d + l * 4); // body
		tail = (const uint8_t*)(d + l * 4); // last 8 byte chunk of `key'

		// for each 4 byte chunk of `key'
		for (i = -l; i != 0; ++i) {
			// next 4 byte chunk of `key'
			k = chunks[i];

			// encode next 4 byte chunk of `key'
			k *= c1;
			k = (k << r1) | (k >> (32 - r1));
			k *= c2;

			// append to hash
			h ^= k;
			h = (h << r2) | (h >> (32 - r2));
			h = h * m + n;
		}

		k = 0;

		// remainder
		switch (len & 3) { // `len % 4'
		case 3: k ^= (tail[2] << 16);
		case 2: k ^= (tail[1] << 8);

		case 1:
			k ^= tail[0];
			k *= c1;
			k = (k << r1) | (k >> (32 - r1));
			k *= c2;
			h ^= k;
		}

		h ^= len;

		h ^= (h >> 16);
		h *= 0x85ebca6b;
		h ^= (h >> 13);
		h *= 0xc2b2ae35;
		h ^= (h >> 16);

		return h;
	}

public:
	//? Size of the map (all reserved slots)
	int32_t m_CurrentSize;
	//? Number of elements in the map
	int32_t m_UsedCount;
	//? Current mask for hashing
	int32_t m_CurrentMask;
	//? When to grow the map
	int32_t m_GrowThreshold;
	Element* m_Elements;
	bool GetContainer(
		TKey Key,

		Element*& Value
	)
	{
		CHashMapHash value_hash = CHashMapCalculateHash(Key);
		int32_t ideal_position = static_cast<int>(value_hash & m_CurrentMask);
		for (
			// Start at the ideal element (the value is probably not here though)

			Element& current_element = this->m_Elements[ideal_position];
			// Continue looping while the hash isn't 0 (meaning we reached the end of the map)
			current_element.m_Hash != 0;
			// Go to the next position
			current_element = this->m_Elements[(++ideal_position) & this->m_CurrentMask]
			)
		{
			if (current_element.m_Key != Key)
				continue;
			Value = &current_element;
			return true;

		}
		return false;
	}
	int32_t GetDistance(
		CHashMapHash Hash,
		int32_t Position
	)
	{
		return (Position + m_CurrentSize - (Hash & m_CurrentMask)) & m_CurrentMask;
	}

	CHashMap<TKey, TValue, TInitialMask>()
	{
		m_CurrentSize = (TInitialMask << 1) & (~TInitialMask);
		m_UsedCount = 0;
		m_CurrentMask = TInitialMask;
		m_GrowThreshold = (m_CurrentSize * 3) / 4;
		m_Elements = reinterpret_cast<Element*>(YYAlloc(sizeof(Element) * m_CurrentSize));
		memset(m_Elements, 0, sizeof(Element) * m_CurrentSize);
	}

	CHashMapHash HashAt(int32_t position)
	{
		if (position < 0 || position >= m_CurrentSize)
			return 0;
		return m_Elements[position].m_Hash;
	}

	TKey& KeyAt(int32_t position)
	{
		return m_Elements[position].m_Key;
	}

	TValue& ValueAt(int32_t position)
	{
		return m_Elements[position].m_Value;
	}

	Element& SetHashAt(int32_t position, CHashMapHash hash)
	{
		m_Elements[position].m_Hash = hash;
		return m_Elements[position];
	}

	Element& SetKeyAt(int32_t position, TKey key)
	{
		m_Elements[position].m_Key = key;
		return m_Elements[position];
	}

	Element& SetValueAt(int32_t position, TValue value)
	{
		m_Elements[position].m_Value = value;
		return m_Elements[position];
	}
	
	void InsertOrAssign (
		CHashMapHash Hash,
		TValue Value,
		bool force = false
	)
	{
		if (m_UsedCount > m_GrowThreshold)
		{
			Error_Show_Action(false, true, "CHashMap::Insert: Map needs to be resized, but it is not implemented yet.");
			return;
		}
		int32_t _position = static_cast<int32_t>(Hash & m_CurrentMask);
		int32_t _startingPosition = _position;
		ForEach([&](const TKey& key, const CHashMapHash& hash, TValue& elem) -> void
		{
			if (hash == Hash)
			{
				elem = Value;
				return;
			}
		});
		while (true){
			if (m_Elements[_position].m_Hash == 0)
			{
				m_Elements[_position].m_Hash = Hash;
				m_Elements[_position].m_Value = Value;
				m_UsedCount++;
				return;
			}
			else if (m_Elements[_position].m_Hash == Hash)
			{
				m_Elements[_position].m_Value = Value;
				return;
			}
			else
			{
				_position = (1 + _position) & m_CurrentMask;	
				if (_position == _startingPosition)
				{
					Error_Show_Action(false, true, "CHashMap::Insert: Map is full, but it is not implemented yet.");
					return;
				}
			}
		}
		Error_Show_Action(false, true, "CHashMap::Insert: Collision detected, but it is not implemented yet.");
		return;
	}
	struct iterator {
		int32_t index;
		CHashMap<TKey, TValue, TInitialMask>* m_Map;
		int m_Count;
		iterator(Element* p, CHashMap<TKey, TValue, TInitialMask>* map) : index(map->m_CurrentSize), m_Map(map), m_Count(m_Map->m_UsedCount) {}
		iterator* operator++() {
			// Move to the next element
			if (index > m_Map->m_CurrentSize) {
				return nullptr;
			}
			Element& elem = m_Map->m_Elements[++index];
			if (elem.m_Hash && !(elem.m_Hash & HASH_DELETED)) {
				// Valid element
				++m_Count;
				return this;
			}
			return nullptr;
		}

		Element* getNextElement()
		{
			if (!m_Map || !m_Map->m_Elements)
				return nullptr;
			
			while (++index <= m_Map->m_CurrentSize)
			{
				Element* elemPtr = getRef();
				if (elemPtr && elemPtr->m_Hash && elemPtr->m_Value && !(elemPtr->m_Hash & HASH_DELETED))
				{
					++m_Count;
					return elemPtr;
				}
			}
			
			return nullptr;
		}

		
		bool operator!=(const iterator& other) { return (index != other.index); }
		bool operator<=(const iterator& other) { return (index <= other.index); }
		bool operator>=(const iterator& other) { return (index >= other.index); }
		bool operator<(const iterator& other)  { return (index < other.index);  }
		bool operator>(const iterator& other)  { return (index > other.index);  }
		bool operator==(const iterator& other) { return (index == other.index); }
		Element *GetDummyItem() 
		{
			static Element dummyItem{};
			if (!dummyItem.m_Value)
				dummyItem.m_Value = TValue{};
			
			if (!dummyItem.m_Hash)
				dummyItem.m_Hash = HASH_DELETED;

			return &dummyItem;
		}
		Element* getRef() {
			if (!m_Map || !m_Map->m_Elements)
				return nullptr;
			return &(m_Map->m_Elements[index]);
		}
		Element getVal() {
			return m_Map->m_Elements[index];
		}
		Element& operator*() { 
			if (!m_Map || !m_Map->m_Elements)
			{
				GetLogger()->LogFormatted("CHashMap iterator dereference invalid index %d", index);
				return *GetDummyItem();
			}
			return m_Map->m_Elements[index];
		}
		Element* operator->() { 
			if (!m_Map || !m_Map->m_Elements)
			{
				GetLogger()->LogFormatted("CHashMap iterator dereference invalid index %d", index);
				return nullptr;
			}
			return &(m_Map->m_Elements[index]); 
		}

		// Improved next() function
		iterator next() {
			if (!m_Map || !m_Map->m_Elements || index >= m_Map->m_CurrentSize) {
				index = m_Map->m_CurrentSize; // Move to end position
				return *this;
			}
			
			++index;
			
			// Validate index is within range
			if (index < 0 || index >= m_Map->m_CurrentSize) {
				index = m_Map->m_CurrentSize; // Move to end position
				return *this;
			}
			
			return *this;
		}
	};

	bool FindNextValue(iterator& it, const Element** const out)
	{
		while (true)
		{
			const Element* elem = &m_Elements[it.index++];
			if (it.index > m_CurrentSize)
			{
				return false;
			}

			auto hash = elem->m_Hash;
			if (hash && !(hash & HASH_DELETED))
			{
				*out = elem;
				return true;
			}
		}
		return false;
	}
	template<typename Func>
	requires std::is_invocable_r_v<void, Func, const TKey&, const CHashMapHash&, TValue&>
	void ForEach(Func func) {
		iterator it = begin();
		Element* val;
		while (val = it.getNextElement())
		{
			if (!val)continue;
			if (!val->m_Value) continue;
			if (!val->m_Key) continue;
			if (val->m_Hash & HASH_DELETED) continue;
			func(val->m_Key, val->m_Hash, val->m_Value);
		}
	}
	template<typename Func>
	requires std::is_invocable_r_v<bool, Func, const TKey&, const CHashMapHash&, TValue&>
	std::vector<Element*> Where(Func func) {
		iterator it = begin();
		Element* val;
		auto results = std::vector<Element*>();
		while (val = it.getNextElement())
		{
			if (!val)
				continue;
			if (!val->m_Value || !val->m_Hash || (val->m_Hash & HASH_DELETED))
				continue;
			if (func(val->m_Key, val->m_Hash, val->m_Value))
			{
				results.push_back(val);
			}
		}
		return results;
	}
	template<typename Func>
	requires std::is_invocable_r_v<bool, Func, const TKey&, const CHashMapHash&, TValue&>
	Element* FirstOrDefault(Func func, Element* defaultValue = nullptr) {
		iterator it = begin();
		Element* val;
		while (val = it.getNextElement())
		{
			if (!val)
				continue;
			if (!val->m_Value || !val->m_Hash || (val->m_Hash & HASH_DELETED))
				continue;
			if (func(val->m_Key, val->m_Hash, val->m_Value))
			{
				return val;
			}
		}
		return defaultValue;
	}
	template<typename Func>
	requires std::is_invocable_r_v<void, Func, const int32_t&, const TKey&, const CHashMapHash&, TValue&>
	void ForEachWithIndex(Func func) {
		iterator it = begin();
		Element* val;
		while (val = it.getNextElement())
		{
			if (!val)
				continue;
			if (!val->m_Value || !val->m_Hash || (val->m_Hash & HASH_DELETED))
				continue;
			func(it.index, val->m_Key, val->m_Hash, val->m_Value);
		}
	}
	
	typename CHashMap<TKey, TValue, TInitialMask>::iterator begin() {
		iterator it(m_Elements, this);
		it.index = 0;
		return it;
	}


	bool GetValue(
		TKey Key,

		TValue& Value
	)
	{
		// Try to get the container
		Element* object_container = nullptr;
		if (!this->GetContainer(
			Key,
			object_container
		))
		{
			return false;
		}

		Value = object_container->m_Value;
		return true;
	}
};

inline bool *getErrorReportingEnabled()
{
	static bool errorReportingEnabled = false;
	return &errorReportingEnabled;
}

struct IBuffer;
struct RValue;

struct IBuffer
{
	
	LIBZHL_API virtual int Base64decode(const char *param_1, int param_2, int param_3);
	LIBZHL_API virtual void Base64encode(RValue *param_1, int param_2, int param_3);
	LIBZHL_API virtual void CRC32(RValue *param_1, int param_2, int param_3);
	LIBZHL_API virtual uint8_t *Compress(int param_1, int param_2, uint *param_3);
	LIBZHL_API virtual void Copy(int param_1, int param_2, IBuffer *param_3, int param_4);
	LIBZHL_API virtual uint8_t *Decompress(uint8_t *param_1);
	LIBZHL_API virtual void Fill(int param_1, int param_2, eBuffer_Type param_3, RValue *param_4, int param_5, bool param_6);
	LIBZHL_API virtual void GetSurface(int param_1);
	LIBZHL_API virtual int Load(const char *param_1, int param_2, int param_3, int param_4);
	LIBZHL_API virtual void MD5(RValue *param_1, int param_2, int param_3);
	LIBZHL_API virtual void Peek(int param_1, eBuffer_Type param_2, RValue *param_3);
	LIBZHL_API virtual void Poke(int param_1, eBuffer_Type param_2, RValue *param_3);
	LIBZHL_API virtual int Read(eBuffer_Type param_1, RValue *param_2);
	LIBZHL_API virtual void Resize(int param_1);
	LIBZHL_API virtual void SHA1(RValue *param_1, int param_2, int param_3);
	LIBZHL_API virtual int Save(const char *param_1, int param_2, int param_3);
	LIBZHL_API virtual int Seek(eBuffer_Seek param_1, int param_2);
	LIBZHL_API virtual void SetSurface(int param_1, int param_2);
	LIBZHL_API void Write(int param_1);
	LIBZHL_API virtual int Write(eBuffer_Type param_1, RValue *param_2);
	LIBZHL_API virtual int WriteArray(eBuffer_Type param_1, uchar *param_2, int param_3);
	
};

struct Buffer_Standard : IBuffer
{
	
};

struct CBackGM
{
	
};
LIBZHL_API void __stdcall Error_Show_Action(const char *err, bool serious, bool _manualError);
LIBZHL_API void __stdcall FREE_RValue(RValue *_pValue);
LIBZHL_API void __stdcall FREE_RValue__Pre(RValue *param_1);

struct RefString {
    const char* m_String;
    int32_t m_RefCount;
    int32_t m_Length;
};

template <typename T1, typename T2>
requires (sizeof(T1) == 4 && sizeof(T2) == 4)
struct RValuePair {
	T1 first;
	T2 second;

	inline operator std::pair<T1, T2>() const {
		return std::pair<T1, T2>(first, second);
	}
	inline operator std::pair<T1, T2>&() {
		return *(std::pair<T1, T2>*)this;
	}
};

struct Action;
struct CCode;
struct EventMap;
#pragma pack(push, 4)
struct RValue
{
	// ########################################################################
	// the following structure is replicated from the YoYo YYC runtime headers for GMS Runtime 2.3.6 and 2.3.7
	// and I added some stuff
	// ########################################################################

	union {
		int32_t m_i32;
		int64_t m_i64;
		double m_Real;
		union
		{
			RefString* m_RefString;
			RefDynamicArrayOfRValue* m_RefArray; // also actions array
			YYObjectBase* m_Object;
			CInstance* m_Instance;
			Action* m_Action; //? Action : public CCode
			vec3* m_RGB;
			vec4* m_RGBA;
			// EventMap* m_EventMap; 		//? EventMap : public YYObjectBase
			RValuePair<PFN_ACTIONHANDLER, YYObjectBase*> m_ActionPair;
		};
		PVOID m_Pointer = nullptr;
	};
	uint32_t m_Flags = 0;
	RValueType m_Kind = VALUE_UNDEFINED;

	//? More fun stuff

	// ########################################################################
	// End of replicated structure
	// ########################################################################

	// the rest of RValue is heavily based on the work done by Archie, creator of YYToolkit and Aurie

template<class _MemberType, class T = _MemberType RValue::*, class _T2>
auto operator->*(_T2 RValue::*_MemberType::* member) {
		switch (typeid(_MemberType).hash_code()) {
		case typeid(RefString*).hash_code():
			if ((m_Kind & MASK_KIND_RVALUE) != VALUE_STRING) Error_Show_Action(true, true, "RValue::operator->*: RValue is not a string.");
				return std::launder(&this->m_RefString)->*member;

			break;
		case typeid(RefDynamicArrayOfRValue*).hash_code():
			if ((m_Kind & MASK_KIND_RVALUE) != VALUE_ARRAY) 
				Error_Show_Action(true, true, "RValue::operator->*: RValue is not an array.");
			return std::launder(&this->m_RefArray)->*member;

		case typeid(YYObjectBase*).hash_code():
			if ((m_Kind & MASK_KIND_RVALUE) != VALUE_OBJECT) 
				Error_Show_Action(true, true, "RValue::operator->*: RValue is not an object.");
			return std::launder(&this->ToObject())->*member;
			
		case typeid(CInstance*).hash_code():
			return std::launder(&this->ToInstance())->*member;

		case typeid(vec3*).hash_code():
			if ((m_Kind & MASK_KIND_RVALUE) != VALUE_VEC3)
				Error_Show_Action(true, true, "RValue::operator->*: RValue is not a vec3.");
			return std::launder(&this->m_RGB)->*member;

		case typeid(vec4*).hash_code():
			if ((m_Kind & MASK_KIND_RVALUE) != VALUE_VEC4) 
				Error_Show_Action(true, true, "RValue::operator->*: RValue is not a vec4.");
			return (*std::launder(&this->m_RGBA))->*member;
			
		case typeid(RValuePair<PFN_ACTIONHANDLER, YYObjectBase*>).hash_code():
			return std::launder(&this->ToActionPair())->*member;

		case typeid(Action*).hash_code():
			if (m_Kind != VALUE_ACTION) 
				Error_Show_Action(true, true, "RValue::operator->*: RValue is not an action.");
			break;
		default:
			break;
		}
		Error_Show_Action(true, true, "RValue::operator->*: Unknown type or unknown error: %s (%s, %s).", this->GetKindName(), typeid(_MemberType).name(), typeid(T).name());
		static _T2 dummyMember = RValue().m_ActionPair->*dummyMember;
		return dummyMember;
	}

	template<typename T>
	requires(sizeof(T) == 4)
	void SetPairFirst(const T& value)
	{
		*(reinterpret_cast<T*>(&std::launder(&this->m_ActionPair)->first)) = value;
	}
	template<typename T>
	requires(sizeof(T) == 4)
	void SetPairSecond(const T& value)
	{
		*(reinterpret_cast<T*>(&std::launder(&this->m_ActionPair)->second)) = value;
	}
	template<typename T>
	requires(sizeof(T) == 4)
	T GetPairFirst() const
	{
		return *reinterpret_cast<const T*>(std::launder(&this->m_ActionPair.first));
	}
	template<typename T>
	requires(sizeof(T) == 4)
	T GetPairSecond() const
	{
		return *reinterpret_cast<const T*>(std::launder(&this->m_ActionPair.second));
	}
	template<typename T>
	requires(sizeof(T) == 4)
	T GetPairFirst()
	{
		return *reinterpret_cast<T*>(std::launder(&this->m_ActionPair.first));
	}
	template<typename T>
	requires(sizeof(T) == 4)
	T GetPairSecond()
	{
		return *reinterpret_cast<T*>(std::launder(&this->m_ActionPair.second));
	}
	// Converts the RValue to a double.
	LIBZHL_API double ToDouble() const;

	// Converts the RValue to a 32-bit integer.
	LIBZHL_API int32_t ToInt32() const;

	// Converts the RValue to a 64-bit integer.
	LIBZHL_API int64_t ToInt64() const;

	// Converts the RValue to a void*.
	LIBZHL_API void* ToPointer() const;

	// Converts the RValue to a boolean.
	LIBZHL_API bool ToBoolean() const;

	LIBZHL_API int32_t GetArrayLength() const;

	LIBZHL_API int32_t GetArrayLength();
	// Returns the stringified kind of the RValue.
	LIBZHL_API const char* GetKindName() const;

	// Converts the RValue to an object.
	LIBZHL_API YYObjectBase* ToObject() const;

	LIBZHL_API RValuePair<PFN_ACTIONHANDLER,YYObjectBase*> ToActionPair() const;
	LIBZHL_API RValuePair<PFN_ACTIONHANDLER,YYObjectBase*> ToActionPair();
	LIBZHL_API const RValuePair<PFN_ACTIONHANDLER,YYObjectBase*>& ToRefActionPair() const;
	LIBZHL_API RValuePair<PFN_ACTIONHANDLER,YYObjectBase*>& ToRefActionPair();

	// Converts the RValue to an instance.
	LIBZHL_API CInstance* ToInstance() const;

	// Converts the RValue to a C-style string.
	LIBZHL_API const char* ToCString() const;

	// Converts the RValue to a string.
	LIBZHL_API std::string ToString() const;

	// Converts the RValue to a UTF-8 string.
	LIBZHL_API std::u8string ToUTF8String() const;

	//! ARRAYS
	RValue& operator=(const RValue& Other);
	RValue& push_back(const RValue& Other);
	RValue& back();
	LIBZHL_API std::vector<RValue*> ToRefVector();

	std::unordered_map<std::string, RValue*> ToRefMap();

	int32_t GetMemberCount() const;

	LIBZHL_API RValue* ToArray();

	LIBZHL_API uint32_t GetKind() const;

	static LIBZHL_API uint32_t GetKind(const RValue& rv);

	LIBZHL_API RValue();

	LIBZHL_API RValue(const RValue& Other);	

	LIBZHL_API RValue(RValue&& Other) noexcept;

	LIBZHL_API RValue& operator=(RValue&& Other) noexcept;

	LIBZHL_API ~RValue();

	RValue(const vec3& Value)
	{
		FREE_RValue(this);
		*reinterpret_cast<vec3*>(std::launder(&this->m_RGB)) = Value;
		this->m_Kind = VALUE_VEC3;
	}
	
	RValue(const vec4& Value)
	{
		FREE_RValue(this);
		*reinterpret_cast<vec4*>(std::launder(&this->m_RGBA)) = Value;
		this->m_Kind = VALUE_VEC4;
	}

	RValue(vec3&& Value) noexcept
	{
		FREE_RValue(this);
		*reinterpret_cast<vec3*>(std::launder(&this->m_RGB)) = std::move(Value);
		this->m_Kind = VALUE_VEC3;
	}

	RValue(vec4&& Value) noexcept
	{
		FREE_RValue(this);
		*reinterpret_cast<vec4*>(std::launder(&this->m_RGBA)) = std::move(Value);
		this->m_Kind = VALUE_VEC4;
	}
	inline RValue& operator=(const vec3& Value) noexcept
	{
		FREE_RValue(&reinterpret_cast<RValue&>(*this));
		*reinterpret_cast<vec3*>(std::launder(&this->m_RGB)) = Value;
		this->m_Kind = VALUE_VEC3;
		return *this;
	}

	inline RValue& operator=(const vec4& Value) noexcept
	{
		FREE_RValue(this);
		*reinterpret_cast<vec4*>(std::launder(&this->m_RGBA)) = Value;
		this->m_Kind = VALUE_VEC4;
		return *this;
	}

	inline RValue& operator=(vec3&& Value) noexcept
	{
		FREE_RValue(this);
		std::move(&(Value.x), &(Value.z) + sizeof(float*), &reinterpret_cast<float&>((*std::launder(&this->m_RGB))->x));
		this->m_Kind = VALUE_VEC3;
		return *this;
	}

	inline RValue& operator=(vec4&& Value) noexcept
	{
		FREE_RValue(this);
		std::move(&(Value.x), &(Value.w) + sizeof(float*), &reinterpret_cast<float&>((*std::launder(&this->m_RGB))->x));
		this->m_Kind = VALUE_VEC4;
		return *this;
	}

	RValue(const int32_t Value)
	{
		FREE_RValue(this);
		*reinterpret_cast<int32_t*>(std::launder(&this->m_i32)) = static_cast<int32_t>(Value);
		this->m_Kind = VALUE_INT32;
	}
	RValue(const int64_t Value)
	{
		FREE_RValue(this);
		*reinterpret_cast<int64_t*>(std::launder(&this->m_i64)) = static_cast<int64_t>(Value);
		this->m_Kind = VALUE_INT64;
	}
	
	RValue( const double Value)
	{
		FREE_RValue(this);
		*reinterpret_cast<double*>(std::launder(&this->m_Real)) = static_cast<double>(Value);
		this->m_Kind = VALUE_REAL;
	}
	RValue( const float Value)
	{
		FREE_RValue(this);
		*reinterpret_cast<double*>(std::launder(&this->m_Real)) = static_cast<double>(Value);
		this->m_Kind = VALUE_REAL;
	}

	RValue(CObjectGM *Value)
	{
		FREE_RValue(this);
		this->m_Pointer = (PVOID)(Value);
		this->m_Kind = VALUE_OBJECT;
	}
	RValue(YYObjectBase *Value)
	{
		FREE_RValue(this);
		this->m_Pointer = (PVOID)(Value);
		this->m_Kind = VALUE_OBJECT;
	}
	
	RValue(CInstanceBase *Value)
	{
		FREE_RValue(this);
		this->m_Pointer = (PVOID)(Value);
		this->m_Kind = VALUE_OBJECT;
	}

	RValue(CInstance *Value)
	{
		FREE_RValue(this);
		this->m_Pointer = (PVOID)(Value);
		this->m_Kind = VALUE_OBJECT;
	}

	RValue(RValue* Pointer) = delete;

	RValue(void* Pointer) = delete;
	// {
	// 	*this = RValue();
	// 
	// 	*reinterpret_cast<PVOID*>(std::launder(&this->m_Pointer)) = Pointer;
	// 	this->m_Kind = VALUE_PTR;
	// }

	LIBZHL_API RValue(std::string_view Value);

	LIBZHL_API RValue(const char* Value);

	LIBZHL_API RValue(bool Value);

	// RValue(Action* actionPointer)
	// {
	// 	FREE_RValue(this);

	// 	memcpy(this, actionPointer, sizeof(RValue));
	// 	this->m_Kind = static_cast<RValueType>(VALUE_ACTION & MASK_KIND_RVALUE);
	// }
	
	LIBZHL_API RValue(const std::map<std::string, RValue>& Values);
	LIBZHL_API RValue(const std::unordered_map<int32_t, RValue>& Values);
	LIBZHL_API RValue(const std::vector<RValue>& Values);
	inline RValue(const std::pair<PFN_ACTIONHANDLER, YYObjectBase*>& Values)
	{
		FREE_RValue(this);

		(*std::launder(&this->m_ActionPair)).first = Values.first;
		(*std::launder(&this->m_ActionPair)).second = Values.second;
		this->m_Kind = VALUE_ACTION;
	}
	inline RValue(Action* actionPointer)
	{
		FREE_RValue(this);
		
		*std::launder(&this->m_Pointer) = (PVOID)(actionPointer);
		this->m_Kind = VALUE_ACTION;
	}
	LIBZHL_API RValue(CCode* codePointer) : RValue(reinterpret_cast<Action*>(codePointer)) {}
	LIBZHL_API RValue(EventMap* eventMapPointer) : RValue(reinterpret_cast<YYObjectBase*>(eventMapPointer)) {}
	// RValue(Action* actionPointer);

	LIBZHL_API RValue& operator=(const std::map<std::string, RValue>& Values);
	LIBZHL_API RValue& operator=(const std::vector<RValue>& Values);
	LIBZHL_API RValue& operator=(const std::pair<PFN_ACTIONHANDLER, YYObjectBase*>& Values)
	{
		FREE_RValue(this);

		(*std::launder(&this->m_ActionPair)).first = Values.first;
		(*std::launder(&this->m_ActionPair)).second = Values.second;
		this->m_Kind = VALUE_ACTION;
		return *this;
	}

	inline RValue& operator=(Action* actionPointer) {
		FREE_RValue(this);
		
		*std::launder(&this->m_Pointer) = (PVOID)(actionPointer);
		this->m_Kind = VALUE_ACTION;
		return *this;
	};
	inline RValue& operator=(CCode* codePointer) {
		return this->operator=(reinterpret_cast<Action*>(codePointer));
	}
	inline RValue& operator=(EventMap* eventMapPointer) {
		return this->operator=(reinterpret_cast<YYObjectBase*>(eventMapPointer));
	};
	// RValue& operator=(Action* actionPointer);

	LIBZHL_API std::vector<RValue> ToVector();

	LIBZHL_API RValue& operator[](unsigned int Index);
	RValue& operator[](RValue Index);

	LIBZHL_API RValue& operator[](std::string_view MemberName);

	LIBZHL_API const RValue operator[](std::string_view MemberName) const;

	LIBZHL_API bool ContainsValue(std::string_view MemberName) const;
	LIBZHL_API bool ContainsValue(int32_t hash) const;

	LIBZHL_API explicit operator bool();

	LIBZHL_API explicit operator double();

	LIBZHL_API explicit operator std::string();

	
	LIBZHL_API operator vec3&()
	{
		return *reinterpret_cast<vec3*>(std::launder(&this->m_RGB));
	}
	
	LIBZHL_API operator vec4&()
	{
		return *reinterpret_cast<vec4*>(std::launder(&this->m_RGBA));
	}

	LIBZHL_API void operator()(CInstance* self, CInstance* other);

	// operator Action*();

	// operator Action&();

	private:
		void __Free();
	public:
	

	LIBZHL_API RValue *GetVariable(RValue *var);
	
};
static_assert(sizeof(RValue) == 16, "RValue size is not 16 bytes!");
#pragma pack(pop)

struct RToken
{
	
	int m_BeforeLength; //int m_Kind;
    RValuePair<PFN_ACTIONHANDLER, YYObjectBase*>* m_BeforeArray; // uint32_t m_Type;
	int m_Index;
	int m_Offset;
	RValue m_Value;
	int m_AfterLength;
	RValuePair<PFN_ACTIONHANDLER, YYObjectBase*>* m_AfterArray;
	int m_Position;

};

struct YYGMLFuncs;
struct YYObjectBase;

struct CCode
{
	virtual ~CCode() {};
	CCode *m_Next;
	int m_Kind;
	int m_Compiled;
	const char *m_Str;
	RToken m_Token;
	RValue m_Value;
	PVOID m_VmInstance;
	PVOID m_VmDebugInfo;
	const char *m_Code;
	const char *m_Name;
	int m_CodeIndex;
	YYGMLFuncs *m_Functions;
	bool m_Watch;
	int m_Offset;
	int m_LocalsCount;
	int m_ArgsCount;
	int m_Flags;
	YYObjectBase *m_Prototype;
	CCode(bool m_Watch = false, int m_Kind = 1, int m_Compiled = 1, const char* m_Name = "", YYGMLFuncs* m_Functions = nullptr, const char* m_Str = "", int m_CodeIndex = 0,
		int m_LocalsCount = 0, int m_ArgsCount = 0, int m_Flags = 0, YYObjectBase* m_Prototype = nullptr, int m_Offset = 0, RToken m_Token = RToken(), RValue m_Value = RValue(),
		PVOID m_VmInstance = nullptr, PVOID m_VmDebugInfo = nullptr, const char* m_Code = "", CCode* m_Next = nullptr);
};
constexpr size_t CCodeSz = sizeof(CCode);
struct CDS_Grid
{
	
};

struct CDS_Priority
{
	
};

struct CDS_Queue
{
	
};

struct CDS_Stack
{
	
};

struct CEvent;
struct CInstance;
struct YYEvent;

struct CEvent
{
	
	LIBZHL_API void Execute(CInstance *CInst_self, CInstance *CInst_other);
	LIBZHL_API bool LoadFromChunk(YYEvent *param_1, uchar *param_2);
	CCode *m_Code;
	int m_OwnerObjectID;
	CEvent(PFN_ACTIONHANDLER pfn, int32_t m_OwnerObjectID = -1);
	CEvent(Action* pAction, int32_t m_OwnerObjectID = -1);
	static CEvent* InsertIntoMap(
		CHashMap<int64_t, CEvent*, 3>* eventMap, int32_t eventID, int32_t eventSubID,
		Action* pAction, int32_t m_OwnerObjectID, bool replaceIfExists = true
	);
	static CEvent* InsertIntoMap(CHashMap<int64_t, CEvent*, 3>* eventMap, int32_t eventID, int32_t eventSubID,
    PFN_ACTIONHANDLER pfn, YYObjectBase* params, int32_t m_OwnerObjectID, bool replaceIfExists, bool after = false);

	CEvent() = default;
};

struct CEventMap;

struct CEventMap
{
	
	LIBZHL_API void CEventMapInsert(int64_t param_1, CEvent *param_2);
	
};

struct CHashMapCInstElem
{
	
};

struct CHashMapChar;

struct CHashMapChar
{
	
	LIBZHL_API int *FindCheckKey(const char *param_1);
	
};

struct CHashMapRValue;

struct CHashMapRValue
{
	
	LIBZHL_API RValue *Find(int slotID);
	LIBZHL_API RValue **GetNthElement(int n, PVOID *out);
	
};

struct CHashMapSSCache;
struct SScriptCacheInfo;

struct CHashMapSSCache
{
	
	LIBZHL_API void Insert(PVOID param_1, SScriptCacheInfo *param_2);
	
};

struct CHashMapYYObj1;

struct CHashMapYYObj1
{
	
	LIBZHL_API void Insert(YYObjectBase *param_1, YYObjectBase *param_2);
	
};

struct CHashMapYYObj2;

struct CHashMapYYObj2
{
	
	LIBZHL_API void Init();
	
};

struct CInstIter;

struct CInstIter
{
	
	LIBZHL_API CInstance *GetNext_FIRST_entry();
	
};

struct CObjectGM;

struct CPhysicsDataGM
{
	float *m_PhysicsVertices;
	bool m_IsPhysicsObject;
	bool m_IsPhysicsSensor;
	bool m_IsPhysicsAwake;
	bool m_IsPhysicsKinematic;
	int m_PhysicsShape;
	int m_PhysicsGroup;
	float m_PhysicsDensity;
	float m_PhysicsRestitution;
	float m_PhysicsLinearDamping;
	float m_PhysicsAngularDamping;
	float m_PhysicsFriction;
	int m_PhysicsVertexCount;
	
};

struct CObjectGM
{
	
	LIBZHL_API CEvent *GetEventRecursive(int ind, int num);
	const char *m_Name;
	CObjectGM *m_ParentObject;
	CHashMap<int, CObjectGM*, 2> *m_ChildrenMap;
	CHashMap<int64_t, CEvent*, 3> *m_EventsMap;
	CPhysicsDataGM m_PhysicsData;
	SLinkedList<CInstance> m_Instances;
	SLinkedList<CInstance> m_InstancesRecursive;
	uint32_t m_Flags;
	int32_t m_SpriteIndex;
	int32_t m_Depth;
	int32_t m_Parent;
	int32_t m_Mask;
	int32_t m_ID;

	CObjectGM() = default;
};

struct CPhysicsObject;
struct CSkeletonInstance;

struct SLink
{
	PVOID m_First;
	PVOID m_Last;
	PVOID m_list;
	
};

struct CInstanceBase
{
	
	virtual ~CInstanceBase() {};
	RValue* m_YYVars;

};

struct CWeakRef;

struct YYObjectBase : CInstanceBase
{
	
	LIBZHL_API void Free(bool p1);
	LIBZHL_API RValue *InternalGetYYVarRef(int param_1);
	LIBZHL_API RValue *InternalGetYYVarRefL(int param_1);
	LIBZHL_API RValue *InternalReadYYVar(int index);
	LIBZHL_API bool Mark4GC(uint *param_1, int param_2);
	LIBZHL_API bool MarkOnlyChildren4GC(uint *param_1, int param_2);
	LIBZHL_API bool MarkThisOnly4GC(uint *param_1, int param_2);
	LIBZHL_API void PreFree();
	LIBZHL_API void ThreadFree(bool param_1, void *GC_Context);
	LIBZHL_API YYObjectBase* constructor(int n_Vars, int param_2);
	LIBZHL_API void *destructor(uint param_1);

	LIBZHL_API RValue *InternalGetYYVarRef(int index) const 
	{
		return const_cast<YYObjectBase*>(this)->InternalGetYYVarRef(index);
	};
	LIBZHL_API RValue *InternalGetYYVarRefL(int index) const {
		return const_cast<YYObjectBase*>(this)->InternalGetYYVarRefL(index);
	};
	LIBZHL_API RValue *InternalReadYYVar(int index) const {
		return const_cast<YYObjectBase*>(this)->InternalReadYYVar(index);
	};
	
	RValue* operator[](const char* name);
    RValue* operator[](int32_t index);

	YYObjectBase *m_pNextObject;
	YYObjectBase *m_pPrevObject;
	YYObjectBase *m_Prototype;
	PVOID m_Pcre;
	PVOID m_Pcre2;
	char *m_Class;
	FNGetOwnProperty m_GetProperty;
	FNDeleteOwnProperty m_DeleteOwnProperty;
	FNDefineOwnProperty m_DefineOwnProperty;
	CHashMap<int, RValue*, 3> *m_YYVarsMap;
	CWeakRef **m_WeakRef;
	uint32_t m_WeakRefCount;
	uint32_t m_VariableCount;
	uint32_t m_Flags;
	uint32_t m_Capacity;
	uint32_t m_Visited;
	uint32_t m_VisitedGC;
	int32_t m_GCGeneration;
	int32_t m_GCCreationFrame;
	int32_t m_Slot;
	YYObjectKind m_ObjectKind;
	int32_t m_RValueInitType;
	int32_t m_CurrentSlot;
	
};

struct YYRECT
{
	float m_Left;
	float m_Top;
	float m_Right;
	float m_Bottom;
	
};

struct CInstance : YYObjectBase
{
	
    
	int32_t GetMemberCount() const {
		return m_YYVarsMap ? m_YYVarsMap->m_UsedCount : 0;
	}
	int32_t GetMemberCount() {
		return m_YYVarsMap ? m_YYVarsMap->m_UsedCount : 0;
	}

	static std::vector<CInstance*> Where(std::function<bool(CInstance*)> func);
	static CInstance* FirstOrDefault(std::function<bool(CInstance*)> func);
	static bool Any(std::function<bool(CInstance*)> func);
	static void ForEach(std::function<void(CInstance*)> func);

    RValue* operator[](const char* name);
    RValue* operator[](int32_t index);

	LIBZHL_API void Activate();
	LIBZHL_API void DeSerialise(IBuffer *param_1, bool param_2);
	LIBZHL_API void Deactivate();
	LIBZHL_API static CInstance *__stdcall Find(int param_1);
	LIBZHL_API void Free(bool param_2);
	LIBZHL_API float GetImageIndex();
	LIBZHL_API float GetImageNumber();
	LIBZHL_API int GetTimer(int param_2);
	LIBZHL_API static void __stdcall HandleInstance(CInstance *param_1);
	LIBZHL_API static bool __stdcall InstanceHandled(CInstance *toHandle);
	LIBZHL_API bool Mark4GC(uint *param_2, int param_3);
	LIBZHL_API void PreFree();
	LIBZHL_API void __stdcall Remove(CInstance *param_1);
	LIBZHL_API void Serialise(IBuffer *param_1);
	LIBZHL_API void SetDeactivated(bool param_1);
	LIBZHL_API void ThreadFree(bool param_2, void *GC_Context);
	LIBZHL_API void constructor(float _x, float _y, int _id, int _objectIndex, bool _hasObjectParent);
	LIBZHL_API void destructor();
	int64_t m_CreateCounter;
	CObjectGM *m_Object;
	CPhysicsObject *m_PhysicsObject;
	CSkeletonInstance *m_SkeletonAnimation;
	PVOID m_SequenceInstance;
	uint32_t m_InstanceFlags;
	int32_t m_ID;
	int32_t m_ObjectIndex;
	int32_t m_SpriteIndex;
	float m_SequencePosition;
	float m_LastSequencePosition;
	float m_SequenceDirection;
	float m_ImageIndex;
	float m_ImageSpeed;
	float m_ImageScaleX;
	float m_ImageScaleY;
	float m_ImageAngle;
	float m_ImageAlpha;
	uint32_t m_ImageBlend;
	float m_X;
	float m_Y;
	float m_XStart;
	float m_YStart;
	float m_XPrevious;
	float m_YPrevious;
	float m_Direction;
	float m_Speed;
	float m_Friction;
	float m_GravityDirection;
	float m_Gravity;
	float m_HorizontalSpeed;
	float m_VerticalSpeed;
	YYRECT m_BoundingBox;
	int m_Timers[12];
	PVOID m_TimelinePath;
	CCode *m_InitCode;
	CCode *m_PrecreateCode;
	CObjectGM *m_OldObject;
	int32_t m_LayerID;
	int32_t m_MaskIndex;
	int16_t m_MouseOverCount;
	CInstance *m_Next;
	CInstance *m_Prev;
	SLink m_collisionLink;
	SLink m_dirtyLink;
	SLink m_withLink;
	float m_Depth;
	float m_CurrentDepth;
	float m_LastImageNumber;
	uint32_t m_collisionTestNumber;
	
};

struct CLayer;

struct CLayerElementBase
{
	int32_t m_Type;
	int32_t m_ID;
	bool m_RuntimeDataInitialized;
	const char *m_Name;
	CLayer *m_Layer;
	LayerElementFlink m_Flink;
	LayerElementBlink m_Blink;
	
};

struct CLayer
{
	
    CLayer* GetNext() { 
        return _ptr_Next; 
    }
	void SetNext(CLayer* _pNext) { 
        _ptr_Next = _pNext; 
    }
	CLayer* GetPrev() { 
        return _ptr_Prev; 
    }
	void SetPrev(CLayer* _pPrev) { 
        _ptr_Prev = _pPrev; 
    }
int32_t m_Id;
	int32_t m_Depth;
	float m_XOffset;
	float m_YOffset;
	float m_HorizontalSpeed;
	float m_VerticalSpeed;
	bool m_Visible;
	bool m_Deleting;
	bool m_Dynamic;
	char *m_Name;
	RValue m_BeginScript;
	RValue m_EndScript;
	RValue m_Effect;
	int m_ShaderID;
	LinkedList<CLayerElementBase> m_Elements;
	CLayer *_ptr_Next;
	CLayer *_ptr_Prev;
	
};

struct CLayerEffectInfo
{
	
};

struct CLayerInstanceElement : CLayerElementBase
{
	int32_t m_InstanceID;
	CInstance *m_Instance;
	
};

struct CLayerElementBase;
struct CLayerOldTilemapElement;
struct CRoom;

struct CLayerManager
{
	
	LIBZHL_API static CLayer *__stdcall AddDynamicLayer(CRoom *param_1, int param_2);
	LIBZHL_API static void __stdcall AddElementToLayer(CRoom *_pRoom, CLayer *_pLayer, CLayerElementBase *_pElement, bool _addToEnd);
	LIBZHL_API static void __stdcall AddInstance(CRoom *param_1, CInstance *param_2);
	LIBZHL_API static void __stdcall AddInstanceToLayer(CRoom *param_1, CLayer *param_2, CInstance *param_3);
	LIBZHL_API static int __stdcall AddNewElement(CRoom *_pRoom, CLayer *_pLayer, CLayerElementBase *_pElement, bool _buildRuntimeData);
	LIBZHL_API static int __stdcall AddNewElementAtDepth(CRoom *param_1, int param_2, byte param_3, bool param_4, bool param_5);
	LIBZHL_API static void __stdcall BuildElementRuntimeData(CRoom *param_1, CLayer *param_2, byte param_3);
	LIBZHL_API static void __stdcall BuildOldTilemapElementRuntimeData(CRoom *param_1, CLayer *param_2, CLayerOldTilemapElement *param_3);
	LIBZHL_API static void __stdcall BuildRoomLayerRuntimeData(CRoom *param_1);
	LIBZHL_API static void __stdcall CleanElementRuntimeData(CRoom *param_1, byte param_2);
	LIBZHL_API static void __stdcall CleanRoomLayers(CRoom *param_1);
	LIBZHL_API static void __stdcall SortDeactivatedInstances(CLayer *param_1);
	
};

struct CLayerOldTilemapElement
{
	
};

struct CLayerSpriteElement : CLayerElementBase
{
	int32_t m_SpriteIndex;
	float m_SequencePosition;
	float m_SequenceDirection;
	float m_ImageIndex;
	float m_ImageSpeed;
	int32_t m_SpeedType;
	float m_ImageScaleX;
	float m_ImageScaleY;
	float m_ImageAngle;
	uint32_t m_ImageBlend;
	float m_ImageAlpha;
	float m_X;
	float m_Y;
	
};

struct CLayerTileElement : CLayerElementBase
{
	bool m_visible;
	int m_index;
	float m_X;
	float m_Y;
	int m_W;
	int m_H;
	float m_imageScaleX;
	float m_imageScaleY;
	float m_imageAngle;
	unsigned int m_imageBlend;
	float m_imageAlpha;
	int m_xo;
	int m_yo;
	
};

struct CPhysicsObject
{
	
};

struct CPhysicsWorld
{
	
};

struct CBackGM;

struct CLayerInstanceElement;
struct CPhysicsWorld;
struct CViewGM;

struct RTile
{
	
};

struct YYRoom;
struct YYRoomInstances;
struct YYRoomTiles;

struct CRoom
{
	
	LIBZHL_API void DeleteInstance(CInstance *self, bool removeGlobal);
	LIBZHL_API void UpdateActive();
	LIBZHL_API void UpdateLayers();
	LIBZHL_API void constructor();
	int32_t m_LastTile;
	CRoom *m_InstanceHandle;
	char *m_Caption;
	int32_t m_Speed;
	int32_t m_Width;
	int32_t m_Height;
	bool m_Persistent;
	uint32_t m_Color;
	bool m_ShowColor;
	CBackGM *Backgrounds[8];
	bool m_EnableViews;
	bool m_ClearScreen;
	bool m_ClearDisplayBuffer;
	CViewGM *m_Views[8];
	char *m_LegacyCode;
	CCode *m_CCode;
	bool m_HasPhysicsWorld;
	int32_t m_PhysicsGravityX;
	int32_t m_PhysicsGravityY;
	float m_PhysicsPixelToMeters;
	OLinkedList<CInstance> m_Active;
	LinkedList<CInstance> m_Deactive;
	CInstance *m_MarkedFirst;
	CInstance *m_MarkedLast;
	int32_t *m_CreationOrderList;
	int32_t m_CreationOrderListSize;
	YYRoom *m_WadRoom;
	unsigned char *m_WadBaseAddress;
	CPhysicsWorld *m_PhysicsWorld;
	int32_t m_TileCount;
	CArrayStructure<RTile> m_Tile;
	YYRoomTiles *m_WadTiles;
	YYRoomInstances *m_WadInstances;
	char *m_Name;
	bool m_IsDuplicate;
	LinkedList<CLayer> m_Layers;
	CHashMap<int, CLayer*, 7> m_LayerLookup;
	CHashMap<int, CLayerElementBase*, 7> m_LayerElementLookup;
	CLayerElementBase *m_LastLayerElementLookedUp;
	CHashMap<int, CLayerInstanceElement*, 7> m_LayerInstanceElementLookup;
	int32_t *m_SeqInstances;
	int32_t m_SeqInstancesCount;
	int32_t m_SeqInstancesMax;
	int32_t *m_EffectLayerIDs;
	int32_t m_EffectLayerIdCount;
	int32_t m_EffectLayerIdMax;
	
};

struct CScript;

struct CScript
{
	
	virtual ~CScript() {};
	void* m_Text;
	CCode* m_Code;
	YYGMLFuncs* m_Functions;
	CInstance* m_StaticObject;
	union {
		char* m_Script;
		int m_CompiledIndex;
	};
	char* m_Name;
	int m_Offset;

	LIBZHL_API bool Compile();
	LIBZHL_API void constructor(const char *param_1);
	
};

struct CScriptRef
{
	
};

struct CSequenceInstance
{
	
};

struct CSkeletonInstance
{
	
};

struct DynamicArray
{
	int m_Length;
	void *m_Array;
	
};

struct YYTexPageEntry;

struct CSprite
{
	DynamicArray m_Masks;
	DynamicArray m_Bitmaps;
	DynamicArray m_Textures;
	YYTexPageEntry **m_TexturePageEntry;
	void *m_pSkeleton;
	char *m_pWADMaskBase;
	char *m_Name;
	void *m_Unk1;
	void *m_Unk2;
	void *m_Sequence;
	void *m_Unk3;
	void *pGCSprite;
	int m_ItemCount;
	int m_BBboxMode;
	YYRECT m_BBox;
	int m_SpriteNumber;
	int m_Width;
	int m_Height;
	int m_XOrig;
	int m_YOrig;
	int m_CullRadius;
	int m_LocalTPE;
	int m_Index;
	int m_MaskCount;
	float m_Speed;
	int m_SpeedType;
	int m_Type;
	bool m_LoadedFromChunk;
	bool m_bDoColcheck;
	bool m_bIsRotatedBounds;
	bool m_bIsMaskCreated;
	bool m_bIsTransp;
	bool m_bIsSmooth;
	bool m_bIsPreload;
	bool m_bIsSepMasks;
	bool m_bIsLocalTPE;
	
};

struct CStream
{
	char m_UnkBool;
	int64 m_Size;
	int64 m_Position;
	void *m_Data;
	
};

struct CThreadLocalStorage
{
	
};

struct CVariableList;
struct RVariable;

struct CVariableList
{
	
	LIBZHL_API RVariable *Find(char *param_1, char *param_2);
	
};

struct CViewGM
{
	
};

struct CWeakRef
{
	
};

struct DLL_RFunction
{
	const char *m_ModuleName;
	PVOID m_DllBaseAddress;
	const char *m_FunctionName;
	MFunction m_Function;
	int m_CallingConvention;
	int m_ArgumentCount;
	int m_ArgumentKinds[17];
	int m_ResultKind;
	uint64_t m_Unknown;
	
};

struct DLValue
{
	__int64 val;
	int m_mmy;
	int m_nd;
	
};

struct DValue
{
	double m_Value;
	int m_Dummy;
	int m_Ind;
	
};

struct DebugInfo
{
	
};

struct GCObjectContainer : YYObjectBase
{
	CHashMap<YYObjectBase*, YYObjectBase*, 3> *m_YYObjectMap;
	
};

struct Graphics
{
	
	LIBZHL_API static void __stdcall Flush();
	LIBZHL_API static bool __stdcall OpenWindow(HWND g_hwnd, int width, int height, int debugMode);
	LIBZHL_API static void __stdcall Quit();
	LIBZHL_API static void __stdcall SetMatrix(int32_t param_1, int32_t param_2);
	LIBZHL_API static void __stdcall SetViewPort();
	
};

struct HashTable
{
	
	LIBZHL_API static int __stdcall compare_keys(const char *cStr, int length, int unk);
	
};

struct HashmapIterator
{
	
};

struct OldTilemapElement
{
	
};

struct RFunction
{
	char m_Name[64];
	TRoutine m_Routine;
	int32_t m_ArgumentCount;
	int32_t m_UsageCount;
	
};

struct RValHashMap
{
	
};

struct RValHashMapElement
{
	
};

struct RVariable
{
	RVariable *m_Next;
	RVariable *m_NotPrev;
	RValue m_Index;
	RValue m_Value;
	int m_Hash;
	
};

struct RVariableRoutine
{
	const char *m_Name;
	FNGetVariable m_GetVariable;
	FNSetVariable m_SetVariable;
	bool m_CanBeSet;
	
};

struct RefDynamicArrayOfRValue : YYObjectBase
{
	int m_refCount;
	int m_flags;
	RValue *m_Array;
	int64_t m_Owner;
	int m_Visited;
	int m_Length;
	
};

struct RenderStateManager;

struct RenderStateManager
{
	
	LIBZHL_API void Force();
	LIBZHL_API void Reset();
	LIBZHL_API void WriteStatesToBuffer();
	
};

struct SScriptCacheInfo
{
	
	LIBZHL_API void constructor(const char *param_1);
	
};

struct TmpStringBuilder;

struct TmpStringBuilder
{
	
	LIBZHL_API void Output(const char *param_1);
	
};

struct Buffer_Standard;
struct VMBuffer;
struct VMDebugInfo;
struct tagIConsole;

struct VM
{
	
	LIBZHL_API static VMDebugInfo *__stdcall DebugInfo(VMBuffer *param_1, int param_2);
	LIBZHL_API static char *__stdcall DebugLine(VMDebugInfo *param_1, const char *param_2);
	LIBZHL_API static int __stdcall DebugLineNumber(VMDebugInfo *param_1, const char *param_2, const char **param_3);
	LIBZHL_API static void __stdcall Disasm(const char *param_1, VMBuffer *param_2, tagIConsole *param_3);
	LIBZHL_API static int __stdcall DisasmOne(const char *param_1, uchar *param_2, int param_3, tagIConsole *param_4);
	LIBZHL_API static void __stdcall ExecDebug(VMExec *param_1, RValue *param_2, bool param_3);
	LIBZHL_API static void __stdcall GetAllInstanceIDs(Buffer_Standard *param_1, bool param_2);
	LIBZHL_API static void __stdcall GetGlobalVariables(Buffer_Standard *param_1, bool param_2);
	LIBZHL_API static void __stdcall GetLocalVariables(Buffer_Standard *param_1, YYObjectBase *param_2, bool param_3, int param_4);
	LIBZHL_API static void __stdcall GetObjectBaseVariables(Buffer_Standard *param_1, YYObjectBase *param_2, bool param_3);
	LIBZHL_API static void __stdcall GetRuntimeState(Buffer_Standard *param_1, int param_2);
	LIBZHL_API static void __stdcall GetRuntimeStateRealtime(Buffer_Standard *param_1, int param_2);
	LIBZHL_API static void __stdcall GetStructureCounts(Buffer_Standard *param_1);
	LIBZHL_API static void __stdcall WriteCallStack(Buffer_Standard *param_1, VMExec *param_2, bool param_3);
	LIBZHL_API static void __stdcall WriteInstanceBuiltInVariables(Buffer_Standard *param_1, CInstance *param_2);
	LIBZHL_API static void __stdcall WriteRValueToBuffer(RValue *param_1, Buffer_Standard *param_2);
	
};

struct VMBuffer
{
	
};

struct VMDebugInfo
{
	
};

struct Vec2
{
	float x;
	float y;
	
};

struct YYEvent
{
	
};

struct YYGMLFuncs
{
	
	const char *m_Name;
	union
	{
		PFUNC_YYGMLScript m_ScriptFunction;
		PFUNC_YYGML m_CodeFunction;
		PFUNC_RAW m_RawFunction;
	};
	PVOID m_FunctionVariables;

	YYGMLFuncs() : m_Name(""), m_CodeFunction(nullptr),  m_FunctionVariables(nullptr) {}
};

struct YYRoom
{
	uint32_t m_NameOffset;
	uint32_t m_Caption;
	int32_t m_Width;
	int32_t m_Height;
	int32_t m_Speed;
	int32_t m_Persistent;
	int32_t m_Color;
	int32_t m_ShowColor;
	uint32_t m_CreationCode;
	int32_t m_EnableViews;
	uint32_t pBackgrounds;
	uint32_t pViews;
	uint32_t pInstances;
	uint32_t pTiles;
	int32_t m_PhysicsWorld;
	int32_t m_PhysicsWorldTop;
	int32_t m_PhysicsWorldLeft;
	int32_t m_PhysicsWorldRight;
	int32_t m_PhysicsWorldBottom;
	float m_PhysicsGravityX;
	float m_PhysicsGravityY;
	float m_PhysicsPixelToMeters;
	
};

struct YYRoomInstances
{
	
};

struct YYRoomTiles
{
	
};

struct YYRunnerInterface
{
	
};

struct YYStrBuilderYY
{
	
};

struct YYTexPageEntry
{
	short m_X;
	short m_Y;
	short m_W;
	short m_H;
	short m_XOff;
	short m_YOff;
	short m_Width;
	short m_Height;
	short m_OriginalWidth;
	short m_OriginalHeight;
	short m_TexturePageID;
	
};


struct tagIConsole
{
	
};

struct CDS_Grid;
struct CDS_Priority;
struct CDS_Queue;
struct CDS_Stack;
struct RefDynamicArrayOfRValue;

LIBZHL_API int __stdcall ASYNCFunc_SpriteAdd(HTTP_REQ_CONTEXT *_pContext, PVOID _p, int *_pMap);
LIBZHL_API void __stdcall ASYNCFunc_SpriteCleanup(HTTP_REQ_CONTEXT *_pContext);
LIBZHL_API void __stdcall AddDirectoryToBundleWhitelist(const char *_pszFilename);
LIBZHL_API void __stdcall AddDirectoryToSaveWhitelist(const char *_pszFilename);
LIBZHL_API void __stdcall AddFileToBundleWhitelist(const char *_pszFilename);
LIBZHL_API void __stdcall AddFileToSaveWhitelist(const char *_pszFilename);
LIBZHL_API void __stdcall AddGlobalObject(YYObjectBase *param_1);
LIBZHL_API PVOID __stdcall Alloc(uint len, const char *srcFile, int lineNr, bool unk);
LIBZHL_API char *__stdcall Arg2String(int param_1);
LIBZHL_API bool __stdcall BOOL_RValue(const RValue *_pValue);
LIBZHL_API bool __stdcall Base64Encode(PVOID input_buf, size_t input_len, PVOID output_buf, size_t output_len);
LIBZHL_API bool __stdcall BufferGetContent(int _index, PVOID *_ppData, int *_pDataSize);
LIBZHL_API int __stdcall BufferWriteContent(int _index, int _dest_offset, PVOID _pSrcMem, int _size, bool _grow, bool _wrap);
LIBZHL_API bool __stdcall BundleFileExists(const char *_pszFileName);
LIBZHL_API bool __stdcall BundleFileName(const char *_name, int _size, const char *_pszFileName);
LIBZHL_API void __stdcall COPY_RValue(RValue *_pDest, const RValue *_pSource);
LIBZHL_API void __stdcall ClearHandledContainer();
LIBZHL_API void __stdcall Code_Constant_Prepare();
LIBZHL_API CInstance *__stdcall Code_CreateStatic();
LIBZHL_API bool __stdcall Code_Execute(CInstance *self, CInstance *other, CCode *func, RValue *args, int flags);
LIBZHL_API bool __stdcall Code_Execute_Special(CInstance *param_1, CInstance *param_2, CCode *param_3, RValue *param_4, int param_5);
LIBZHL_API int __stdcall Code_Variable_FindAlloc_Slot_From_Name(YYObjectBase *obj, const char *name);
LIBZHL_API char *__stdcall Code_Variable_Find_Name(const char *fn_name, int objind, int varind);
LIBZHL_API int __stdcall Code_Variable_Find_Set(const char *name, int scope, int setme);
LIBZHL_API void __stdcall Code_Variable_Init(int num_global, int num_instance, int num_local);
LIBZHL_API void __stdcall ConvertStackEntryStringToDouble(uchar **param_1, double *param_2, uchar *param_3, uchar *param_4, uchar param_5, VMExec *param_6, char *param_7);
LIBZHL_API void __stdcall CreateAsyncEventWithDSMap(int _map, int _event);
LIBZHL_API void __stdcall CreateAsyncEventWithDSMapAndBuffer(int _map, int _buffer, int _event);
LIBZHL_API int __stdcall CreateBuffer(int _size, eBuffer_Format _bf, int _alignment);
LIBZHL_API int __stdcall CreateDsMap(int _num);
LIBZHL_API HSPRITEASYNC __stdcall CreateSpriteAsync1(int *_pSpriteIndex, int _xOrig, int _yOrig, int _numImages, int _flags);
LIBZHL_API HSPRITEASYNC __stdcall CreateSpriteAsync2(int *_pSpriteIndex, int _xOrig, int _yOrig, int _numImages, int _flags);
LIBZHL_API void __stdcall DebugConsoleOutput(const char *fmt);
LIBZHL_API void __stdcall Debug_AddTag(uint32_t type, const char *tagName);
LIBZHL_API void __stdcall Debug_WriteErrorMessage();
LIBZHL_API void __stdcall Debug_WriteOutput();
LIBZHL_API void __stdcall Debug_WriteSurfaceIds();
LIBZHL_API void __stdcall DestroyDsMap(int _index);
LIBZHL_API void __stdcall DeterminePotentialRoot(YYObjectBase *_pContainer, YYObjectBase *_pObj);
LIBZHL_API uchar *__stdcall DoAnd(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoBreak(uint param_1, uchar param_2, uchar *param_3, uchar *param_4, VMExec *param_5, bool param_6);
LIBZHL_API uchar *__stdcall DoCall(uint ret, uchar *param_2, uchar *ccode_index, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoCallScript(CScript *param_1, int param_2, uchar *param_3, VMExec *param_4, YYObjectBase *param_5, YYObjectBase *param_6);
LIBZHL_API uchar *__stdcall DoConv(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoDiv(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoDup(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API void __stdcall DoGenerationalGC(int param_1);
LIBZHL_API uchar *__stdcall DoMul(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoOr(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoPop(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoPopEnv(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoPushEnv(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoRem(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoShl(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoShr(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoSub(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API uchar *__stdcall DoXor(uint param_1, uchar *param_2, uchar *param_3, VMExec *param_4);
LIBZHL_API void __stdcall DsListAddInt64(int _dsList, int64 _value);
LIBZHL_API void __stdcall DsListAddMap(int _dsList, int _mapIndex);
LIBZHL_API void __stdcall DsListClear(int _dsList);
LIBZHL_API int __stdcall DsListCreate();
LIBZHL_API void __stdcall DsMapAddBool(int _index, const char *_pKey, bool value);
LIBZHL_API bool __stdcall DsMapAddDouble(int _index, const char *_pKey, double value);
LIBZHL_API bool __stdcall DsMapAddInt64(int _index, const char *_pKey, int64 value);
LIBZHL_API void __stdcall DsMapAddRValue(int _index, const char *_pKey, RValue *value);
LIBZHL_API bool __stdcall DsMapAddString(int _index, const char *_pKey, const char *pVal);
LIBZHL_API void __stdcall DsMapClear(int _dsMap);
LIBZHL_API void __stdcall EndRoom(bool ending);
LIBZHL_API void __stdcall Error_Show_Action(const char *err, bool serious, bool _manualError);
LIBZHL_API bool __stdcall ExecuteIt(CInstance *selfinst, CInstance *otherinst, CCode *code, RValue *res, int flags);
LIBZHL_API void __stdcall Extension_Prepare();
LIBZHL_API void __stdcall FUN_0368c220();
LIBZHL_API void __stdcall FUN_0368c350();
LIBZHL_API void __stdcall FUN_0368c550();
LIBZHL_API void __stdcall FUN_0368c5e0();
LIBZHL_API void __stdcall FUN_0368c750();
LIBZHL_API void __stdcall FUN_0368c800();
LIBZHL_API void __stdcall FUN_0368c8b0();
LIBZHL_API void __stdcall FUN_0368c9d0();
LIBZHL_API void __stdcall FUN_0368cbf0();
LIBZHL_API void __stdcall FUN_0368cc00();
LIBZHL_API void __stdcall FUN_0368cc60();
LIBZHL_API void __stdcall FUN_0368d190();
LIBZHL_API void __stdcall FUN_0368d1e0();
LIBZHL_API void __stdcall FUN_0368db90();
LIBZHL_API void __stdcall FUN_036ae67e();
LIBZHL_API void __stdcall F_CreateD3DDevice();
LIBZHL_API void *__stdcall F_JS_ToInt32(RValue *ret, RValue *toConv);
LIBZHL_API void __stdcall F_LayerExists(RValue *param_1, CInstance *param_2, CInstance *param_3, int param_4, RValue *param_5);
LIBZHL_API void __stdcall FreeAllVertexData();
LIBZHL_API void __stdcall Function_Add(const char *name, TRoutine routine, int argnumb, bool regonly);
LIBZHL_API void __stdcall Function_Replace(const char *name, TRoutine routine);
LIBZHL_API bool __stdcall GET_RValue(RValue *_pRet, const RValue *_pV, YYObjectBase *_pPropSelf, int _index, bool fPrepareArray, bool fPartOfSet);
LIBZHL_API char *__stdcall GET_STRING_RValue(const RValue *param_1);
LIBZHL_API void __stdcall GLOBAL_RVALUE_HEAPSIZE();
LIBZHL_API void __stdcall GR_D3D_End();
LIBZHL_API bool __stdcall GR_D3D_Finish_Frame(bool present);
LIBZHL_API bool __stdcall GR_D3D_Init(HWND g_hwnd, int width, int height, int debugdisplay);
LIBZHL_API void __stdcall GR_D3D_Present();
LIBZHL_API bool __stdcall GR_D3D_Reset();
LIBZHL_API void __stdcall GR_D3D_Set_Region(int param_1, int param_2);
LIBZHL_API void __stdcall GR_D3D_Set_View_Area(float param_1, float param_2, float param_3, float param_4, float param_5);
LIBZHL_API void __stdcall GR_D3D_Settings_Init();
LIBZHL_API bool __stdcall GR_D3D_Start_Frame();
LIBZHL_API void __stdcall GR_Surface_FreeAll();
LIBZHL_API RValue *__stdcall GetArray(RefDynamicArrayOfRValue *Args);
LIBZHL_API void __stdcall GetMainWindowRect(tagRECT tag);
LIBZHL_API CDS_Grid **__stdcall GetTheGrids(int *param_1);
LIBZHL_API CDS_Priority **__stdcall GetThePriorityQueues(int *param_1);
LIBZHL_API CDS_Queue **__stdcall GetTheQueues(int *param_1);
LIBZHL_API CDS_Stack **__stdcall GetTheStacks(int *param_1);
LIBZHL_API int __stdcall HASH_RValue(const RValue *_pValue);
LIBZHL_API int __stdcall INT32_RValue(const RValue *param_1);
LIBZHL_API int64 __stdcall INT64_RValue(const RValue *_pValue);
LIBZHL_API void __stdcall InitGraphics();
LIBZHL_API void __stdcall InitLLVM();
LIBZHL_API void __stdcall InitSLLVM(SLLVMVars *param_1);
LIBZHL_API void __stdcall InvalidateFrozenVBs();
LIBZHL_API void __stdcall JSThrowTypeError(const char *param_1);
LIBZHL_API char *__stdcall KIND_NAME_RValue(const RValue *_pV);
LIBZHL_API int __stdcall KIND_RValue(const RValue *_pValue);
LIBZHL_API void __stdcall LOCK_RVALUE_MUTEX();
LIBZHL_API void __stdcall MarkAndSweepGen(int param_1, int param_2, bool param_3);
LIBZHL_API void __stdcall MarkInstancesAsDirty(int param_1);
LIBZHL_API CObjectGM *__stdcall Object_Data(int hash);
LIBZHL_API bool __stdcall Object_Prepare();
LIBZHL_API PVOID __stdcall PTR_RValue(const RValue *_pValue);
LIBZHL_API uchar *__stdcall PerformReturn(uchar *param_1, VMExec *param_2);
LIBZHL_API void __stdcall Perform_Event_Object(CInstance *param_1, CInstance *param_2, int objind, int param_4, int param_5);
LIBZHL_API void __stdcall PopContextStack(int param_1);
LIBZHL_API void __stdcall ProcessObjectDisposeList();
LIBZHL_API double __stdcall REAL_RValue(const RValue *_pValue);
LIBZHL_API void __stdcall ReleaseConsoleOutput(const char *fmt);
LIBZHL_API void __stdcall ResetGCRefs();
LIBZHL_API int __stdcall Room_Add();
LIBZHL_API CRoom *__stdcall Room_Data(int RoomID);
LIBZHL_API int __stdcall Room_Duplicate(int param_1);
LIBZHL_API int __stdcall Room_Find(const char *param_1);
LIBZHL_API void __stdcall Room_Name();
LIBZHL_API void __stdcall Room_Prepare();
LIBZHL_API void __stdcall Room_Prepare_fake();
LIBZHL_API void __stdcall RunnerLoadDataFile();
LIBZHL_API void __stdcall RunnerLoadGame();
LIBZHL_API void __stdcall SET_RValue(RValue *_pDest, RValue *_pV, YYObjectBase *_pPropSelf, int _index);
LIBZHL_API bool __stdcall SaveFileExists(const char *_pszFileName);
LIBZHL_API bool __stdcall SaveFileName(const char *_name, int _size, const char *_pszFileName);
LIBZHL_API CScript *__stdcall ScriptFromId(int param_1);
LIBZHL_API int __stdcall Script_Find_Id(const char *name);
LIBZHL_API void __stdcall Script_Free();
LIBZHL_API bool __stdcall Script_Perform(int ind, CInstance *selfinst, CInstance *otherinst, int argc, RValue *res, RValue *arg);
LIBZHL_API void __stdcall Script_Prepare();
LIBZHL_API void __stdcall SetSurfaceTarget();
LIBZHL_API void __stdcall ShowMessage(const char *msg);
LIBZHL_API void __stdcall StartRoom(int param_1, bool param_2);
LIBZHL_API void __stdcall StructAddBool(RValue *_pStruct, const char *_pKey, bool _value);
LIBZHL_API void __stdcall StructAddDouble(RValue *_pStruct, const char *_pKey, double _value);
LIBZHL_API void __stdcall StructAddInt(RValue *_pStruct, const char *_pKey, int _value);
LIBZHL_API void __stdcall StructAddRValue1(RValue *_pStruct, const char *_pKey, RValue *_pValue);
LIBZHL_API void __stdcall StructAddRValue2(RValue *_pStruct, const char *_pKey, RValue *_pValue);
LIBZHL_API void __stdcall StructCreate(RValue *_pStruct);
LIBZHL_API void __stdcall SwitchRoom(int param_1);
LIBZHL_API void __stdcall TickDebugger();
LIBZHL_API void __stdcall TimeLine_Prepare();
LIBZHL_API void __stdcall Timeline_timeline_visibleflashstart_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall Timeline_timeline_visibleflashstart_1(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall Timing_Sleep(int64 slp, bool precise);
LIBZHL_API int64 __stdcall Timing_Time();
LIBZHL_API void __stdcall UNLOCK_RVALUE_MUTEX();
LIBZHL_API void __stdcall VMError(VMExec *param_1, const char *param_2);
LIBZHL_API int32_t __stdcall Variable_BuiltIn_Find(const char *name);
LIBZHL_API bool __stdcall Variable_GetValue_Direct(YYObjectBase *param_1, int param_2, int param_3, byte param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall Variable_GetValue_OBJTYPE(int param_1, int param_2, int param_3, byte param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall Variable_SetValue_Direct(YYObjectBase *param_1, uint param_2, int param_3, RValue *param_4);
LIBZHL_API LRESULT __stdcall WndProc(HWND h_Wnd, uint param2, WPARAM param_3, LPARAM param_4);
LIBZHL_API PVOID __stdcall YYAlloc(int _size);
LIBZHL_API void __stdcall YYArrayResize(RValue **p_pArrayData, int memBlockSize, const char *file, int unk);
LIBZHL_API void __stdcall YYCreateArray(RValue *pRValue, int n_values, double *values);
LIBZHL_API void __stdcall YYCreateString(RValue *param_1, const char *param_2);
LIBZHL_API void __stdcall YYError(const char *fmt);
LIBZHL_API void __stdcall YYGML_DeleteWithIterator(SWithIterator *param_1, YYObjectBase **param_2, YYObjectBase **param_3);
LIBZHL_API CInstance *__stdcall YYGML_FindInstance(int param_1);
LIBZHL_API CInstance *__stdcall YYGML_GetStaticObject(int param_1);
LIBZHL_API int __stdcall YYGML_NewWithIteratorEx(SWithIterator *param_1, YYObjectBase **param_2, YYObjectBase **param_3, int param_4);
LIBZHL_API bool __stdcall YYGML_Variable_GetValue(int param_1, int param_2, int param_3, RValue *param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall YYGML_Variable_GetValue_OBJTYPE(int param_1, int param_2, int param_3, byte param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall YYGML_Variable_SetValue(int param_1, int param_2, int param_3, byte param_4);
LIBZHL_API bool __stdcall YYGML_WithIteratorNext(SWithIterator *param_1, YYObjectBase **param_2, YYObjectBase **param_3);
LIBZHL_API bool __stdcall YYGetBool(RValue *_pBase, int _index);
LIBZHL_API float __stdcall YYGetFloat(RValue *_pBase, int _index);
LIBZHL_API int32_t __stdcall YYGetInt32(RValue *_pBase, int _index);
LIBZHL_API int64 __stdcall YYGetInt64(RValue *_pBase, int _index);
LIBZHL_API PVOID __stdcall YYGetPtr(RValue *_pBase, int _index);
LIBZHL_API intptr_t __stdcall YYGetPtrOrInt(RValue *_pBase, int _index);
LIBZHL_API double __stdcall YYGetReal(RValue *_pBase, int _index);
LIBZHL_API char *__stdcall YYGetString(const RValue *_pBase, int _index);
LIBZHL_API uint32_t __stdcall YYGetUint32(RValue *_pBase, int _index);
LIBZHL_API HYYMUTEX __stdcall YYMutexCreate(const char *_name);
LIBZHL_API void __stdcall YYMutexDestroy(HYYMUTEX hMutex);
LIBZHL_API void __stdcall YYMutexLock(HYYMUTEX hMutex);
LIBZHL_API void __stdcall YYMutexUnlock(HYYMUTEX hMutex);
LIBZHL_API PVOID __stdcall YYRealloc(PVOID pOriginal, int _newSize);
LIBZHL_API void __stdcall YYSetString(RValue *out, const char *param_2);
LIBZHL_API char *__stdcall YYStrDup(const char *param_1);
LIBZHL_API void __stdcall YYstrnlen();
LIBZHL_API void __stdcall __gml_free();
LIBZHL_API uchar *__stdcall boxType(RValue *param_1, eVM_Type param_2, uchar *param_3);
LIBZHL_API void __stdcall build_function_index();
LIBZHL_API void __stdcall builtin_variable_define();
LIBZHL_API void __stdcall gml_GlobalScript_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_1(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_2(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_3(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_4(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_BallCreate(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_BallStep(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DOCUMENTATION_README(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DirtyBloom_define(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DirtyBloom_disable(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DirtyBloom_enable(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DirtyBloom_enabled(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DirtyBloom_ghostsdefine(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DirtyBloom_set_strength(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DissolveSettings(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DissolveShader(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_DrawStatesDebug(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_README(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_get(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_get_colour(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_get_colour_element(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_get_element(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_get_internal(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_set(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_set_colour(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_set_element(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___background_set_internal(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___global_object_depths(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___init_action(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___init_background(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___init_d3d(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___init_global(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___init_view(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___view_get(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___view_set(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript___view_set_internal(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_action_current_room(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_action_end_game(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_action_kill_object(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_action_next_room(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_action_restart_game(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_action_wrap(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_avoid_circle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_avoid_rectangle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_background_get_texture(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_cleanmem(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_cleanmem_get_mem(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_cleanmem_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_colorpicker(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_create_shockwave(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_curve_get_value(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_d3d_set_fog(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_d3d_transform_add_translation(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_d3d_transform_set_identity(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_d3d_transform_set_rotation_z(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_debug(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_9slice(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_SHADER_blur(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_background_tiled(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_circle_curve(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_circle_width(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_circular_progress_bar(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_crosshair_quarter(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_healthbar_circular(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify_ext(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify_fast(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify_laser(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify_path(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify_path_circle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_magnify_path_simple(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_path_gradient(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_rectangle_width(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_reset(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_set_alpha_test(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_set_blend_mode(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_draw_set_blend_mode_ext(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_gfx_blur(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_gfx_blur_directional(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_gfx_blur_gaussian(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_gfx_blur_masked(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_gfx_blur_radial(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_gfx_blur_twist(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_image_scale(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_instance_create(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_line_solid_intersection(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_lines_intersect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_macros(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_object_get_depth(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_path_random(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_path_random_circle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_random_color(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scp_set(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_AlertEnemy(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_DailyReward(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_KPI_Arena(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_reopen_lobby(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_request_join_lobby(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_send_CreateAccount(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_send_Login(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_send_PasswordReset(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_send_RequestPasswordReset(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_send_packet_queue(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_MS_set_referal(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_activereload(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_add_chat_entry(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ammotypes(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ammotypes_classbased(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_antibullet_laser(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_array_choose(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_attachments_apply(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_attachments_apply_ultimate(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_attachments_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_attachments_init_ultimate(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_badge_display(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_basic_unit_defense_stats(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_basic_unit_defensive_stats_calc(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_bloodsplat_large(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_boss_dialogs(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_bullet_chain_get_hits(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_button(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_button_check(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_button_intro(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_calculate_targetlaser_distance(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_change_gamemode(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_check_DLCS(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_check_achievements(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_check_gamepad_key(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_check_key(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_check_unlock(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chest_loottable(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chest_loottable_ammo(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chest_loottable_health(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chest_select_loottable(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chunk_connect_path(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chunk_getPathSet(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chunk_helper_sets_connect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_chunk_isPathConnected(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_class_descriptions(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_class_sprites(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_class_upgrades(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_classcheck(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_collision_line(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_color_transistion(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_colors_borderdecal_finder(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_console_input(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_convert_talenttext(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_create_bullet(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_create_bullet_effects(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_create_bullet_shell(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_curses_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_damage_bot(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_damage_numbers(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_deathrecap(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_dispatch_headhunters(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_display_prompt_tooltip(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_distance_sqr(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_draw_decal_on_chunks(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_draw_interacting_bar(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_draw_pausebackground(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_draw_targetlaser(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_drop_bossLoot(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_drop_item(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ds_grid_get(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ds_grid_set(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ds_grid_set_region(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_encrypt_password(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_ctrl_enforcerCoverPos(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_get_closest_fire_pos(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_get_random_position(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_grid_get_cell(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_optimize_path(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_reached_destination(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_set_stateChanged(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enemy_stop_moving(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enm_base_destructor(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enm_knockback(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enm_sensor_check(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enm_voiceover(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_enter_next_floor(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_bindings(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_bindings_items(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_credits(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_game(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_help(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_help2(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_language(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_other(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_esc_video(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_exception_handling(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_fieldNodes_distance(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_fieldNodes_getFieldNodesForShape(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_fightCheck(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_file_delete(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_file_exists(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_filter_layer(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_find_chunk_surface(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_find_highest_parent(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_find_talent_id(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_find_talent_objectindex(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_find_vect_in_list(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_fit_shape_in_node_list(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_fit_shape_in_rect_list(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_gamepad_check_itemBlock(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_getSpawnInSpriteRect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_all_teams(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_array_for_particle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_closest_obj_in_range(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_collision_free(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_collision_normal_dir(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_collision_rectangle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_gamepad_key_name(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_input(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_input_name(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_inst_for_networkId(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_interpolated_shifted_player_pos(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_key_name(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_other_team(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_percentage(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_random_pickup_pos(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_shape_dimensions(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_spawn_pos_on_radius(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_get_team(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_glow_splat_draw(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_goto_room(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_grids_add_circle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_grids_add_rectangle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_grids_clear_circle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_grids_clear_rectangle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_handle_switch_online(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_has_arena_pack(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_has_supporter_pack(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_has_supporter_pack2(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_has_synthetik(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_heal_bot(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_healing_laser_turret(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_healthbar(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_helptext(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_humanoid_attack_movement(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_humanoid_attack_movement_create(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_humanoid_movement(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_increase_packetID(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_indentifypotion_effect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ini_open(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_ini_write_string(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_instance_create(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_instance_nearest(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_invert_layer(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_is_field_In_rect_list(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_is_in_layer(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_is_parent(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_is_player_stuck(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_is_pos_in_enemy_view(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_is_pos_in_rectangle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_item_activeandcooldown(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_item_descriptions(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_item_leveling(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_item_tooltip(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_item_variants_allitems_bonus(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_item_variants_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_join_team(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_leave_team(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_list_add_unique(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_list_contains(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_list_create(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_list_delete(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_list_destroy(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_load_language(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_load_language_backup(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_load_languages_fixed(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_load_progress(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_load_statistics(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_localPlayer_add_screenshake(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_localization(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_map_to_string(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_microchip_gaining(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mod_ini_read_real(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mod_ini_read_string(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_modweapon_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mouseover(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mp_get_closest_grid_pos(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mp_grid_cast(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mp_grid_get_cell(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_mystery_reward(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_nearest_team_member(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_net_add_statuseffect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_net_cleanup_remote(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_net_subscribe_newsletter(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_net_write_packet(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_destroy(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_get_region_name(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_instantiate(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_instantiateSingle(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_instantiate_bullet(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_receiving_client(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_networking_receiving_server(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_new_lobby_notification(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_node_A_Star(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_notification(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_object_debug(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_object_is_parent(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_particle_effects_unit(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_pauseCheck(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_perk_save(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_perk_system(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_perks_chips(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_perks_mars(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_pickup_loottable(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_plating_gaining(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_play_sound_at(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_ammo_gaining(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_currency_change(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_data_change(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_exp_change(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_health_damage(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_health_healing(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_interest_change(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_keycard_gain(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_labfunds_change(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_multiplayer_revive(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_player_set_stealth(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_popup(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_powerup(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_prefetch_queue(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_progress_ini_save(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_prop_shadow_ao_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_prop_spawn_random(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_quest_finished(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_quest_rewards(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_questlog(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_choose(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_exponential(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_gaussian(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_get_value(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_irandom(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_random(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_random_range(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_set_seed(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_random_shuffle_list(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_randomstatboost_gaining(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_range_and_soundcheck(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_raritycolors_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_raycast(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_rebind_input(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_rebinding_button(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_remap_valuerange(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_reset_player(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_reset_tokens(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_roll_ammotype(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_roll_next_room(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_rumble(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_savegame_ini(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_search_audio_at(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_select_from_spawntable(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_send_ArenaMissionSelect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_send_LobbyUpdate(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_send_MS_packet(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_send_global_chat_message(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_send_net_rpc(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_send_packet(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_set_Achievement(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_set_depth(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_settings_ini(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_settings_ini_save(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_settings_load(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_setup_chunk_fields(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_setup_patrolling(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_shared_networking_event(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_shrine_effect_active(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_skyscraper_draw(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_slider(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_sort_objList_by_depth(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_sort_objList_by_dist(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_sort_vectList_by_dist(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_sound(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_alarm_squad(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_chunk_field(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_bigRoom(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_bigRoom_noWalls(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_extra(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_mediumRoom(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_mediumRoom_noWalls(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_smallRoom(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_rnd_smallRoom_noWalls(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_spawn_room(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_split_string(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_statboost(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_statistics_ini(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_statistics_saving(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_statuseffects_collision(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_string_trim_whites(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_talent_desctiptions(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_talent_selection(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_talents_new(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_technologies(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_terrorlevel_increase(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_tooltip(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_tooltipbig_weapons_getdata(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_transfer_item_coop(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_tween0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_unlock_ammo(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_unlock_descriptions(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_unlock_outgame_progress(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_update_particle_pos(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_url_open(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_vial_gaining(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_view_cone(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_descriptions(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_drop(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_firing(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_init_free(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_pickup(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_tooltipbars(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_variants_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_scr_weapon_variants_init_ultimate(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_set_enemy_path(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_slice_init(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_slice_pragma(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_spr_perks_artefacts(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_texture_set_repeat(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_add(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_delete(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_exists(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_background(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_depth(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_height(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_ids(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_left(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_top(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_width(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_x(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_get_y(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_layer_find(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tile_set_blend(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_EASE_enums(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_ease_lerp(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_ease_progress(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_ease_start_end(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect2(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect2_rot(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect3(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect3_sqr(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect4(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_add(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_comp(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_cross(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_dist(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_div(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_divr(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_dot(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_is_zero(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_len(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_mult(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_multr(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_norm(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_perp(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_rev(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_scaler(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_to_vect2(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_to_vect3(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tmc_vect_to_vect4(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_tolerance(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_GlobalScript_vect(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_MCI_command(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_Object_DirtyBloom_obj_Create_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_DirtyBloom_obj_Draw_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_DirtyBloom_obj_PreCreate_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_DirtyBloom_obj_Step_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_Create_0(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_Other_10(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_Other_11(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_Other_12(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_Other_14(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_Other_62(CInstance *Self, CInstance *Other);
LIBZHL_API void __stdcall gml_Object_MasterServerHTTPReq_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_PARENT_ALL_BONI_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_PARENT_ALL_BONI_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_PARENT_ALL_BONI_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_PARENT_ALL_BONI_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_PARENT_ALL_BONI_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_Path_PARENT_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_Path_PARENT_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_Path_start_PARENT_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_Path_start_PARENT_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_path_start_brightguard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_T_path_start_brightguard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_2_epxlos_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_2_epxlos_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_2_epxlos_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_3_explos_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_3_explos_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_3_explos_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_back_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_back_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_back_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_back_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_top_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_top_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_top_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_FOG_top_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_GOG_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_GOG_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_LUT_end_Draw_75(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_LUT_end_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_LUT_start_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_LUT_start_Draw_77(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_LUT_start_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Localization_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Localization_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Localization_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_MOBA_Waypoint_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_MOBA_Waypoint_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_MOBA_ctrl_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_MOBA_ctrl_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Origin_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Origin_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_PSN_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_PSN_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Xbox_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_Xbox_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_airhorn_effect_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_airhorn_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_airhorn_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_airhorn_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alarmpole_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alarmpole_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alarmpole_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alarmpole_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alarmpole_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_bullets_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_bullets_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_bullets_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_items_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_items_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_items_KeyPress_112(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_items_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_items_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_solid_Alarm_6(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_solid_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_solid_Other_25(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_all_solid_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_KeyPress_37(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_KeyPress_38(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_KeyPress_39(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_KeyPress_40(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_allgame_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alublock_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alublock_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alublock_bigwide_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alublock_bigwide_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alublock_round_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_alublock_round_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ammo_pack2_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ammo_pack2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ammo_pack_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ammo_pack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ammo_pack_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ammo_pack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_analytics_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_analytics_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_analytics_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_androidconsole_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_androidconsole_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_androidconsole_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_instagib_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_instagib_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_instagib_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_itemupgrade_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_itemupgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_itemupgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_madness_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_madness_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_madness_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_mysterybonus_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_mysterybonus_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_mysterybonus_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_mysterybonus_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_randomrun_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_randomrun_Alarm_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_randomrun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_randomrun_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_randomrun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_strafe_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_artefact_strafe_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_audio_emitter_water_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_aura_crystal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_aura_crystal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_aura_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_aura_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_aura_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_back_ambientlight_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_back_ambientlight_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_back_ambientlight_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_back_colorchange_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_back_colorchange_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_back_colorchange_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_background_pipes_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_background_pipes_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_background_pipes_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_background_tiled_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_background_tiled_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_background_tiled_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_badge_display_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_badge_display_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_badge_display_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_electric_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_electric_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_explosive_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_explosive_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_explosive_gaz_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_explosive_gaz_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_explosive_gaz_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_oil_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_oil_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_oil_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_poison_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_poison_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_spawner_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_tankshell_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_tankshell_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_barrel_tankshell_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beam_armageddon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beam_armageddon_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beam_armageddon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beartrap_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beartrap_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beartrap_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_beartrap_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindercage_cable_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindercage_cable_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindercage_double_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindercage_double_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_default_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bindings_default_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_splashes_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_splashes_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_splashes_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_splats_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_splats_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_splats_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_sprinkles_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_sprinkles_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blood_sprinkles_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_background_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_background_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_background_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_background_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_bokeh_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_bokeh_Draw_77(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_blur_bokeh_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_APC_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_APC_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_APC_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_APC_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_APC_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_armageddon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_armageddon_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_armageddon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_armageddon_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_fortress_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_fortress_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_fortress_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_fortress_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_heart_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_heart_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_heart_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_heart_Other_24(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_heart_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_heart_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_intro_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_intro_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_intro_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_intro_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_intro_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_spider_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_spider_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_spider_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_spider_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_triple_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_triple_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_triple_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_triple_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_boss_triple_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bossroom_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bossroom_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_breachmarker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_breachmarker_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_breachmarker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_breachradius_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_breachradius_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_breachradius_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bridge_1_Alarm_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bridge_1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bridge_1_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bridge_1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_airhorn_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_airhorn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_airhorn_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_airhorn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammo2_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammo2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammo2_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammo2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammo2_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammoreg_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammoreg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammoreg_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ammoreg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_critchance_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_critchance_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_critchance_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_critchance_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_onlyexp_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_onlyexp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_onlyexp_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_onlyexp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_expboost_onlyexp_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_firerate_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_firerate_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_firerate_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_firerate_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_flaregun_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_flaregun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_flaregun_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_flaregun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_healthreg_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_healthreg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_healthreg_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_healthreg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_keycard_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_keycard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_keycard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_keycard_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ministim_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ministim_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ministim_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_ministim_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_movespeed_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_movespeed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_movespeed_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_movespeed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_overclock_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_overclock_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_overclock_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_overclock_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_overclock_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_psycloud_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_psycloud_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_psycloud_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_psycloud_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_psycloud_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_shield_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_shield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_shield_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_shield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_smokescreen_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_smokescreen_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_smokescreen_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_smokescreen_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_speedboost_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_speedboost_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_speedboost_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_speedboost_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_stimpack_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_stimpack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_stimpack_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_stimpack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_timed_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_timed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_timed_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_timed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buff_timed_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Alarm_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Alarm_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_buffs_debuffs_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_bolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_bolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_buck_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_buck_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_fire_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_fire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_flech_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_flech_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_helsing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_helsing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_masterkey2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_masterkey2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_masterkey_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_masterkey_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_slug_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_10g_slug_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_127_sabot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_127_sabot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_127_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_127_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_144_sabot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_144_sabot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_144_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_144_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_473_alt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_473_alt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_473_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_473_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_556_sabot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_556_sabot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_556_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_556_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_556_sp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_556_sp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_ap_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_ap_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_cata_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_cata_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_hs_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_hs_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_toxic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_88_toxic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_ap_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_ap_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_hp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_hp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_hs_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_hs_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_sp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_sp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_sp_magnum_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_9mm_sp_magnum_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_arrow_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_arrow_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_arrow_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_arrow_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_chaos_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_chaos_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_damn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bloodbolt_damn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_breachingcharge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_breachingcharge_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_breachingcharge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_breachingcharge_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bullethell_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bullethell_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_bullethell_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_beam_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_beam_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_orb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_chain_orb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_coil_x1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_coil_x1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_coil_x4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_coil_x4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_ballistic_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_ballistic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_ballistic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_drone_mortar_ballistic_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemy_poison_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemy_poison_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemy_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemy_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemypistol_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemypistol_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemyrifle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemyrifle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemysniper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_enemysniper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_fireball_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_fireball_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_fireball_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flak1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flak1_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flak1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flamethrower_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flamethrower_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flamethrower_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_flamethrower_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_bounce_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_bounce_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_bounce_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_enm_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_enm_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_enm_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_enm_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_enm_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_enm_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_rubber_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_rubber_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_grenade_rubber_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_ironorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_ironorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_ironorb_enemy_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_ironorb_enemy_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_jetmg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_jetmg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_kunai_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_kunai_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_kunai_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_beam_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_beam_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_beam_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_beam_item_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_beam_item_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_crystal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_crystal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_fusion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_fusion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_fusion_slow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_fusion_slow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_ion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_ion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_drone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_drone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_drone_challenge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_drone_challenge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_fire_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_fire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_gold_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_gold_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_gold_pen_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_gold_pen_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_pellet_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_pellet_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_reducedDMG_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_line_reducedDMG_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_plasma_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_plasma_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_railgun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_railgun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_railgun_item_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_railgun_item_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_shieldnuke_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_shieldnuke_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_slow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_slow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_worker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_laser_worker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_nail_sc_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_nail_sc_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_particle_explo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_particle_explo_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_particle_explo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_boss2_A_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_boss2_A_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_followmouse_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_followmouse_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_homing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_homing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_homingslow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_homingslow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_particle2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_particle2_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_particle2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_psyBall_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_psyBall_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_psyBall_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_rapidfire_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_rapidfire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_rapidfire_xm29_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_rapidfire_xm29_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_crazy_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_rocket_turret_crazy_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_seeker_item_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_seeker_item_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_sniperturret_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_sniperturret_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_taser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_taser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_taser_enm_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_taser_enm_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_tomahawk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_tomahawk_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_tomahawk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turret_crystalmortar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turret_crystalmortar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turret_mortar_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turret_mortar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turret_mortar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turret_mortar_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turretvierling_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_turretvierling_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_vierling_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullet_vierling_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullettrail_curvedsmoke_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullettrail_curvedsmoke_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_bullettrail_curvedsmoke_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cablestack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cablestack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_Draw_72(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chat_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_ammo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_ammo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_artefact_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_artefact_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_classperk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_classperk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_currency_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_currency_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_goldensmall_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_goldensmall_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_health_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_health_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_cursed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_cursed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_metal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_metal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_timed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_timed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_wood_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_item_wood_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_mystery_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_mystery_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Alarm_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_radio_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_radio_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_spwn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_spwn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_spwn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_upgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_upgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_challenge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_challenge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_cursed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_cursed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_metal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_metal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_timed_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_timed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_timed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_wood_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_wood_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_wood_arena_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_weapon_wood_arena_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_wepupgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chest_wepupgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_A_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_A_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_B_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_B_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_X_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_X_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_Y_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_Y_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_Z_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_Z_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_floor_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_floor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_floor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_Alarm_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_Alarm_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_Alarm_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_KeyPress_85(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_surfacedraw_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_surfacedraw_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunk_surfacedraw_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunkedge_grid_right_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunkedge_grid_top_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunkedge_grid_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_chunkedge_pillar_right_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_class_upgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_class_upgrade_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_class_upgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_climbblock_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_climbblock_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_climbwall_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_climbwall_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_clothbox_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_clothbox_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cogwheel_A_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cogwheel_A_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cogwheel_A_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cogwheelground_large_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cogwheelground_large_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cogwheelground_large_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_color_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_color_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_color_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_barrier_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_barrier_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_barrier_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_barrier_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_btr_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_btr_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_btr_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_btr_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_cube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_cube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_milestone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_milestone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_pillar_solo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_pillar_solo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_rect_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_rect_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_rect_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_rect_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tank_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tank_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tank_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tank_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tankbarrier_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tankbarrier_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tent_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tent_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tent_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conc_tent_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concback_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concback_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concback_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concpipe_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concpipe_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concrete_decal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concrete_decal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_concrete_decal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conctext_02_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_conctext_02_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_container_horizontal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_container_horizontal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_container_vertical_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_container_vertical_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_containeroil_hor_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_containeroil_hor_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_containeroil_vert_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_containeroil_vert_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_KeyPress_112(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_KeyPress_90(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_control_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cratesmall_spawner_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cratesmall_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cratesmall_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crawlpipe_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crawlpipe_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_white_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_white_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_credits_particle_white_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_bigwall_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_bigwall_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_cube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_cube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_milestone45_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_milestone45_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_milestone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_milestone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_pyramid_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_pyramid_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_rect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_rect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_rect_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_d_rect_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_decal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_decal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_decal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_floating_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_floating_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_floating_big_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_floating_medium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_floating_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_generator_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_generator_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_genetorline_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_genetorline_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_greenglow_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_greenglow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_groundstones_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_groundstones_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_lamp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_lamp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_lamp_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_low_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_low_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_medium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_milestone_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_milestone_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_milestone_medium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_milestone_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_pillarspike_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_pillarspike_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_pillarspike_medium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_pillarspike_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_pillarspike_medium_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_smallwall_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_smallwall_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_solopillar_low_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_solopillar_low_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_solopillar_med_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_solopillar_med_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spire_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spire_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spire_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spire_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spritechange_ring_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spritechange_ring_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_spritechange_ring_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_teleporter_A_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_crystal_teleporter_A_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_curse_star_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_curse_star_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_curse_star_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_KeyPress_37(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_KeyPress_38(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_KeyPress_39(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_KeyPress_40(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_camera_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_lockon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_lockon_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_lockon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_lockon_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_mainmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_mainmenu_Mouse_53(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_mainmenu_Mouse_54(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_mainmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cursor_mainmenu_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_KeyPress_27(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_gameend_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_gameend_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_gameend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_gameend_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_gameend_KeyPress_27(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cutscene_cinematic_gameend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_electro_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_electro_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f1_spawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f1_spawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f1_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f2_b_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f2_b_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f5_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f5_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f6_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_f6_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_silo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinder_silo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinderspawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinderspawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_cylinderspawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dagger_stab_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dagger_stab_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dagger_stab_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathlocation_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathstatistics_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathstatistics_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathstatistics_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathstatistics_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathstatistics_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_deathstatistics_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debris_metal1_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debris_metal1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debris_metal1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debris_metal1_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_antishield_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_antishield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_antishield_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_antishield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_antishield_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_bleeding_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_bleeding_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_bleeding_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_bleeding_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_burning_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_burning_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_burning_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_burning_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_cheat_mod_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_cheat_mod_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_cheat_mod_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_healing_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_healing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_healing_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_healing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_loop_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_loop_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_loop_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_loop_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_loop_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_loop_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_maxhealth_curse_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_maxhealth_curse_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_maxhealth_curse_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_maxhealth_curse_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_move_barbed_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_move_barbed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_move_barbed_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_move_barbed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_movespeed_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_movespeed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_movespeed_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_movespeed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_poison_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_poison_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_poison_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_poison_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_shard_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_shard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_shard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_stun_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_stun_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debuff_stun_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debug_save_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_debug_save_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_decaldraw_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_decaldraw_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_decaldraw_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dodge_ghosting_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dodge_ghosting_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dodge_ghosting_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_heartenter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_heartenter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_heartenter_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearthack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearthack_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearthack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearthack_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearttank_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearttank_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearttank_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_hearttank_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Other_13(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_door_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_doublemissilestack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_doublemissilestack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_antibullet_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_antibullet_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_antibullet_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_engineer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_engineer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_healing_turret_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_healing_turret_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_healing_turret_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_laserline_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_laserline_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_laserline_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_laserline_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_mainmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_mainmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_mainmenu_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_support_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_support_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drone_player_support_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drones_shadow_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_drones_shadow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dust_particle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dust_particle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_dynamic_collision_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_laststand_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_laststand_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_laststand_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_slowcircle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_slowcircle_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_slowcircle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_slowcircle_spark_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_slowcircle_spark_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_effect_slowcircle_spark_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_electroblock_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_electroblock_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_electrohouse_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_electrohouse_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_electrohouse_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_electrohouse_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemy_ctrl_Alarm_7(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemy_ctrl_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemy_ctrl_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemy_ctrl_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemy_ctrl_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemy_ctrl_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemyteleport_spot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemyteleport_spot_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enemyteleport_spot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_MELEE_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_MELEE_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_POI_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_POI_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_POI_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_plasma_boss_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_plasma_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_plasma_boss_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_plasma_boss_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_plasma_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_riotshield_boss_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_riotshield_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_riotshield_boss_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_riotshield_boss_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_riotshield_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_sniper_boss_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_sniper_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_sniper_boss_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_sniper_boss_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_RG_sniper_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_alarm_o_bot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_assassin_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_assassin_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_assassin_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_assassin_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballbot_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballbot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballbot_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballbot_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballbot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small2_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small2_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_balldefender_red_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballmedic_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballmedic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballmedic_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballmedic_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballmedic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballretract_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballretract_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_ballretract_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Alarm_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_13(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_14(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_20(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_23(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_24(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_25(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_base_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_minus5_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_minus5_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_plus10_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_plus10_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_plus5_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_budget_plus5_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_chrono_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_chrono_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_chrono_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_chrono_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_chrono_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystal_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystal_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_laser_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_laser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_laser_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_laser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_mortar_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_mortar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_mortar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_crystalturret_mortar_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_custom_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_custom_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_custom_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_custom_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_destroyed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_destroyed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_dissolve_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_dissolve_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_dissolve_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_dissolve_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_doomba_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_doomba_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_doomba_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_doomba_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_doomba_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_doomba_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mg_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_missile_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_missile_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_missile_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_missile_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mortar_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mortar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mortar_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_mortar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_particleball_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_particleball_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_particleball_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_shield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_shield_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_shield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_drone_shield_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_dronemissile_x57_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_dronemissile_x57_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_elite_sg_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_elite_sg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_elite_sg_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_elite_sg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_enforcer_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_enforcer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_enforcer_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_enforcer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_engineer_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_engineer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_engineer_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_engineer_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_engineer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_engineer_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flaktank_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flaktank_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flaktank_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flaktank_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamer_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamer_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamethrower_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamethrower_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_flamethrower_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidergatling_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidergatling_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidergatling_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidergatling_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderjet_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderjet_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderjet_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderrailgun_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderrailgun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderrailgun_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderrailgun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidershotgun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidershotgun_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_glidershotgun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderstriker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderstriker_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_gliderstriker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_arena_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_arena_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_quest_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_quest_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_headhunter_quest_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_Other_20(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_Other_24(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hover_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hovertank_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hovertank_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hovertank_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hovertank_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Other_20(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Other_24(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_humanoid_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hypertrooper_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hypertrooper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hypertrooper_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_hypertrooper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mainbattletank_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mainbattletank_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mainbattletank_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mainbattletank_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mg_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mg_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_mg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minisentry_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minisentry_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minisentry_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minitank_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minitank_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minitank_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minitank_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_minitank_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_overseer_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_overseer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_overseer_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_overseer_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_overseer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_parent_all_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_phaser_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_phaser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_phaser_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_phaser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_phaser_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_railgunner_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_railgunner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_railgunner_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_railgunner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_plasma_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_plasma_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_plasma_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_plasma_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_riotshield_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_riotshield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_riotshield_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_riotshield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_sniper_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_sniper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_sniper_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_redguard_sniper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_riotshield_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_riotshield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_riotshield_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_riotshield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_room_spawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_room_spawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_room_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sawbladebot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sawbladebot_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sawbladebot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sawbladebot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentrysniper_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentrysniper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentrysniper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentryturret_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentryturret_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentryturret_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sentryturret_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Other_14(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_Other_20(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_black_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_black_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_black_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_sniper_black_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spawn_point_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_disco_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_disco_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_disco_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_disco_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_disco_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_explo_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_explo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_explo_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_explo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_explo_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_rocket_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_rocket_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_rocket_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_rocket_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_spider_rocket_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Other_14(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Other_23(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_Other_24(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_tank_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_threadedshotgun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_threadedshotgun_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_threadedshotgun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_treasureroomba_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_MOBA_Base_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_healing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_laser_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_laser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_laser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_mortar_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_mortar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_mortar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_mortar_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Alarm_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_13(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_14(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_20(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_23(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Other_24(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_point_defense_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_radar_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_radar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_radar_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_radar_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_radar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_radar_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_rocket_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_rocket_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_rocket_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_rocket_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_sniper_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_sniper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_sniper_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_sniper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_vierling_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_vierling_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_vierling_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_turret_vierling_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_workerbot_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_workerbot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_workerbot_MOBA_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_workerbot_MOBA_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_workerbot_MOBA_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_workerbot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_worldspaceui_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_enm_worldspaceui_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_escapesettings_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_escapesettings_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_escapesettings_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_escapesettings_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_euro_sandbags_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_euro_sandbags_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_gain_ring_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_gain_ring_Draw_72(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_gain_ring_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death2_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_exp_particle_death_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explo_neutrino_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explo_neutrino_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explo_neutrino_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explo_neutrino_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_akira_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_akira_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_akira_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_akira_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_antishield_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_antishield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_antishield_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_antishield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_heartsmall_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_heartsmall_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_blue_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_blue_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_eclipse_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_eclipse_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_purple_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_purple_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_red_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_item_red_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_beam_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_crystal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_crystal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_crystal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_crystal_weak_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_crystal_weak_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_crystal_weak_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_drone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_drone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_drone_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_fire_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_fire_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_fire_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_fire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_fire_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flak_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flak_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flak_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flak_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flare_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flare_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flare_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flash_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flash_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flash_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_flash_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_frag_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_frag_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_laserspike_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_laserspike_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_laserspike_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_laserspike_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_plasma_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_plasma_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_plasma_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_player_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_player_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_player_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_poison_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_poison_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_poison_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_poison_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_poison_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_psycloud_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_psycloud_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_psycloud_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_red_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_red_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_red_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_reso_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_reso_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_reso_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_resowave_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_resowave_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_resowave_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_roadflare_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_roadflare_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_roadflare_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_simple_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_simple_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_simple_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_smoke_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_smoke_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_smoke_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_sphere_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_sphere_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_sphere_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_colorgrading_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_colorgrading_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_explosion_stun_colorgrading_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_1x1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_1x1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_1x2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_1x2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_1x3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_1x3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_2x1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_2x1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_2x2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_2x2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_2x3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_2x3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_3x1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_3x1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_3x2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_3x2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_3x3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_3x3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_4x4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_4x4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_bossHelicopter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_bossHelicopter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceDummyPath_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceDummyPath_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceField_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceField_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceFree_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceFree_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceInvisiblePath_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceInvisiblePath_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forcePath_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forcePath_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceSpecialPath_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceSpecialPath_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_forceSpecial_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_godShrine_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_godShrine_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_node_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_node_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_teleporter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_teleporter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_teleporter_end_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_field_teleporter_start_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_fields_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_fire_tiny_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_fire_tiny_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_fire_tiny_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_fire_tiny_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_fire_tiny_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_armored_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_armored_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_Other_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_Other_25(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_plating_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_plating_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_shield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_shield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_standard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_dummy_standard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_spawner_auto_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_spawner_auto_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_firingrange_spawner_auto_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flamethrower_particle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flamethrower_particle_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flamethrower_particle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flamethrower_particle_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flamethrower_particle_ball_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flamethrower_particle_ball_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flyingcrystals_tiny_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_flyingcrystals_tiny_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_cube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_high_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_high_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_alu_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_copper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_copper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_turbine2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_turbine2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_turbine_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generator_turbine_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generatoralu_battery_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generatoralu_battery_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generatoralu_tube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generatoralu_tube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_generatoralu_tube_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_globe_statue_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_globe_statue_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_glow1_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_glow1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_plasma_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_plasma_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_marker_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_primer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_primer_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_grenade_primer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ground_transformator_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ground_transformator_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ground_vrm_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ground_vrm_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_groundeffect_crystal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_groundeffect_crystal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_groundeffect_crystal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hazards_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hazards_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_headshot_collision_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_headshot_collision_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_headshot_collision_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_headshot_collision_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_healing_circle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_healing_circle_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_healing_circle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_electropole_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_electropole_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_armageddon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_armageddon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_armageddon_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_loop_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_loop_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_loop_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_kill_loop_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_reactor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_reactor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_room_events_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_room_events_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_heart_room_events_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hearthack_station_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hearthack_station_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hearthack_station_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hearthack_station_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hearthack_station_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_cylinder_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_cylinder_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_dynTest_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_dynTest_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_rocket1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_rocket1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_rocket2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_rocket2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_rocketStack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_rocketStack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_tankShellStack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_tankShellStack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_tankShell_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helicopterBoss_tankShell_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helpmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_helpmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hit_circle_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hit_circle_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hit_circle_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hit_decal_glow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hit_decal_glow_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_hit_decal_glow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_house_a0_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_house_a0_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_house_a1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_house_a1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_dust_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_dust_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_dust_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_dust_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_spark_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_spark_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_spark_effect_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_sparks_cracker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_sparks_cracker_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_impact_sparks_cracker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ingame_ctrl_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item100_breachingcharge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item100_breachingcharge_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item100_breachingcharge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item100_breachingcharge_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item101_resonator_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item101_resonator_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item101_resonator_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item102_airhorn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item102_airhorn_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item102_airhorn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item102_airhorn_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item103_brawndo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item103_brawndo_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item103_brawndo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item103_brawndo_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item104_eclipse_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item104_eclipse_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item104_eclipse_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item107_ministim_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item107_ministim_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item107_ministim_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item107_ministim_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item108_toolbox_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item108_toolbox_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item108_toolbox_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item108_toolbox_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item109_reloader2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item109_reloader2_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item109_reloader2_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item109_reloader2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item109_reloader2_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item10_overdrive_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item10_overdrive_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item10_overdrive_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item10_overdrive_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item110_spider_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item110_spider_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item110_spider_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item111_heatsink_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item111_heatsink_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item111_heatsink_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item111_heatsink_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item112_tptp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item112_tptp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item112_tptp_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item113_button_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item113_button_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item113_button_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item113_button_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item114_fireaura_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item114_fireaura_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item114_fireaura_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item114_fireaura_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item115_decoy_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item115_decoy_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item115_decoy_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item116_decoyshield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item116_decoyshield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item117_gps_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item117_gps_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item117_gps_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item117_gps_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item118_goldnugget_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item118_goldnugget_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item118_goldnugget_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item118_goldnugget_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item119_upgradekit2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item119_upgradekit2_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item119_upgradekit2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item119_upgradekit2_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item11_revive_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item11_revive_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item11_revive_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item11_revive_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item120_detector_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item120_detector_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item120_detector_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item120_detector_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item121_tomahawk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item121_tomahawk_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item121_tomahawk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item121_tomahawk_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item122_targetcpu_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item122_targetcpu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item122_targetcpu_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item122_targetcpu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item122_targetcpu_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item123_cover_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item123_cover_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item124_gunner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item124_gunner_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item124_gunner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item124_gunner_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item125_powerpack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item125_powerpack_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item125_powerpack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item125_powerpack_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item126_missiledrone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item126_missiledrone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item126_missiledrone_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item127_building_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item127_building_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item127_building_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item127_building_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item128_remover_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item128_remover_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item128_remover_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item128_remover_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item129_research_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item129_research_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item129_research_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item129_research_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item12_reflector_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item12_reflector_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item12_reflector_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item12_reflector_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item130_riotguard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item130_riotguard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item130_riotguard_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item131_command_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item131_command_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item131_command_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item132_dynamite_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item132_dynamite_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item132_dynamite_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item133_magnum2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item133_magnum2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item133_magnum_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item133_magnum_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item133_magnum_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item136_missilecontrol_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item136_missilecontrol_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item136_missilecontrol_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item136_missilecontrol_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item13_devilsdice_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item13_devilsdice_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item13_devilsdice_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item13_devilsdice_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item140_shaker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item140_shaker_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item140_shaker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item140_shaker_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item143_itemlink_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item143_itemlink_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item143_itemlink_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item143_itemlink_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item147_armageddon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item147_armageddon_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item147_armageddon_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item147_armageddon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item147_armageddon_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item148_fusionorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item148_fusionorb_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item148_fusionorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item148_fusionorb_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item149_lasercaster_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item149_lasercaster_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item149_lasercaster_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item14_goggles_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item14_goggles_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item14_goggles_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item14_goggles_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item15_shield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item15_shield_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item15_shield_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item15_shield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item15_shield_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item16_upgradekit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item16_upgradekit_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item16_upgradekit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item16_upgradekit_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item17_ironorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item17_ironorb_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item17_ironorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item17_ironorb_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item18_potion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item18_potion_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item18_potion_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item18_potion_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item18_potion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item18_potion_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item19_lightningorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item19_lightningorb_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item19_lightningorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item19_lightningorb_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item1_chaospotion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item1_chaospotion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item1_chaospotion_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item20_fireorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item20_fireorb_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item20_fireorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item20_fireorb_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item21_orbwind_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item21_orbwind_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item21_orbwind_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item21_orbwind_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item22_bloodrite_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item22_bloodrite_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item22_bloodrite_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item22_bloodrite_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item23_extrashot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item23_extrashot_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item23_extrashot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item23_extrashot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item24_heart_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item24_heart_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item24_heart_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item24_heart_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item25_refresher_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item25_refresher_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item25_refresher_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item25_refresher_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item26_statskit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item26_statskit_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item26_statskit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item26_statskit_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item27_berserk_Alarm_9(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item27_berserk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item27_berserk_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item27_berserk_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item27_berserk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item27_berserk_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item28_dronelaser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item28_dronelaser_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item28_dronelaser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item28_dronelaser_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item29_slowmo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item29_slowmo_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item29_slowmo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item29_slowmo_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item2_hyperfeed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item2_hyperfeed_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item2_hyperfeed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item2_hyperfeed_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item30_methadone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item30_methadone_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item30_methadone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item30_methadone_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item31_vampirism_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item31_vampirism_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item31_vampirism_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item31_vampirism_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item32_tsunami_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item32_tsunami_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item32_tsunami_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item32_tsunami_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item33_shieldburst_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item33_shieldburst_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item33_shieldburst_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item33_shieldburst_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item34_grenade_he_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item34_grenade_he_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item34_grenade_he_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item35_grenade_flash_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item35_grenade_flash_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item35_grenade_flash_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item36_grenade_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item36_grenade_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item36_grenade_stun_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item37_grenade_plasma_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item37_grenade_plasma_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item37_grenade_plasma_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item39_slowfield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item39_slowfield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item39_slowfield_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item3_splash_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item3_splash_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item3_splash_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item40_steeltrap_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item40_steeltrap_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item40_steeltrap_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item41_powershot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item41_powershot_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item41_powershot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item41_powershot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item42_plating_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item42_plating_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item42_plating_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item42_plating_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item43_switcher_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item43_switcher_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item43_switcher_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item43_switcher_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item44_exploheadshot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item44_exploheadshot_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item44_exploheadshot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item44_exploheadshot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item45_uranium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item45_uranium_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item45_uranium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item45_uranium_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item46_missile_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item46_missile_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item46_missile_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item46_missile_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item47_missilehoming_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item47_missilehoming_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item47_missilehoming_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item47_missilehoming_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item48_laser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item48_laser_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item48_laser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item48_laser_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item49_mindcontrol_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item49_mindcontrol_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item49_mindcontrol_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item49_mindcontrol_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item4_cloak_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item4_cloak_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item4_cloak_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item50_laststand_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item50_laststand_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item50_laststand_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item50_laststand_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item51_stinger_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item51_stinger_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item51_stinger_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item52_bombardment_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item52_bombardment_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item52_bombardment_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item53_chargeshot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item53_chargeshot_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item53_chargeshot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item53_chargeshot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item54_c4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item54_c4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item54_c4_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item55_stim_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item55_stim_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item55_stim_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item55_stim_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item55_stim_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item55_stim_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item56_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item56_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item56_spawner_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item57_gundrone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item57_gundrone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item57_gundrone_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item58_seekers_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item58_seekers_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item58_seekers_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item58_seekers_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item59_noobtube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item59_noobtube_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item59_noobtube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item59_noobtube_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item5_crystal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item5_crystal_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item5_crystal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item5_crystal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item5_crystal_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item60_aimbot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item60_aimbot_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item60_aimbot_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item60_aimbot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item60_aimbot_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item61_rosarius_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item61_rosarius_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item61_rosarius_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item61_rosarius_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item61_rosarius_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item62_colibri_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item62_colibri_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item62_colibri_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item62_colibri_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item63_cellreplacer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item63_cellreplacer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item63_cellreplacer_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item64_chalice_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item64_chalice_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item64_chalice_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item65_stunbolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item65_stunbolt_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item65_stunbolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item65_stunbolt_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item66_akira_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item66_akira_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item66_akira_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item66_akira_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item67_boots_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item67_boots_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item67_boots_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item67_boots_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item68_ring_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item68_ring_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item68_ring_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item68_ring_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item69_taser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item69_taser_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item69_taser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item69_taser_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item6_reloader_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item6_reloader_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item6_reloader_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item6_reloader_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item70_bloodbolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item70_bloodbolt_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item70_bloodbolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item70_bloodbolt_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item71_sentry_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item71_sentry_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item71_sentry_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item71_sentry_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item72_snipersentry_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item72_snipersentry_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item72_snipersentry_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item73_sentryfuture_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item73_sentryfuture_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item73_sentryfuture_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item74_perkkit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item74_perkkit_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item74_perkkit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item74_perkkit_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item75_glass_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item75_glass_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item75_glass_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item75_glass_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item76_bloodthirst_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item76_bloodthirst_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item76_bloodthirst_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item76_bloodthirst_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item77_chip_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item77_chip_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item77_chip_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item77_chip_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item78_sawblade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item78_sawblade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item78_sawblade_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item79_bolt_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item79_bolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item79_bolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item79_bolt_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item7_vial_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item7_vial_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item7_vial_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item80_masterkey_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item80_masterkey_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item80_masterkey_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item80_masterkey_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item81_tanto_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item81_tanto_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item81_tanto_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item82_flare_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item82_flare_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item82_flare_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item82_flare_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item83_grenade_smoke_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item83_grenade_smoke_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item83_grenade_smoke_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item84_flechette_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item84_flechette_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item84_flechette_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item84_flechette_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item85_kunai_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item85_kunai_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item85_kunai_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item85_kunai_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item86_commandoflare_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item86_commandoflare_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item86_commandoflare_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item87_beamblue_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item87_beamblue_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item87_beamblue_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item87_beamblue_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item88_beampurple_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item88_beampurple_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item88_beampurple_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item89_beamred_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item89_beamred_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item89_beamred_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item8_injection_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item8_injection_Other_19(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item8_injection_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item8_injection_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item8_injection_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item90_shuriken1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item90_shuriken1_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item90_shuriken1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item90_shuriken1_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item91_shuriken2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item91_shuriken2_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item91_shuriken2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item91_shuriken2_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item92_fanknives_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item92_fanknives_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item92_fanknives_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item92_fanknives_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item93_flyingknife_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item93_flyingknife_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item93_flyingknife_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item93_flyingknife_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item94_dagger_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item94_dagger_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item94_dagger_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item95_livingbomb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item95_livingbomb_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item95_livingbomb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item95_livingbomb_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item96_stickybomb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item96_stickybomb_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item96_stickybomb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item96_stickybomb_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item97_minelaser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item97_minelaser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item97_minelaser_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item98_minisentry_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item98_minisentry_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item98_minisentry_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item98_minisentry_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item99_battlecry_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item99_battlecry_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item99_battlecry_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item99_battlecry_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item9_fang_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item9_fang_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item9_fang_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item9_fang_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_drop_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_drop_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_drop_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_empty_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_empty_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_KeyPress_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Other_14(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_slot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_slot_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_slot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_item_slot_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_itemstats_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_itemstats_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_itemstats_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_itemstats_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_marker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_marker_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_marker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_marker_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_splat_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_splat_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_kill_splat_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lastkill_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lastkill_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lastkill_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_background_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_background_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_background_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_newsletter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_newsletter_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_newsletter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_newsletter_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_overlay_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_overlay_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_overlay_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_overlay_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_showreel_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_showreel_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_showreel_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_launcher_showreel_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_levelManager_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_levelManager_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_levelManager_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_levelManager_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_levelManager_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_alarm_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_alarm_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_alarm_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_alarm_ctrl_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_alarm_ctrl_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_alarm_ctrl_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_arealight_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_arealight_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_arealight_boss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_arealight_boss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_arealight_huge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_arealight_huge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_firing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_firing_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_firing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_firing_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_flickering_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_flickering_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_flickering_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_heart0_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_heart0_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_heart1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_heart1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_heart2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_heart2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_hit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_hit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_mainmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_mainmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_mainmenu_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_mainmenu_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightemitter_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_hit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_hit_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_hit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_strike_divine_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_strike_divine_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_strike_divine_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_strike_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_strike_stun_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_lightning_strike_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_line_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_line_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_line_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_loca_button_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_loca_button_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_loca_button_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_localPlayer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_localPlayer_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_localPlayer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_localPlayer_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_logmenu_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_machine_ground_A_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_machine_ground_A_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_machine_ground_A_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_main_ui_S1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_main_ui_S1_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_main_ui_S1_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_main_ui_S1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mainmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mainmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_enemyradio_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_enemyradio_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_enemyradio_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_generic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_generic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_help_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_help_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_object_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_marker_object_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particle_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particle_Draw_64(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particle_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particleemitter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particleemitter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_menu_particleemitter_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_metal_decal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_metal_decal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_metal_decal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_militarycrate_cube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_militarycrate_cube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_militarycrate_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_militarycrate_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_militarycrate_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_militarycrate_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilecurve_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilecurve_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilelarge_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilelarge_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepart_high_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepart_high_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepart_low_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepart_low_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepartred_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepartred_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepartred_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepartred_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepod_large_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilepod_large_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilesilo_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilesilo_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilesilo_quad_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_missilesilo_quad_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_acc_input_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_acc_input_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_cityscreen_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_cityscreen_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_cityscreen_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_cityscreen_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_classes_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_classes_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_classes_menu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_classes_menu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_classes_menu_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_classes_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_community_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_community_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_community_menu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_community_menu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_community_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_community_menu_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_creditsdisplay_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_creditsdisplay_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_creditsdisplay_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_creditsdisplay_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_difficultymenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_difficultymenu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_difficultymenu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_difficultymenu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_difficultymenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_glow_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_glow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_itemselect_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_itemselect_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_itemselect_menu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_itemselect_menu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_itemselect_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_menu_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_menu_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_menu_parent_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_menu_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_menu_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_menu_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_multiplayer_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_multiplayer_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_multiplayer_menu_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_multiplayer_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_readylobby_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_research_trigger_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_research_trigger_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_startgamedecal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_statistics_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_statistics_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_statistics_menu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_statistics_menu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_statistics_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_switchtalent_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_switchtalent_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_switchtalent_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_talentselection_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_talentselection_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_talentselection_menu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_talentselection_menu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_talentselection_menu_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_talentselection_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_unlocks_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_unlocks_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_unlocks_menu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_unlocks_menu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_unlocks_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_weaponmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_weaponmenu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_weaponmenu_Mouse_60(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_weaponmenu_Mouse_61(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mm_weaponmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_ballistic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_ballistic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_drone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_drone_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_shadow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_shadow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mortarshot_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_moving_entity_PARENT_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_moving_entity_PARENT_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_moving_entity_PARENT_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_moving_entity_PARENT_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_moving_entity_PARENT_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_moving_entity_PARENT_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_multiplayer_marker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_multiplayer_marker_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_multiplayer_marker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_music_track_loop_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_music_track_loop_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_music_track_loop_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_music_track_loop_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_1_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_1_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_laser_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_laser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_laser_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_muzzleflash_laser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_instagib_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_instagib_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_instagib_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_instagib_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_item_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_item_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_item_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_madness_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_madness_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_madness_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_perk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_perk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_perk_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_randomrun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_randomrun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_randomrun_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_reward_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_reward_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_reward_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mystery_reward_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mysterybonus_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mysterybonus_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_mysterybonus_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nem_budget_minus10_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nem_budget_minus10_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Other_13(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Other_25(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Other_68(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_MS_client_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_chat_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_global_chat_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_global_chat_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_global_chat_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_global_chat_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_global_chat_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_station_spawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_station_spawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_net_station_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_client_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_client_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_client_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_client_Other_68(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_client_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_client_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_Other_68(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_networking_server_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nonbombat_vacuum_randomMove_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nonbombat_vacuum_randomMove_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nonbombat_vacuum_randomMove_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nonbombat_vacuum_randomMove_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_drone_randomMove_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_drone_randomMove_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_drone_randomMove_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_drone_randomMove_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_drone_randomMove_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_ground_Parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_propsHover_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_propsHover_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_propsHover_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_noncombat_target_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_notification_circle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_notification_circle_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_notification_circle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nowall_manual_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_nowall_manual_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_null_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_null_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oil_pool_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oil_pool_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oil_pool_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oil_pool_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oil_pool_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oilconv_low_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oilconv_low_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_ball_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_ball_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_high_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_high_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_long_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_long_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_long_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_long_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_low_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_low_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_pipes_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_oiltank_pipes_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_class_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_class_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_class_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_fire_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_fire_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_fire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_fusion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_fusion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_iron_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_iron_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_iron_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_lightning_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_lightning_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_lightning_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_parent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_ripper_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_ripper_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_ripper_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_wind_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_wind_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_orb_wind_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overclock_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overclock_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overclock_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_aboveui_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_aboveui_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_aboveui_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_aboveui_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_aboveui_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_belowui_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_belowui_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_overlays_belowui_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_base_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_base_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_base_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_base_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_base_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_base_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_laser_sprinkles_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_laser_sprinkles_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particle_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_chunky_metal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_chunky_metal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_chunky_metal_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_chunky_metal_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_chunky_wood_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_chunky_wood_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_concrete_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_concrete_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_dirt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_explo_dirt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_concrete_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_concrete_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_concrete_chunky_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_concrete_chunky_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_metal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_metal_Keyboard_32(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_metal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_oil_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_oil_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_sparks_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_sparks_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_wood_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_wood_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_wood_chunky2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_wood_chunky2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_wood_chunky_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_impact_wood_chunky_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_laserline_spark_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_laserline_spark_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_laserline_spark_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_oilpools_flying_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_oilpools_flying_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_oilpools_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_oilpools_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_shell_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_shell_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_particles_shell_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_bot_right_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_bot_right_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_horizontal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_horizontal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_left_bot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_left_bot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_right_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_top_left_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_top_left_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_vertical_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_path_vertical_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pathfinding_allowed_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pathfinding_allowed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pathfinding_allowed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pausemenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pausemenu_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pausemenu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pausemenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pausemenu_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_aegis_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_aegis_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_aegis_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_aegis_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_assaultgunner_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_assaultgunner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_assaultgunner_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_assaultgunner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_assaultgunner_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_assaultgunner_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_bloodinfusion_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_bloodinfusion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_bloodinfusion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_bloodinfusion_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_breacher_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_classweapon_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_classweapon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_classweapon_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_classweapon_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_classweapon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_classweapon_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_closer_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_closer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_closer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_closer_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_commando_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_commando_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_commando_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_commando_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_commando_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_commando_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_cover_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_cover_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_cover_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_cover_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dance_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dance_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dance_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dance_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_discipline_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_discipline_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_discipline_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dodgeheat_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dodgeheat_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dodgeheat_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_dodgeheat_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_drone1_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_drone1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_drone1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_edge_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_edge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_edge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_edge_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_ejectsurge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_ejectsurge_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_engineer_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_fieldration_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_fieldration_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_force_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_force_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_fortify_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_fortify_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_fortify_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_heatrecharge_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_heatrecharge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_heatrecharge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_heatrecharge_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_iframe_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_iframe_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_iframe_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_iframe_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_itemcdvariant_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_itemcdvariant_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_itemcdvariant_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_itemcdvariant_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_killer_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_killer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_killer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_killshield_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_killshield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_killshield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_parent_Alarm_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_parent_Alarm_9(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_parent_Draw_72(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_parent_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_perfection_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_perfection_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_pistolextender_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_pistolextender_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_pistolextender_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powerstep_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powerstep_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powerstep_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powerstep_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powertuning_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powertuning_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powertuning_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_powertuning_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_randomperk_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_randomperk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reactivereload_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reactivereload_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reactivereload_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reactivereload_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadstack_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadstack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadstack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadstack_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge2_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge2_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_reloadsurge_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_riotguard_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_riotguard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_riotguard_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_riotguard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_riotguard_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_riotguard_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scarred_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scarred_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scarred_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scarred_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scavengerbits_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scavengerbits_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scavengerbits_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_scavengerbits_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_shotgunmaster_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_shotgunmaster_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_specializedammo_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_specializedammo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_specializedammo_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_specializedammo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_statusextender_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_statusextender_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_statusextender_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_stealback_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_stealback_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_stealback_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_stealback_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_transmutate_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_transmutate_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_transmutate_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_wepupgrade_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_wepupgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_wepupgrade_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_perk_wepupgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_ammopack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_ammopack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_ammoreg_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_ammoreg_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_arenatoken_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_armageddon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_armageddon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_artefact_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_artefact_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_bolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_bolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_chip_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_credits_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_credits_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_damage_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_damage_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_data_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_data_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_doubleexp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_doubleexp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_heartcore_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_heartcore_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_shared_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_shared_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_shared_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_shared_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_item_shared_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_keycard_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_keycard_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_medikit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_medikit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_outgame_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_Alarm_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_perkkit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_perkkit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_quest_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_quest_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_ring_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_ring_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_scrap_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_scrap_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_shield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_shield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_speed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_speed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_spirit_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_spirit_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_spirit_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_upgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_upgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_vial_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_vial_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_weppart_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pickup_weppart_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pillar_platform_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pillar_platform_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pillar_platform_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pillar_platform_below_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pillar_platform_below_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipe_center_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipe_center_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipehigh_center_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipehigh_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipehigh_leftend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipehigh_leftend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_botend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_botend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_center_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_leftend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_leftend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_rightend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_rightend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_topend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_topend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_vertcenter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipelow_vertcenter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipes_large_stack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipes_large_stack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipes_large_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipes_large_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipes_valveprop_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipes_valveprop_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipeshigh_rightend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pipeshigh_rightend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_placeroom_manualfloor_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_placeroom_manualfloor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_placeroom_manualfloor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_placeroom_movablespace_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_placeroom_movablespace_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_plating_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_plating_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_plating_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_KeyPress_78(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Other_25(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_bullets_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_mainmenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_mainmenu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_mainmenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_mainmenu_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_spawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_spawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_player_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_plywoodstack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_plywoodstack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_spawnpos_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_pointdefense_station_armageddon_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_area_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_area_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_area_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_pool_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_pool_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_pool_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_pool_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_poison_pool_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_announcement_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_announcement_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_announcement_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_exp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_exp_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_exp_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_exp_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_exp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_skillduration_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_skillduration_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_popup_skillduration_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_primer_he_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_primer_he_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_primer_plasma_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_primer_plasma_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_primer_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_primer_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_prop_destroyed_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_prop_destroyed_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_prop_destroyed_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_destructible_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_destructible_Alarm_9(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_destructible_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_destructible_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_destructible_Other_25(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_destructible_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_notsolid_parent_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_notsolid_parent_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_notsolid_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_notsolid_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_solid_parent_Alarm_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_solid_parent_Alarm_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_solid_parent_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_solid_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_props_solid_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quadstackrocket_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quadstackrocket_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quest_headhunter_spawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quest_headhunter_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quest_hearthack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quest_hearthack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quest_hearthack_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_alert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_alert_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_alert_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_alert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_defense_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_defense_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_defense_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_defuseammo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_hack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_hack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_headhunters_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_headhunters_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_nightvision_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_nightvision_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_valve_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questcontrol_valve_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questhack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questhack_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questhack_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questhack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questhack_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_generic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_parent_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_parent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_slots_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_slots_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questlog_slots_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quests_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quests_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quests_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quests_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_quests_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questvalve_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questvalve_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questvalve_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questvalve_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_questvalve_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_radar_cube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_radar_cube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_radar_cube_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_bot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_botleft_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_botright_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_corner_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_corner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_end_top_R_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_end_top_R_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_end_vert_bot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_end_vert_bot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_end_vert_top_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_end_vert_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_inner_botleft_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_inner_botright_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_inner_topleft_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_inner_topright_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_left_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_Alarm_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_right_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_straights_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_straights_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_topleft_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_railing_topright_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_random_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_random_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectcable_stack_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectcable_stack_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectpipe_stack2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectpipe_stack2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectpipe_stack2_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectpipe_stack2_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectpipe_stack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rectpipe_stack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_remotePlayer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_remotePlayer_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_remotePlayer_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_remotePlayer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_remotePlayer_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketcover_hor0_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketcover_hor0_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketfuel_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketfuel_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketfuel_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketfuel_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor0_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor0_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_hor4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_ver0_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_ver0_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_ver1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_ver1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_ver2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_big_ver2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstack_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstackred_big_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstackred_big_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstackred_big_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_rocketstackred_big_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_roll_classitem_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_roll_classitem_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_roll_classitem_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Alarm_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_KeyPress_122(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_room_ctrl_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_circle_bot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_circle_bot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_circle_top_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_circle_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_vert_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sandbags_vert_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sculpt_quad_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sculpt_quad_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sculpt_rect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sculpt_rect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_ammo_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_ammo_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_ammo_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_ammo_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_artefact_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_artefact_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_artefact_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_artefact_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_perk_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_perk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_perk_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_perk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_stat_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_stat_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_stat_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_selection_menu_stat_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_servermessage_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_servermessage_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_set_lobby_config_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_set_lobby_config_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_set_lobby_config_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_set_lobby_config_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_set_lobby_config_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shellscylinders_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shellscylinders_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_effect_jet_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_effect_jet_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_particle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_particle_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_particle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_explode_particle_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_gone_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_gone_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shield_gone_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shieldboost_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shieldboost_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shieldboost_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shielddrone_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shielddrone_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shielddrone_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_2x2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_2x2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_god_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_god_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_god_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_god_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_item_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_item_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_itemupgr_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_itemupgr_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_synergy_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_synergy_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_teleport_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_teleport_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_teleport_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_weaponbuy_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_weaponbuy_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_weaponbuy_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_weaponupgrade_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_weaponupgrade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_spawn_weaponupgrade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_KeyPress_32(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_shrine_tooltip_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_mastervol_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_mastervol_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_mastervol_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_musicvol_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_musicvol_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_slider_input_musicvol_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_explo_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_explo_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_explo_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_firing_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_firing_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_firing_effect_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_firing_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_impact_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_impact_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_smoke_parent_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sonar_ping_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sonar_ping_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_sonar_ping_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_spark_explosion_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_spark_explosion_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_spark_explosion_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_spawnnextroom_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_spawnnextroom_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_spawnnextroom_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_special_2x2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_special_2x2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_special_3x3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_special_3x3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_specialcodewarning_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_specialobject_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_crate_small_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_crate_small_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_crates_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_crates_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_spawner_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_spawner_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_spawner_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stack_spawner_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stackelecto_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stackelecto_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stackelectro_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stackelectro_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_Keyboard_32(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_start_game_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_spawner_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_trigger_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_trigger_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_trigger_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_trigger_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_trigger_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_station_trigger_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_Other_72(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_statistics_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stats_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stats_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stats_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stats_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stats_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_stats_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_particle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_particle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_particle_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_particle_spark_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_particle_spark_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_healing_particle_spark_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_slow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_slow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_slow_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_slow_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_slow_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_slow_effect_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_stun_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_toxicblob_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_toxicblob_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_status_toxicblob_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_steam_init_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_steam_init_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_steeltrap_effect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_steeltrap_effect_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_steeltrap_effect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_spawner_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_spawner_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_spawner_support_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_ghosting_spawner_support_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_supporter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_supporter_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_strafe_supporter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_KeyPress_51(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_KeyPress_76(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_surface_light_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_talent_selection_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_talent_selection_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tankshells_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tankshells_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tanto_swing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tanto_swing_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tanto_swing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrier_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrier_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrier_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrier_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrier_rect_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrier_rect_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrierstack_big_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrierstack_big_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrierstack_big_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrierstack_big_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrierstack_big_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tcarrierstack_big_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_circle_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_circle_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_teleporter_circle_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_terrorlevel_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_terrorlevel_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_terrorlevel_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_terrorlevel_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_text_input_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tirejump_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tirejump_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tirestack_low_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tirestack_low_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tirestack_medium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tirestack_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_damagetext_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_damagetext_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_damagetext_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_damagetext_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_hp_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_hp_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_hp_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_hp_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_hp_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_Alarm_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltip_terrorlevel_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_Draw_73(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_weapons_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_weapons_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_weapons_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tooltipbig_weapons_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tower_missile0_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tower_missile0_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tower_missile1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tower_missile1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tower_radar_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_tower_radar_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_towerprogress_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_towerprogress_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_towerprogress_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_bloodbolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_bloodbolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_bloodbolt_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_crazyrocket_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_crazyrocket_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_crazyrocket_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_dark_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_dark_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_meddark_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_meddark_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_spiky_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_rocket_spiky_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_seeker_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trail_seeker_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trainingcontainer_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trainingcontainer_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trainingcontainer_ver_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trainingcontainer_ver_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_12tube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_12tube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_6tube_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_6tube_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_battery_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_battery_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_dualtower_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_dualtower_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_high_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_high_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_medium_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_medium_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_stack_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_stack_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_transformator_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_transformator_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_tub_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_transistor_tub_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_barbedwire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_barbedwire_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_chainmine_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_chainmine_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_chainmine_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_electrofield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_electrofield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_firespewer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_firespewer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_antishield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_antishield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stealthparent_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stealthparent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stealthparent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stealthparent_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stealthparent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stealthparent_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_stun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_toxic_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_mine_toxic_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minelayer_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minelayer_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minelayer_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minimine_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minimine_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minimine_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_minimine_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_slow_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_slow_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_slow_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_slowfield_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_slowfield_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_slowfield_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_spike_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_spike_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trap_spike_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trashcontainer_horizontal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trashcontainer_horizontal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trashcontainer_vertical_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_trashcontainer_vertical_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_boss_spawn_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_boss_spawn_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_boss_spawn_random_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_boss_spawn_random_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_spwn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_spwn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_spwn_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turret_spwn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_turretshrinespawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_upgrade_station_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_upgrade_station_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_upgrademenu_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_upgrademenu_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_vehiclespawn_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_vehiclespawn_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_vehiclespawn_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilation_floor1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilation_floor1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilation_floor1_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilation_floor4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilation_floor4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilationground_large_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilationground_large_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_ventilationground_large_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_ballbot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_ballbot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_heart_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_heart_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_parent_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_spiderboss_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_voidzone_spiderboss_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_barrier_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_barrier_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_barrier_vert_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_barrier_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_conc_vert_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_fence_gate_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_fence_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_fence_vert_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_heliBoss_vert_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_hor_center_Alarm_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_hor_center_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_hor_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_leftend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_leftend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_fat1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_fat1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_fat2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_fat2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_hor2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_hor_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_hor_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_vert_center_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_metal_vert_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_rightend_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_rightend_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_test_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_test_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_top_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_vert_bot_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_vert_bot_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_vert_center_Alarm_9(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_vert_center_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_wall_vert_center_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_12G_19_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_12G_19_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_12G_19_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_12G_19_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_4G_16_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_4G_16_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_4G_16_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_4G_16_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A5C_81_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A5C_81_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A5C_81_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A5C_81_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A5C_81_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A9_55_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A9_55_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A9_55_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_A9_55_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AA_43_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AA_43_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AA_43_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AA_43_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AB_56_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AB_56_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AB_56_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ACR_58_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ACR_58_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ACR_58_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ACR_58_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AEK_41_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AEK_41_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AEK_41_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AEK_41_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AKS_74_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AKS_74_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AKS_74_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AKS_74_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AMD_24_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AMD_24_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AMD_24_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AMD_24_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AN94_82_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AN94_82_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AN94_82_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AN94_82_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ANH_66_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ANH_66_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ANH_66_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ANH_66_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AR15_80_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AR15_80_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AR15_80_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_AR15_80_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BFR_7_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BFR_7_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BFR_7_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BFR_7_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BOW_53_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BOW_53_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BOW_53_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BOW_53_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BRN_50_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BRN_50_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BRN_50_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_BRN_50_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CAR_27_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CAR_27_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CAR_27_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CAR_27_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CG_37_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CG_37_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CG_37_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CG_37_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CG_37_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CG_37_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CLG_8_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CLG_8_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CLG_8_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CLG_8_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CLG_8_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Alarm_9(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CL_2_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CP_38_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CP_38_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CP_38_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CP_38_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CRGE_29_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CRGE_29_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CRGE_29_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_CRGE_29_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DB_4_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DB_4_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DB_4_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DB_4_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_61_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_61_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_61_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_61_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_94_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_94_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_94_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DE_94_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMN_28_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMN_28_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMN_28_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMR_12_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMR_12_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMR_12_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DMR_12_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DS_39_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DS_39_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DS_39_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DS_39_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DS_39_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_DS_39_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_Damnation_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_Damnation_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_EXP_51_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_EXP_51_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FB_52_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FB_52_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FB_52_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FB_52_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FC_36_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FC_36_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FC_36_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FC_36_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_14_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_14_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_14_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_14_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_92_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_92_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_92_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FMG_92_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FT_35_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FT_35_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FT_35_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_FT_35_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_G17_63_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_G17_63_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_G17_63_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_G17_63_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GAT_72_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GAT_72_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GAT_72_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GAT_72_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GL_78_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GL_78_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GL_78_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GL_78_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GM6_20_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GM6_20_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GM6_20_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_GM6_20_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HLP_64_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HLP_64_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HLP_64_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HLP_64_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HMG_47_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HMG_47_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HMG_47_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HMG_47_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HON_34_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HON_34_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HON_34_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_HON_34_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_IMP_71_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_IMP_71_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_IMP_71_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_IMP_71_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_INT_54_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_INT_54_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_INT_54_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_INT_54_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ION_25_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ION_25_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ION_25_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ION_25_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K7_98_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K7_98_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K7_98_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K7_98_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K95_75_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K95_75_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K95_75_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K95_75_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K98_59_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K98_59_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K98_59_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K98_59_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_K98_59_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_KSG_60_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_KSG_60_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_KSG_60_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_KSG_60_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LAR_40_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LAR_40_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LAR_40_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LAR_40_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LAR_40_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LAR_40_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LC_73_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LC_73_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LC_73_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LC_73_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LDMR_22_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LDMR_22_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LDMR_22_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LDMR_22_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_70_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_70_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_70_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_70_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_70_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_77_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_77_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_77_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_77_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_77_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LEV_77_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LG_85_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LG_85_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LG_85_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LG_85_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSG_17_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSG_17_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSG_17_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSG_17_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSMG_18_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSMG_18_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSMG_18_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSMG_18_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSP_21_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSP_21_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSP_21_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_LSP_21_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14T_91_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14T_91_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14T_91_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14T_91_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14_87_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14_87_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14_87_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M14_87_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M163_90_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M163_90_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M163_90_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M163_90_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M16Grenade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M16Grenade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M16_86_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M16_86_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M16_86_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M16_86_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M32_23_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M32_23_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M32_23_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_M32_23_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MC_62_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MC_62_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MC_62_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MC_62_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG42_26_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG42_26_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG42_26_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG42_26_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG42_26_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG5_5_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG5_5_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG5_5_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MG5_5_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ML_32_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ML_32_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ML_32_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ML_32_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MOD_102_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MOD_102_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MOD_102_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MOD_103_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MOD_103_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_MOD_103_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_NG_48_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_NG_48_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_NG_48_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_NG_48_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICWGrenade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICWGrenade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICW_3_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICW_3_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICW_3_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICW_3_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_OICW_3_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_P90_83_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_P90_83_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_P90_83_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_P90_83_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Alarm_3(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Alarm_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_KeyPress_78(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Other_14(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Other_15(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PARENT_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PC_49_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PC_49_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PC_49_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PC_49_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PSG_95_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PSG_95_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PSG_95_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PSG_95_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PTL_6_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PTL_6_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PTL_6_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PTL_6_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PXS_10_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PXS_10_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PXS_10_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_PXS_10_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_QBZ_96_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_QBZ_96_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_QBZ_96_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_QBZ_96_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_QBZ_96_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_QBZ_96_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_R5K_15_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_R5K_15_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_R5K_15_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_R5K_15_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REP_11_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REP_11_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REP_11_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REP_11_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REV_9_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REV_9_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REV_9_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_REV_9_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RG_46_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RG_46_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RG_46_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RG_46_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ROD_80_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ROD_80_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ROD_80_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_ROD_80_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPG_57_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPG_57_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPG_57_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPG_57_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPG_57_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPK_44_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPK_44_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPK_44_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RPK_44_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RS_79_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RS_79_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RS_79_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_RS_79_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_S12_31_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_S12_31_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_S12_31_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_S12_31_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SB_45_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SB_45_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SB_45_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SB_45_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SC_89_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SC_89_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SC_89_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SC_89_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SPC_13_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SPC_13_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SPC_13_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SPC_13_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SRP_1_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SRP_1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SRP_1_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SRP_1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SRP_1_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STG_88_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STG_88_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STG_88_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STG_88_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STY_69_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STY_69_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STY_69_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_STY_69_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SUP_67_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SUP_67_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SUP_67_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SUP_67_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_SUP_67_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89Grenade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89Grenade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89_97_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89_97_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89_97_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89_97_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_T89_97_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TEC_84_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TEC_84_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TEC_84_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TEC_84_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TMP_33_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TMP_33_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TMP_33_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_TMP_33_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_65_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_65_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_65_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_65_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_76_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_76_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_76_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_UMP_76_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VAL_42_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VAL_42_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VAL_42_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VAL_42_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VEC_30_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VEC_30_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VEC_30_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_VEC_30_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_YL_68_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_YL_68_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_YL_68_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_YL_68_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_bosscube_laser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_bosscube_laser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_bosscube_rocket_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_bosscube_rocket_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_bosscube_rocket_homing_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_bosscube_rocket_homing_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_breachingcharge_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_breachingcharge_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_droneplayer_laser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_droneplayer_laser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_droneplayer_laserfire_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_droneplayer_laserfire_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_grenade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_grenade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_masterkey_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_masterkey_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_plasmagrenade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_plasmagrenade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_stungrenade2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_stungrenade2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_stungrenade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_stungrenade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_taser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_enm_taser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemBloodbolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemBloodbolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemFireorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemFireorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemFlare_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemFlare_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemGrenadelauncher_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemGrenadelauncher_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemHeatsink_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemHeatsink_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemHomingMissile_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemHomingMissile_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemLaser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemLaser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemLasercaster_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemLasercaster_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMagnum_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMagnum_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMasterkey2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMasterkey2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMasterkey_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMasterkey_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMissile_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemMissile_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemPowerbolt_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemPowerbolt_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemRailgun_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemRailgun_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemSeekers_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemSeekers_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemStickybomb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemStickybomb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemTaser_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemTaser_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_fanknives_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_fanknives_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_flyingknife_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_flyingknife_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_kunai_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_kunai_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_shuriken1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_shuriken1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_shuriken2_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_shuriken2_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_tomahawk_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_item_tomahawk_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemironorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemironorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemlightningorb_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_itemlightningorb_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_lasermineitem_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_lasermineitem_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_m75_93_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_m75_93_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_m75_93_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_m75_93_Other_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_m75_93_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_m75_93_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_mouseover_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_sawblade_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_sawblade_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_spawner_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_spawner_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_spawner_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_toggler_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_toggler_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_toggler_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_toggler_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_toggler_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Alarm_7(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_CleanUp_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Other_10(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Other_13(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_Other_22(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weapon_uber_parent_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weaponstats_S1_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weaponstats_S1_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weaponstats_S1_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weaponstats_S1_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_weaponstats_S1_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_big_horizontal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_big_horizontal_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_big_horizontal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_big_vertical_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_big_vertical_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_big_vertical_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_decal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_decal_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_decal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_normal_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_normal_Destroy_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_obj_woodcrate_normal_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_6(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_7(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Alarm_9(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Draw_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_KeyPress_17(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_KeyPress_18(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_KeyPress_32(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_KeyPress_51(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_KeyPress_71(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Keyboard_16(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_11(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_12(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_13(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_4(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_5(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_69(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Other_72(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_PreCreate_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Step_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Step_1(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_outgame_Step_2(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_spr_crystal_spire_milestone_Create_0(CInstance *Self, CInstance *Other);
// LIBZHL_API void __stdcall gml_Object_spr_crystal_spire_milestone_PreCreate_0(CInstance *Self, CInstance *Other);
LIBZHL_API RValue *__stdcall gml_Script_BallCreate(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_BallStep(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DOCUMENTATION_README(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DirtyBloom_define(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DirtyBloom_disable(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DirtyBloom_enable(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DirtyBloom_enabled(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DirtyBloom_ghostsdefine(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DirtyBloom_set_strength(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DissolveSettings(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DissolveShader(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_DrawStatesDebug(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_README(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___0_scr_get_array_for_particle_gml_GlobalScript_scr_get_array_for_particle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___1_scr_shared_networking_event_gml_GlobalScript_scr_shared_networking_event(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___2_scr_shared_networking_event_gml_GlobalScript_scr_shared_networking_event(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___3_scr_shared_networking_event_gml_GlobalScript_scr_shared_networking_event(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___4_scr_shared_networking_event_gml_GlobalScript_scr_shared_networking_event(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___5_gml_Object_outgame_Create_0(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___6_gml_Object_obj_enm_base_parent_Create_0(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___7_gml_Object_obj_moving_entity_PARENT_Other_22(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script____struct___8_gml_Object_obj_moving_entity_PARENT_Other_22(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_get(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_get_colour(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_get_colour_element(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_get_element(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_get_internal(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_set(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_set_colour(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_set_element(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___background_set_internal(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___global_object_depths(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___init_action(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___init_background(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___init_d3d(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___init_global(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___init_view(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___view_get(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___view_set(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script___view_set_internal(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_action_current_room(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_action_end_game(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_action_kill_object(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_action_next_room(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_action_restart_game(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_action_wrap(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_anon__2310(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_avoid_circle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_avoid_rectangle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_background_get_texture(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_cleanmem(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_cleanmem_get_mem(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_cleanmem_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_colorpicker(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_create_shockwave(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_curve_get_value(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_d3d_set_fog(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_d3d_transform_add_translation(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_d3d_transform_set_identity(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_d3d_transform_set_rotation_z(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_debug(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_9slice(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_SHADER_blur(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_background_tiled(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_circle_curve(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_circle_width(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_circular_progress_bar(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_crosshair_quarter(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_healthbar_circular(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify_ext(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify_fast(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify_laser(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify_path(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify_path_circle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_magnify_path_simple(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_path_gradient(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_rectangle_width(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_reset(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_set_alpha_test(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_set_blend_mode(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_draw_set_blend_mode_ext(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_gfx_blur(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_gfx_blur_directional(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_gfx_blur_gaussian(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_gfx_blur_masked(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_gfx_blur_radial(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_gfx_blur_twist(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_image_scale(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_instance_create(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API void __stdcall gml_Script_line_solid_intersection(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_lines_intersect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_macros(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_object_get_depth(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_path_random(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_path_random_circle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_random_color(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scp_set(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_AlertEnemy(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_DailyReward(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_KPI_Arena(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_reopen_lobby(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_request_join_lobby(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_send_CreateAccount(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_send_Login(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_send_PasswordReset(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_send_RequestPasswordReset(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_send_packet_queue(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_MS_set_referal(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_activereload(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_add_chat_entry(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ammotypes(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ammotypes_classbased(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_antibullet_laser(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_array_choose(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_attachments_apply(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_attachments_apply_ultimate(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_attachments_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_attachments_init_ultimate(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_badge_display(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_basic_unit_defense_stats(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_basic_unit_defensive_stats_calc(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_bloodsplat_large(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_boss_dialogs(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_bullet_chain_get_hits(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_button(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_button_check(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_button_intro(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_calculate_targetlaser_distance(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_change_gamemode(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_check_DLCS(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_check_achievements(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_check_gamepad_key(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_check_key(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_check_unlock(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chest_loottable(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chest_loottable_ammo(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chest_loottable_health(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chest_select_loottable(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chunk_connect_path(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chunk_getPathSet(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chunk_helper_sets_connect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_chunk_isPathConnected(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_class_descriptions(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_class_sprites(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_class_upgrades(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_classcheck(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_collision_line(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_color_transistion(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_colors_borderdecal_finder(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_console_input(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_convert_talenttext(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_create_bullet(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_create_bullet_effects(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_create_bullet_shell(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_curses_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_damage_bot(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_damage_numbers(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_deathrecap(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_dispatch_headhunters(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_display_prompt_tooltip(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_distance_sqr(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_draw_decal_on_chunks(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_draw_interacting_bar(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_draw_pausebackground(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_draw_targetlaser(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_drop_bossLoot(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_drop_item(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ds_grid_get(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ds_grid_set(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ds_grid_set_region(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_encrypt_password(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_ctrl_enforcerCoverPos(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_get_closest_fire_pos(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_get_random_position(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_grid_get_cell(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_optimize_path(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_reached_destination(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_set_stateChanged(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enemy_stop_moving(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enm_base_destructor(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enm_knockback(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enm_sensor_check(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enm_voiceover(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_enter_next_floor(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_bindings(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_bindings_items(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_credits(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_game(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_help(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_help2(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_language(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_other(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_esc_video(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_exception_handling(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_fieldNodes_distance(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_fieldNodes_getFieldNodesForShape(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_fightCheck(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_file_delete(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_file_exists(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_filter_layer(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_find_chunk_surface(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_find_highest_parent(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_find_talent_id(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_find_talent_objectindex(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_find_vect_in_list(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_fit_shape_in_node_list(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_fit_shape_in_rect_list(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_gamepad_check_itemBlock(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_getSpawnInSpriteRect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_all_teams(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_array_for_particle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_closest_obj_in_range(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_collision_free(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_collision_normal_dir(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_collision_rectangle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_gamepad_key_name(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_input(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_input_name(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_inst_for_networkId(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_interpolated_shifted_player_pos(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_key_name(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_other_team(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_percentage(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_random_pickup_pos(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_shape_dimensions(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_spawn_pos_on_radius(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_get_team(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_glow_splat_draw(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_goto_room(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_grids_add_circle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_grids_add_rectangle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_grids_clear_circle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_grids_clear_rectangle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_handle_switch_online(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_has_arena_pack(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_has_supporter_pack(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_has_supporter_pack2(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_has_synthetik(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_heal_bot(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_healing_laser_turret(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_healthbar(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_helptext(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_humanoid_attack_movement(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_humanoid_attack_movement_create(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_humanoid_movement(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_increase_packetID(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_indentifypotion_effect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ini_open(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_ini_write_string(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_instance_create(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_instance_nearest(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_invert_layer(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_is_field_In_rect_list(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_is_in_layer(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_is_parent(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_is_player_stuck(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_is_pos_in_enemy_view(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_is_pos_in_rectangle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_item_activeandcooldown(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_item_descriptions(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_item_leveling(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_item_tooltip(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_item_variants_allitems_bonus(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_item_variants_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_join_team(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_leave_team(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_list_add_unique(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_list_contains(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_list_create(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_list_delete(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_list_destroy(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_load_language(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_load_language_backup(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_load_languages_fixed(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_load_progress(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_load_statistics(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_localPlayer_add_screenshake(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_localization(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_map_to_string(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_microchip_gaining(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mod_ini_read_real(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mod_ini_read_string(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_modweapon_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mouseover(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mp_get_closest_grid_pos(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mp_grid_cast(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mp_grid_get_cell(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_mystery_reward(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_nearest_team_member(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_net_add_statuseffect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_net_cleanup_remote(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_net_subscribe_newsletter(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_net_write_packet(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_destroy(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_get_region_name(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_instantiate(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_instantiateSingle(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_instantiate_bullet(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_receiving_client(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_networking_receiving_server(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_new_lobby_notification(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_node_A_Star(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_notification(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_object_debug(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_object_is_parent(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_particle_effects_unit(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_pauseCheck(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_perk_save(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_perk_system(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_perks_chips(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_perks_mars(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_pickup_loottable(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_plating_gaining(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_play_sound_at(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_ammo_gaining(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_currency_change(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_data_change(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_exp_change(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_health_damage(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_health_healing(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_interest_change(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_keycard_gain(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_labfunds_change(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_multiplayer_revive(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_player_set_stealth(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_popup(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_powerup(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_prefetch_queue(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_progress_ini_save(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_prop_shadow_ao_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_prop_spawn_random(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_quest_finished(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_quest_rewards(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_questlog(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_choose(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_exponential(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_gaussian(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_get_value(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_irandom(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_random(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_random_range(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_set_seed(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_random_shuffle_list(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_randomstatboost_gaining(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_range_and_soundcheck(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_raritycolors_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_raycast(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_rebind_input(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_rebinding_button(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_remap_valuerange(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_reset_player(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_reset_tokens(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_roll_ammotype(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_roll_next_room(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_rumble(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_savegame_ini(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_search_audio_at(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_select_from_spawntable(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_send_ArenaMissionSelect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_send_LobbyUpdate(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_send_MS_packet(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_send_global_chat_message(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_send_net_rpc(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_send_packet(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_set_Achievement(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_set_depth(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_settings_ini(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_settings_ini_save(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_settings_load(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_setup_chunk_fields(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_setup_patrolling(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_shared_networking_event(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_shrine_effect_active(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_skyscraper_draw(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_slider(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_sort_objList_by_depth(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_sort_objList_by_dist(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_sort_vectList_by_dist(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_sound(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_alarm_squad(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_chunk_field(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_bigRoom(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_bigRoom_noWalls(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_extra(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_mediumRoom(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_mediumRoom_noWalls(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_smallRoom(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_rnd_smallRoom_noWalls(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_spawn_room(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_split_string(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_statboost(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_statistics_ini(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_statistics_saving(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_statuseffects_collision(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_string_trim_whites(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_talent_desctiptions(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_talent_selection(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_talents_new(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_technologies(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_terrorlevel_increase(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_tooltip(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_tooltipbig_weapons_getdata(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_transfer_item_coop(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_tween0(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_unlock_ammo(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_unlock_descriptions(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_unlock_outgame_progress(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_update_particle_pos(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_url_open(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_vial_gaining(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_view_cone(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_descriptions(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_drop(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_firing(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_init_free(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_pickup(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_tooltipbars(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_variants_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_scr_weapon_variants_init_ultimate(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_set_enemy_path(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_slice_init(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_slice_pragma(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_spr_perks_artefacts(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_texture_set_repeat(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_add(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_delete(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_exists(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_background(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_depth(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_height(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_ids(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_left(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_top(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_width(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_x(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_get_y(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_layer_find(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tile_set_blend(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_EASE_enums(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_ease_lerp(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_ease_progress(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_ease_start_end(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect2(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect2_rot(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect3(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect3_sqr(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect4(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_add(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_comp(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_cross(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_dist(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_div(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_divr(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_dot(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_is_zero(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_len(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_mult(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_multr(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_norm(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_perp(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_rev(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_scaler(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_to_vect2(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_to_vect3(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tmc_vect_to_vect4(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_tolerance(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API RValue *__stdcall gml_Script_vect(CInstance *Self, CInstance *Other, RValue *Result, int ArgumentCount, RValue **Arguments);
LIBZHL_API void __stdcall gml_YoYo_AchievementsAvailable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_AddVirtualKey(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_CheckSecurity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_DeleteVirtualKey(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetCPUDetails(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetConfig(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetDevice(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetPlatform(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetSessionKey(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetTiltX(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetTiltY(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetTiltZ(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_GetTimer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_HideVirtualKey(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_IsKeypadOpen(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_LoginAchievements(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_LogoutAchievements(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseCheckButton(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseCheckButtonPressed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseCheckButtonReleased(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseX(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseXRaw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseY(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_MouseYRaw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_AcceptChallenge(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_AddAchievement(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_AddLeaderboard(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_IsOnline(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_SendChallenge(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_SendChallengeResult(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_SendInvite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_SendSocial(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_SetURL(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OF_StartDashboard(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OpenURL(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OpenURL_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_OpenURL_full(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_PostAchievement(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_PostScore(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_YoYo_ShowVirtualKey(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml___SFAIL(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml___SPRINT(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____CopyStatic___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____ForInIteratorDelete___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____ForInIteratorGetNext___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____ForInIterator___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____GetInstance___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____GlobalScope___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____Global___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____NewArray___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____NewGMLArray___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____NewGMLObject___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____NewObject___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____NewProperty___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____NullObject___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____Null___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____Other___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____This___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____array_set_owner___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____delete___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____finish_catch___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____finish_finally___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____instanceof___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____new___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____script_ref___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____throw___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____try_hook___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____try_unhook___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml____typeof___(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_abs(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_event(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_get_challenges(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_get_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_get_pic(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_increment(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_load_friends(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_load_leaderboard(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_load_progress(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_reset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_send_challenge(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_show(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_show_achievements(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_show_challenge_notifications(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_achievement_show_leaderboards(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_alarm_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_alarm_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_analytics_event(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_analytics_event_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_angle_difference(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_channel_evaluate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_channel_new(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_get_channel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_get_channel_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_animcurve_point_new(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ansi_char(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_application_get_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_application_surface_draw_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_application_surface_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_application_surface_is_enabled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_arccos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_arcsin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_arctan(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_arctan2(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_equals(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_get_2D(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_height_2d(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_insert(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_length_2d(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_pop(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_push(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_resize(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_set_2D(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_set_2D_post(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_set_2D_pre(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_set_post(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_set_pre(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_array_sort(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_add_tags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_clear_tags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_get_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_get_tags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_get_type(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_has_any_tag(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_has_tags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_asset_remove_tags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_channel_num(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_create_buffer_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_create_play_queue(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_create_stream(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_create_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_debug(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_destroy_stream(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_destroy_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_falloff(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_free(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_listener_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_pitch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_vx(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_vy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_vz(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_get_z(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_pitch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_set_listener_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_emitter_velocity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_falloff_set_model(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_free_buffer_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_free_play_queue(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_listener_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_listener_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_listener_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_master_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_recorder_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_recorder_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_get_type(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_is_loaded(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_load(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_load_progress(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_set_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_stop_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_group_unload(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_is_paused(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_is_playing(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_get_data(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_orientation(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_set_orientation(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_set_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_set_velocity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_listener_velocity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_master_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_music_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_music_is_playing(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_pause_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_pause_music(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_pause_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_pause_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_play_in_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_play_music(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_play_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_play_sound_at(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_play_sound_on(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_queue_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_resume_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_resume_music(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_resume_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_resume_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_set_listener_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_set_master_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_get_gain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_get_listener_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_get_pitch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_get_track_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_is_playable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_pitch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_set_listener_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sound_set_track_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_start_recording(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_start_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_stop_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_stop_music(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_stop_recording(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_stop_sound(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_stop_sync_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sync_group_debug(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sync_group_get_track_pos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_sync_group_is_playing(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_system(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_audio_system_is_available(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_base64_decode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_base64_encode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_bool(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_browser_input_capture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_async_group_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_async_group_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_async_group_option(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_base64_decode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_base64_decode_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_base64_encode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_compress(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_copy_from_vertex_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_crc32(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_create_from_vertex_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_create_from_vertex_buffer_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_decompress(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_fill(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_get_address(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_get_alignment(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_get_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_get_surface(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_get_type(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_load(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_load_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_load_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_load_partial(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_md5(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_peek(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_poke(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_resize(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_save_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_save_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_seek(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_set_surface(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_set_used_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_sha1(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_sizeof(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_tell(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_buffer_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_apply(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_create_view(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_active(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_begin_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_default(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_end_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_proj_mat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_update_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_border_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_border_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_mat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_speed_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_speed_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_target(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_get_view_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_begin_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_default(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_end_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_proj_mat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_update_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_border(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_mat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_pos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_camera_set_view_target(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ceil(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_choose(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_chr(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clamp(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_add_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_change(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_change_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clickable_set_style(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clipboard_get_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clipboard_has_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_clipboard_set_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_cloud_file_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_cloud_string_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_cloud_synchronise(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_code_is_compiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_circle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_circle_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_ellipse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_ellipse_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_line(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_line_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_point_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_collision_rectangle_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_color_get_blue(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_color_get_green(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_color_get_hue(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_color_get_red(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_color_get_saturation(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_color_get_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_cos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_darccos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_darcsin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_darctan(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_darctan2(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_compare_date(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_compare_datetime(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_compare_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_create_datetime(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_current_datetime(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_date_of(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_date_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_datetime_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_day_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_days_in_month(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_days_in_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_day(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_day_of_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_hour(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_hour_of_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_minute(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_minute_of_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_month(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_second(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_second_of_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_timezone(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_week(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_weekday(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_get_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_hour_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_day(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_hour(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_minute(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_month(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_second(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_week(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_inc_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_is_today(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_leap_year(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_minute_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_month_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_second_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_set_timezone(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_time_of(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_time_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_valid_datetime(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_week_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_date_year_span(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dcos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_debug_event(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_debug_get_callstack(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_degtorad(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_device_mouse_dbclick_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_device_mouse_x_to_gui(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_device_mouse_y_to_gui(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_directory_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_directory_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_directory_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_dpi_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_dpi_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_frequency(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_gui_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_gui_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_orientation(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_sleep_margin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_timing_method(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_get_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_mouse_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_mouse_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_mouse_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_reset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_set_gui_maximise(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_set_gui_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_set_sleep_margin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_set_timing_method(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_set_ui_visibility(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_display_set_windows_alternate_sync(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_distance_to_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_distance_to_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dot_product(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dot_product_3d(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dot_product_3d_normalised(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dot_product_normalised(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_arrow(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_button(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_circle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_circle_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_clear_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_ellipse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_ellipse_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_enable_drawevent(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_enable_skeleton_blendmodes(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_enable_swf_aa(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_flush(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_circle_precision(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_enable_skeleton_blendmodes(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_font(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_halign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_lighting(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_swf_aa_level(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_get_valign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_getpixel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_getpixel_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_healthbar(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_highscore(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_light_define_ambient(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_light_define_direction(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_light_define_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_light_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_light_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_light_get_ambient(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_line(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_line_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_line_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_line_width_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_path(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_point_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_primitive_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_primitive_begin_texture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_primitive_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_rectangle_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_roundrect(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_roundrect_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_roundrect_color_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_roundrect_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_self(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_circle_precision(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_font(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_halign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_lighting(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_swf_aa_level(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_set_valign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_skeleton(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_skeleton_collision(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_skeleton_instance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_skeleton_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_general(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_part(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_part_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_pos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_stretched(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_stretched_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_tiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_sprite_tiled_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_general(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_part(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_part_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_stretched(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_stretched_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_tiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_surface_tiled_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_ext_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_ext_transformed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_ext_transformed_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_transformed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_text_transformed_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_texture_flush(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_tile(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_tilemap(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_triangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_triangle_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_vertex(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_vertex_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_vertex_texture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_draw_vertex_texture_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_add_disk(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_add_grid_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_add_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_disk_max(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_disk_mean(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_disk_min(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_disk_sum(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_max(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_mean(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_min(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_get_sum(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_multiply(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_multiply_disk(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_multiply_grid_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_multiply_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_resize(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_set_disk(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_set_grid_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_set_post(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_set_pre(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_set_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_shuffle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_sort(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_value_disk_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_value_disk_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_value_disk_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_value_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_value_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_value_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_grid_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_find_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_find_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_insert(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_is_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_is_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_mark_as_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_mark_as_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_replace(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_set_post(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_set_pre(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_shuffle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_sort(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_list_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_add_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_add_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_find_first(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_find_last(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_find_next(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_find_previous(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_find_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_is_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_is_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_keys_to_array(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_replace(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_replace_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_replace_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_secure_load(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_secure_load_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_secure_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_secure_save_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_set_post(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_set_pre(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_values_to_array(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_map_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_change_priority(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_delete_max(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_delete_min(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_delete_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_find_max(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_find_min(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_find_priority(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_priority_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_dequeue(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_enqueue(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_head(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_tail(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_queue_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_set_precision(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_pop(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_push(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_top(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ds_stack_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dsin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_dtan(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_effect_create_above(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_effect_create_below(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_environment_get_variable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_event_inherited(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_event_perform(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_event_perform_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_event_perform_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_event_user(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_exception_unhandled_handler(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_exp(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_extension_get_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_extension_stubfunc_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_extension_stubfunc_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_external_call(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_external_define(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_external_free(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_attributes(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_close(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_open(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_read_byte(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_rewrite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_seek(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_bin_write_byte(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_find_close(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_find_first(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_find_next(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_rename(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_close(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_eof(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_eoln(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_open_append(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_open_from_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_open_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_open_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_read_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_read_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_readln(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_write_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_write_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_file_text_writeln(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_filename_change_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_filename_dir(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_filename_drive(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_filename_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_filename_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_filename_path(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_floor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_add_enable_aa(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_add_get_enable_aa(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_add_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_add_sprite_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_cache_glyph(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_bold(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_first(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_fontname(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_italic(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_last(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_texture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_get_uvs(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_replace_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_replace_sprite_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_font_set_cache_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_frac(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_get_parameter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_get_parameter_names(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_get_parameters(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_set_parameter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_fx_set_parameters(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_get_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_load(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_load_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_restart(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_save_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_game_set_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_add_hardware_mapping_from_file(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_add_hardware_mapping_from_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_axis_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_axis_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_button_check(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_button_check_pressed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_button_check_released(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_button_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_button_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_axis_deadzone(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_button_threshold(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_description(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_device_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_guid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_hardware_mappings(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_mapping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_get_option(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_hat_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_hat_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_is_connected(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_is_supported(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_remove_mapping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_set_axis_deadzone(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_set_button_threshold(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_set_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_set_option(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_set_vibration(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gamepad_test_mapping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gc_collect(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gc_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gc_get_stats(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gc_get_target_frame_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gc_is_enabled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gc_target_frame_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_double_tap_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_double_tap_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_drag_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_drag_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_flick_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_double_tap_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_double_tap_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_drag_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_drag_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_flick_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_pinch_angle_away(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_pinch_angle_towards(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_pinch_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_rotate_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_rotate_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_get_tap_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_pinch_angle_away(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_pinch_angle_towards(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_pinch_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_rotate_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_rotate_time(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gesture_tap_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_integer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_integer_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_login_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_open_filename(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_open_filename_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_save_filename(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_save_filename_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_get_string_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gif_add_surface(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gif_open(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gif_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gif_save_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gml_release_mode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_alphatestenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_alphatestref(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode_dest(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode_destalpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode_ext_sepalpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode_src(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_blendmode_srcalpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_colorwriteenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_cullmode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_fog(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_state(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_max_aniso(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_max_aniso_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_max_mip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_max_mip_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_min_mip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_min_mip_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_mip_bias(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_mip_bias_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_mip_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_mip_enable_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_mip_filter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_tex_mip_filter_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_texfilter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_texfilter_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_texrepeat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_texrepeat_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_zfunc(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_ztestenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_get_zwriteenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_pop_state(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_push_state(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_alphatestenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_alphatestref(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_blendenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_blendmode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_blendmode_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_blendmode_ext_sepalpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_colorwriteenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_cullmode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_fog(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_state(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_max_aniso(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_max_aniso_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_max_mip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_max_mip_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_min_mip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_min_mip_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_mip_bias(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_mip_bias_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_mip_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_mip_enable_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_mip_filter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_tex_mip_filter_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_texfilter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_texfilter_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_texrepeat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_texrepeat_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_zfunc(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_ztestenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gpu_set_zwriteenable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gxc_input_playback(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gxc_record_input_playback(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_gxc_save_input_playback(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_highscore_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_highscore_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_highscore_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_highscore_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_http_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_http_get_file(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_http_get_request_crossorigin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_http_post_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_http_request(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_http_set_request_crossorigin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_iap_activate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_close(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_key_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_key_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_open(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_open_from_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_read_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_read_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_section_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_section_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_write_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ini_write_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_activate_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_activate_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_activate_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_activate_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_change(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_create_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_create_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_deactivate_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_deactivate_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_deactivate_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_deactivate_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_find(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_furthest(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_id_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_nearest(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_number(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_place(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_place_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instance_position_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_instanceof(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_int64(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_io_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_irandom(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_irandom_range(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_array(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_bool(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_infinity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_int32(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_int64(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_matrix(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_method(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_nan(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_numeric(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_ptr(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_struct(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_undefined(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_vec3(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_is_vec4(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_json_decode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_json_encode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_json_parse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_json_stringify(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_check(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_check_direct(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_check_pressed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_check_released(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_get_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_get_numlock(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_key_press(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_key_release(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_set_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_set_numlock(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_unset_map(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_virtual_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_virtual_hide(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_virtual_show(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_keyboard_virtual_status(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_add_instance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_htiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_stretch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_vtiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_get_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_htiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_stretch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_vtiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_background_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_clear_fx(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_destroy_instances(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_element_move(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_force_draw_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_all_elements(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_element_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_element_type(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_forced_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_fx(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_hspeed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_id_at_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_script_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_script_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_shader(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_target_room(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_vspeed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_has_instance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_hspeed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_instance_get_instance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_is_draw_depth_forced(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_reset_target_room(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_script_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_script_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_headdir(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_headpos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_instance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_sequence(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_speedscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_get_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_headdir(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_headpos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_is_finished(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_is_paused(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_pause(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_play(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_speedscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sequence_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_set_fx(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_set_target_room(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_set_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_shader(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_change(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_get_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_sprite_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_change(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_alpha(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_get_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_xscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tile_yscale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tilemap_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tilemap_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tilemap_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_tilemap_get_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_vspeed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_layer_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_lengthdir_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_lengthdir_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_lerp(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ln(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_load_csv(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_load_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_log10(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_log2(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_logn(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_make_color_hsv(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_make_color_rgb(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_math_get_epsilon(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_math_set_epsilon(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_build(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_build_identity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_build_lookat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_build_projection_ortho(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_build_projection_perspective(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_build_projection_perspective_fov(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_multiply(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_stack_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_stack_is_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_stack_pop(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_stack_push(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_stack_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_stack_top(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_matrix_transform_vertex(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_max(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_md5_file(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_md5_string_unicode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_md5_string_utf8(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mean(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_median(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_merge_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_method(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_method_get_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_method_get_self(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_min(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_motion_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_motion_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mouse_check_button(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mouse_check_button_pressed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mouse_check_button_released(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mouse_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mouse_wheel_down(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mouse_wheel_up(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_bounce_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_bounce_solid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_contact_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_contact_solid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_outside_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_outside_solid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_random(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_snap(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_towards_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_move_wrap(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_add_cell(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_add_instances(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_add_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_clear_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_clear_cell(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_clear_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_draw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_get_cell(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_path(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_grid_to_ds_grid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_linear_path(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_linear_path_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_linear_step(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_linear_step_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_potential_path(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_potential_path_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_potential_settings(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_potential_step(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_mp_potential_step_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_connect(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_connect_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_connect_raw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_connect_raw_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_create_server(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_create_server_raw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_create_socket(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_create_socket_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_resolve(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_send_broadcast(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_send_packet(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_send_raw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_send_udp(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_send_udp_raw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_set_config(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_network_set_timeout(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_event_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_event_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_parent(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_persistent(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_physics(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_solid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_get_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_is_ancestor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_set_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_set_parent(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_set_persistent(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_set_solid(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_set_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_object_set_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ord(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_check_permission(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_get_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_get_language(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_get_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_is_network_connected(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_is_paused(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_lock_orientation(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_powersave_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_os_request_permission(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_parameter_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_parameter_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_burst(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_destroy_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_region(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_emitter_stream(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_particles_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_particles_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_particles_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_particles_create_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_automatic_draw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_automatic_update(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_create_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_depth(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_draw_order(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_drawit(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_get_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_layer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_system_update(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_alpha1(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_alpha2(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_alpha3(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_blend(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_color1(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_color2(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_color3(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_color_hsv(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_color_mix(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_color_rgb(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_death(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_direction(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_gravity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_life(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_orientation(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_scale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_shape(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_part_type_step(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_add_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_append(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_assign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_change_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_clear_points(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_delete_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_duplicate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_flip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_closed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_kind(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_number(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_point_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_point_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_point_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_precision(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_insert_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_mirror(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_rescale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_reverse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_rotate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_set_closed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_set_kind(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_set_precision(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_shift(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_path_start(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_apply_angular_impulse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_apply_force(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_apply_impulse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_apply_local_force(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_apply_local_impulse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_apply_torque(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_draw_debug(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_add_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_bind(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_bind_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_angular_damping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_awake(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_box_shape(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_chain_shape(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_circle_shape(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_collision_group(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_density(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_edge_shape(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_friction(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_kinematic(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_linear_damping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_polygon_shape(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_restitution(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_fixture_set_sensor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_get_density(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_get_friction(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_get_restitution(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_distance_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_enable_motor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_friction_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_gear_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_get_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_prismatic_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_pulley_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_revolute_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_rope_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_set_value(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_weld_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_joint_wheel_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_mass_properties(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_delete_region_box(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_delete_region_circle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_delete_region_poly(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_draw(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_draw_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_damping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_data(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_data_particle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_density(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_gravity_scale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_group_flags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_max_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_get_radius(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_add_point(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_box(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_circle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_ang_vel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_angle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_centre_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_centre_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_data(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_inertia(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_mass(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_vel_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_vel_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_join(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_group_polygon(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_category_flags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_damping(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_density(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_flags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_gravity_scale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_group_flags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_max_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_particle_set_radius(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_pause_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_remove_fixture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_set_density(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_set_friction(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_set_restitution(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_test_overlap(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_world_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_world_draw_debug(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_world_gravity(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_world_update_iterations(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_physics_world_update_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_place_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_place_free(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_place_meeting(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_place_snapped(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_point_direction(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_point_distance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_point_distance_3d(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_point_in_circle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_point_in_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_point_in_triangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_position_change(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_position_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_position_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_position_meeting(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_power(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ps4_share_screenshot_enable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_ptr(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_push_cancel_local_notification(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_push_get_application_badge_number(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_push_get_first_local_notification(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_push_get_next_local_notification(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_push_local_notification(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_push_set_application_badge_number(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_radtodeg(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_random(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_random_get_seed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_random_range(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_random_set_seed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_randomize(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_real(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_rectangle_in_circle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_rectangle_in_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_rectangle_in_triangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_assign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_duplicate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_get_camera(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_get_viewport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_goto(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_goto_next(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_goto_previous(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_instance_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_instance_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_next(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_previous(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_restart(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_background_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_camera(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_caption(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_persistent(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_view_enabled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_viewport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_room_set_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_round(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_scheduler_resolution_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_scheduler_resolution_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_screen_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_screen_save_part(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_script_execute(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_script_execute_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_script_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_script_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_destroy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_fps(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get_fps(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get_loopmode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get_objects(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_get_tracks(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_instance_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_instance_override_object(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_keyframe_new(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_keyframedata_new(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_loopmode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_get_animcurves(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_get_keyframe(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_get_numkeyframes(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_get_tracks(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_get_type(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sequence_track_new(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sha1_file(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sha1_string_unicode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sha1_string_utf8(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_current(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_enable_corner_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_get_sampler_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_get_uniform(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_is_compiled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_reset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set_uniform_f(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set_uniform_f_array(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set_uniform_i(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set_uniform_i_array(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set_uniform_matrix(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shader_set_uniform_matrix_array(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shaders_are_supported(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_shop_leave_rating(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_debug_message(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_debug_overlay(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_error(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_message(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_message_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_question(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_question_async(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_show_video(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_get_duration(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_get_event_frames(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_get_frame(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_get_frames(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_mix(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_set_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_animation_set_frame(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_attachment_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_attachment_create_colour(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_attachment_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_attachment_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_bone_data_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_bone_data_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_bone_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_bone_state_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_bone_state_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_collision_draw_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_find_slot(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_get_bounds(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_get_minmax(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_get_num_bounds(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_skin_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_skin_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_skin_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_slot_alpha_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_slot_colour_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_slot_colour_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_slot_data(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_slot_data_instance(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_skeleton_slot_list(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sleep(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_adapt(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_border(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_caption(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_cursor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_fullscreen(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_interrupt(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_main(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_scale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_stop_key(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_stop_mouse(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_set_top(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_show_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_show_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_splash_show_video(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_add_from_surface(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_assign(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_collision_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_create_from_surface(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_duplicate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_flush(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_flush_multi(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_bbox_bottom(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_bbox_left(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_bbox_mode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_bbox_right(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_bbox_top(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_nineslice(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_number(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_speed_type(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_texture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_tpe(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_uvs(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_xoffset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_get_yoffset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_merge(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_nineslice_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_prefetch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_prefetch_multi(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_replace(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_save_strip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_alpha_from_sprite(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_bbox(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_bbox_mode(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_cache_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_cache_size_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_nineslice(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_offset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sprite_set_speed(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sqr(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_sqrt(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_activate_overlay(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_activate_overlay_browser(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_activate_overlay_store(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_activate_overlay_user(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_available_languages(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_clear_achievement(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_create_leaderboard(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_current_game_language(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_download_friends_scores(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_download_scores(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_download_scores_around_user(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_persisted(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_read(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_share(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_write(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_file_write_file(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_achievement(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_app_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_persona_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_quota_free(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_quota_total(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_stat_avg_rate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_stat_float(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_stat_int(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_user_account_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_user_persona_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_get_user_steam_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_initialised(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_is_cloud_enabled_for_account(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_is_cloud_enabled_for_app(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_is_overlay_activated(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_is_overlay_enabled(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_is_screenshot_requested(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_is_user_logged_on(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_reset_all_stats(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_reset_all_stats_achievements(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_send_screenshot(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_set_achievement(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_set_stat_avg_rate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_set_stat_float(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_set_stat_int(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_stats_ready(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_create_item(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_create_query_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_create_query_all_ex(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_create_query_user(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_create_query_user_ex(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_download(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_get_item_install_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_get_item_update_info(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_get_item_update_progress(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_get_subscribed_items(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_num_subscribed_items(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_add_excluded_tag(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_add_required_tag(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_allow_cached_response(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_cloud_filename_filter(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_match_any_tag(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_ranked_by_trend_days(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_return_long_description(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_return_total_only(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_query_set_search_text(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_request_item_details(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_send_query(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_set_item_content(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_set_item_description(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_set_item_preview(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_set_item_tags(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_set_item_title(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_set_item_visibility(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_start_item_update(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_submit_item_update(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_subscribe_item(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_ugc_unsubscribe_item(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_upload_score(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_upload_score_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_upload_score_buffer_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_upload_score_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_user_installed_dlc(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_steam_user_owns_dlc(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_byte_at(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_byte_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_char_at(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_digits(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_format(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_hash_to_newline(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_height_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_insert(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_last_pos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_last_pos_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_length(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_letters(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_lettersdigits(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_lower(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_ord_at(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_pos(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_pos_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_repeat(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_replace(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_replace_all(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_set_byte_at(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_upper(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_string_width_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_copy(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_copy_part(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_create_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_depth_disable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_free(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_get_depth_disable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_get_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_get_target(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_get_target_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_get_texture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_get_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_getpixel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_getpixel_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_reset_target(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_resize(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_save(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_save_part(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_set_target(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_surface_set_target_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tag_get_asset_ids(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tag_get_assets(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tan(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_debug_messages(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_flush(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_get_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_get_texel_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_get_texel_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_get_uvs(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_get_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_global_scale(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_is_ready(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_prefetch(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_preload(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_set_priority(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texture_set_stage(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texturegroup_get_fonts(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texturegroup_get_sprites(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texturegroup_get_textures(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_texturegroup_get_tilesets(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_get_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_get_flip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_get_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_get_mirror(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_get_rotate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_set_empty(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_set_flip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_set_index(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_set_mirror(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tile_set_rotate(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_at_pixel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_cell_x_at_pixel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_cell_y_at_pixel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_frame(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_global_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_tile_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_tile_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_tileset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_set_at_pixel(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_set_global_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_set_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_set_mask(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_set_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_tileset(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tilemap_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tileset_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tileset_get_texture(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_tileset_get_uvs(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_get_name(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_max_moment(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_moment_add(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_moment_add_script(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_moment_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_timeline_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_transition_define(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_transition_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_typeof(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_url_get_domain(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_uwp_livetile_tile_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_global_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_global_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_global_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_instance_exists(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_instance_get(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_instance_get_names(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_instance_names_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_instance_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_struct_remove(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_struct_set_post(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_variable_struct_set_pre(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_argb(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_colour(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_create_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_create_buffer_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_create_buffer_from_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_create_buffer_from_buffer_ext(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_delete_buffer(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_float1(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_float2(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_float3(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_float4(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_add_colour(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_add_custom(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_add_normal(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_add_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_add_position_3d(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_add_textcoord(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_begin(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_delete(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_format_end(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_freeze(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_get_buffer_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_get_number(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_normal(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_position_3d(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_submit(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_texcoord(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_vertex_ubyte4(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_camera(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_hport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_surface_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_wport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_xport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_get_yport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_camera(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_hport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_surface_id(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_wport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_xport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_view_set_yport(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_weak_ref_alive(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_weak_ref_any_alive(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_weak_ref_create(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_win8_livetile_tile_notification(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_center(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_default(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_device(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_caption(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_cursor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_fullscreen(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_showborder(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_showicons(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_sizeable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_stayontop(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_visible_rects(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_handle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_has_focus(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_mouse_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_mouse_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_mouse_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_caption(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_cursor(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_fullscreen(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_max_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_max_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_min_height(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_min_width(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_position(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_rectangle(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_showborder(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_showicons(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_size(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_sizeable(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_stayontop(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_set_visible(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_view_mouse_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_view_mouse_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_view_mouse_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_views_mouse_get_x(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_views_mouse_get_y(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_window_views_mouse_set(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_license_trial_version(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_back_content(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_back_content_wide(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_back_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_back_image_wide(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_back_title(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_count(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_cycle_images(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_front_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_front_image_small(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_front_image_wide(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_icon_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_small_background_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_small_icon_image(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_title(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_winphone_tile_wide_content(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_yyAsm(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall gml_zip_unzip(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
LIBZHL_API void __stdcall memLogEvent();
LIBZHL_API void __stdcall memLogPopContext();
LIBZHL_API void __stdcall memLogPushContext(const char *context);
LIBZHL_API int __stdcall pRValueToHash(RValue *in);
LIBZHL_API void __stdcall thunk_FUN_03875ed1();
LIBZHL_API void __stdcall utf8_add_char();
LIBZHL_API void __stdcall utf8_extract_char();
LIBZHL_API void __stdcall utf8_strlen();
LIBZHL_API byte __stdcall wide_AreStringsEqualUnrolled(byte *s1, byte *s2);

extern LIBZHL_API CRoom **Run_Room;
	
extern LIBZHL_API CInstance **g_pGetRValueContainer;
	
extern LIBZHL_API CInstance **g_pGlobalInstance;
	
extern LIBZHL_API CInstance **g_otherInstance;
	
extern LIBZHL_API CInstance ***g_id2Instance;
	
	
    

#define NAME_CHAR_LIMIT 32
#define DO_LIMIT_NAME(var) var[NAME_CHAR_LIMIT-1] = '\0'
namespace Organik 
{
	struct Message;

	namespace Utils {
		struct Settings 
		{
			static bool g_EnableMultiplayerCompat; // Enable compatibility with 26.1 multiplayer;
			static bool g_EnableInvincibility; // Enable invincibility in the game; 
			static inline bool* GetEnableBugWebhook() {// Enable bug reporting webhook;
				static bool g_EnableBugWebhook = false;
				return &g_EnableBugWebhook;
			}; 
			#include "GuiSettingsInline.h"
			
			static bool g_SettingsLoaded;
			static std::pair<std::unique_lock<std::mutex>, GUISettings*&> GetUISettings();
		};
		void GetCurrentRoom(CRoom*& room);
		std::string url_encode(const std::string &value);
		void bugWebhook(const std::string &errorMessage);
		std::string SendHTTPSPost(const std::string& _server, const std::string& _pae, const std::string& _params = "", const std::string& _body = "");
		static ID3D11Device** const g_D3DDevice = (ID3D11Device**) (0x04267DF4);
		static ID3D11DeviceContext** const g_D3DContext = (ID3D11DeviceContext**)(0x04267DF8);
		bool isInitializationDone();
		
		static char playerNameBuffer[NAME_CHAR_LIMIT];
		#define DEFAULT_NAME "Player"
		void GetPlayerName(char outName[NAME_CHAR_LIMIT]);
		static volatile bool g_needUpdateActiveInstances = false;
		std::mt19937* getrng();
		static inline std::unordered_map<int32_t, std::string> GetVariableMap(CInstance* inst)
		{
			// auto it = inst->m_YYVarsMap->ForEach([](int32_t key, RValue& val) -> void {
				
			// });
		}
	}
	struct UIElement
	{
	protected:
		std::string name = std::string("", 256);
		int32_t hash;
	public:
		static int createCount;
		UIElement() : name(std::to_string(++createCount)) {};
		UIElement(const std::string& name) : name(name) {
			hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name.c_str());
		};
		UIElement(const char* name) : name(std::string(name)) {
			++createCount;
			hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name);
		};
		UIElement(const std::string& name, int32_t instanceID) : name(std::string(name)) {
			++createCount;
			hash = Code_Variable_FindAlloc_Slot_From_Name(nullptr, name.c_str());
		};
		virtual void GetName(char* buf) {
			if (!buf)
			{
				MessageBoxA(NULL, "UIElement::GetName was passed a null buffer!", "Error", MB_OK | MB_ICONERROR);
				return;
			}
			size_t bufSize = max(static_cast<size_t>(256), name.size());
			memset(buf, 0, bufSize);
			strncpy(buf, name.c_str(), bufSize - 1);
			buf[bufSize - 1] = '\0';
		};
		virtual int32_t GetHash() {
			return Code_Variable_FindAlloc_Slot_From_Name(nullptr, name.c_str());
		};
		virtual ~UIElement() = default;
		virtual void Draw(bool& out_mousedOver, bool* p_open = nullptr, const std::string &title = "") {};
	};

	class ChatTab;
	class PartyChatTab;
	class GlobalChatTab;
	class UIManager;
};
using namespace Organik;


CInstance *GetGlobalInstance();
inline static HWND GetWindowHandle()
{
	static HWND windowHandle = nullptr;
	if (!windowHandle) {
		HWND temp = FindWindowA("YYGameMakerYY", NULL);
		if (temp && IsWindow(temp) && IsWindowVisible(temp)) {
			windowHandle = temp;
		} else {
			return nullptr;
		}
	}
	return windowHandle;
}
typedef void (__stdcall *PFN_Builtin)(RValue &Result,CInstance* Self,CInstance* Other,int ArgumentCount,RValue Arguments[]);
#pragma auto_inline(off)
#ifndef DO_BUILTIN_H
#define DO_BUILTIN_H
template <typename T>
requires (std::is_pointer_v<T>)
RValue DoBuiltin(T fn, std::vector<RValue> args)
{
	RValue result;
	PFN_Builtin func = reinterpret_cast<PFN_Builtin>(fn);
	if (!func || !GetGlobalInstance())
	{
		std::string errorTxt = Organik::GetLogger()->ParseFormatting("DoBuiltin (@ %p), GetGlobalInstance() or func is nullptr.\nGetGlobalInstance(): %p\nOriginal args: %p\nargs.data(): %p", 
																			(void*)fn,                                              (void*)GetGlobalInstance(),  (void*)&args,   (void*)args.data());
		
		Error_Show_Action(errorTxt.c_str(), true, true);
	} else {
		func(result, GetGlobalInstance(), GetGlobalInstance(), static_cast<int>(args.size()), args.data());
	}
	
	return std::move(result);
}
#endif // DO_BUILTIN_H
#ifndef DO_BUILTIN_REF_H
#define DO_BUILTIN_REF_H
template <typename T>
requires (std::is_pointer_v<T>)
static void DoBuiltinRef(T fn, RValue &out, std::vector<RValue> args)
{
	PFN_Builtin func = reinterpret_cast<PFN_Builtin>((PVOID)fn);
	if (!func || !GetGlobalInstance())
	{
		std::string errorTxt = Organik::GetLogger()->ParseFormatting("DoBuiltin (@ %p), GetGlobalInstance() or func is nullptr.\nGetGlobalInstance(): %p\nOriginal args: %p\nargs.data(): %p", 
																			(void*)fn,                                              (void*)GetGlobalInstance(),  (void*)&args,   (void*)args.data());
		
		Error_Show_Action(errorTxt.c_str(), true, true);
	} else {
		func(out, GetGlobalInstance(), GetGlobalInstance(), static_cast<int>(args.size()), args.data());
	}
	
	return;
}
#endif // DO_BUILTIN_REF_H
#pragma auto_inline(on)
#include "DefinitionHelpers/InstanceHelper.h"
#include "DefinitionHelpers/VariableHelper.h"
