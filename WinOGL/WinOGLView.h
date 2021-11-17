
// WinOGLView.h : CWinOGLView クラスのインターフェイス
//
#include <gl/GL.h>
#pragma once
#include "AdminControl.h"

class CWinOGLView : public CView
{
protected: // シリアル化からのみ作成します。
	CWinOGLView() noexcept;
	DECLARE_DYNCREATE(CWinOGLView)

// 属性
public:
	CWinOGLDoc* GetDocument() const;
	double ClickX, ClickY;
	double NowX = 0, NowY = 0;
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
};

#ifndef _DEBUG  // WinOGLView.cpp のデバッグ バージョン
inline CWinOGLDoc* CWinOGLView::GetDocument() const
   { return reinterpret_cast<CWinOGLDoc*>(m_pDocument); }
#endif

