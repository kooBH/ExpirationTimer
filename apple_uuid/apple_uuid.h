#ifndef _H_APPLE_UUID_H_
#define _H_APPLE_UUID_H_

#include <iostream>
#include <string>
#include <IOKit/IOKitLib.h>
#include <CoreFoundation/CoreFoundation.h>

std::string get_UUID();

bool check_UUID(std::string valid_uuid);

#endif