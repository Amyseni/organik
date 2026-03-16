struct CSequenceInstance;
struct CBackGM;
struct CCode;
struct CEvent;
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
struct DynamicArray;
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
struct HashmapIterator;
struct YYRoomInstances;
struct YYRoomTiles;
struct YYRunnerInterface;
struct CSequenceInstance;


struct CHashMapRValue {

};
struct CHashMapYYObj2 {

};
struct CInstIter {

};
struct CHashMapYYObj1 {

};
struct CHashMapChar {

};
struct SScriptCacheInfo {

};
struct CHashMapSSCache {

};
struct TmpStringBuilder {

};
struct CVariableList {

};
struct CEventMap {

};
struct CHashMapCInstElem {

};

struct YYGMLFuncs {
	
	
};
struct DynamicArray {
    int m_Length;
    void* m_Array;
};
struct HashmapIterator {

};
struct RFunction
{
	char m_Name[64];
	TRoutine m_Routine;
	int32_t m_ArgumentCount;
	int32_t m_UsageCount;
};
struct HashTable {

};
struct Graphics {

};
struct YYRECT {
    float m_Left;
    float m_Top;
    float m_Right;
    float m_Bottom;
};
struct CDS_Priority {

};
struct VMDebugInfo {

};
struct RTile {

};
struct RVariableRoutine
{
    const char* m_Name;
    FNGetVariable m_GetVariable;
    FNSetVariable m_SetVariable;
    bool m_CanBeSet;
};
struct CLayerOldTilemapElement
{

};
struct SLink
{
	PVOID m_First;
	PVOID m_Last;
	PVOID m_List;
};
struct VMBuffer {

};
struct tagIConsole {

};
struct Vec2 {
    float x;
    float y;
};

