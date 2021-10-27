// version项目
#include <iostream>
#include <windows.h>

// 利用进程和操作系统的版本信息的简单示例
int main() {
  // 提取这个进程的ID号
  DWORD dwIdThis = ::GetCurrentProcessId();
  // 获得这一进程和报告所需的版本，也可以发送0以便指明这一进程
  DWORD dwVerReq = ::GetProcessVersion(dwIdThis);
  WORD wMajorReq = (WORD)(dwVerReq > 16);
  WORD wMinorReq = (WORD)dwVerReq & 0xffff;
  std ::cout << "Process ID: " << dwIdThis << ", requires OS: " << wMajorReq
             << wMinorReq << std ::endl;
  // 设置版本信息的数据结构，以便保存操作系统的版本信息
  OSVERSIONINFOEX osvix;
  ::ZeroMemory(&osvix, sizeof(osvix));
  osvix.dwOSVersionInfoSize = sizeof(osvix);
  // 提取版本信息和报告
  #pragma warning(disable : 4996)
  ::GetVersionEx(reinterpret_cast<LPOSVERSIONINFO>(&osvix));
  std ::cout << "Running on OS: " << osvix.dwMajorVersion << "."
             << osvix.dwMinorVersion << std ::endl;
  // 如果是NTS (Windows 7) 系统，则提高其优先权
  if (osvix.dwPlatformId == VER_PLATFORM_WIN32_NT &&
      osvix.dwMajorVersion >= 5) {
    // 改变优先级
    ::SetPriorityClass(::GetCurrentProcess(), // 利用这一进程
                       HIGH_PRIORITY_CLASS);  // 改变为high
                                              // 报告给用户
    std ::cout << "Task Manager should now indicate this "
                  "process is high priority."
               << std ::endl;
  }
  system("pause");
  return 0;
}
