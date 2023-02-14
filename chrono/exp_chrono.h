#ifndef _H_EXP_CHRONO_
#define _H_EXP_CHRONO_

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

// https://stackoverflow.com/questions/44475868/check-expiration-date-in-c11
bool is_expired( std::chrono::system_clock::time_point issued_time );

std::chrono::system_clock::time_point operator""_issued(const char* s);

/* Example
std::cout << (is_expired( 20221020_issued ) ? "expired" : "valid") << std::endl;
 
*/

#endif
