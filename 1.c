#include<stdio.h>
#include<time.h>
#include<math.h>
#include<easys.h>
//定义常量
#define PI              3.14592653589  //圆周率
#define UNIT_GROUND         0          //表示地面
#define UNIT_WALL           1          //表示墙
#define LIGHT_A            PI/3        //灯光的角度范围
#define LIGHT_R            120	    //灯光的照射距离
#define WIDTH              480         //矿井的宽度
#define HEIGHT             480	    //矿井的高度
#define SCREENWIDTN        640         //屏幕宽度
#define SCREENHEIGHT       480         //屏幕高度
#define UNIT 			  20          //每个墙壁单位的大小
#define PLAYER_R    	   5		    //游戏者的半径
//定义常量
const  SIZEg_utMap =(23,23);		    //矿井地图的尺寸（基于UNIT单位）
const  POINT 	g_utPlayer=(1,1);	    //游戏者的位置（基于UNIT单位）
const  POINT   g_utExit=(21,22);	    //出口位置（基于UNIT单位）
const  POINT   g_ptOffset=(10,10);	    //矿井显示在屏幕上的偏移量
//////////////////////////////////////////////////////////////////////
//定义全局变量//
POINT g_ptPlayer;					//游戏者的位置
POINT g_ptMouse;					//鼠标位置
IMAGE g_imgMap(WIDTH,HEIGHT);			//矿井平面图
DWORD* g_bufMap;					//矿井平面图的另存指针
IMAGE g_imgRender(WIDTH,HEIGHT);		//渲染
DWORD* g_bufRender;  				//渲染的另存指针
DWORD* g_bufScreen;					//屏幕的另存指针
//枚举用户的控制命令
enum CMD( CMD_QUIT =1;CMD_UP=2;CMD_DOWN=4;CMD_LEFT=8;CMD_RIGRHT=16;CMD_RESTART=32);
//////////////////////////////////////////////////////////////////////
//函数声明//
//初始化//
void Welcome();					//控制游戏界面
void ReadyGo();					//准备开始游戏
void InitGame();					//初始化游戏数据
//矿井生成
void MakeMaze(int width,int height);    //初始化（备注：高度必须是奇数）
void TraveMaze(int x,int y,BYTE** aryMap);//遍历(x,y)四周
void DrawWall(int x,int y,bool left,bool top,bool right;bool bottom);
//画一面墙
//绘制
void Paint();						//绘制视野范围内的矿井
void Lighting(int _x,int _y,double _a);	//在指定位置和角度“照明”
void DrawPlayer();					//绘制游戏者
void DrawExit();					//绘制出口
//处理用户控制
int GETCmd();						//获取用户输入的命令
void OnUp();						//向上移动
void OnLeft();						//向左移动
void OnRight();					//向右移动
void OnDown();						//向下移动
bool CheckWin();					//检查是否到出口
/////////////////////////////////////////////////////////////////////
//函数定义//
//主程序
void main()
{
	//初始化
	initgraph(SCREENWIDTH,SCREENHEIGHT); 	//创建绘图窗口
	srand((unsigned)time(NULL));			//设置随机种子
	//显示主页面
	Welcome();
	//游戏过程
	int c；
	do{
		ReadyGo();
		while(true)
		{
			//获得用户输入
			c=GetCmd;
			//处理用户输入
			if(c&CMD_UP)      OnUp();
			if(c&CMD_DOWN)	   OnDown();
			if(c&CMD_LEFT)    OnLeft();
			if(c&CMD_RIGHT)   Onright();
			if(c&CMD_RESTART) 
			{
				if(MessageBox(GetHWnd(),_T("您确定重来一局吗？"),_T("询问"),				MB_OKCANCEL | MB_ICONQUESTION)==IDOK)
					break;
			}
			if(c&CMD_QUIT)
			{
				if(MessageBox(GetHWnd(),_T("您确定要退出游戏吗？"),_T("询问"),				MB_OKCANCEL | MB_ICONQUESTION)==IDOK)
						break;
			}       
			//绘制场景
			Paint();
			//判断是否走出矿井
			if(CheckWin())
			{
					//是否再来一局
					HWND hwnd=GetHWnd();
					if(MessageBox(hwnd,_T("恭喜你走出矿井！\n您想再来一局吗？"),_T("恭喜"),MB_YESNO | MB_ICONQUESTION)!=IDYES)
			          c=CMD_QUIT;
					break;
			}
			//延时
			Sleep(16);
		}
	

	}
	while(!(c&CMD_QUIT))
	//关闭图形模式
	closegraph;
}
//准备开始游戏
void ReadyGo
{
	//初始化
	InitGame();
	//停电前兆
	int time[7]={1000,50,500,50,50,50,50};
	int i,x,y;
	for(i=0;i<7;i++)
	{
		if(i%2==0)
		{
			putimage(0,0,&g_imgMap);
			DrawPlayer();
			DrawExit();
		}
		else
			clearrectangle(0,0,WIDTH-1,HEIGHT-1);
		Sleep(time[i]);
	}
	//电力缓慢中断
	for(i=255;i>=0;i--)
	{
		for(y=(HEIGHT-1)*SCREENWIDTH;y>=0;y-=SCREENWIDTH)
			for(x=0;x<WIDTH;x++)
				if(g_bufScreen[y+x]!=0)
					g-bufScreen[y+x]=g_bufScreen[y+x]-0x050505;
		FlushBatchDraw();
		DrawPlayer();
		DrawExit();
		Sleep(50);	
	}
	//绘制游戏区
	Paint();
}	
//绘制游戏界面
void Welcome()
{
	setfillcolor(DARKGRAY);
	solidrectangle(WIDTH,0,SCREENWIDTH-1,SCREENHEIGHT-1);
	//设置字体样式
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
	//绘制标题
	settextstyle(24,0,,_T("宋体"));
	outtexttxy(512,40,_T("矿井逃生"))；
	//绘制操作说明
	RECT r ={488,100,632,470}；
	settextstyle(12,0,_T("宋体"));
     drawtext(_T("[游戏说明]\n 矿井里的电路又出问题了。迅速借助你的头灯，在漆黑的矿井里找到出口逃出去吧。\n\n[控制说明]\n方向键：移动\nA/S/D/W: 移动\n 鼠标:控制照射方向\nF2:  重来一局\n ESC:退出游戏"),&r,DT_WORDBREAK);
	outtextxy(495,465,_T("Powered by yw80@qq.com"));
}
//初始化游戏数据
void InitGame()
{
	//获得窗口显存指针
	g_bufRender=GetImageBuffer(&g_imgRender);
	g_bufMap=GetImageBuffer(&g_imgMap);
	g_bufScreen=GetImageBuffer(NULL);
	//设置Render环境
     SetWorkkingImage(&g_imgRender);
}
	
