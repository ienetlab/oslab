#include <iostream>
#include <string.h>
#include <windows.h>
#include <winioctl.h>
using namespace std;
struct Disk // 关于Disk结构的定义
{
  HANDLE handle;
  DISK_GEOMETRY disk_info;
};

Disk disk;
HANDLE Floppy;
static LONGLONG sector;
bool flag;
Disk physicDisk(char driverLetter); // 函数声明
bool sectorRead(Disk theDisk, unsigned logicSectorNumber,
                char *buffer);                        // 函数声明
void sectorDump(Disk theDisk, int logicSectorNumber); // 函数声明

int main(void) {
  int secNum;
  char choise;
  disk = physicDisk('C'); // 选择要查看的磁盘
  if (flag) {
    while (1) {
      cout << "********************************************" << endl;
      cout
          << "*    0. 查看指定目录信息:     *"
          << endl;
      cout << "*    1. 退出:      "
              " *"
           << endl;
      cin >> choise;
      switch (choise) {
      case '0': // 查看指定的扇区
        cout << "总共有" << (long)sector << "个扇区!" << endl;
        cout << "请输入要查看的扇区号码:" << endl;
        cin >> secNum;
        if (secNum < sector && secNum >= 0)
          sectorDump(disk, secNum);
        else
          cout << "没有这个扇区!!!" << endl;
        // exit
        break;

      case '1':
        exit(0);
        break;
      default:
        cout << "输入错误!!!请重新输入!!!" << endl;
        cout << endl;
      }
    }
  }
  system("pause");
  return 0;
}

Disk physicDisk(char driverLetter) // 创建磁盘句柄，并打开磁盘，返回disk
{
  flag = true;
  DISK_GEOMETRY *temp = new DISK_GEOMETRY;
  char device[] = "\\\\.\\C:";
  device[4] = driverLetter;
  Floppy = INVALID_HANDLE_VALUE;
  Floppy = CreateFile(device,       // 将要打开的驱动器名
                      GENERIC_READ, // 存取的权限
                      FILE_SHARE_READ | FILE_SHARE_WRITE, // 共享的权限
                      NULL,                               // 默认的属性位
                      OPEN_EXISTING, // 创建驱动器方式
                      0,             // 所创建的驱动器类型
                      NULL);         // 指向模板文件的类型

  if (Floppy == INVALID_HANDLE_VALUE) // 打开失败，返回错误代码
  {
    cout << "不能打开磁盘，错误代码: " << GetLastError() << endl;
    if(GetLastError() == 5) {
      cout << "请以管理员身份运行本程序" << endl;
    }
    flag = false;
    return disk;
  }
  DWORD bytereturned;
  BOOL Result;
  disk.handle = Floppy;
  // 获得物理磁盘的几何信息
  Result = DeviceIoControl(Floppy,
                          IOCTL_DISK_GET_DRIVE_GEOMETRY,
                          NULL, 0,
                          temp, sizeof(*temp),
                          &bytereturned,
                          (LPOVERLAPPED)NULL);
  if (!Result) // 如果失败，返回错误代码
  {
    cout << "打开失败" << endl;
    cout << "错误代码位:" << GetLastError() << endl;
    flag = false;
    return disk;
  }
  disk.disk_info = *temp;
  // 输出的是整个物理磁盘的信息
  cout << "柱面数为:" << (unsigned long)disk.disk_info.Cylinders.QuadPart
       << endl
       << endl;
  cout << "每柱面的磁道数为:" << disk.disk_info.TracksPerCylinder << endl
       << endl;
  cout << "每磁道的扇区数为:" << disk.disk_info.SectorsPerTrack << endl << endl;
  cout << "每扇区的字节数为:" << disk.disk_info.BytesPerSector << endl << endl;
  sector = disk.disk_info.Cylinders.QuadPart *
           (disk.disk_info.TracksPerCylinder) *
           (disk.disk_info.SectorsPerTrack);
  double DiskSize =
      (double)disk.disk_info.Cylinders.QuadPart * // 相乘获得磁盘大小
      (disk.disk_info.TracksPerCylinder) * (disk.disk_info.SectorsPerTrack) *
      (disk.disk_info.BytesPerSector);

  cout << "磁盘大小为:" << DiskSize / (1024 * 1024) << "MB." << endl;
  cout << driverLetter << "盘所在的磁盘共有" << (long)sector << "个扇区"
       << endl;
  delete temp;
  return disk;
}

void sectorDump(Disk theDisk, int logicSectorNumber) // 如果读成功，输出读的结果
{
  BOOL sDresult;
  char *buffer = new char[theDisk.disk_info.BytesPerSector];
  sDresult = sectorRead(disk, logicSectorNumber, buffer);
  if (sDresult) {
    for (int i = 0; i < int(theDisk.disk_info.BytesPerSector); i++)
      cout << buffer[i];
    cout << endl; // 换行
    cout << endl;
    cout << endl;
  }
  delete buffer;
}

bool sectorRead(Disk theDisk, unsigned logicSectorNumber,
                char *buffer) // 读指定的扇区
{
  HANDLE handle1 = theDisk.handle;
  BOOL sDresult, Rresult;
  DWORD numberofread;
  // 定位读扇区的位置
  sDresult = SetFilePointer(
      handle1, theDisk.disk_info.BytesPerSector * logicSectorNumber, NULL,
      FILE_BEGIN);
  // 读扇区，并返回读的结果
  Rresult = ReadFile(handle1, buffer, theDisk.disk_info.BytesPerSector,
                     &numberofread, NULL);
  if (Rresult) // 如果读扇区成功
    return true;
  else // 如果失败,返回错误代码
  {
    cout << "读指定的扇区失败" << endl;
    cout << "错误代码为:" << GetLastError() << endl;
    return false;
  }
}
