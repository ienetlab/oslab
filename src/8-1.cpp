#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;
struct operation {
  int time;       //起始时间
  int block;      //内存页数
  int oper;       //操作
  int protection; //权限
};

int main() {
  FILE *file;
  file = fopen("opfile", "wb"); //"opfile"为二进制用确定内存操作
  operation op;

  srand((unsigned)time(NULL));

  for (int j = 0; j < 6; j++) {
    for (int i = 0; i < 5; i++) {
      // 0-PAGE_READONLY;
      // 1-PAGE_READWRITE;
      // 2-PAGE_EXECUTE;
      // 3-PAGE_ECECUTE_READ;
      // 4-PAGE_EXECUTE_READWRITE;

      op.time = rand() % 1000; //随即生成等待时间
      printf("op.time=%d", op.time);
      op.block = rand() % 5 + 1; //随即生成块大小
      printf(", op.block=%d\n", op.block);
      op.oper = j;
      op.protection = i;
      fwrite(&op, sizeof(operation), 1, file); //将生成的结构写入文件
    }
  }
  fclose(file);
  system("pause");
  return 0;
}
