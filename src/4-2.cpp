// mappings项目
#include <iostream>
#include <windows.h>
// 仲裁访问的互斥体
static HANDLE g_hMutexMapping = INVALID_HANDLE_VALUE;
// 增加共享内存中的数值的简单线程
static DWORD WINAPI ThreadProc(LPVOID IpParam) {
  // 将参数看作句柄
  HANDLE hMapping = reinterpret_cast<HANDLE>(IpParam);
  // 等待对文件的访问
  ::WaitForSingleObject(g_hMutexMapping, INFINITE);
  // 映射视图
  LPVOID pFile = ::MapViewOfFile(hMapping,            // 保存文件的对象
                                 FILE_MAP_ALL_ACCESS, // 获得读写权限
                                 0,  // 在文件的开头处 (高32位) 开始
                                 0,  // ... (低32位)
                                 0); // 映射整个文件
  if (pFile != NULL) {
    // 将数据看作长整数
    LONG *pnData = reinterpret_cast<LONG *>(pFile);
    // 改动数据
    ++(*pnData);
    // 显示新数值
    std ::cout << "thread: " << ::GetCurrentThreadId() << ", value: " << (*pnData)
               << std ::endl;
    // 释放文件视图
    ::UnmapViewOfFile(pFile);
    pFile = NULL;
  }
  // 释放对文件的访问权
  ::ReleaseMutex(g_hMutexMapping);
  return (0);
}
// 创建共享数据空间
HANDLE MakeSharedFile() {
  // 创建文件映射对象
  HANDLE hMapping =
      ::CreateFileMapping(INVALID_HANDLE_VALUE, // 使用页式文件临时文件
                          NULL,                 // 缺省的安全性
                          PAGE_READWRITE,       // 可读写权
                          0,                    // 最大容量 (高32位)
                          sizeof(LONG),         // ... (低32位)
                          NULL);                // 匿名的
  if (hMapping != INVALID_HANDLE_VALUE) {
    // 在文件映射上创建视图
    LPVOID pData = ::MapViewOfFile(hMapping,            // 保存文件的对象
                                   FILE_MAP_ALL_ACCESS, // 获得读写权
                                   0,  // 在文件的开头处(高32位)开始
                                   0,  // ... (低32位)
                                   0); // 映射整个文件
    if (pData != NULL) {
      ::ZeroMemory(pData, sizeof(LONG));
    }
    // 关闭文件视图
    ::UnmapViewOfFile(pData);
  }
  return (hMapping);
}
int main() {
  // 创建数据文件
  HANDLE hMapping = ::MakeSharedFile();
  // 创建仲裁的互斥体
  g_hMutexMapping = ::CreateMutex(NULL, FALSE, NULL);
  // 根据文件创建100个线程来读写
  for (int nTotal = 100; nTotal > 0; --nTotal) {
    // 启动线程
    HANDLE hThread = ::CreateThread(NULL,       // 缺省的安全性
                                    0,          // 缺省堆栈
                                    ThreadProc, // 线程函数
                                    reinterpret_cast<LPVOID>(hMapping), // 增量
                                    0,     // 无特殊的创建标志
                                    NULL); // 忽略线程id
    // 等待最后的线程释放
    if (nTotal == 1) {
      std ::cout << "all threads created, waiting..." << std ::endl;
      ::WaitForSingleObject(hThread, INFINITE);
    }
    //::Sleep(500); // 放慢显示速度，方便观察
    // 释放指向线程的句柄
    ::CloseHandle(hThread);
    hThread = INVALID_HANDLE_VALUE;
  }
  // 关闭对象
  ::CloseHandle(hMapping);
  hMapping = INVALID_HANDLE_VALUE;
  ::CloseHandle(g_hMutexMapping);
  g_hMutexMapping = INVALID_HANDLE_VALUE;
  system("pause");
  return 0;
}
