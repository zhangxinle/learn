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
setbkmode(TRANSPARENT);
SetWorkingImage(NULL);
//创建矿井
MakeMaze(g_utMap.cx,gut_map.cy);
//设置游戏者的位置
g_ptplayer.x = g_utplayer.x * UNIT+UNIT / 2 + g_ptOffset.x;
g_ptplayer.y = g_utplayer.y * UNIT+UNIT / 2 + g_ptOffset.y;
//生成矿井，初始化（矿井跨百度必须为奇数）
void MakeMaze(int width,int height)
{
	if(weith % 2 !=1 || height % 2 != 1)
		return 0;
	int x,y;
	//定义矿井的二维数组，并初始化未墙壁
	//宽高比实际多二，因为两边各有一个哨兵，方便处理数据
	BYTE** aryMap=new BYTE*[width+2];
	for(x=0;x<width+2;x++)
	{
		aryMap[x]=new BYTE[height+2];
		memset(aryMap[x],UNIT_WALL.height+2);
	}
	//定义边界，哨兵功能
	for(x=0;x<=width+1;x++)
		aryMap[x][0]=aryMap[x][height+1]=UNIT_GROUND;
	for(y=0;y<=height;y++)
		aryMap[0][y]=aryMap[width+1][y]=UNIT_GROUND;
	//从任意点开始遍历，生成矿井
	TravelMaze(((rand() % (width-1)) & Oxfffe)+2,(rand() % (height-1)) &Oxfffe + 2,aryMap);
	//将矿井绘制在IMAGE对象上
	SetWorkingImage(&g_imgMap);
	cleardevice();
	for(y=1;y<=heught;y++)
		for(x=1;x<=width;x++)
			if(aryMap[x][y]==UNIT_WALL)
				DrawWall(x,y,aryMap[x-1][y])==UNIT_WALL,
					aryMap[x][y-1]==UNIT_WALL,
					aryMap[x+1][y]==UNIT_WALL,
					aryMAp[x][y+1]==UNIT_WALL;
	SetWorkingImage(NULL);
}
//生成矿井，遍历(x,y)四周
void TravelMaze(int x,int y,BYTE** aryMap)
{
	//定义遍历方向
	int d[4][2]={0,1,1,0,0,-1,-1,0};
	//将便利方向乱序
	int n,t,i;
	for(i=0;i<4;i++)
	{
		n=rand()%4;
		t=d[i][0],d[i][0]=d[n][0],d[n][0]=t;
		t=d[i][1],d[i][1]=d[n][1],d[n][1]=t;

	}
	//尝试周围四个方向
	aryMap[x][y]=UNIT_GROUND;
	for(i=0;i<4;i++)
		if(aryMap[x+2*d[i][0]][y+2*d[i][1]]==UNIT_WALL)
		{
			aryMap[x+d[i][0]][y+d[i][1]]=UNIT_GROUND;
			TravelMaze(x+d[i][0]*2,y+d[i][1]*2,aryMap);//递归

		}
}
//生成矿井；划一面墙
//参数：left/top/right/bottom 表示墙是否与旁边相接
void DrawWall(int x,int y,bool left,bool top,bool right,bool bottom)
{
	//墙壁厚 4 pixel
	int cx,cy;
	cx=x*UNIT-UNIT/2-2+10;
	cy=y*UNIT-UNIT/2-2+10;
	if(left) solidrectangle(x*UNIT-UNIT+10,cy,cx+4,cy+4);
	if(top)	solidrectangle(cx,y*UNIT-UNIT+10,cx+4,cy+4);
	if(right) solidrectangle(cx,cy,x*UNIT+9,cy+4);
	if(bottom) solidrectangle(cx,cy,cx+4,y*UNIT+9);
}
//绘制视野范围内的矿井
void Paint()
{
	//设置绘图目标为Render对象
	SetWorkingImage(&g_imgRender);
	//清空Render对象
	cleardevice();
	//计算视野角度
	double dx,dy,a;
	dx=g_ptMouse.x-g_ptplayer.x;
	dy=g_ptMouse.y-g_ptplayer.y;
	if(dx == 0 && dy != 0)
		a=0;
	else if(dx != 0 && dy != 0)
		a=atan(dy/dx);
	else if(dx == 0)
		a=(dy>0) ? PI/2 : PI*3/2;
	else 
		a=0;
	if(dx<0)
		a += PI;
	if(a<0)
		a += PI*2;
	//绘制灯光
	Lighting(g_ptplayer.x,g_ptplayer.y,a)
	//画游戏者
	DrawPlayer();
	//画出口
	DrawExit();
	//设置绘图目标为窗口
	SetWorkingImage(NULL);
}
//在指定位置和角度照明
void Lighting(int _x,int _y,double _a)
{
	int i;			//循环变量
	int x,y;		//临时坐标
	double a;		//临时角度
	//计算灯光照亮角度区域
	double a1 = _a- LIGHT_A/2;
	double a2 = _a+ LIGHT_A/2;
	for(a=a1;a<a2;a += PI/360)
	{
		for(int r=0;r<LIGHT_R;r++)
		{
			//计算照射到的位置
			x=(int)(_x+cos(a)*r);
			y=(int)(_y+sin(a)*r);
			//光线超出屏幕范围终止
			if(x<0 || x>=WIDTH || y>=HEIGHT-1)
				break;
			//光线碰触到建筑物终止
			if(g_bufMap[y*WIDTH+x])
				break;
			//光线叠加
			g_bufRender[y*WIDTH+x] += Ox202000;//表示淡黄色
		}
	}
	//计算光照扇形区域的最小包围矩形
	//方法：获得七个点的最值：圆心，圆弧两端，○　与xy轴的四个交点
	//第一步：初始化七个点
	POINT pt[7];
	pt[0].x=_x;
	pt[0].y=_y;
	pt[1].x=int(_x+LIGHT_R*cos(a1)+0.5);
	pt[1].y=int(_y+LIGHT_R*sin(a1)+0.5);
	pt[2].x=int(_x+LIGHT_R*cos(a2)+0.5);
       	pt[2].y=int(_y+LIGHT_R*sin(a2)+0.5);
	for(a=ceil(a1*4/(2*PI))*(PI/2),i=3;a<a2;a+=PI/2,i++)
	{
		pt[i].x=int(_x+LIGHT_R*cos(a)+0.5);
	        pt[i].y=int(_y+LIGHT_R*sin(a)+0.5);


	}
	//第二步：求七个点的最大值最小值，得到最小包围矩阵
	i--;
	RECT r={pt[i].x,pt[i].y,pt[i].x,pt[i].y}
	for(--i;i>=0;i--)
	{
		if(pt[i].x<r.left)
			r.left=pt[i].x;
		if(pt[i].x>r.right)
			r.right=pt[i].x;
		if(pt[i].y<r.top)
			r.top=pt[i].y;
		if(pt[i].y>r.bottom)
			r.bottom = pt[i].y;
	}
	//调整矩形范围
	if(r.left<0)
		r.left = 0;
	if(r.top<1)
		r.top = 1;
	if(r.right>=WIDTH)
		r.right = WIDTH-1;
	if(r.bottom>= HEIGHT-1) 
		r.bottom=HEIGHT-2; 
	//修正曝光过度的点
	for(y=r.top;y<=r.bottom;y++)
		for(x=r.left;x<=r.right;x++)
		{
			i=y*WIDTH+x;
			if(g_bufRender[i]>Oxffff00)
				g_bufRender[i]=Oxffff00;
		}
	//将光线模糊处理（避开建筑物）
	for(y=r.top;y<=r.bottom;y++)
                for(x=r.left;x<=r.right;x++)
		{
			i= y*WIDTH+x;
			if(!g_bufMap[i])
				g_bufRender[i]=RGB((GetRValue(g_bufRender[i-WIDTH])+GetRValue(g_bufRender[i-1])+GetRValue(g_bufRender[i])+
					GetRValue(g_bufRender[i+1])+GetRValue(g_bufRender[i+WIDTH]))/5,
					g_bufRender[i]=RGB((GetRValue(g_bufRender[i-WIDTH])+GetRValue(g_bufRender[i-1])+GetRValue(g_bufRender[i])+
                                        GetRValue(g_bufRender[i+1])+GetRValue(g_bufRender[i+WIDTH]))/5,
					g_bufRender[i]=RGB((GetRValue(g_bufRender[i-WIDTH])+GetRValue(g_bufRender[i-1])+GetRValue(g_bufRender[i])+
                                        GetRValue(g_bufRender[i+1])+GetRValue(g_bufRender[i+WIDTH]))/5
		}


}
	
	
