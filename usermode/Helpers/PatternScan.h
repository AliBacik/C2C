#pragma once
#include <cstdint>
#include <vector>
#include <string>
#include "../Core/MemoryMgr.h"

// CS2 process memory'sinde byte pattern tarar.
// "?" wildcard olarak kabul edilir.
namespace PatternScan
{
    // Pattern string'i parse et: "48 8D 3D ? ? ? ? E8"
    inline std::vector<int16_t> Parse(const std::string& pattern)
    {
        std::vector<int16_t> bytes;
        const char* p = pattern.c_str();
        while (*p)
        {
            if (*p == ' ') { p++; continue; }
            if (*p == '?') {
                bytes.push_back(-1);
                p++;
                if (*p == '?') p++;
            } else {
                bytes.push_back((int16_t)strtol(p, nullptr, 16));
                p += 2;
            }
        }
        return bytes;
    }

    // moduleBase adresinden baslayarak pattern'i tara, bulunan adresi dondur (0 = bulunamadi)
    // ReadMemory max 0x1000 byte okuyabildigindan chunk'li tarama yapiyoruz.
    inline DWORD64 Scan(DWORD64 moduleBase, SIZE_T moduleSize, const std::string& pattern)
    {
        auto sig = Parse(pattern);
        if (sig.empty()) return 0;

        const SIZE_T CHUNK   = 0x1000;
        const SIZE_T OVERLAP = (SIZE_T)sig.size() - 1; // chunk sinirinda kayma olmasin
        std::vector<uint8_t> buf(CHUNK + OVERLAP, 0);
        size_t sigLen = sig.size();

        for (SIZE_T offset = 0; offset < moduleSize; offset += CHUNK)
        {
            SIZE_T readSize = min(CHUNK + OVERLAP, moduleSize - offset);
            if (!memoryManager.ReadMemory(moduleBase + offset, buf[0], readSize))
                continue;

            SIZE_T scanEnd = min(CHUNK, readSize);
            for (SIZE_T i = 0; i + sigLen <= readSize; i++)
            {
                bool found = true;
                for (size_t j = 0; j < sigLen; j++)
                {
                    if (sig[j] != -1 && buf[i + j] != (uint8_t)sig[j])
                    {
                        found = false;
                        break;
                    }
                }
                if (found)
                    return moduleBase + offset + i;
            }
        }
        return 0;
    }

    // RIP-relative adres coz: match adresinden offsetBytes sonra 4-byte signed rel32 var,
    // instructionSize = tam instruction uzunlugu (rel32'den sonraki byte = bir sonraki instruction)
    inline DWORD64 ResolveRelative(DWORD64 matchAddress, int relOffset, int instructionSize)
    {
        int32_t rel = 0;
        memoryManager.ReadMemory<int32_t>(matchAddress + relOffset, rel);
        return matchAddress + instructionSize + (int64_t)rel;
    }
}
