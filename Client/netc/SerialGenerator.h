/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Client/netc/SerialGenerator.h
 *  PURPOSE:     Custom serial generation for MTA client
 *
 *****************************************************************************/

#pragma once

#include <string>
#include <windows.h>

class SerialGenerator
{
public:
    SerialGenerator();
    ~SerialGenerator();

    // Get the current serial (generates on first call)
    const char* GetSerial();
    
    // Force regenerate serial
    void RegenerateSerial();

private:
    // Generate serial from hardware info
    std::string GenerateFromHardware();
    
    // Generate custom serial (your own logic here)
    std::string GenerateCustomSerial();
    
    // Get CPU ID
    std::string GetCPUID();
    
    // Get HDD Serial
    std::string GetHDDSerial();
    
    // Get MAC Address
    std::string GetMACAddress();
    
    // Hash string to fixed length
    std::string HashString(const std::string& input);

    std::string m_strSerial;
    bool m_bGenerated;
};
