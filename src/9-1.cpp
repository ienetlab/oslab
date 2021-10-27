/*****************************************************************
/*
/*****************************************************************
/*-------------------------------------------------------------
/*三种模式
/*	1.	FILE_FLAG_NOBUFFER
/*	2.  FILE_FLAG_SEQUENTIAL_SCAN
/*	3.  FILE_FLAG_BUFFERING|FILE_FLAG_OVERLAPPED
/*------------------------------------------------------------
/*五种操作
/*	1.	charactor	+1
/*	2.	charactor	-1
/*	3.	charactor	-32
/*	4.	charactor	+32
/*	5.	charactor	*1
/*---------------------------------------------------------------
*/
#include <iostream>
#include <windows.h>
using namespace std;
//三种模式
void filter_nobuffer(char *source, char *sink, void (*func)(char *addr));
void filter_sequen(char *source, char *sink, void (*func)(char *addr));
void filter_overlp(char *source, char *sink, void (*func)(char *addr));
//五种不同功能的操作
void f1(char *addr);
void f2(char *addr);
void f3(char *addr);
void f4(char *addr);
void f5(char *addr);

#define BUFFER_SIZE 1024 //定义缓冲区的大小，这里设为1024字节
char *buffer;            //这里的缓冲区被定义成char型

int main() {
  //分配缓冲区
  buffer = new char[BUFFER_SIZE];
  //记录执行filter函数的开始时间
  DWORD tick;

  //用于三种模式各自的平均时间
  DWORD nobuffer_average_time = 0;
  DWORD sequen_average_time = 0;
  DWORD overlp_average_time = 0;
  //采用无缓存模式调用filter函数10次
  cout << "*无文件高速缓存模式正在运行......" << endl;

  DWORD nobuffer_start_time = GetTickCount();

  tick = nobuffer_start_time;
  filter_nobuffer("source.txt", "nobuffer_1.txt", f1);
  cout << "nobuffer 0-1:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_1.txt", "nobuffer_2.txt", f2);
  cout << "nobuffer 1-2:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_2.txt", "nobuffer_3.txt", f3);
  cout << "nobuffer 2-3:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_3.txt", "nobuffer_4.txt", f4);
  cout << "nobuffer 3-4:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_4.txt", "nobuffer_5.txt", f5);
  cout << "nobuffer 4-5:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_5.txt", "nobuffer_6.txt", f1);
  cout << "nobuffer 5-6:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_6.txt", "nobuffer_7.txt", f2);
  cout << "nobuffer 6-7:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_7.txt", "nobuffer_8.txt", f3);
  cout << "nobuffer 7-8:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_8.txt", "nobuffer_9.txt", f4);
  cout << "nobuffer 8-9:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_nobuffer("nobuffer_9.txt", "nobuffer_10.txt", f5);
  DWORD nobuffer_end_time = GetTickCount();
  cout << "nobuffer 9-10:" << nobuffer_end_time - tick << " ms." << endl;

  //采用高速缓存模式调用filter函数10次
  cout << "*使用文件高速缓存模式正在运行......" << endl;

  DWORD sequen_start_time = GetTickCount();

  tick = sequen_start_time;
  filter_sequen("source.txt", "sequen_1.txt", f1);
  cout << "sequen 0-1:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_1.txt", "sequen_2.txt", f2);
  cout << "sequen 1-2:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_2.txt", "sequen_3.txt", f3);
  cout << "sequen 2-3:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_3.txt", "sequen_4.txt", f4);
  cout << "sequen 3-4:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_4.txt", "sequen_5.txt", f5);
  cout << "sequen 4-5:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_5.txt", "sequen_6.txt", f1);
  cout << "sequen 5-6:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_6.txt", "sequen_7.txt", f2);
  cout << "sequen 6-7:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_7.txt", "sequen_8.txt", f3);
  cout << "sequen 7-8:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_8.txt", "sequen_9.txt", f4);
  cout << "sequen 8-9:" << GetTickCount() - tick << " ms. " << endl;

  tick = GetTickCount();
  filter_sequen("sequen_9.txt", "sequen_10.txt", f5);
  DWORD sequen_end_time = GetTickCount();
  cout << "sequen 9-10:" << sequen_end_time - tick << " ms. " << endl;

  //采用异步模式调用filter函数10次
  cout << "异步传输模式正在运行......" << endl;

  DWORD overlp_start_time = GetTickCount();

  tick = overlp_start_time;
  filter_overlp("source.txt", "overlp_1.txt", f1);
  cout << "overlp 0-1:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_1.txt", "overlp_2.txt", f2);
  cout << "overlp 1-2:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_2.txt", "overlp_3.txt", f3);
  cout << "overlp 2-3:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_3.txt", "overlp_4.txt", f4);
  cout << "overlp 3-4:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_4.txt", "overlp_5.txt", f5);
  cout << "overlp 4-5:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_5.txt", "overlp_6.txt", f1);
  cout << "overlp 5-6:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_6.txt", "overlp_7.txt", f2);
  cout << "overlp 6-7:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_7.txt", "overlp_8.txt", f3);
  cout << "overlp 7-8:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_8.txt", "overlp_9.txt", f4);
  cout << "overlp 8-9:" << GetTickCount() - tick << " ms." << endl;

  tick = GetTickCount();
  filter_overlp("overlp_9.txt", "overlp_10.txt", f5);
  DWORD overlp_end_time = GetTickCount();
  cout << "overlp 9-10:" << overlp_end_time - tick << " ms." << endl;

  //输出三种模式下的平均时间以做对比
  cout << "*三种模式的平均用时如下：" << endl;
  cout << ".无模式高速缓存模式平均用时："
       << (nobuffer_end_time - nobuffer_start_time) / 10 << " ms." << endl;
  cout << ".使用文件高速缓存模式平均用时："
       << (sequen_end_time - sequen_start_time) / 10 << " ms." << endl;
  cout << ".异步传输模式平均用时："
       << (overlp_end_time - overlp_start_time) / 10 << " ms." << endl
       << endl;
  system("pause");
  return 0;
}

