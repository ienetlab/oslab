# include <windows.h>
# include <iostream>
// 确定自己的优先权的简单应用程序
int main() {
  // 从当前进程中提取句柄
  HANDLE hProcessThis = :: GetCurrentProcess( ) ;
  // 请求内核提供该进程所属的优先权类
  DWORD dwPriority = :: GetPriorityClass(hProcessThis) ;
    // 发出消息，为用户描述该类
  std :: cout << "Current process priority: " ;
  switch(dwPriority)
  {
    case HIGH_PRIORITY_CLASS:
      std :: cout << "High" ;
      break;
    case NORMAL_PRIORITY_CLASS:
      std :: cout << "Normal" ;
      break;
    case IDLE_PRIORITY_CLASS:
      std :: cout << "Idle" ;
      break;
    case REALTIME_PRIORITY_CLASS:
      std :: cout << "Realtime" ;
      break;
    default:
      std :: cout << "<unknown>" ;
      break;
  }
  std :: cout << std :: endl;
  // 按任意键继续
  getchar();
  // 请求内核提供该进程所属的优先权类
  DWORD dwPriority_1 = :: GetPriorityClass(hProcessThis) ;
    // 发出消息，为用户描述该类
  std :: cout << "Current process priority: " ;
  switch(dwPriority_1)
  {
    case HIGH_PRIORITY_CLASS:
      std :: cout << "High" ;
      break;
    case NORMAL_PRIORITY_CLASS:
      std :: cout << "Normal" ;
      break;
    case IDLE_PRIORITY_CLASS:
      std :: cout << "Idle" ;
      break;
    case REALTIME_PRIORITY_CLASS:
      std :: cout << "Realtime" ;
      break;
    default:
      std :: cout << "<unknown>" ;
      break;
  }
  std :: cout << std :: endl;
  system("pause");
  return 0;
}
