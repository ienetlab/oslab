// 工程vmwalker
#include <iomanip>
#include <iostream>
#include <windows.h>
#include <shlwapi.h>
#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "shlwapi.lib") // 如果不是用MSVC编译，需要在编译参数加上 -lshlwapi 而不是这么写

// 以可读方式对用户显示保护的辅助方法。
// 保护标记表示允许应用程序对内存进行访问的类型
// 以及操作系统强制访问的类型
inline bool TestSet(DWORD dwTarget, DWORD dwMask) {
  return ((dwTarget & dwMask) == dwMask);
}

#define SHOWMASK(dwTarget, type)      \
if (TestSet(dwTarget, PAGE_##type)) { \
  std ::cout << ", " << #type;        \
}

void ShowProtection(DWORD dwTarget) {
  SHOWMASK(dwTarget, READONLY);
  SHOWMASK(dwTarget, GUARD);
  SHOWMASK(dwTarget, NOCACHE);
  SHOWMASK(dwTarget, READWRITE);
  SHOWMASK(dwTarget, WRITECOPY);
  SHOWMASK(dwTarget, EXECUTE);
  SHOWMASK(dwTarget, EXECUTE_READ);
  SHOWMASK(dwTarget, EXECUTE_READWRITE);
  SHOWMASK(dwTarget, EXECUTE_WRITECOPY);
  SHOWMASK(dwTarget, NOACCESS);
}
// 遍历整个虚拟内存并对用户显示其属性的工作程序的方法
void WalkVM(HANDLE hProcess) {
  // 首先，获得系统信息
  SYSTEM_INFO si;
  ::ZeroMemory(&si, sizeof(si));
  ::GetSystemInfo(&si);
  // 分配要存放信息的缓冲区
  MEMORY_BASIC_INFORMATION mbi;
  ::ZeroMemory(&mbi, sizeof(mbi));
  // 循环整个应用程序地址空间
  LPCVOID pBlock = (LPVOID)si.lpMinimumApplicationAddress;
  while (pBlock < si.lpMaximumApplicationAddress) {
    // 获得下一个虚拟内存块的信息
    if (::VirtualQueryEx(hProcess,                    // 相关的进程
                         pBlock,                      // 开始位置
                         &mbi,                        // 缓冲区
                         sizeof(mbi)) == sizeof(mbi)) // 大小的确认
    {
      // 计算块的结尾及其大小
      LPCVOID pEnd = (PBYTE)pBlock + mbi.RegionSize;
      TCHAR szSize[MAX_PATH];
      ::StrFormatByteSize(mbi.RegionSize, szSize, MAX_PATH);
      // 显示块地址和大小
      std ::cout.fill('0');
      std ::cout << std ::hex << std ::setw(8) << (LONGLONG)pBlock << "-"
                 << std ::hex << std ::setw(8) << (LONGLONG)pEnd
                 << (::strlen(szSize) == 7 ? " (" : " (") << szSize << ") ";
      // 显示块的状态
      switch (mbi.State) {
      case MEM_COMMIT:
        std ::cout << "Committed";
        break;
      case MEM_FREE:
        std ::cout << "Free";
        break;
      case MEM_RESERVE:
        std ::cout << "Reserved";
        break;
      }

      // 显示保护
      if (mbi.Protect == 0 && mbi.State != MEM_FREE) {
        mbi.Protect = PAGE_READONLY;
      }
      ShowProtection(mbi.Protect);
      // 显示类型
      switch (mbi.Type) {
      case MEM_IMAGE:
        std ::cout << ", Image";
        break;
      case MEM_MAPPED:
        std ::cout << ", Mapped";
        break;
      case MEM_PRIVATE:
        std ::cout << ", Private";
        break;
      }
      // 检验可执行的影像
      TCHAR szFilename[MAX_PATH];
      if (::GetModuleFileName((HMODULE)pBlock, // 实际虚拟内存的模块句柄
                              szFilename,      // 完全指定的文件名称
                              MAX_PATH) > 0) // 实际使用的缓冲区大小
      {
        // 除去路径并显示
        ::PathStripPath(szFilename);
        std ::cout << ", Module: " << szFilename;
      }
      std ::cout << std ::endl;
      // 移动块指针以获得下一下个块
      pBlock = pEnd;
    }
  }
}
int main() {
  // 遍历当前进程的虚拟内存
  ::WalkVM(::GetCurrentProcess());
  system("pause");
  return 0;
}
