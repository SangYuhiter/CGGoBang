
// CGGobangView.cpp: CCGGobangView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "CGGobang.h"
#endif

#include <windows.h>
#include "CGGobangDoc.h"
#include "CGGobangView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

COLORREF blackcolor = RGB(0, 0, 0), whitecolor = RGB(255, 255, 255);

// CCGGobangView

IMPLEMENT_DYNCREATE(CCGGobangView, CView)

BEGIN_MESSAGE_MAP(CCGGobangView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_GAME19X19, &CCGGobangView::OnGame19x19)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_INITGAME, &CCGGobangView::OnInitgame)
END_MESSAGE_MAP()

// CCGGobangView 构造/析构

CCGGobangView::CCGGobangView()
{
	// TODO: 在此处添加构造代码

	player_black.clear();
	black_pointNum = 0;
	player_white.clear();
	white_pointNum = 0;

	blackplayflag = FALSE;
	whiteplayflag = FALSE;

	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			chess_logic[i][j].chesscolor = 0;
			chess_logic[i][j].chessuseflag = FALSE;
			for (int k = 0; k < 8; k++)
			{
				chess_logic[i][j].chessflag[k] = 0;
			}
		}
	}
	chess_pointNum = 0;
}

CCGGobangView::~CCGGobangView()
{
}

BOOL CCGGobangView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CCGGobangView 绘图

void CCGGobangView::OnDraw(CDC* /*pDC*/)
{
	CCGGobangDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CCGGobangView 打印

BOOL CCGGobangView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CCGGobangView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CCGGobangView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CCGGobangView 诊断

#ifdef _DEBUG
void CCGGobangView::AssertValid() const
{
	CView::AssertValid();
}

void CCGGobangView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCGGobangDoc* CCGGobangView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCGGobangDoc)));
	return (CCGGobangDoc*)m_pDocument;
}
#endif //_DEBUG

void CCGGobangView::OnGame19x19()
{
	// TODO: 在此添加命令处理程序代码
	DrawBackground();
	//执黑先行
	blackplayflag = TRUE;
}


// 绘制棋盘背景
void CCGGobangView::DrawBackground()
{
	// TODO: 在此处添加实现代码.
	CDC* pCD = GetWindowDC();
	CPoint p1, p2, p3, p4;
	int i,dx,dy;
	//棋盘在角上
	dx = 0, dy = 0;
	for (i = 0; i < 19; i++)
	{
		p1.x = 50 * (i + 1) + dx; p1.y = 50 + dy;
		p2.x = 50 * (i + 1) + dx; p2.y = 950 + dy;
		pCD->MoveTo(p1);
		pCD->LineTo(p2);
		p3.x = 50 + dx; p3.y = 50 * (i + 1) + dy;
		p4.x = 950 + dx; p4.y = 50 * (i + 1) + dy;
		pCD->MoveTo(p3);
		pCD->LineTo(p4);
	}
	ReleaseDC(pCD);
	return;
}

void CCGGobangView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (blackplayflag && !whiteplayflag)
	{
		player_black.push_back(point);
		black_pointNum++;
		chess_pointNum++;
		if (DrawChessPoint())
		{
			blackplayflag = FALSE;
			whiteplayflag = TRUE;
		}
		return;
	}
	if (whiteplayflag && !blackplayflag)
	{
		player_white.push_back(point);
		white_pointNum++;
		chess_pointNum++;
		if (DrawChessPoint())
		{
			whiteplayflag = FALSE;
			blackplayflag = TRUE;
		}
		return;
	}
	CView::OnLButtonDown(nFlags, point);
}


// 绘制棋子,绘制成功返回1，失败返回-1
int CCGGobangView::DrawChessPoint()
{
	// TODO: 在此处添加实现代码.
	//规格化落子位置
	if (StandardPointPosition())
	{
		//黑子
		if (blackplayflag && !whiteplayflag)
		{
			DrawPoint(blackcolor);
		}
		if (whiteplayflag && !blackplayflag)
		{
			DrawPoint(whitecolor);
		}
		return 1;
	}
	return -1;
}


