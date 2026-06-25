#include "SchemaSystem.h"
#include <iostream>
#include <algorithm>

std::string SchemaSystem::ReadString(DWORD64 address) const
{
    if (address == 0) return {};
    char buf[256] = {};
    memoryManager.ReadMemory(address, buf, sizeof(buf) - 1);
    buf[255] = '\0';
    return std::string(buf);
}

void SchemaSystem::ParseClass(DWORD64 classAddress, SchemaClass& cls)
{
    DWORD64 namePtr = 0;
    memoryManager.ReadMemory<DWORD64>(classAddress + 0x08, namePtr);
    cls.name = ReadString(namePtr);

    int16_t fieldCount = 0;
    memoryManager.ReadMemory<int16_t>(classAddress + 0x24, fieldCount);
    if (fieldCount <= 0 || fieldCount > 20000)
        return;

    DWORD64 fieldsVec = 0;
    memoryManager.ReadMemory<DWORD64>(classAddress + 0x30, fieldsVec);
    if (fieldsVec == 0)
        return;

    for (int16_t i = 0; i < fieldCount; i++)
    {
        DWORD64 fieldAddress = fieldsVec + (0x20 * (DWORD64)i);

        DWORD64 fieldNamePtr = 0;
        memoryManager.ReadMemory<DWORD64>(fieldAddress, fieldNamePtr);
        std::string fieldName = ReadString(fieldNamePtr);

        int32_t fieldOffset = 0;
        memoryManager.ReadMemory<int32_t>(fieldAddress + 0x10, fieldOffset);

        if (!fieldName.empty())
            cls.fields[fieldName] = (DWORD)fieldOffset;
    }
}

void SchemaSystem::ParseScope(DWORD64 scopeAddress, ModuleScope& scope)
{
    DWORD64 namePtr = 0;
    memoryManager.ReadMemory<DWORD64>(scopeAddress + 0x08, namePtr);
    scope.name = ReadString(namePtr);

    // 1024 bucket hash table (offset 0x560 + 0x90)
    DWORD64 hashVector = scopeAddress + 0x560 + 0x90;
    for (int i = 0; i < 1024; i++)
    {
        DWORD64 current = 0;
        memoryManager.ReadMemory<DWORD64>(hashVector + ((DWORD64)i * 24) + 0x28, current);

        while (current != 0)
        {
            DWORD64 data = 0;
            memoryManager.ReadMemory<DWORD64>(current + 0x10, data);
            if (data != 0)
            {
                SchemaClass cls;
                ParseClass(data, cls);
                if (!cls.name.empty())
                    scope.classes[cls.name] = std::move(cls);
            }
            DWORD64 next = 0;
            memoryManager.ReadMemory<DWORD64>(current + 0x08, next);
            current = next;
        }
    }

    // free_list (HashAllocatedBlob, offset 0x560 + 0x20)
    DWORD64 blob = 0;
    memoryManager.ReadMemory<DWORD64>(scopeAddress + 0x560 + 0x20, blob);
    while (blob != 0)
    {
        DWORD64 data = 0;
        memoryManager.ReadMemory<DWORD64>(blob + 0x10, data);
        if (data != 0)
        {
            SchemaClass cls;
            ParseClass(data, cls);
            if (!cls.name.empty())
                scope.classes[cls.name] = std::move(cls);
        }
        DWORD64 next = 0;
        memoryManager.ReadMemory<DWORD64>(blob, next);
        blob = next;
    }
}

bool SchemaSystem::Init(DWORD64 schemaModuleBase)
{
    if (schemaModuleBase == 0)
        return false;

    m_scopes.clear();

    // schemasystem.dll icerisinde SchemaSystem singleton'ini bul.
    // Pattern: 48 8D 3D ?? ?? ?? ?? E8 ?? ?? ?? ?? 48 8B BD ?? ?? ?? ?? 31 F6
    // ReadMemory max 0x1000 byte okuyabildigindan chunk'li tarama yapiyoruz.
    const SIZE_T MODULE_SIZE = 0x800000; // ~8MB
    const SIZE_T CHUNK       = 0x1000;
    const SIZE_T OVERLAP     = 32;
    std::vector<uint8_t> buf(CHUNK + OVERLAP, 0);
    DWORD64 schemaSystemPtr  = 0;

    for (SIZE_T off = 0; off < MODULE_SIZE && schemaSystemPtr == 0; off += CHUNK)
    {
        SIZE_T readSize = (std::min)(CHUNK + OVERLAP, MODULE_SIZE - off);
        if (!memoryManager.ReadMemory(schemaModuleBase + off, buf[0], readSize))
            continue;

        for (SIZE_T i = 0; i + 12 <= readSize; i++)
        {
            if (buf[i] == 0x48 && buf[i+1] == 0x8D && buf[i+2] == 0x3D && buf[i+7] == 0xE8)
            {
                int32_t rel = 0;
                memcpy(&rel, &buf[i + 3], sizeof(int32_t));
                DWORD64 candidate = schemaModuleBase + off + i + 7 + (int64_t)rel;

                int32_t scopeCount = 0;
                if (!memoryManager.ReadMemory<int32_t>(candidate + 0x1F0, scopeCount))
                    continue;
                if (scopeCount <= 0 || scopeCount > 64)
                    continue;

                schemaSystemPtr = candidate;
                break;
            }
        }
    }

    if (schemaSystemPtr == 0)
    {
        std::cout << "[SchemaSystem] pattern bulunamadi\n";
        return false;
    }

    int32_t scopeCount = 0;
    memoryManager.ReadMemory<int32_t>(schemaSystemPtr + 0x1F0, scopeCount);
    DWORD64 scopesVec = 0;
    memoryManager.ReadMemory<DWORD64>(schemaSystemPtr + 0x1F8, scopesVec);

    std::cout << "[SchemaSystem] " << scopeCount << " scope bulundu\n";

    for (int32_t i = 0; i < scopeCount; i++)
    {
        DWORD64 scopeAddr = 0;
        memoryManager.ReadMemory<DWORD64>(scopesVec + ((DWORD64)i * 8), scopeAddr);
        if (scopeAddr == 0) continue;

        ModuleScope scope;
        ParseScope(scopeAddr, scope);
        if (!scope.name.empty())
        {
            std::cout << "[SchemaSystem] scope: " << scope.name
                      << " (" << scope.classes.size() << " class)\n";
            m_scopes[scope.name] = std::move(scope);
        }
    }

    return !m_scopes.empty();
}

DWORD SchemaSystem::Get(const std::string& className, const std::string& fieldName) const
{
    for (const auto& [scopeName, scope] : m_scopes)
    {
        auto classIt = scope.classes.find(className);
        if (classIt == scope.classes.end()) continue;

        auto fieldIt = classIt->second.fields.find(fieldName);
        if (fieldIt == classIt->second.fields.end()) continue;

        return fieldIt->second;
    }
    std::cout << "[SchemaSystem] bulunamadi: " << className << "::" << fieldName << "\n";
    return 0;
}
