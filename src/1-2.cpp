# include <windows.h>// 标准的include
// 告诉连接器与包括MessageBox API函数的user32库进行连接
# pragma comment(lib, "user32.lib" )
// 这是一个可以弹出信息框然后退出的筒单的应用程序
int APIENTRY WinMain( HINSTANCE  /* hInstance */ ,
                      HINSTANCE  /* hPrevInstance */ ,
                      LPSTR    /* lpCmdLine */,
                      int    /* nCmdShow */ )
{
  :: MessageBox(
  NULL,                // 没有父窗口
  "Hello, Windows 7" , // 消息框中的文本
  "Greetings",         // 消息框标题
  MB_OK) ;             // 其中只有一个OK按钮

  // 返回0以便通知系统不进入消息循环
  return(0) ;
}
