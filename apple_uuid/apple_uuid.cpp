
#include "apple_uuid.h"

std::string get_UUID(){
    std::string uuidString;
    io_service_t platformExpert = IOServiceGetMatchingService(kIOMasterPortDefault, IOServiceMatching("IOPlatformExpertDevice"));
    
    if (platformExpert) {
        // Fetch IOPlatformUUID
        CFStringRef uuidCF = (CFStringRef)IORegistryEntryCreateCFProperty(
            platformExpert, 
            CFSTR("IOPlatformUUID"), 
            kCFAllocatorDefault,
            0
        );

        if (uuidCF) {
            // CFString -> std::string 
            const char *cStr = CFStringGetCStringPtr(uuidCF, kCFStringEncodingUTF8);
            if (cStr) {
                uuidString = cStr;
            } else {
                // Alternative conversion method using buffer
                CFIndex length = CFStringGetLength(uuidCF);
                CFIndex maxSize = CFStringGetMaximumSizeForEncoding(length, kCFStringEncodingUTF8) + 1;
                char *buffer = new char[maxSize];
                if (CFStringGetCString(uuidCF, buffer, maxSize, kCFStringEncodingUTF8)) {
                    uuidString = buffer;
                }
                delete[] buffer;
            }
            CFRelease(uuidCF);
        }
        IOObjectRelease(platformExpert);
    } else {
        std::cerr << "Error: Could not find IOPlatformExpertDevice." << std::endl;
        return "";
    }
    return uuidString;
}

bool check_UUID(std::string valid_uuid){
    std::string current_uuid = get_UUID();
    if(current_uuid.empty()) {
        std::cerr << "Failed to retrieve current UUID." << std::endl;
        return false;
    }
    if (current_uuid == valid_uuid) {
        //std::cout << "UUID is valid." << std::endl;
        return true;
    } else {
        //std::cerr << "UUID is invalid." << std::endl;
        return false;
    }
}