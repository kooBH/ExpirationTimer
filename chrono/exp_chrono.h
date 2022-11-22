#ifndef _H_EXP_CHRONO_
#define _H_EXP_CHRONO_

#include <iostream>
#include <chrono>
#include <sstream>
#include <iomanip>

// https://stackoverflow.com/questions/44475868/check-expiration-date-in-c11
inline bool is_expired( std::chrono::system_clock::time_point issued_time )
{
  using namespace std;
  using namespace std::chrono;
  typedef duration< int, ratio_multiply < hours::period, ratio<24> >::type > days;
  return duration_cast< days >( system_clock::now() - issued_time ) > days { 1 };
}

inline auto operator""_issued( const char* s ) // see http://e...content-available-to-author-only...e.com/w/cpp/language/user_literal
{
  std::istringstream iss { s };
  std::tm t {};
  iss >> std::get_time( &t, "%Y%m%d" );
  return std::chrono::system_clock::from_time_t( std::mktime( &t ) );
}

/* Example
std::cout << (is_expired( 20221020_issued ) ? "expired" : "valid") << std::endl;
 
*/

#endif