// 规格化落子位置,正常返回1，子越界返回-1，该位置已落子返回-2
int CCGGobangView::StandardPointPosition()
{
	// TODO: 在此处添加实现代码.
	//中心点为（500，500）
	int cx = 500, cy = 500;
	int x, y,logic_x,logic_y;
	if (blackplayflag)
	{
		x = player_black[black_pointNum - 1].x;
		y = player_black[black_pointNum - 1].y;
	}
	if (whiteplayflag)
	{
		x = player_white[white_pointNum - 1].x;
		y = player_white[white_pointNum - 1].y;
	}
	//子越界
	if (abs(x - cx) > 9 * 50 + 25 || abs(y - cy) > 9 * 50 + 25)
	{
		if (blackplayflag)
		{
			if (black_pointNum != 0)
			{
				player_black.pop_back();
				black_pointNum--;
				chess_pointNum--;
			}
		}
		if (whiteplayflag)
		{
			if (white_pointNum != 0)
			{
				player_white.pop_back();
				white_pointNum--;
				chess_pointNum--;
			}
		}
		return -1;
	}
	//规格化过程
	if (abs(x - cx) % 50 > 25)	//子在中心左边靠近左边结点或子在中心右边靠近右边结点
		x = (x - cx) / 50 + 1;
	else
		x = (x - cx) / 50;
	if (blackplayflag)
	{
		if (x - cx > 0)
		{
			player_black[black_pointNum - 1].x = cx - 50 * x;
			logic_x = 10 + x;
		}
		else
		{
			player_black[black_pointNum - 1].x = cx + 50 * x;
			logic_x = 10 - x;
		}
			
	}
	if (whiteplayflag)
	{
		if (x - cx > 0)
		{
			player_white[white_pointNum - 1].x = cx - 50 * x;
			logic_x = 10 + x;
		}
		else
		{
			player_white[white_pointNum - 1].x = cx + 50 * x;
			logic_x = 10 - x;
		}
			
	}
	if (abs(y - cy) % 50 > 25)	//子在中心上边靠近上边结点或子在中心下边靠近下边结点
		y = (y - cy) / 50 + 1;
	else
		y = (y - cy) / 50;
	if (blackplayflag)
	{
		if (y - cy > 0)
		{
			player_black[black_pointNum - 1].y = cy - 50 * y;
			logic_y = 10 + y;
		}
		else
		{
			player_black[black_pointNum - 1].y = cy + 50 * y;
			logic_y = 10 - y;
		}
	}
	if (whiteplayflag)
	{
		if (y - cy > 0)
		{
			player_white[white_pointNum - 1].y = cy - 50 * y;
			logic_y = 10 + y;
		}
		else
		{
			player_white[white_pointNum - 1].y = cy + 50 * y;
			logic_y = 10 - y;
		}
	}
	//该位置已落子
	if (chess_logic[logic_x][logic_y].chessuseflag)
	{
		if (blackplayflag)
		{
			if (black_pointNum != 0)
			{
				player_black.pop_back();
				black_pointNum--;
				chess_pointNum--;
			}
		}
		if (whiteplayflag)
		{
			if (white_pointNum != 0)
			{
				player_white.pop_back();
				white_pointNum--;
				chess_pointNum--;
			}
		}
		return -2;
	}
	else
		AddChessPoint(logic_x, logic_y);
	if (chess_pointNum >= 9)
	{
		for (int i = 0; i < 8; i++)
		{
			if (chess_logic[logic_x][logic_y].chessflag[i] >= 5)
			{
				CString str;
				if (blackplayflag)
				{
					str = "黑方赢";
				}
				if (whiteplayflag)
				{
					str = "白方赢";
				}
				MessageBox(str);
			}
		}
	}
	return 1;
}


// 绘制一个特定颜色的棋子
void CCGGobangView::DrawPoint(int color)
{
	// TODO: 在此处添加实现代码.
	CDC *pDC = GetWindowDC();
	CBrush newBrush;
	newBrush.CreateSolidBrush(color);
	pDC->SelectObject(newBrush);
	if(blackplayflag)
		pDC->Ellipse(player_black[black_pointNum - 1].x - 20, player_black[black_pointNum - 1].y - 20, 
					 player_black[black_pointNum - 1].x + 20, player_black[black_pointNum - 1].y + 20);
	if (whiteplayflag)
		pDC->Ellipse(player_white[white_pointNum - 1].x - 20, player_white[white_pointNum - 1].y - 20,
					 player_white[white_pointNum - 1].x + 20, player_white[white_pointNum - 1].y + 20);
	return;
}


// 添加棋子到棋子数组中
void CCGGobangView::AddChessPoint(int logic_x,int logic_y)
{
	// TODO: 在此处添加实现代码.
	if (blackplayflag)
	{
		chess_logic[logic_x][logic_y].chesscolor = 1;
	}
	if (whiteplayflag)
	{
		chess_logic[logic_x][logic_y].chesscolor = -1;
	}
	chess_logic[logic_x][logic_y].chessuseflag = TRUE;
	//动态规划
	for (int i = 0; i < 8; i++)
	{
		switch (i)
		{
		case 0:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x - 1][logic_y - 1].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[0] = chess_logic[logic_x - 1][logic_y - 1].chessflag[0] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[0] = 1;
			break;
		case 1:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x][logic_y - 1].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[1] = chess_logic[logic_x][logic_y - 1].chessflag[1] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[1] = 1;
			break;
		case 2:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x + 1][logic_y - 1].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[2] = chess_logic[logic_x + 1][logic_y - 1].chessflag[2] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[2] = 1;
			break;
		case 3:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x + 1][logic_y].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[3] = chess_logic[logic_x + 1][logic_y].chessflag[3] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[3] = 1;
			break;
		case 4:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x + 1][logic_y + 1].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[4] = chess_logic[logic_x + 1][logic_y + 1].chessflag[4] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[4] = 1;
			break;
		case 5:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x][logic_y + 1].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[5] = chess_logic[logic_x][logic_y + 1].chessflag[5] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[5] = 1;
			break;
		case 6:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x - 1][logic_y + 1].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[6] = chess_logic[logic_x - 1][logic_y + 1].chessflag[6] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[6] = 1;
			break;
		case 7:
			if (chess_logic[logic_x][logic_y].chesscolor == chess_logic[logic_x - 1][logic_y].chesscolor)
				chess_logic[logic_x][logic_y].chessflag[7] = chess_logic[logic_x - 1][logic_y].chessflag[7] + 1;
			else
				chess_logic[logic_x][logic_y].chessflag[7] = 1;
			break;
		default:
			break;
		}
	}
	return;
}

void CCGGobangView::OnInitgame()
{
	// TODO: 在此添加命令处理程序代码
	player_black.clear();
	black_pointNum = 0;
	player_white.clear();
	white_pointNum = 0;

	blackplayflag = FALSE;
	whiteplayflag = FALSE;

	for (int i = 0; i < 19; i++)
	{
		for (int j = 0; j < 19; j++)
		{
			chess_logic[i][j].chesscolor = 0;
			for (int k = 0; k < 8; k++)
			{
				chess_logic[i][j].chessflag[k] = 0;
			}
		}
	}
	Invalidate();
}