struct DValue
{
    double m_Value;
    int m_Dummy;
    int m_Ind;
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
struct CSprite
{
    DynamicArray m_Masks;
    DynamicArray m_Bitmaps;
    DynamicArray m_Textures;
    YYTexPageEntry** m_TexturePageEntry;
    void* m_pSkeleton;
    char* m_pWADMaskBase;
    char* m_Name;
    void* m_Unk1;
    void* m_Unk2;
    void* m_Sequence;
    void* m_Unk3;
    void* pGCSprite;
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
struct DLValue
{
    __int64 val;
    int m_mmy;
    int m_nd;
};

struct RValue;


struct CCode {
    void *vtbl;

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

struct CStream {
    char m_UnkBool;
    int64 m_Size;
    int64 m_Position;
    void* m_Data;
};

struct CScript {
};

struct CInstanceBase {

};

struct YYObjectBase {
    CInstanceBase _base;
	YYObjectBase *m_pNextObject;
    YYObjectBase *m_pPrevObject;
	YYObjectBase *m_Prototype;
	PVOID m_Pcre;
	PVOID m_Pcre2;
	char *m_Class;
    FNGetOwnProperty m_GetProperty;
    FNDeleteOwnProperty m_DeleteOwnProperty;
    FNDefineOwnProperty m_DefineOwnProperty;
	CHashMap_18int___RValueZ1___3* m_YYVarsMap;
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
struct RefDynamicArrayOfRValue
{
    YYObjectBase _base;
	int m_refCount;
	int m_flags;
	RValue* m_Array;
	int64_t m_Owner;
	int m_Visited;
	int m_Length;
};
struct RValue
{
};
struct DLL_RFunction {
    const char* m_ModuleName;
    PVOID m_DllBaseAddress;
    const char* m_FunctionName;
    MFunction m_Function;
    int m_CallingConvention;
    int m_ArgumentCount;
    int m_ArgumentKinds[17];
    int m_ResultKind;
    uint64_t m_Unknown;
};
struct YYEvent {

};

struct RValHashMap {

};
struct CThreadLocalStorage {

};
struct RValHashMapElement {

};
struct VTable_IBuffer {
    void Free();
    int Write(eBuffer_Type param_1, RValue *param_2);
    int WriteArray(eBuffer_Type param_1, uchar *param_2, int param_3);
    int Read(eBuffer_Type param_1, RValue *param_2);
    int Seek(eBuffer_Seek param_1, int param_2);
    void Peek(int param_1, eBuffer_Type param_2, RValue* param_3);
    void Poke(int param_1, eBuffer_Type param_2, RValue* param_3);
    int Save(const char* param_1, int param_2, int param_3);
    int Load(const char* param_1, int param_2, int param_3, int param_4);
    void Base64encode(RValue* param_1, int param_2, int param_3);
    int Base64decode(const char* param_1, int param_2, int param_3);
    void MD5(RValue* param_1, int param_2, int param_3);
    void SHA1(RValue* param_1, int param_2, int param_3);
    void Resize(int param_1);
    void Copy(int param_1, int param_2, IBuffer* param_3, int param_4);
    void Fill(int param_1, int param_2, eBuffer_Type param_3, RValue* param_4, int param_5, bool param_6);
    void GetSurface(int param_1);
    void SetSurface(int param_1, int param_2);
    uint8_t* Compress(int param_1, int param_2, uint32_t* param_3);
    uint8_t* Decompress(uint32_t* param_1);
};

struct CLayerElementBase {
    int32_t m_Type;
    int32_t m_ID;
    bool m_RuntimeDataInitialized;
    const char* m_Name;
    CLayer* m_Layer;
    LayerElementFlink m_Flink;
    LayerElementBlink m_Blink;
};

struct CLayerInstanceElement {
    CLayerElementBase _base;
    int32_t m_InstanceID;
    CInstance* m_Instance;
};

struct CLayerSpriteElement {
    CLayerElementBase _base;
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

struct CLayerTileElement {
    CLayerElementBase _base;
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

struct CLayer {
    int32_t m_Id;
    int32_t m_Depth;
    float m_XOffset;
    float m_YOffset;
    float m_HorizontalSpeed;
    float m_VerticalSpeed;
    bool m_Visible;
    bool m_Deleting;
    bool m_Dynamic;
    char* m_Name;
    RValue m_BeginScript;
	RValue m_EndScript;
	RValue m_Effect;
	int m_ShaderID;
    LinkedList_17CLayerElementBase m_Elements;
    CLayer* _ptr_Next;
	CLayer* _ptr_Prev;
};
struct YYRoomTiles {};
struct YYRoomInstances {};

struct CRoom {		
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
    OLinkedList_9CInstance m_Active; 
    LinkedList_9CInstance m_Deactive; 
    CInstance *m_MarkedFirst; 
    CInstance *m_MarkedLast; 
    int32_t *m_CreationOrderList; 
    int32_t m_CreationOrderListSize; 
    YYRoom *m_WadRoom; 
    unsigned char* m_WadBaseAddress; 
    CPhysicsWorld *m_PhysicsWorld; 
    int32_t m_TileCount; 
    CArrayStructure_5RTile m_Tile; 
    YYRoomTiles *m_WadTiles; 
    YYRoomInstances *m_WadInstances; 
    char *m_Name; 
    bool m_IsDuplicate; 
    LinkedList_6CLayer m_Layers;     
    CHashMap_18int___CLayerZ1___7 m_LayerLookup; 
    CHashMap_29int___CLayerElementBaseZ1___7 m_LayerElementLookup; 
    CLayerElementBase* m_LastLayerElementLookedUp; 
    CHashMap_34int___CLayerInstanceElementZ1___7 m_LayerInstanceElementLookup; 
    int32_t *m_SeqInstances; 
    int32_t m_SeqInstancesCount; 
    int32_t m_SeqInstancesMax; 
    int32_t *m_EffectLayerIDs; 
    int32_t m_EffectLayerIdCount; 
    int32_t m_EffectLayerIdMax; 
};
struct VM {

};
struct DebugInfo {

};

struct iterator { 

};
struct RenderStateManager {

};

struct YYRoom {
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

struct CInstanceBase {
    
};

struct CPhysicsDataGM {
    float* m_PhysicsVertices;
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

struct CEvent {
    CCode* m_Code;
    int m_OwnerObjectID;
};

struct CObjectGM {
    const char* m_Name;
    CObjectGM* m_ParentObject;
    CHashMap_21int___CObjectGMZ1___2* m_ChildrenMap;
    CHashMap_18int___CEventZ1___3* m_EventsMap;
    CPhysicsDataGM m_PhysicsData;
    SLinkedList_9CInstance m_Instances;
    SLinkedList_9CInstance m_InstancesRecursive;
    uint32_t m_Flags;
    int32_t m_SpriteIndex;
    int32_t m_Depth;
    int32_t m_Parent;
    int32_t m_Mask;
    int32_t m_ID;
};

struct GCObjectContainer
{
    YYObjectBase _base;
    CHashMap_35YYObjectBaseZ1___YYObjectBaseZ1___3* m_YYObjectMap;
};

struct SLink {
    PVOID m_First;
    PVOID m_Last;
    PVOID m_list;
};
struct CInstance {
    YYObjectBase _base;
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
    struct YYRECT m_BoundingBox;
    int m_Timers[12];
    PVOID m_TimelinePath;
    CCode* m_InitCode;
    CCode* m_PrecreateCode;
    CObjectGM* m_OldObject;
    int32_t m_LayerID;
    int32_t m_MaskIndex;
    int16_t m_MouseOverCount;
    CInstance* m_Next;
    CInstance* m_Prev;
    struct SLink m_collisionLink;
    struct SLink m_dirtyLink;
    struct SLink m_withLink;
    float m_Depth;
    float m_CurrentDepth;
    float m_LastImageNumber;
    uint32_t m_collisionTestNumber;
};

struct CLayerManager 
{

};
struct OldTilemapElement 
{
    
};
struct YYStrBuilderYY {

};
struct Buffer_Standard {
    IBuffer _base;

};
struct CDS_Grid {

};
struct CDS_Queue {

};
struct CDS_Stack {

};
struct RVariable
{
	RVariable* m_Next;
	RVariable* m_NotPrev;

	RValue m_Index;
	RValue m_Value;
	int m_Hash;
};