/*------------------------------------------------------------------*/
//对文件内容进行的5种操作
// f1  +1
// f2  -1
// f3  *1
// f4  >>
// f5  <<
void f1(char *addr) { *addr = (unsigned char)*addr + 1; }
void f2(char *addr) { *addr = (unsigned char)*addr - 1; }
void f3(char *addr) { *addr = (unsigned char)*addr * 1; }
void f4(char *addr) { *addr = (unsigned char)*addr >> 1; }
void f5(char *addr) { *addr = (unsigned char)*addr << 1; }

/*-------------------------------------------------------------*/
//没有文件高速缓存的filter函数
void filter_nobuffer(char *source, char *sink, void (*func)(char *addr)) {
  HANDLE handle_src, handle_dst; //定义原文件与目标文件的句柄
  BOOL cycle;                    //用来判断一个缓冲区是否被写满
  DWORD NumberOfBytesRead, NumberOfBytesWrite, index; //读的字节数、写的字节数
                                                      //打开原文件
  handle_src = CreateFile(source, GENERIC_READ, NULL, NULL, OPEN_EXISTING,
                          FILE_FLAG_NO_BUFFERING, NULL);

  //创建目标文件
  handle_dst =
      CreateFile(sink, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);

  if (handle_src == INVALID_HANDLE_VALUE ||
      handle_dst == INVALID_HANDLE_VALUE) {
    cout << "CreatFile Invocation Error!" << endl;
    exit(1);
  }
  cycle = TRUE;

  //用来cycle判断文件什么时候读完
  while (cycle) {
    //从原文件读数据送入缓冲区
    if (ReadFile(handle_src, buffer, BUFFER_SIZE, &NumberOfBytesRead, NULL) ==
        FALSE) {
      cout << "ReadFile Error!" << endl;
      exit(1);
    }

    //当读不满一个缓冲区时，说明达到文件末尾，结束循环
    if (NumberOfBytesRead < BUFFER_SIZE)
      cycle = FALSE;
    //对文件内容进行的操作
    for (index = 0; index < NumberOfBytesRead; index++)
      func(&buffer[index]);

    //将缓冲区中的数据写入目标文件
    if (WriteFile(handle_dst, buffer, NumberOfBytesRead, &NumberOfBytesWrite,
                  NULL) == FALSE) {
      cout << "WriteFile Error!" << endl;
      exit(1);
    }
  }

  //关闭文件句柄
  CloseHandle(handle_src);
  CloseHandle(handle_dst);
}

/*-------------------------------------------------------------*/

