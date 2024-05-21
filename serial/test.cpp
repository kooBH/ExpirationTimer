#include <windows.h>
#include <memory>
#include <string>

#include <iostream>
#include <Windows.h>
#include <Setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>


#include <Windows.h>
#include <iostream>
#include <string>

#include <stdio.h>
#include <stdexcept>

#pragma comment (lib, "Setupapi.lib")


// https://oroboro.com/usb-serial-number/



//returns the serial number of the first physical drive in a std::string or an empty std::string in case of failure
//based on http://codexpert.ro/blog/2013/10/26/get-physical-drive-serial-number-part-1/
std::string getFirstHddSerialNumber() {
  //get a handle to the first physical drive
  HANDLE h = CreateFileW(L"\\\\.\\PhysicalDrive3", 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  if (h == INVALID_HANDLE_VALUE) return {};
  //an std::unique_ptr is used to perform cleanup automatically when returning (i.e. to avoid code duplication)
  std::unique_ptr<std::remove_pointer<HANDLE>::type, void(*)(HANDLE)> hDevice{ h, [](HANDLE handle) {CloseHandle(handle); } };
  //initialize a STORAGE_PROPERTY_QUERY data structure (to be used as input to DeviceIoControl)
  STORAGE_PROPERTY_QUERY storagePropertyQuery{};
  storagePropertyQuery.PropertyId = StorageDeviceProperty;
  storagePropertyQuery.QueryType = PropertyStandardQuery;
  //initialize a STORAGE_DESCRIPTOR_HEADER data structure (to be used as output from DeviceIoControl)
  STORAGE_DESCRIPTOR_HEADER storageDescriptorHeader{};
  //the next call to DeviceIoControl retrieves necessary size (in order to allocate a suitable buffer)
  //call DeviceIoControl and return an empty std::string on failure
  DWORD dwBytesReturned = 0;
  if (!DeviceIoControl(hDevice.get(), IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
    &storageDescriptorHeader, sizeof(STORAGE_DESCRIPTOR_HEADER), &dwBytesReturned, NULL))
    return {};
  //allocate a suitable buffer
  const DWORD dwOutBufferSize = storageDescriptorHeader.Size;
  std::unique_ptr<BYTE[]> pOutBuffer{ new BYTE[dwOutBufferSize]{} };
  //call DeviceIoControl with the allocated buffer
  if (!DeviceIoControl(hDevice.get(), IOCTL_STORAGE_QUERY_PROPERTY, &storagePropertyQuery, sizeof(STORAGE_PROPERTY_QUERY),
    pOutBuffer.get(), dwOutBufferSize, &dwBytesReturned, NULL))
    return {};
  //read and return the serial number out of the output buffer
  STORAGE_DEVICE_DESCRIPTOR* pDeviceDescriptor = reinterpret_cast<STORAGE_DEVICE_DESCRIPTOR*>(pOutBuffer.get());
  const DWORD dwSerialNumberOffset = pDeviceDescriptor->SerialNumberOffset;
  if (dwSerialNumberOffset == 0) return {};
  const char* serialNumber = reinterpret_cast<const char*>(pOutBuffer.get() + dwSerialNumberOffset);
  return serialNumber;

}


int dev() {
  std::string serialNumber = getFirstHddSerialNumber();
  if (serialNumber.empty())
    std::cout << "failed to retrieve serial number\n";
  else
    std::cout << "serial number: " << serialNumber << "\n";


    HDEVINFO hDevInfo;
    SP_DEVINFO_DATA deviceInfoData;
    DWORD bufferSize = 1000;
    char buffer[1000];

    // 디바이스 정보를 검색하기 위해 초기화
    hDevInfo = SetupDiGetClassDevsA(&GUID_DEVCLASS_DISKDRIVE, 0, 0, DIGCF_PRESENT);

    if (hDevInfo == INVALID_HANDLE_VALUE) {
      std::cerr << "SetupDiGetClassDevs failed" << std::endl;
      return 1;
    }

    // 각 디바이스에 대한 정보 얻기
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (int i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &deviceInfoData); ++i) {
      // 디바이스의 시리얼 번호 얻기
      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_FRIENDLYNAME, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_FRIENDLYNAME: " << buffer << std::endl;
      }


      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_HARDWAREID, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_HARDWAREID: " << buffer << std::endl;
      }

      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_PHYSICAL_DEVICE_OBJECT_NAME: " << buffer << std::endl;
      }


      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_CLASSGUID, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_CLASSGUID : " << buffer << std::endl;
      }


      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_UNUSED0, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_BASE_CONTAINERID " << buffer << std::endl;
      }

      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_DEVICEDESC, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_DEVUCEDESC " << buffer << std::endl;
      }

      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_PHYSICAL_DEVICE_OBJECT_NAME, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_PHYSICAL_DEVICE_OBJECT_NAME " << buffer << std::endl;
      }

      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_UI_NUMBER, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_UI_NUMBER " << buffer << std::endl;
      }

      if (SetupDiGetDeviceRegistryPropertyA(hDevInfo, &deviceInfoData, SPDRP_SECURITY_SDS, NULL, (PBYTE)buffer, bufferSize, NULL)) {
        std::cout << "디바이스 " << i << "SPDRP_SECURIRTY_SDS" << buffer << std::endl;
      }

      printf("===============\n");

    }

    // 핸들 정리
    SetupDiDestroyDeviceInfoList(hDevInfo);



}

