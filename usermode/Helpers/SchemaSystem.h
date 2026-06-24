#pragma once
#include <string>
#include <unordered_map>
#include "../Core/MemoryMgr.h"

// CS2 schemasystem.dll'den runtime'da class field offset'lerini okur.
// Deadlocked-rust schema.rs ile birebir ayni yapisal offset'leri kullanir.
class SchemaSystem
{
public:
    bool Init(DWORD64 schemaModuleBase);

    // "CCSPlayerPawn", "m_iIDEntIndex" -> field offset
    DWORD Get(const std::string& className, const std::string& fieldName) const;

private:
    struct Field {
        std::string name;
        DWORD offset = 0;
    };

    struct SchemaClass {
        std::string name;
        std::unordered_map<std::string, DWORD> fields;
    };

    struct ModuleScope {
        std::string name;
        std::unordered_map<std::string, SchemaClass> classes;
    };

    std::unordered_map<std::string, ModuleScope> m_scopes;

    std::string ReadString(DWORD64 address) const;
    void ParseScope(DWORD64 scopeAddress, ModuleScope& scope);
    void ParseClass(DWORD64 classAddress, SchemaClass& cls);
};

inline SchemaSystem g_schema;
