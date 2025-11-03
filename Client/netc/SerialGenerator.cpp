/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/netc/SerialGenerator.cpp
 *  PURPOSE:     Custom serial generation implementation
 *
 *****************************************************************************/

#include "SerialGenerator.h"
#include <sstream>
#include <iomanip>
#include <intrin.h>
#include <iphlpapi.h>
#include <algorithm>

#pragma comment(lib, "iphlpapi.lib")

SerialGenerator::SerialGenerator() : m_bGenerated(false)
{
}

SerialGenerator::~SerialGenerator()
{
}

const char* SerialGenerator::GetSerial()
{
    if (!m_bGenerated)
    {
        // Choose your method:
        // 1. Custom fixed serial (for testing)
        // m_strSerial = GenerateCustomSerial();
        
        // 2. Hardware-based serial (more realistic)
        m_strSerial = GenerateFromHardware();
        
        m_bGenerated = true;
    }
    return m_strSerial.c_str();
}

void SerialGenerator::RegenerateSerial()
{
    m_bGenerated = false;
    GetSerial();
}

std::string SerialGenerator::GenerateCustomSerial()
{
    // OPTION 1: Fixed custom serial for testing
    return "CUSTOM1234567890ABCDEF12345678";
    
    // OPTION 2: Random serial
    // std::stringstream ss;
    // for (int i = 0; i < 32; i++)
    // {
    //     ss << std::hex << (rand() % 16);
    // }
    // return ss.str();
}

std::string SerialGenerator::GenerateFromHardware()
{
    // Combine multiple hardware identifiers
    std::string combined;
    
    // Add CPU ID
    std::string cpuid = GetCPUID();
    combined += cpuid;
    
    // Add HDD Serial
    std::string hdd = GetHDDSerial();
    combined += hdd;
    
    // Add MAC Address
    std::string mac = GetMACAddress();
    combined += mac;
    
    // Hash to 32 character serial
    return HashString(combined);
}

std::string SerialGenerator::GetCPUID()
{
    int cpuInfo[4] = {0};
    __cpuid(cpuInfo, 0);
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (int i = 0; i < 4; i++)
    {
        ss << std::setw(8) << cpuInfo[i];
    }
    
    return ss.str();
}

std::string SerialGenerator::GetHDDSerial()
{
    DWORD serialNumber = 0;
    DWORD maxComponentLength = 0;
    DWORD fileSystemFlags = 0;
    
    if (GetVolumeInformationA(
        "C:\",
        NULL, 0,
        &serialNumber,
        &maxComponentLength,
        &fileSystemFlags,
        NULL, 0))
    {
        std::stringstream ss;
        ss << std::hex << std::setfill('0') << std::setw(8) << serialNumber;
        return ss.str();
    }
    
    return "00000000";
}

std::string SerialGenerator::GetMACAddress()
{
    IP_ADAPTER_INFO adapterInfo[16];
    DWORD dwBufLen = sizeof(adapterInfo);
    
    DWORD dwStatus = GetAdaptersInfo(adapterInfo, &dwBufLen);
    if (dwStatus == ERROR_SUCCESS)
    {
        PIP_ADAPTER_INFO pAdapterInfo = adapterInfo;
        
        std::stringstream ss;
        ss << std::hex << std::setfill('0');
        for (UINT i = 0; i < pAdapterInfo->AddressLength; i++)
        {
            ss << std::setw(2) << (int)pAdapterInfo->Address[i];
        }
        
        return ss.str();
    }
    
    return "000000000000";
}

std::string SerialGenerator::HashString(const std::string& input)
{
    // Simple hash function - you can replace with MD5/SHA1 for production
    unsigned long hash = 5381;
    
    for (char c : input)
    {
        hash = ((hash << 5) + hash) + c;
    }
    
    std::stringstream ss;
    ss << std::hex << std::setfill('0') << std::setw(32);
    
    // Generate 32 character string from hash
    for (int i = 0; i < 8; i++)
    {
        ss << std::setw(8) << (hash ^ (i * 0x12345678));
    }
    
    std::string result = ss.str();
    
    // Ensure exactly 32 characters
    if (result.length() > 32)
        result = result.substr(0, 32);
    else if (result.length() < 32)
        result.append(32 - result.length(), '0');
    
    // Convert to uppercase
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    
    return result;
}