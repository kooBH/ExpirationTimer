#include "exp_chrono.h"

int main()
{
  std::cout << (is_expired( 20221020_issued ) ? "expired" : "valid") << std::endl;
  std::cout << (is_expired( 20221119_issued ) ? "expired" : "valid") << std::endl;
  std::cout << (is_expired( 20221120_issued ) ? "expired" : "valid") << std::endl;
  std::cout << (is_expired( 20221121_issued ) ? "expired" : "valid") << std::endl;
  std::cout << (is_expired( 22221122_issued ) ? "expired" : "valid") << std::endl;
  return 0;
}