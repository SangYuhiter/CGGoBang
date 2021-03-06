
// CGGobangView.h: CCGGobangView 类的接口
//
#include <vector>
using std::vector;

#include <math.h>
#pragma once

//棋子结点（动态规划数据部分，用于判断棋子输赢）
struct chesspoint {
	int chesscolor;	//棋子颜色:1黑色，-1白色
	bool chessuseflag;	//棋子是否被占用
	int chessflag[8];	//棋子标志，一个棋子周围的8个位置，标识当前组成的线段最大长度,0号为左上角，顺时针计数
};

class CCGGobangView : public CView
{
protected: // 仅从序列化创建
	CCGGobangView();
	DECLARE_DYNCREATE(CCGGobangView)

// 特性
public:
	CCGGobangDoc* GetDocument() const;
	
	//黑棋与白棋棋子与棋子数
	vector <CPoint> player_black;
	int black_pointNum;
	vector <CPoint> player_white;
	int white_pointNum;

	//当前谁可下子
	bool blackplayflag;
	bool whiteplayflag;

	//棋子数组：用于判断输赢
	chesspoint chess_logic[19][19];
	int chess_pointNum;	//总棋子个数

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CCGGobangView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGame19x19();
	afx_msg void OnInitgame();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	// 绘制棋盘背景
	void DrawBackground();
	// 绘制棋子,绘制成功返回1，失败返回-1
	int DrawChessPoint();
	// 规格化落子位置,正常返回1，子越界返回-1，该位置已落子返回-2
	int StandardPointPosition();
	// 绘制一个特定颜色的棋子
	void DrawPoint(int color);
	// 添加棋子到棋子数组中
	void AddChessPoint(int logic_x,int logic_y);
};

#ifndef _DEBUG  // CGGobangView.cpp 中的调试版本
inline CCGGobangDoc* CCGGobangView::GetDocument() const
   { return reinterpret_cast<CCGGobangDoc*>(m_pDocument); }
#endif

