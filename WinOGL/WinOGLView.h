
// WinOGLView.h : CWinOGLView クラスのインターフェイス
//
#include <gl/GL.h>
#pragma once
#include "AdminControl.h"
#include "pch.h"

class CWinOGLView : public CView
{
protected: // シリアル化からのみ作成します。
	CWinOGLView() noexcept;
	DECLARE_DYNCREATE(CWinOGLView)

// 属性
public:
	CWinOGLDoc* GetDocument() const;
	double ClickX = 0, ClickY = 0;
	double NowX = 0, NowY = 0;
	double PreX = 0, PreY = 0;
	double MoveX = 0, MoveY = 0;
	double RotateX = 0, RotateY = 0;
	double scale = 1.0;
	bool InitView = false;
// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 実装
public:
	virtual ~CWinOGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
private:
	HGLRC m_hRC;

	CAdminControl AC;
	boolean LDown = false;
	boolean RDown = false;
	boolean NowMove = false;
	boolean TryToMove = false;
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnCursor();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnEdit();
	afx_msg void OnUpdateEdit(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCursor(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSurface();
	afx_msg void OnUpdateSurface(CCmdUI* pCmdUI);
	afx_msg void OnDelete();
	afx_msg void OnUpdateDelete(CCmdUI* pCmdUI);
	afx_msg void OnXyz();
	afx_msg void OnUpdateXyz(CCmdUI* pCmdUI);
	afx_msg void OnView();
	afx_msg void OnUpdateView(CCmdUI* pCmdUI);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSolid();
	afx_msg void OnUpdateSolid(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument() const
   { return reinterpret_cast<CWinOGLDoc*>(m_pDocument); }
#endif

