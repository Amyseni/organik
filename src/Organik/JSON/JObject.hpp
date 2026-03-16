#pragma once
#include "Synthetik.h"

static std::unordered_map<uint32_t, std::string_view> PlayerStatVars;
static std::shared_mutex statsLock;

static decltype(auto) StatHashesRead( )
{
    using namespace std;
    return make_pair(
        move(shared_lock<shared_mutex>(statsLock)),
        forward <const decltype(PlayerStatVars)& >(PlayerStatVars)
    );
}
static decltype(auto) StatHashesWrite( )
{
    using namespace std;
    return make_pair(
        move(unique_lock<shared_mutex>(statsLock)),
        forward <decltype(PlayerStatVars)* const>(&PlayerStatVars)
    );
}
static std::unordered_map<CHashMapHash, uint64_t> hashCache;
static std::shared_mutex hashCacheMutex;
static const std::pair<CObjectGM*, int32_t> ReturnOwner(CHashMapHash hashedObjVarName)
{
    uint64_t combinedID;

    {
        std::shared_lock<std::shared_mutex> lock(hashCacheMutex);
        combinedID = hashCache[hashedObjVarName];
    }
    int32_t varID = combinedID & UINT32_MAX;
    int32_t objIdx = ((combinedID & ((uint64_t) UINT32_MAX << 32)) >> 32) & UINT32_MAX;
    return std::pair(Object_Data(objIdx), varID);
}

static const std::pair<CObjectGM*, int32_t> LookupObjOwner(const std::string_view& objVarName)
{
    CHashMapHash hash = CHashMapCalculateHash((const std::string_view&)objVarName);
    {
        std::shared_lock<std::shared_mutex> lock(hashCacheMutex);
        if (hashCache[hash])
        {
            lock.unlock( );
            ((std::shared_mutex&) hashCacheMutex) = *lock.release( );
            return ReturnOwner(hashCache[hash]);
        }
    }
    int32_t objID;
    int32_t varID;
    {
        std::istringstream iss(std::string(objVarName.data( )));
        std::string objName;
        std::string varName;
        if (std::getline(iss, objName, '.') && std::getline(iss, varName, '.'))
        {
            objID = parseRValueNumber<int32_t>(DoBuiltin(&gml_asset_get_index, RValue(objName.data( ))));
            varID = Code_Variable_FindAlloc_Slot_From_Name(objName.data( ));
            if (objID <= 0 || varID <= 0)
            Error_Show_Action_Alt(true, true, "%s.%s did not yield a valid object ID and variable ID", objName.data( ), varName.data( ));
        }
        else
            Error_Show_Action_Alt(true, true, "%s did not yield a valid object name and variable name", objVarName.data( ));

        //?? if anything failed, program has terminated by this point
    }
    {

        std::unique_lock<std::shared_mutex> lock(hashCacheMutex);
        hashCache[hash] = auto((uint64_t) objID << 32 | (uint64_t) varID);
    }

    return ReturnOwner(hashCache[hash]);
}
struct JObjectManager;

struct JObjectManager
{
    static JObjectManager* Get( )
    {
        static JObjectManager JManager;
        return &JManager;
    }
    

    void LoadJObjects(const std::filesystem::path& file, RValue& outArray, size_t& outSize)
    {
        outArray = RValue(std::vector<RValue>{});
        std::ifstream inFile(file.string( ).data( ), std::ios_base::in);
        {
            std::ifstream inFile((entry.path( ) / "variant.json").generic_string( ));
            std::ostringstream strStream;
            std::string line;
            while (std::getline(inFile, line))
            {
                strStream << line << "\n";
            }
            RValue fileContentsRV;
            YYCreateString(&fileContentsRV, strStream.str( ).data( ));
            outArray = DoBuiltin(&gml_json_parse, fileContentsRV);
            if (newObject.m_Kind != VALUE_OBJECT && variantObject.m_Kind != VALUE_ARRAY)
            {
                Error_Show_Action("Failed to parse game object JSON file.", true, true);
                //! program terminates
            }
        }
    }
}; 
