// 工程vmeminfo
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib") // 如果不是用MSVC编译，需要在编译参数加上 -lshlwapi 而不是这么写
int main() {
  // 首先获得系统信息
  SYSTEM_INFO si;
  ::ZeroMemory(&si, sizeof(si));
  ::GetSystemInfo(&si);
  // 使用外壳辅助程序对一些尺寸进行格式化
  TCHAR szPageSize[MAX_PATH];
  ::StrFormatByteSize(si.dwPageSize, szPageSize, MAX_PATH);
  LONGLONG dwMemSize = (LONGLONG)si.lpMaximumApplicationAddress -
                    (LONGLONG)si.lpMinimumApplicationAddress;
  TCHAR szMemSize[MAX_PATH];
  ::StrFormatByteSize(dwMemSize, szMemSize, MAX_PATH);
  // 将内存信息显示出来
  std ::cout << "Virtual memory page size: " << szPageSize << std ::endl;
  std ::cout.fill('0');
  std ::cout << "Minimum application address: 0x" << std ::hex << std ::setw(8)
             << (LONGLONG)si.lpMinimumApplicationAddress << std ::endl;
  std ::cout << "Maximum application address: 0x" << std ::hex << std ::setw(8)
             << (LONGLONG)si.lpMaximumApplicationAddress << std ::endl;
  std ::cout << "Total available virtual memory: " << szMemSize << std ::endl;
  system("pause");
  return 0;
}
