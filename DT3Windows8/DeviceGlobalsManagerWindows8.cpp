//==============================================================================
///	
///	File: DeviceGlobalsManagerWindows8.cpp
///	
/// Copyright (C) 2000-2013 by Smells Like Donkey, Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "pch.h"

#include "DeviceGlobalsManagerWindows8.hpp"
#include "Error.hpp"
#include "System.hpp"
#include "DeviceConsole.hpp"
#include "Factory.hpp"



//==============================================================================
//==============================================================================

namespace DT2 {

extern void s2s (Platform::String ^s1, String &s2);
extern void s2s (String &s1, Platform::String ^&s2);

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION(DeviceGlobalsManagerWindows8)

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

DeviceGlobalsManagerWindows8::DeviceGlobalsManagerWindows8 (void)
{
    Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;

    // Check for the container
    bool hasContainer = localSettings->Containers->HasKey("DT2");
    if (!hasContainer)
        return;

    // Iterate through the settings
    auto values = localSettings->Containers->Lookup("DT2")->Values;

    for (auto it = values->First(); it->HasCurrent; it->MoveNext()) {
        Platform::String^ key = it->Current->Key;
        Platform::String^ value = it->Current->Value->ToString();

		String key_s;
		String value_s;
		DTuint mode = PERSISTENT;

        s2s(key,key_s);
        s2s(value,value_s);
			
		if (key_s[0] == '-') {
			key_s.erase(0,1);
			value_s = MoreMath::fromObfuscated(key_s, value_s);
			mode = PERSISTENT_OBFUSCATED;
		}
		
		LOG_MESSAGE << "Reading persistent value: " << key_s << " = " << value_s;
		setGlobal (key_s, value_s, mode); 
    }
}
			
DeviceGlobalsManagerWindows8::~DeviceGlobalsManagerWindows8 (void)
{
	save();
}

//==============================================================================
//==============================================================================

void DeviceGlobalsManagerWindows8::save (void)
{
    Windows::Storage::ApplicationDataContainer^ localSettings = Windows::Storage::ApplicationData::Current->LocalSettings;

    // Check for the container
    bool hasContainer = localSettings->Containers->HasKey("DT2");
    if (!hasContainer) {
        localSettings->CreateContainer("DT2", Windows::Storage::ApplicationDataCreateDisposition::Always);
    }

    // Add values
    auto values = localSettings->Containers->Lookup("DT2")->Values;

    map_iter i;
	FOR_EACH (i, _globals) {
        if (i->second._type == PERSISTENT || i->second._type == PERSISTENT_OBFUSCATED) {
			String key_s = i->second._name;
			String value_s = i->second._value;
			
			if (i->second._type == PERSISTENT_OBFUSCATED) {
				value_s = MoreMath::toObfuscated(key_s, value_s);
				key_s = "-" + key_s;
				LOG_MESSAGE << "Writing persistent value: " << key_s << " = " << i->second._value << "(" << value_s << ")";
			} else {
				LOG_MESSAGE << "Writing persistent value: " << key_s << " = " << value_s;
			}
						 
            // Add the value
            Platform::String ^key;
            Platform::String ^value;

            s2s(key_s, key);
            s2s(value_s, value);

            values->Insert(key, value);
        }
    }




 //   AutoSpinLock lock(&_lock);

	//HKEY hk; 
	//DWORD dwDisp; 

	//if (RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("SOFTWARE\\" PROJECT_NAME), 0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hk, &dwDisp)) {
	//	return;
	//}
	//map_iter i;

	//FOR_EACH (i, _globals) {
 //       if (i->second._type == PERSISTENT || i->second._type == PERSISTENT_OBFUSCATED) {
	//		String name = i->second._name;
	//		String value = i->second._value;
	//		
	//		if (i->second._type == PERSISTENT_OBFUSCATED) {
	//			value = MoreMath::toObfuscated(name, value);
	//			name = "-" + name;
	//			LOG_MESSAGE << "Writing persistent value: " << name << " = " << i->second._value << "(" << value << ")";
	//		} else {
	//			LOG_MESSAGE << "Writing persistent value: " << name << " = " << value;
	//		}
	//					 
	//		if (RegSetValueExA(	hk, name.cStr(), 0, REG_SZ, (const BYTE*) value.cStr(), (DWORD) value.size() + 1)) {
	//			RegCloseKey(hk); 
	//			return;
	//		}
 // 		}
 //   }
	//	
	//RegCloseKey(hk); 

}

//==============================================================================
//==============================================================================


} // DT2