/*
serial number: 040162a46a3e81719910
디바이스 0SPDRP_FRIENDLYNAME: ST2000DM008-2FR102
디바이스 0SPDRP_HARDWAREID: SCSI\Disk________ST2000DM008-2FR10001
디바이스 1SPDRP_FRIENDLYNAME: ADATA SU900
디바이스 1SPDRP_HARDWAREID: SCSI\DiskADATA______________SU900Q012
디바이스 2SPDRP_FRIENDLYNAME:  USB  SanDisk 3.2Gen1 USB Device
디바이스 2SPDRP_HARDWAREID: USBSTOR\Disk_USB_____SanDisk_3.2Gen11.00
디바이스 3SPDRP_FRIENDLYNAME: Samsung SSD 860 EVO M.2 500GB
디바이스 3SPDRP_HARDWAREID: SCSI\DiskSamsung_SSD_860_EVO_M.2_RVT2

장치 헤시
9BBB145B

드라이버 키
{4d36e967-e325-11ce-bfc1-08002be10318}\0005

클래스 GUID
{4d36e967-e325-11ce-bfc1-08002be10318}

기본 컨테이너 ID
{4446e898-033e-52fb-a501-dbad5b81e80f}


*/

int reg() {
    HKEY hKey;
    LPCSTR registryPath = "SYSTEM\\CurrentControlSet\\Enum\\USB";
    LPCSTR valueName = "SampleValueName"; // 값을 읽을 레지스트리 값의 이름

    // 레지스트리 키 열기
    LONG result = RegOpenKeyEx(HKEY_LOCAL_MACHINE, registryPath, 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
      std::cerr << "Failed to open registry key" << std::endl;
      return 1;
    }

    // 레지스트리 값 읽기
    DWORD valueType;
    DWORD dataSize;
    result = RegQueryValueEx(hKey, valueName, NULL, &valueType, NULL, &dataSize);

    if (result == ERROR_SUCCESS) {
      if (valueType == REG_DWORD) {
        DWORD dwordValue;
        result = RegQueryValueEx(hKey, valueName, NULL, &valueType, (LPBYTE)&dwordValue, &dataSize);
        if (result == ERROR_SUCCESS) {
          std::cout << "DWORD Value: " << dwordValue << std::endl;
        }
      }
      else if (valueType == REG_SZ) {
        std::wstring strValue(dataSize / sizeof(wchar_t), L'\0');
        result = RegQueryValueEx(hKey, valueName, NULL, &valueType, (LPBYTE)&strValue[0], &dataSize);
        if (result == ERROR_SUCCESS) {
          std::wcout << L"String Value: " << strValue << std::endl;
        }
      }
      else {
        std::cerr << "Unsupported value type" << std::endl;
      }
    }
    else {
      std::cerr << "Failed to read registry value" << std::endl;
    }

    // 레지스트리 키 닫기
    RegCloseKey(hKey);
}

void wmic() {
  char buffer[2048];

  FILE* pipe = _popen("wmic diskdrive get Model, SerialNumber", "r");

  if (!pipe) {
   std::cout << "popen failed!";
  }
  int idx = 0;
  while (!feof(pipe)) {
    std::cout << "line " << idx++ << " : ";
    if (fgets(buffer, 128, pipe) != NULL)
      std::cout << buffer;
  }
  _pclose(pipe);
  return;
    
}


int main() {
  wmic();
    return 0;
}
