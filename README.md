# ExpirationTimer


## CMake
```CMake
include(lib/ExpirationTimer/chrono/exp_chrono.cmake)
```

## Usage  
+ NOTE : only works on ```Release``` mode

```cpp
if (is_expired(20230328_issued)) {
      printf("Program Expired!! Please contact contact the distributor for further information.");
      exit(-1);
  }
```
