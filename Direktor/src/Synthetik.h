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
#include "zhl_internal.h"
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

struct YYLink
{
	YYLink *p_next;
	YYLink *m_Last;
	PVOID *list;
};
    
    using CHashMapHash = uint32_t;

template <typename TKey, typename TValue>
struct CHashMapElement
{
	TValue m_Value;
	TKey m_Key;
	CHashMapHash m_Hash;
};

template <typename TKey, typename TValue, int TInitialMask>
struct CHashMap
{
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
	int32_t m_UsedCount;
	int32_t m_CurrentSize;
	int32_t m_CurrentMask;
	int32_t m_GrowThreshold;
	CHashMapElement<TKey, TValue>* m_Elements;
	void(*m_DeleteValue)(TKey* Key, TValue* Value);
	bool GetContainer(
		TKey Key,

		CHashMapElement<TKey, TValue>*& Value
	)
	{
		CHashMapHash value_hash = CHashMapCalculateHash(Key);
		int32_t ideal_position = static_cast<int>(value_hash & m_CurrentMask);
		for (
			// Start at the ideal element (the value is probably not here though)

			CHashMapElement<TKey, TValue>& current_element = this->m_Elements[ideal_position];
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

	bool GetValue(
		TKey Key,

		TValue& Value
	)
	{
		// Try to get the container
		CHashMapElement<TKey, TValue>* object_container = nullptr;
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



struct IBuffer;
struct RValue;

struct LIBZHL_INTERFACE IBuffer
{
	
	virtual ~IBuffer() {}
	
	LIBZHL_API virtual void Write(int param_2);
	
	virtual int WriteArray(uint8_t &_pSrc,int Size) LIBZHL_PLACEHOLDER
	virtual int Read(RValue &_pOut) LIBZHL_PLACEHOLDER
	virtual int Seek(int _val) LIBZHL_PLACEHOLDER
	virtual void Peek(eBuffer_Type _type,RValue &_pOut) LIBZHL_PLACEHOLDER
	virtual void Poke(eBuffer_Type _type,RValue &_pIn) LIBZHL_PLACEHOLDER
	virtual int Save(int _offset,int _size) LIBZHL_PLACEHOLDER
	virtual int Load(int _src_offset,int _src_size,int _dest_offset) LIBZHL_PLACEHOLDER
	virtual void Base64Encode(int _offset,int _size) LIBZHL_PLACEHOLDER
	virtual void Base64Decode(int _offset,int _size) LIBZHL_PLACEHOLDER
	virtual void MD5(int _offset,int _size) LIBZHL_PLACEHOLDER
	virtual void SHA1(int _offset,int _size) LIBZHL_PLACEHOLDER
	virtual void Resize() LIBZHL_PLACEHOLDER
	virtual void Copy(int _src_size,IBuffer *_pDest,int _dest_off) LIBZHL_PLACEHOLDER
	virtual void Fill(int _size,eBuffer_Type _type,RValue &_pIn,int _stride,bool fill_gaps) LIBZHL_PLACEHOLDER
	virtual void GetSurface() LIBZHL_PLACEHOLDER
	virtual void SetSurface(int _offset) LIBZHL_PLACEHOLDER
	virtual uint8_t *Compress(int _size,uint32_t &resultSize) LIBZHL_PLACEHOLDER
	virtual uint8_t *Decompress() LIBZHL_PLACEHOLDER
	
};

struct Buffer_Standard : IBuffer
{
	
};

struct CBackGM
{
	
};

struct CCode;

struct RToken
{
	
};

struct CInstanceBase
{
	
	virtual ~CInstanceBase() {};
	RValue* m_YYVars;

};

struct CWeakRef;
struct YYObjectBase;

struct YYObjectBase : CInstanceBase
{
	
	LIBZHL_API int FUN_035efac0(int index);
	LIBZHL_API void Free(bool p1);
	LIBZHL_API RValue *InternalGetYYVarRef(int param_1);
	LIBZHL_API RValue *InternalGetYYVarRefL(int param_1);
	LIBZHL_API RValue *InternalReadYYVar(int index);
	LIBZHL_API bool Mark4GC(uint *param_1, int param_2);
	LIBZHL_API bool MarkOnlyChildren4GC(uint *param_1, int param_2);
	LIBZHL_API bool MarkThisOnly4GC(uint *param_1, int param_2);
	LIBZHL_API void PreFree();
	LIBZHL_API void ThreadFree(bool param_1, void *GC_Context);
	LIBZHL_API void constructor(int param_1, int param_2);
	LIBZHL_API void *destructor(uint param_1);
	YYObjectBase *m_pNextObject;
	YYObjectBase *m_pPrevObject;
	YYObjectBase *m_Prototype;
	PVOID pcre;
	PVOID pcre2;
	char *m_Class;
	FNGetOwnProperty m_GetProperty;
	FNDeleteProperty m_DeleteOwnProperty;
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

struct RefDynamicArrayOfRValue : YYObjectBase
{
	int m_refCount;
	int m_flags;
	RValue *m_Array;
	int64_t m_Owner;
	int visited;
	int length;
	
};

#pragma pack(push, 4)
struct RValue
{
	
		union {
			int32_t m_i32;
			int64_t m_i64;
			double m_Real;
			union 
			{
				RefDynamicArrayOfRValue* m_RefArray;
				YYObjectBase* m_Object;
				CInstance* m_Instance;
			};
			PVOID m_Pointer = nullptr;
		};

		uint32_t m_Flags = 0;
		RValueType m_Kind = VALUE_UNDEFINED;

		// ########################################################################
		// End of replicated header code
		// ########################################################################

		/* Explicit conversions */

		int32_t GetArrayLength() const;
		
		// Converts the RValue to a double.
		double ToDouble() const;

		// Converts the RValue to a 32-bit integer.
		int32_t ToInt32() const;

		// Converts the RValue to a 64-bit integer.
		int64_t ToInt64() const;

		// Converts the RValue to a void*.
		void* ToPointer() const;

		// Converts the RValue to a boolean.
		bool ToBoolean() const;

		int32_t GetArrayLength();

		// Returns the stringified kind of the RValue.
		const char* GetKindName() const;

		// Converts the RValue to an object.
		YYObjectBase* ToObject() const;

		// Converts the RValue to an instance.
		CInstance* ToInstance() const;

		// Converts the RValue to a C-style string.
		const char* ToCString() const;

		// Converts the RValue to a string.
		std::string ToString() const;

		// Converts the RValue to a UTF-8 string.
		std::u8string ToUTF8String() const;

		std::map<std::string, RValue*> ToRefMap();

		std::map<std::string, RValue> ToMap() const;

		std::vector<RValue*> ToRefVector();

		std::vector<RValue> ToVector() const;

		int32_t GetMemberCount() const;

		RValue* ToArray();

		RValue();

		RValue(const RValue& Other);

		RValue& operator=(const RValue& Other);

		~RValue();

		
		RValue(const int32_t Value)
		{
			*this = RValue();

			this->m_i64 = static_cast<int64_t>(Value);
			this->m_Kind = VALUE_INT32;
		}
		RValue(const int64_t Value)
		{
			*this = RValue();

			this->m_i64 = static_cast<int64_t>(Value);
			this->m_Kind = VALUE_INT64;
		}
		
		RValue( const double Value)
		{
			*this = RValue();

			this->m_Real = static_cast<double>(Value);
			this->m_Kind = VALUE_REAL;
		}
		RValue( const float Value)
		{
			*this = RValue();

			this->m_Real = static_cast<double>(Value);
			this->m_Kind = VALUE_REAL;
		}

		
		RValue(CObjectGM *Value)
		{
			*this = RValue();

			this->m_Pointer = (PVOID)(Value);
			this->m_Kind = VALUE_OBJECT;
		}
		RValue(YYObjectBase *Value)
		{
			*this = RValue();

			this->m_Pointer = (PVOID)(Value);
			this->m_Kind = VALUE_OBJECT;
		}
		RValue(CInstanceBase *Value)
		{
			*this = RValue();

			this->m_Pointer = (PVOID)(Value);
			this->m_Kind = VALUE_OBJECT;
		}
		RValue(CInstance *Value)
		{
			*this = RValue();

			this->m_Pointer = (PVOID)(Value);
			this->m_Kind = VALUE_OBJECT;
		}

		RValue(IN RValue* Pointer) = delete;

		RValue(void* Pointer);

		RValue(const std::vector<RValue>& Values);

		RValue(std::string_view Value);

		RValue(std::u8string_view Value);

		RValue(const char* Value);

		RValue(const char8_t* Value);

		RValue(bool Value);

		RValue(const std::map<std::string, RValue>& Values);

		RValue& operator[](size_t Index);

		RValue operator[](size_t Index) const;

		RValue& operator[](std::string_view MemberName);

		const RValue& operator[](std::string_view MemberName) const;

		bool ContainsValue(std::string_view MemberName) const;

		explicit operator bool();

		explicit operator double();

		explicit operator std::string();

		explicit operator std::u8string();

		explicit operator int32_t();

		explicit operator int64_t();

	private:
		void __Free();

	public:
	

	void Serialise(IBuffer* _buffer);
	void DeSerialise(IBuffer* _buffer);

	LIBZHL_API RValue *GetVariable(RValue *var);
	LIBZHL_API static int __stdcall ParamSize(RValueType param_1);
	
};

#pragma pack(pop)

struct YYGMLFuncs;

struct CCode
{
	PVOID *_vptr;
	CCode *m_Next;
	int m_Kind;
	int m_Compiled;
	const char *m_Str;
	RToken m_Token;
	RValue m_Value;
	PVOID m_VmInstance;
	PVOID m_VmDebugInfo;
	char *m_Code;
	const char *m_Name;
	int m_CodeIndex;
	YYGMLFuncs *m_Functions;
	bool m_Watch;
	int m_Offset;
	int m_LocalsCount;
	int m_ArgsCount;
	int m_Flags;
	YYObjectBase *m_Prototype;
	
};

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

struct CEvent
{
	
	LIBZHL_API static void __stdcall Execute(void *CInst_self, void *CInst_other, CCode *code, RValue *params);
	CCode *m_Code;
	int m_OwnerObjectID;
	
};

struct CInstance;

struct CEvent;

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
	const char *m_Name;
	CObjectGM *m_ParentObject;
	CHashMap<int, CObjectGM*, 2> *m_ChildrenMap;
	CHashMap<int, CEvent*, 3> *m_EventsMap;
	CPhysicsDataGM m_PhysicsData;
	LinkedList<CInstance> m_Instances;
	LinkedList<CInstance> m_InstancesRecursive;
	uint32_t m_Flags;
	int32_t m_SpriteIndex;
	int32_t m_Depth;
	int32_t m_Parent;
	int32_t m_Mask;
	int32_t m_ID;
	
};

struct CPhysicsObject;
struct CSkeletonInstance;

struct SLink
{
	PVOID m_First;
	PVOID m_Last;
	PVOID m_list;
	
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
	LIBZHL_API static void __stdcall Remove(CInstance *param_1);
	LIBZHL_API void Serialise(IBuffer *param_1);
	LIBZHL_API void SetDeactivated(bool param_1);
	LIBZHL_API void ThreadFree(bool param_2, void *GC_Context);
	LIBZHL_API void constructor(float param_2, float param_3, int param_4, int param_5, bool param_6);
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
	LIBZHL_API static int __stdcall AddNewElementAtDepth(CRoom *param_1, int param_2, uchar param_3, bool param_4, bool param_5);
	LIBZHL_API static void __stdcall BuildElementRuntimeData(CRoom *param_1, CLayer *param_2, uchar param_3);
	LIBZHL_API static void __stdcall BuildOldTilemapElementRuntimeData(CRoom *param_1, CLayer *param_2, CLayerOldTilemapElement *param_3);
	LIBZHL_API static void __stdcall BuildRoomLayerRuntimeData(CRoom *param_1);
	LIBZHL_API static void __stdcall CleanElementRuntimeData(CRoom *param_1, uchar param_2);
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

struct CStream;

struct CScript
{
	CStream *m_Text;
	CCode *s_Code;
	YYGMLFuncs *m_Functions;
	CInstance *m_CompiledObject;
	ScriptIndex m_Script;
	const char *m_Name;
	int m_Offset;
	
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

struct CStream
{
	char m_UnkBool;
	int64 m_Size;
	int64 m_Position;
	void *m_Data;
	
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
	int dummy;
	int kind;
	
};

struct DValue
{
	double val;
	int dummy;
	int kind;
	
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
	LIBZHL_API static bool __stdcall OpenWindow(void *g_hwnd, int width, int height, int flags);
	LIBZHL_API static void __stdcall Quit();
	LIBZHL_API static void __stdcall SetMatrix(int32_t param_1, int32_t param_2);
	LIBZHL_API static void __stdcall SetViewPort();
	
};

struct HashTable
{
	
	LIBZHL_API static int __stdcall calculate_string_hash(char *param_1);
	LIBZHL_API static int __stdcall compare_keys(char *cStr, int length, int unk);
	
};

struct List
{
	
};

struct RFunction
{
	char m_Name[64];
	TRoutine m_Routine;
	int32_t m_ArgumentCount;
	int32_t m_UsageCount;
	
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

struct RenderStateManager;

struct RenderStateManager
{
	
	LIBZHL_API void Force();
	LIBZHL_API void Reset();
	LIBZHL_API void WriteStatesToBuffer();
	
};

struct SScriptCacheInfo
{
	
};

struct TmpStringBuilder;

struct TmpStringBuilder
{
	
	LIBZHL_API void Output(char *param_1);
	
};

struct Buffer_Standard;
struct VMBuffer;
struct VMDebugInfo;
struct tagIConsole;

struct VM
{
	
	LIBZHL_API static VMDebugInfo *__stdcall DebugInfo(VMBuffer *param_1, int param_2);
	LIBZHL_API static char *__stdcall DebugLine(VMDebugInfo *param_1, char *param_2);
	LIBZHL_API static int __stdcall DebugLineNumber(VMDebugInfo *param_1, char *param_2, char **param_3);
	LIBZHL_API static void __stdcall Disasm(char *param_1, VMBuffer *param_2, tagIConsole *param_3);
	LIBZHL_API static int __stdcall DisasmOne(char *param_1, uchar *param_2, int param_3, tagIConsole *param_4);
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

struct YYGMLFuncs
{
	const char *m_Name;
	FunctionUnion m_Function;
	PVOID m_FunctionVariables;
	
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

struct YYStrBuilderYY;

struct YYStrBuilderYY
{
	
	LIBZHL_API void growBuffer(int param_1);
	
};

struct iterator;

struct iterator
{
	
	LIBZHL_API CInstance *GetNext_FIRST_entry();
	
};

struct tagIConsole
{
	
};

struct CDS_Grid;
struct CDS_Priority;
struct CDS_Queue;
struct CDS_Stack;
struct CScript;
struct List;

LIBZHL_API int __stdcall ASYNCFunc_SpriteAdd(HTTP_REQ_CONTEXT *_pContext, void *_p, int *_pMap);
LIBZHL_API void __stdcall ASYNCFunc_SpriteCleanup(HTTP_REQ_CONTEXT *_pContext);
LIBZHL_API void __stdcall AddDirectoryToBundleWhitelist(char *_pszFilename);
LIBZHL_API void __stdcall AddDirectoryToSaveWhitelist(char *_pszFilename);
LIBZHL_API void __stdcall AddFileToBundleWhitelist(char *_pszFilename);
LIBZHL_API void __stdcall AddFileToSaveWhitelist(char *_pszFilename);
LIBZHL_API void __stdcall AddGlobalObject(YYObjectBase *param_1);
LIBZHL_API void *__stdcall Alloc(uint len, char *srcFile, int lineNr, bool unk);
LIBZHL_API char *__stdcall Arg2String(int param_1);
LIBZHL_API bool __stdcall BOOL_RValue(const RValue *_pValue);
LIBZHL_API bool __stdcall Base64Encode(void *input_buf, size_t input_len, void *output_buf, size_t output_len);
LIBZHL_API bool __stdcall BufferGetContent(int _index, void **_ppData, int *_pDataSize);
LIBZHL_API int __stdcall BufferWriteContent(int _index, int _dest_offset, void *_pSrcMem, int _size, bool _grow, bool _wrap);
LIBZHL_API void __stdcall CInstAddRValue(RValue *_pStruct, char *_pKey, RValue *_pValue);
LIBZHL_API void __stdcall COPY_RValue(RValue *_pDest, RValue *_pSource);
LIBZHL_API void __stdcall COPY_RValue_do__Post(RValue *param_1, RValue *param_2);
LIBZHL_API void __stdcall ClearHandledContainer();
LIBZHL_API void __stdcall Code_Constant_Prepare();
LIBZHL_API CInstance *__stdcall Code_CreateStatic();
LIBZHL_API bool __stdcall Code_Execute(CInstance *self, CInstance *other, CCode *func, RValue *args, int flags);
LIBZHL_API bool __stdcall Code_Execute_Special(CInstance *param_1, CInstance *param_2, CCode *param_3, RValue *param_4, int param_5);
LIBZHL_API int __stdcall Code_Variable_FindAlloc_Slot_From_Name(YYObjectBase *obj, char *name);
LIBZHL_API char *__stdcall Code_Variable_Find_Name(char *fn_name, int objind, int varind);
LIBZHL_API int __stdcall Code_Variable_Find_Set(char *name, int scope, int setme);
LIBZHL_API void __stdcall Code_Variable_Init(int num_global, int num_instance, int num_local);
LIBZHL_API void __stdcall ConvertStackEntryStringToDouble(uchar **param_1, double *param_2, uchar *param_3, uchar *param_4, uchar param_5, VMExec *param_6, char *param_7);
LIBZHL_API void __stdcall CreateAsyncEventWithDSMap(int _map, int _event);
LIBZHL_API void __stdcall CreateAsyncEventWithDSMapAndBuffer(int _map, int _buffer, int _event);
LIBZHL_API int __stdcall CreateBuffer(int _size, eBuffer_Format _bf, int _alignment);
LIBZHL_API int __stdcall CreateDsMap(int _num);
LIBZHL_API HSPRITEASYNC __stdcall CreateSpriteAsync(int *_pSpriteIndex, int _xOrig, int _yOrig, int _numImages, int _flags);
LIBZHL_API void __stdcall DebugConsoleOutput(char *fmt);
LIBZHL_API void __stdcall Debug_AddTag(uint32_t type, char *tagName);
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
LIBZHL_API void __stdcall DsMapAddBool(int _index, char *_pKey, bool value);
LIBZHL_API bool __stdcall DsMapAddDouble(int _index, char *_pKey, double value);
LIBZHL_API bool __stdcall DsMapAddInt64(int _index, char *_pKey, int64 value);
LIBZHL_API void __stdcall DsMapAddRValue(int _index, char *_pKey, RValue *value);
LIBZHL_API bool __stdcall DsMapAddString(int _index, char *_pKey, char *pVal);
LIBZHL_API void __stdcall DsMapClear(int _dsMap);
LIBZHL_API void __stdcall EndRoom(bool ending);
LIBZHL_API void __stdcall Error_Show_Action(char *err, bool serious, bool _manualError);
LIBZHL_API bool __stdcall ExecuteIt(CInstance *selfinst, CInstance *otherinst, CCode *code, RValue *res, int flags);
LIBZHL_API void __stdcall Extension_Prepare();
LIBZHL_API void __stdcall FREE_RValue(RValue *_pValue);
LIBZHL_API void __stdcall FREE_RValue__Pre(RValue *param_1);
LIBZHL_API RValue *__stdcall FUN_0041df60(YYObjectBase *param_1, int32_t param_2, RValue *param_3, int param_4, uint32_t *param_5);
LIBZHL_API RValue *__stdcall FUN_005852e0(YYObjectBase *param_1, int32_t param_2, RValue *param_3);
LIBZHL_API RValue *__stdcall FUN_005b4180(YYObjectBase *param_1, int32_t param_2, RValue *param_3, int param_4, uint32_t *param_5);
LIBZHL_API RValue *__stdcall FUN_005c2ea0(YYObjectBase *param_1, YYObjectBase *param_2, RValue *param_3, int param_4, uint32_t *param_5);
LIBZHL_API RValue *__stdcall FUN_005daed0(YYObjectBase *param_1, YYObjectBase *param_2, RValue *param_3);
LIBZHL_API RValue *__stdcall FUN_00764bb0(YYObjectBase *param_1, int32_t param_2, RValue *param_3);
LIBZHL_API RValue *__stdcall FUN_008cffc0(YYObjectBase *param_1, int32_t param_2, RValue *param_3, int param_4, uint32_t *param_5);
LIBZHL_API RValue *__stdcall FUN_00e44200(YYObjectBase *param_1, int32_t param_2, RValue *param_3);
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
LIBZHL_API void *__stdcall F_JS_ToInt32(RValue *ret, RValue *toConv);
LIBZHL_API void __stdcall F_LayerExists(RValue *param_1, CInstance *param_2, CInstance *param_3, int param_4, RValue *param_5);
LIBZHL_API void __stdcall FreeAllVertexData();
LIBZHL_API void __stdcall Function_Add(char *name, TRoutine routine, int argnumb, bool regonly);
LIBZHL_API void __stdcall Function_Replace(char *name, TRoutine routine);
LIBZHL_API bool __stdcall GET_RValue(RValue *_pRet, RValue *_pV, YYObjectBase *_pPropSelf, int _index, bool fPrepareArray, bool fPartOfSet);
LIBZHL_API char *__stdcall GET_STRING_RValue(RValue *param_1);
LIBZHL_API void __stdcall GLOBAL_RVALUE_HEAPSIZE();
LIBZHL_API void __stdcall GR_D3D_End();
LIBZHL_API bool __stdcall GR_D3D_Finish_Frame(bool param_1);
LIBZHL_API bool __stdcall GR_D3D_Init(HWND g_hwnd, int width, int height, int debugdisplay);
LIBZHL_API bool __stdcall GR_D3D_Reset();
LIBZHL_API void __stdcall GR_D3D_Set_Region(int param_1, int param_2);
LIBZHL_API void __stdcall GR_D3D_Set_View_Area(float param_1, float param_2, float param_3, float param_4, float param_5);
LIBZHL_API void __stdcall GR_D3D_Settings_Init();
LIBZHL_API bool __stdcall GR_D3D_Start_Frame();
LIBZHL_API void __stdcall GR_Surface_FreeAll();
LIBZHL_API CDS_Grid **__stdcall GetTheGrids(int *param_1);
LIBZHL_API CDS_Priority **__stdcall GetThePriorityQueues(int *param_1);
LIBZHL_API CDS_Queue **__stdcall GetTheQueues(int *param_1);
LIBZHL_API CDS_Stack **__stdcall GetTheStacks(int *param_1);
LIBZHL_API int __stdcall HASH_RValue(const RValue *_pValue);
LIBZHL_API void __stdcall HTTP_Get(char *_pFilename, int _type, PFUNC_async _async, PFUNC_cleanup _cleanup, void *_pV);
LIBZHL_API void __stdcall HTTP_Post(char *_pFilename, char *_pPost, PFUNC_async *_async, PFUNC_cleanup *_cleanup, void *_pV);
LIBZHL_API void __stdcall HTTP_Request(char *_url, char *_method, char *_headers, char *_pBody, PFUNC_async *_async, PFUNC_cleanup *_cleanup, void *_pV, int _contentLength);
LIBZHL_API int __stdcall INT32_RValue(const RValue *param_1);
LIBZHL_API int64 __stdcall INT64_RValue(const RValue *_pValue);
LIBZHL_API void __stdcall InitGraphics();
LIBZHL_API void __stdcall InitLLVM();
LIBZHL_API void __stdcall InitSLLVM(SLLVMVars *param_1);
LIBZHL_API void __stdcall InvalidateFrozenVBs();
LIBZHL_API void __stdcall JSThrowTypeError(char *param_1);
LIBZHL_API RValue *__stdcall JUMP_TO_RET(RValue *Args);
LIBZHL_API char *__stdcall KIND_NAME_RValue(const RValue *_pV);
LIBZHL_API int __stdcall KIND_RValue(RValue *_pValue);
LIBZHL_API void __stdcall LOCK_RVALUE_MUTEX();
LIBZHL_API void __stdcall MarkAndSweepGen(int param_1, int param_2, bool param_3);
LIBZHL_API void __stdcall MarkInstancesAsDirty(int param_1);
LIBZHL_API CObjectGM *__stdcall Object_Data(int hash);
LIBZHL_API bool __stdcall Object_Prepare();
LIBZHL_API void *__stdcall PTR_RValue(const RValue *_pValue);
LIBZHL_API uchar *__stdcall PerformReturn(uchar *param_1, VMExec *param_2);
LIBZHL_API void __stdcall PopContextStack(int param_1);
LIBZHL_API void __stdcall ProcessObjectDisposeList();
LIBZHL_API double __stdcall REAL_RValue(const RValue *_pValue);
LIBZHL_API void __stdcall ReleaseConsoleOutput(char *fmt);
LIBZHL_API void __stdcall ResetGCRefs();
LIBZHL_API int __stdcall Room_Add();
LIBZHL_API CRoom *__stdcall Room_Data(int RoomID);
LIBZHL_API int __stdcall Room_Duplicate(int param_1);
LIBZHL_API int __stdcall Room_Find(char *param_1);
LIBZHL_API void __stdcall Room_Name();
LIBZHL_API void __stdcall Room_Prepare();
LIBZHL_API void __stdcall Room_Prepare_fake();
LIBZHL_API void __stdcall RunnerLoadDataFile();
LIBZHL_API void __stdcall RunnerLoadGame();
LIBZHL_API void __stdcall SET_RValue(RValue *_pDest, RValue *_pV, YYObjectBase *_pPropSelf, int _index);
LIBZHL_API bool __stdcall SaveFileExists(char *_pszFileName);
LIBZHL_API bool __stdcall SaveFileName(char *_name, int _size, char *_pszFileName);
LIBZHL_API CScript *__stdcall ScriptFromId(int param_1);
LIBZHL_API int __stdcall Script_Find_Id(char *name);
LIBZHL_API void __stdcall Script_Free();
LIBZHL_API bool __stdcall Script_Perform(int ind, CInstance *selfinst, CInstance *otherinst, int argc, RValue *res, RValue *arg);
LIBZHL_API void __stdcall Script_Prepare();
LIBZHL_API void __stdcall ShowMessage(char *msg);
LIBZHL_API void __stdcall StartRoom(int param_1, bool param_2);
LIBZHL_API void __stdcall StructAddBool(RValue *_pStruct, char *_pKey, bool _value);
LIBZHL_API void __stdcall StructAddDouble(RValue *_pStruct, char *_pKey, double _value);
LIBZHL_API void __stdcall StructAddInt(RValue *_pStruct, char *_pKey, int _value);
LIBZHL_API void __stdcall StructAddRValue(RValue *_pStruct, const char *_pKey, RValue *_pValue);
LIBZHL_API void __stdcall StructCreate(RValue *_pStruct);
LIBZHL_API void __stdcall SwitchRoom(int param_1);
LIBZHL_API void __stdcall TickDebugger();
LIBZHL_API void __stdcall TimeLine_Prepare();
LIBZHL_API void __stdcall Timing_Sleep(int64 slp, bool precise);
LIBZHL_API int64 __stdcall Timing_Time();
LIBZHL_API void __stdcall UNLOCK_RVALUE_MUTEX();
LIBZHL_API void __stdcall VMError(VMExec *param_1, char *param_2);
LIBZHL_API void __stdcall Variable_BuiltIn_Find();
LIBZHL_API bool __stdcall Variable_GetValue_Direct(YYObjectBase *param_1, int param_2, int param_3, uchar param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall Variable_GetValue_OBJTYPE(int param_1, int param_2, int param_3, uchar param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall Variable_SetValue_Direct(YYObjectBase *param_1, uint param_2, int param_3, RValue *param_4);
LIBZHL_API LRESULT __stdcall WndProc(HWND h_Wnd, uint param2, WPARAM param_3, LPARAM param_4);
LIBZHL_API void *__stdcall YYAlloc(int _size);
LIBZHL_API void __stdcall YYCreateArray(RValue *pRValue, int n_values, double *values);
LIBZHL_API void __stdcall YYCreateString(RValue *param_1, char *param_2);
LIBZHL_API void __stdcall YYError(char *fmt);
LIBZHL_API void __stdcall YYGML_DeleteWithIterator(SWithIterator *param_1, YYObjectBase **param_2, YYObjectBase **param_3);
LIBZHL_API CInstance *__stdcall YYGML_FindInstance(int param_1);
LIBZHL_API CInstance *__stdcall YYGML_GetStaticObject(int param_1);
LIBZHL_API int __stdcall YYGML_NewWithIteratorEx(SWithIterator *param_1, YYObjectBase **param_2, YYObjectBase **param_3, int param_4);
LIBZHL_API bool __stdcall YYGML_Variable_GetValue(int param_1, int param_2, int param_3, uchar param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall YYGML_Variable_GetValue_OBJTYPE(int param_1, int param_2, int param_3, uchar param_4, bool param_5, bool param_6);
LIBZHL_API bool __stdcall YYGML_Variable_SetValue(int param_1, int param_2, int param_3, uchar param_4);
LIBZHL_API bool __stdcall YYGML_WithIteratorNext(SWithIterator *param_1, YYObjectBase **param_2, YYObjectBase **param_3);
LIBZHL_API bool __stdcall YYGetBool(RValue *_pBase, int _index);
LIBZHL_API float __stdcall YYGetFloat(RValue *_pBase, int _index);
LIBZHL_API int32_t __stdcall YYGetInt32(RValue *_pBase, int _index);
LIBZHL_API int64 __stdcall YYGetInt64(RValue *_pBase, int _index);
LIBZHL_API void *__stdcall YYGetPtr(RValue *_pBase, int _index);
LIBZHL_API intptr_t __stdcall YYGetPtrOrInt(RValue *_pBase, int _index);
LIBZHL_API double __stdcall YYGetReal(RValue *_pBase, int _index);
LIBZHL_API char *__stdcall YYGetString(const RValue *_pBase, int _index);
LIBZHL_API uint32_t __stdcall YYGetUint32(RValue *_pBase, int _index);
LIBZHL_API HYYMUTEX __stdcall YYMutexCreate(char *_name);
LIBZHL_API void __stdcall YYMutexDestroy(HYYMUTEX hMutex);
LIBZHL_API void __stdcall YYMutexLock(HYYMUTEX hMutex);
LIBZHL_API void __stdcall YYMutexUnlock(HYYMUTEX hMutex);
LIBZHL_API void *__stdcall YYRealloc(void *pOriginal, int _newSize);
LIBZHL_API void __stdcall YYSetString(RValue *out, char *in);
LIBZHL_API char *__stdcall YYStrDup(char *param_1);
LIBZHL_API void __stdcall YYstrnlen();
LIBZHL_API void __stdcall __COPY_RValue_do__Post(RValue *param_1, RValue *param_2);
LIBZHL_API uchar *__stdcall boxType(RValue *param_1, eVM_Type param_2, uchar *param_3);
LIBZHL_API void __stdcall build_function_index();
LIBZHL_API void __stdcall gml_MCI_command(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
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
LIBZHL_API void __stdcall gml_effect_clear(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
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
LIBZHL_API void __stdcall gml_winphone_tile_background_color(RValue *Result, CInstance *Self, CInstance *Other, int ArgumentCount, RValue *Arguments);
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
LIBZHL_API void __stdcall memLogPushContext(char *context);
LIBZHL_API void __stdcall utf8_add_char();
LIBZHL_API void __stdcall utf8_extract_char();
LIBZHL_API void __stdcall utf8_strlen();
LIBZHL_API uchar __stdcall wide_AreStringsEqualUnrolled(uchar *s1, uchar *s2);

extern LIBZHL_API CInstance **g_pGetRValueContainer;
	
extern LIBZHL_API CInstance **g_pGlobalInstance;
	
extern LIBZHL_API CInstance **g_otherInstance;
	
extern LIBZHL_API CInstance ***g_id2Instance;
	
extern LIBZHL_API CRoom **Run_Room;
	
extern LIBZHL_API CInstance **g_GlobalCInstReal;
	
extern LIBZHL_API HWND *g_hWnd;
	
	
    

    namespace Organik 
    {

    };
    using namespace Organik;


