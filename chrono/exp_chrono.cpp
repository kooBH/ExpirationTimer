#include "exp_chrono.h"

// https://stackoverflow.com/questions/44475868/check-expiration-date-in-c11
bool is_expired( std::chrono::system_clock::time_point issued_time )
{
  using namespace std;
  using namespace std::chrono;
  typedef duration< int, ratio_multiply < hours::period, ratio<24> >::type > days;

  return duration_cast< days >( system_clock::now() - issued_time ) > days { 1 };
}
std::chrono::system_clock::time_point operator""_issued(const char* s){
//inline  std::chrono::time_point<std::chrono::system_clock> issued(const char* s){
  std::istringstream iss { s };
  std::tm t {};
  iss >> std::get_time( &t, "%Y%m%d" );
  return std::chrono::system_clock::from_time_t( std::mktime( &t ) );
}

/* Example
std::cout << (is_expired( 20221020_issued ) ? "expired" : "valid") << std::endl;
 
*/

