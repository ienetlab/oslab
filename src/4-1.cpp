// fileobj项目
#include <iostream>
#include <windows.h>
// 要使用的文件名
static LPCTSTR g_szFileName = "w2kdg.Fileobj.file.data.txt";
// 在数据文件中读取当前数据的简单线程时将传递来的该数据增加，并写回数据文件中
static DWORD WINAPI ThreadProc(LPVOID lpParam) {
  // 将参数翻译为长整数
  LONGLONG nAdd = reinterpret_cast<LONGLONG>(lpParam); // LONG -> LONGLONG for x64 (JiJi)
  // 建立完全的指定文件名 (包括路径信息)
  TCHAR szFullName[MAX_PATH];
  ::GetTempPath(MAX_PATH, szFullName); // 取得路径
  ::strcat(szFullName, g_szFileName);
  std::cout << szFullName;
  // 打开文件对象
  HANDLE hFile = ::CreateFile(szFullName, // 文件的完全名称
                              GENERIC_READ | GENERIC_WRITE, // 具有所有的访问权
                              FILE_SHARE_READ,       // 允许其他线程读取
                              NULL,                  // 缺省的安全性
                              OPEN_ALWAYS,           // 创建或打开文件
                              FILE_ATTRIBUTE_NORMAL, // 普通文件
                              NULL);                 // 无模板文件
  if (hFile != INVALID_HANDLE_VALUE) {
    // 读取当前数据
    LONG nValue(0);
    DWORD dwXfer(0);
    ::ReadFile(hFile,                             // 要读取的文件
               reinterpret_cast<LPVOID>(&nValue), // 缓冲区
               sizeof(nValue),                    // 缓冲区容量
               &dwXfer,                           // 读取的字节数
               NULL);                             // 无重叠I/O
    if (dwXfer == sizeof(nValue)) {
      // 显示当前数据
      std ::cout << "read: " << nValue << std ::endl;
    }
    // 增加数值
    nValue += nAdd;
    // 写回永久存储介质
    ::SetFilePointer(hFile, 0, NULL, FILE_BEGIN);
    ::WriteFile(hFile,                              // 要写入的文件
                reinterpret_cast<LPCVOID>(&nValue), // 数据
                sizeof(nValue),                     // 缓冲区容量
                &dwXfer,                            // 写入的字节数
                NULL);                              // 无重叠I/O
    if (dwXfer == sizeof(nValue)) {
      std ::cout << "write: " << nValue << std ::endl;
    }
    // 清除文件
    ::CloseHandle(hFile);
    hFile = INVALID_HANDLE_VALUE;
  }
  return (0);
}
int main() {
  // 创建100个线程从文件中进行读写
  for (int nTotal = 100; nTotal > 0; --nTotal) {
    // 启动线程
    HANDLE hThread = ::CreateThread(NULL,       // 缺省的安全性
                                    0,          // 缺省的堆栈
                                    ThreadProc, // 线程函数
                                    reinterpret_cast<LPVOID>(1), // 增量
                                    0,     // 无特殊的创建标志
                                    NULL); // 忽略线程id
    // 等待线程完成
    ::WaitForSingleObject(hThread, INFINITE);
    ::Sleep(500); // 放慢显示速度，方便观察
                  // 释放指向线程的句柄
    ::CloseHandle(hThread);
    hThread = INVALID_HANDLE_VALUE;
  }
  system("pause");
  return 0;
}
