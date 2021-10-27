// proclist项目
# include <windows.h>
# include <tlhelp32.h>
# include <iostream>
// 当在用户模式机内核模式下都提供所耗时间时，在内核模式下进行所耗时间的64位计算的帮助方法
DWORD GetKernelModePercentage(const FILETIME &ftKernel,
                              const FILETIME &ftUser) {
  // 将FILETIME结构转化为64位整数
  ULONGLONG qwKernel =
      (((ULONGLONG)ftKernel.dwHighDateTime) << 32) + ftKernel.dwLowDateTime;
  ULONGLONG qwUser =
      (((ULONGLONG)ftUser.dwHighDateTime) << 32) + ftUser.dwLowDateTime;
  // 将消耗时间相加，然后计算消耗在内核模式下的时间百分比
  ULONGLONG qwTotal = qwKernel + qwUser;
  DWORD dwPct;
  if (qwTotal == 0) {
    dwPct = 0;
  } else {
    dwPct = (DWORD)(((ULONGLONG)100 * qwKernel) / qwTotal);
  }
  return (dwPct);
}
// 以下是将当前运行进程名和消耗在内核模式下的时间百分数都显示出来的应用程序
int main() {
  // 对当前系统中运行的进程拍取"快照"
  HANDLE hSnapshot =
    ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, // 提取当前进程
                                0); // 如果是当前进程，就将其忽略
  // 初始化进程入口
  PROCESSENTRY32 pe;
  ::ZeroMemory(&pe, sizeof(pe));
  pe.dwSize = sizeof(pe);
  // 按所有进程循环
  BOOL bMore = ::Process32First(hSnapshot, &pe);
  while (bMore) {
    // 打开用于读取的进程
    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, // 指明要得到信息
                                    FALSE, // 不必继承这一句柄
                                    pe.th32ProcessID); // 要打开的进程
    if (hProcess != NULL) {
      // 找出进程的时间
      FILETIME ftCreation, ftExit, ftKernelMode, ftUserMode;
      ::GetProcessTimes(hProcess,      // 所感兴趣的进程
                        &ftCreation,   // 进程的启动时间 (绝对的)
                        &ftExit,       // 结束时间 (如果有的话)
                        &ftKernelMode, // 在内核模式下消耗的时间
                        &ftUserMode);  // 在用户模式下消耗的时间
      // 计算内核模式消耗的时间百分比
      DWORD dwPctKernel =
          ::GetKernelModePercentage(ftKernelMode, // 在内核模式上消耗的时间
                                    ftUserMode); // 在用户模式下消耗的时间
      // 向用户显示进程的某些信息
      std ::cout << "Process ID : " << pe.th32ProcessID << ", EXE file : " << pe.szExeFile << ", % in kernel mode : " << dwPctKernel << std ::endl;
      // 消除句柄
      ::CloseHandle(hProcess);
    }
    // 转向下一个进程
    bMore = ::Process32Next(hSnapshot, &pe);
  }
  system("pause");
  return 0;
}
