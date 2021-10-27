// 工程largealloc
// 本代码不支持使用除MSVC以外的编译器进行编译

#include <iostream>
#include <windows.h>
// 尝试以指定长度的零数字填充内存块的简便方法
void FillZero(LPVOID pBlock, LONGLONG dwSize) {
  __try {
    BYTE *arFill = (BYTE *)pBlock;
    for (LONGLONG dwFill = 0; dwFill < dwSize; ++dwFill) {
      arFill[dwFill] = 0;
    }
    std ::cout << "Memory zeroed." << std ::endl;
  }
  __except(EXCEPTION_EXECUTE_HANDLER) {
    std ::cout << "Could not zero memory." << std ::endl;
  }
}
int main() {
  // 简单的常数
  LONGLONG c_dwGigabyte = 1 << 30;
  LONGLONG c_dwMegabyte = 1 << 20;
  // 使用内存分配来获得1GB块
  {
    LPVOID pBlock = ::malloc(c_dwGigabyte);
    ::FillZero(pBlock, c_dwMegabyte);
    ::free(pBlock);
  }
  // 使用虚拟分配以获得物理1GB块
  {
    LPVOID pBlock = ::VirtualAlloc(NULL,            // 不指定起始地址
                                   c_dwGigabyte,    // 要求1GB
                                   MEM_COMMIT,      // 调配物理存储
                                   PAGE_READWRITE); // 对此的读写操作
    ::FillZero(pBlock, c_dwMegabyte);
    ::VirtualFree(pBlock, 0, MEM_RELEASE);
  }
  // 使用虚拟分配以获得虚拟1GB块
  {
    LPVOID pBlock = ::VirtualAlloc(NULL,            // 不指定起始地址
                                   c_dwGigabyte,    // 要求1GB
                                   MEM_RESERVE,     // 不调配物理存储
                                   PAGE_READWRITE); // 对此的读写操作
    ::FillZero(pBlock, c_dwMegabyte);
    ::VirtualFree(pBlock, 0, MEM_RELEASE);
  }
  // 使用虚拟分配调配获得虚拟1GB块，再为其调配1MB物理存储
  {
    LPVOID pBlock = ::VirtualAlloc(NULL,            // 不指定起始地址
                                   c_dwGigabyte,    // 要求1GB
                                   MEM_RESERVE,     // 不调配物理存储
                                   PAGE_READWRITE); // 对此的读写操作
    ::VirtualAlloc(pBlock, c_dwMegabyte, MEM_COMMIT, PAGE_READWRITE);
    ::FillZero(pBlock, c_dwMegabyte);
    ::VirtualFree(pBlock, 0, MEM_RELEASE);
  }
  system("pause");
  return 0;
}