void filter_sequen(char *source, char *sink, void (*func)(char *addr)) {
  HANDLE handle_src, handle_dst; //定义原文件与目标文件的句柄
  BOOL cycle;                    //用来判断一个缓冲区是否被写满
  DWORD NumberOfBytesRead, NumberOfBytesWrite, index; //读的字节数、写的字节数
                                                      //打开原文件
  handle_src = CreateFile(source, GENERIC_READ, NULL, NULL, OPEN_EXISTING,
                          FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  //创建目标文件
  handle_dst = CreateFile(sink, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS,
                          FILE_FLAG_SEQUENTIAL_SCAN, NULL);

  if (handle_src == INVALID_HANDLE_VALUE ||
      handle_dst == INVALID_HANDLE_VALUE) {
    cout << "CreatFile Invocation Error!" << endl;
    exit(1);
  }
  cycle = TRUE;

  //用来cycle判断文件什么时候读完
  while (cycle) {
    //从原文件读数据送入缓冲区
    if (ReadFile(handle_src, buffer, BUFFER_SIZE, &NumberOfBytesRead, NULL) ==
        FALSE) {
      cout << "ReadFile Error!" << endl;
      exit(1);
    }

    //当读不满一个缓冲区时，说明达到文件末尾，结束循环
    if (NumberOfBytesRead < BUFFER_SIZE)
      cycle = FALSE;
    //对文件内容进行的操作
    for (index = 0; index < NumberOfBytesRead; index++)
      func(&buffer[index]);

    //将缓冲区中的数据写入目标文件
    if (WriteFile(handle_dst, buffer, NumberOfBytesRead, &NumberOfBytesWrite,
                  NULL) == FALSE) {
      cout << "WriteFile Error!" << endl;
      exit(1);
    }
  }

  //关闭文件句柄
  CloseHandle(handle_src);
  CloseHandle(handle_dst);
}

/*-------------------------------------------------------------*/

void filter_overlp(char *source, char *sink, void (*func)(char *addr)) {
  HANDLE handle_src, handle_dst; //定义原文件与目标文件的句柄
  BOOL cycle;                    //用来判断一个缓冲区是否被写满
  DWORD NumberOfBytesRead, NumberOfBytesWrite, index,
      dwError;           //读的字节数、写的字节数
  OVERLAPPED overlapped; // overlapped 结构
                         //打开原文件
  handle_src = CreateFile(source, GENERIC_READ, NULL, NULL, OPEN_EXISTING,
                          FILE_FLAG_NO_BUFFERING | FILE_FLAG_OVERLAPPED, NULL);

  //创建目标文件
  handle_dst =
      CreateFile(sink, GENERIC_WRITE, NULL, NULL, CREATE_ALWAYS, NULL, NULL);

  if (handle_src == INVALID_HANDLE_VALUE ||
      handle_dst == INVALID_HANDLE_VALUE) {
    cout << "CreatFile Invocation Error!" << endl;
    exit(1);
  }
  cycle = TRUE;

  //对overlapped结构初始化
  overlapped.hEvent = NULL;
  overlapped.Offset = -BUFFER_SIZE;
  overlapped.OffsetHigh = 0;

  //用来cycle判断文件什么时候读完
  while (cycle) {
    //计算文件的偏移量
    overlapped.Offset = overlapped.Offset + BUFFER_SIZE;

    //从原文件读数据送入缓冲区
    if (ReadFile(handle_src, buffer, BUFFER_SIZE, &NumberOfBytesRead,
                 &overlapped) == FALSE) {
      switch (dwError = GetLastError()) {
        //读文件结尾
      case ERROR_HANDLE_EOF:
        cycle = FALSE;
        break;
      case ERROR_IO_PENDING:
        if (GetOverlappedResult(handle_src, &overlapped, &NumberOfBytesRead,
                                TRUE) == FALSE) {
          cout << "GetOverlappedResult Error!" << endl;
          exit(1);
        }
        break;
      default:
        break;
      }
    }

    //当不满一个个缓存区时，说明达到文件末尾，结束循环
    if (NumberOfBytesRead < BUFFER_SIZE)
      cycle = FALSE;

    //对文件内容进行的操作
    for (index = 0; index < NumberOfBytesRead; index++)
      func(&buffer[index]);

    //将缓冲区中的数据写入目标文件
    if (WriteFile(handle_dst, buffer, NumberOfBytesRead, &NumberOfBytesWrite,
                  NULL) == FALSE) {
      cout << "WriteFile Error!" << endl;
      exit(1);
    }
  }

  //关闭文件句柄
  CloseHandle(handle_src);
  CloseHandle(handle_dst);
}

/************************THE END********************************/
