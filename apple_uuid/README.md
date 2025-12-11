


- 사용 예시
```
  printf("%s\n", get_UUID().c_str());

  if(check_UUID(TARGET_UUID)){
    printf("Valid Device\n");
  }else{
    printf("Invalid Device\n");
    exit(-1);
  }


```

- Note 
x86 아키텍처 지원 빌드 시

```
Undefined symbols for architecture x86_64:
  "_IOObjectRelease", referenced from:
      get_UUID() in apple_uuid.cpp.o
  "_IORegistryEntryCreateCFProperty", referenced from:
      get_UUID() in apple_uuid.cpp.o
  "_IOServiceGetMatchingService", referenced from:
      get_UUID() in apple_uuid.cpp.o
  "_IOServiceMatching", referenced from:
      get_UUID() in apple_uuid.cpp.o
  "_kIOMasterPortDefault", referenced from:
      get_UUID() in apple_uuid.cpp.o
ld: symbol(s) not found for architecture x86_64
```

에러 발생 시에는
```cmake
find_library(IOKIT_LIBRARY IOKit)
target_link_libraries(YourTargetName PUBLIC ${IOKIT_LIBRARY})
```
를 cmake에 추가. 