
// WinOGLView.cpp : CWinOGLView クラスの実装
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS は、プレビュー、縮小版、および検索フィルター ハンドラーを実装している ATL プロジェクトで定義でき、
// そのプロジェクトとのドキュメント コードの共有を可能にします。
#ifndef SHARED_HANDLERS
#include "WinOGL.h"
#endif

#include "WinOGLDoc.h"
#include "WinOGLView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CWinOGLView

IMPLEMENT_DYNCREATE(CWinOGLView, CView)

BEGIN_MESSAGE_MAP(CWinOGLView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CURSOR, &CWinOGLView::OnCursor)
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_EDIT, &CWinOGLView::OnEdit)
	ON_UPDATE_COMMAND_UI(ID_EDIT, &CWinOGLView::OnUpdateEdit)
	ON_UPDATE_COMMAND_UI(ID_CURSOR, &CWinOGLView::OnUpdateCursor)
	ON_WM_RBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_SURFACE, &CWinOGLView::OnSurface)
	ON_UPDATE_COMMAND_UI(ID_SURFACE, &CWinOGLView::OnUpdateSurface)
	ON_COMMAND(ID_DELETE, &CWinOGLView::OnDelete)
	ON_UPDATE_COMMAND_UI(ID_DELETE, &CWinOGLView::OnUpdateDelete)
	ON_COMMAND(ID_XYZ, &CWinOGLView::OnXyz)
	ON_UPDATE_COMMAND_UI(ID_XYZ, &CWinOGLView::OnUpdateXyz)
	ON_COMMAND(ID_VIEW, &CWinOGLView::OnView)
	ON_UPDATE_COMMAND_UI(ID_VIEW, &CWinOGLView::OnUpdateView)
	ON_WM_MBUTTONDOWN()
END_MESSAGE_MAP()

// CWinOGLView コンストラクション/デストラクション

CWinOGLView::CWinOGLView() noexcept
{
	// TODO: 構築コードをここに追加します。

}

CWinOGLView::~CWinOGLView()
{
}

BOOL CWinOGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	return CView::PreCreateWindow(cs);
}

// CWinOGLView 描画

void CWinOGLView::InitViewMode()
{
	AC.AllMove(-MoveX, -MoveY);
	MoveX = 0;
	MoveY = 0;
	InitView = true;
}

void CWinOGLView::OnDraw(CDC* pDC)
{
	CWinOGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: この場所にネイティブ データ用の描画コードを追加します。
	wglMakeCurrent(pDC->m_hDC, m_hRC);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT  | GL_DEPTH_BUFFER_BIT
	);

	//xyz軸の表示
	if (AC.axisMode) {
		AC.DrawAxis();
	}

	//図形の表示
	AC.Draw();

	//十字カーソルの表示
	if (AC.cursorMode && !AC.viewMode) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		AC.DrawCursor(rect, NowX, NowY);
	}

	//視点の移動
	if (AC.viewMode) {
		glLoadIdentity();
		glRotatef(RotateX + AddRotateX, 1.0, 0, 0);
		glRotatef(RotateY + AddRotateY, 0, 1.0, 0);
		glScalef(scale, scale, scale);
	}

	//視点の初期化
	if (InitView) {
		glLoadIdentity();
		RotateX = 0;
		RotateY = 0;
		scale = 1.0;
		InvalidateRect(0, false);
		InitView = false;
	}
	glFlush();
	SwapBuffers(pDC->m_hDC);
	wglMakeCurrent(pDC->m_hDC, NULL);
}


// CWinOGLView の診断

#ifdef _DEBUG
void CWinOGLView::AssertValid() const
{
	CView::AssertValid();
}

void CWinOGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinOGLDoc* CWinOGLView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinOGLDoc)));
	return (CWinOGLDoc*)m_pDocument;
}
#endif //_DEBUG


// CWinOGLView メッセージ ハンドラー


void CWinOGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	LDown = true;

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得
	
	ClickX = (double)point.x / rect.Width(); //X正規化座標系
	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1; //Y正規化座標系
	ClickX = ClickX * 2 - 1; //Xワールド座標系
	ClickY = ClickY * 2 - 1; //Yワールド座標系
	double hi;
	
	if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
		hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else {
		hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	//全てのモードに入っていない時に点を追加する
	if(!AC.editMode && !AC.deleteMode && !AC.viewMode){
		AC.CreateShape(ClickX, ClickY);
	}

	//点・形状を動かしてよいかを判定する
	if (!AC.Inclusion(NULL, ClickX, ClickY) || (AC.GetControlPoint() != NULL && AC.Distance(AC.GetControlPoint(), ClickX, ClickY) < select_dist * 2)) {
		NowMove = true;
	}

	RedrawWindow();
	CView::OnLButtonDown(nFlags, point);
}

void CWinOGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	LDown = false;
	NowMove = false;

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	ClickX = (double)point.x / rect.Width(); //X正規化座標系
	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1; //Y正規化座標系
	ClickX = ClickX * 2 - 1; //Xワールド座標系
	ClickY = ClickY * 2 - 1; //Yワールド座標系
	double hi;

	if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
		hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else {
		hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	/* 編集モードまたは削除モードの処理 */
	//左のドラッグをしていなければ選択
	if ((AC.editMode || AC.deleteMode) && !TryToMove) {
		AC.SelectShape(ClickX, ClickY);
	}
	TryToMove = false;

	RedrawWindow();
	CView::OnLButtonUp(nFlags, point);
}

void CWinOGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	if (AC.cursorMode || AC.editMode || AC.deleteMode || AC.viewMode) {
		CRect rect;
		GetClientRect(rect); // 描画領域の大きさを取得

		NowX = (double)point.x / rect.Width(); //X正規化座標系
		NowY = (double)point.y / rect.Height();
		NowY = (NowY - 1) * -1; //Y正規化座標系
		NowX = NowX * 2 - 1; //Xワールド座標系
		NowY = NowY * 2 - 1; //Yワールド座標系
		double hi;

		if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
			hi = (double)rect.Width() / rect.Height();
			NowX = NowX * hi;
		}
		else {
			hi = (double)rect.Height() / rect.Width();
			NowY = NowY * hi;
		}
	}

	/* 編集モードの処理 */
	//点・形状の移動
	if (AC.editMode && LDown) {
		if (NowMove) {
			AC.MovePoint(NULL, NowX, NowY);
			AC.MoveShape(NULL, NowX, NowY);
		}
		if (AC.Distance(ClickX, ClickY, NowX, NowY) >= 0.1) {
			TryToMove = true;
		}
	}
	//形状の回転
	if (AC.editMode && RDown) {
		if (AC.GetBasePoint() != NULL) {
			AC.RotateShape(NULL, AC.GetBasePoint()->GetX(), AC.GetBasePoint()->GetY(), NowX, NowY);
		}
	}

	/* 削除モードの処理 */
	//点の移動
	if (AC.deleteMode && LDown) {
		AC.MovePoint(NULL, NowX, NowY);
	}

	/* 視点変更モードの処理 */
	//視点の平行移動
	if (AC.viewMode && LDown) {
		MoveX = MoveX + NowX - PreX;
		MoveY = MoveY + NowY - PreY;
		AC.AllMove(NowX - PreX, NowY - PreY);
		InitView = true;
	}
	//視点の回転
	if (AC.viewMode && RDown) {
		AddRotateX = (NowY - ClickY) * 180;
		AddRotateY = (NowX - ClickX) * 180;
	}

	PreX = NowX;
	PreY = NowY;
	RedrawWindow();
	CView::OnMouseMove(nFlags, point);
}

BOOL CWinOGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	/* 編集モードの処理 */
	if (AC.editMode) {
		//基点が存在している場合,図形の拡大縮小
		if (AC.GetBasePoint() != NULL) {
			AC.ResizeShape(NULL, AC.GetBasePoint()->GetX(), AC.GetBasePoint()->GetY(), zDelta);
		}
	}

	/*始点変更モードの処理*/
	//視点の拡大縮小
	if (AC.viewMode) {
		int angle = (int)zDelta / 15;
		if(angle > 0) {
			scale *= pow(1.005, abs(angle));
		}
		else if (angle < 0) {
			scale *= pow(0.995, abs(angle));
		}
	}
	
	RedrawWindow();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CWinOGLView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	ClickX = (double)point.x / rect.Width(); //X正規化座標系
	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1; //Y正規化座標系
	ClickX = ClickX * 2 - 1; //Xワールド座標系
	ClickY = ClickY * 2 - 1; //Yワールド座標系
	double hi;

	if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
		hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else {
		hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	/*編集モードの処理*/
	//形状が選択されている状態のとき,基点の追加(位置変更)
	if (AC.editMode && AC.NowSelect() == 3) {
		AC.AddBasePoint(ClickX, ClickY);
	}

	RedrawWindow();
	CView::OnMButtonDown(nFlags, point);
}

void CWinOGLView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	RDown = true;

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	ClickX = (double)point.x / rect.Width(); //X正規化座標系
	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1; //Y正規化座標系
	ClickX = ClickX * 2 - 1; //Xワールド座標系
	ClickY = ClickY * 2 - 1; //Yワールド座標系
	double hi;

	if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
		hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else {
		hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	/* 編集モードの処理 */
	//点の削除,辺上に点を追加
	if (AC.editMode) {
		AC.DeletePoint(NULL);
		AC.DivideEdge(NULL, ClickX, ClickY);
	}

	/*削除モードの処理*/
	//辺以外の全ての削除
	if (AC.deleteMode) {
		AC.Delete(ClickX, ClickY);
	}

	RedrawWindow();
	CView::OnRButtonDown(nFlags, point);
}

void CWinOGLView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。
	RDown = false;

	CRect rect;
	GetClientRect(rect); // 描画領域の大きさを取得

	ClickX = (double)point.x / rect.Width(); //X正規化座標系
	ClickY = (double)point.y / rect.Height();
	ClickY = (ClickY - 1) * -1; //Y正規化座標系
	ClickX = ClickX * 2 - 1; //Xワールド座標系
	ClickY = ClickY * 2 - 1; //Yワールド座標系
	double hi;

	if (rect.Width() > rect.Height()) {     //画面サイズに合わせてX,Yを調整
		hi = (double)rect.Width() / rect.Height();
		ClickX = ClickX * hi;
	}
	else {
		hi = (double)rect.Height() / rect.Width();
		ClickY = ClickY * hi;
	}

	/* 視点変更モードの処理 */
	//視点の回転
	if (AC.viewMode) {
		RotateX = RotateX + AddRotateX;
		RotateY = RotateY + AddRotateY;

		AddRotateX = 0;
		AddRotateY = 0;
	}

	RedrawWindow();
	CView::OnRButtonUp(nFlags, point);
}

void CWinOGLView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	RedrawWindow();
	CView::OnRButtonDblClk(nFlags, point);
}


int CWinOGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: ここに特定な作成コードを追加してください。
	PIXELFORMATDESCRIPTOR pfd =
	{
	sizeof(PIXELFORMATDESCRIPTOR),
	1,
	PFD_DRAW_TO_WINDOW |
	PFD_SUPPORT_OPENGL |
	PFD_DOUBLEBUFFER,
	PFD_TYPE_RGBA,
	32,
	0,0,0,0,0,0,
	0,0,0,0,0,0,0,
	24,
	0,0,
	PFD_MAIN_PLANE,
	0,
	0,0,0
	};
	CClientDC clientDC(this);
	int pixelFormat = ChoosePixelFormat(clientDC.m_hDC,
		&pfd);
	SetPixelFormat(clientDC.m_hDC, pixelFormat, &pfd);
	m_hRC = wglCreateContext(clientDC.m_hDC);

	return 0;
}


void CWinOGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	wglDeleteContext(m_hRC);
}


BOOL CWinOGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ここにメッセージ ハンドラー コードを追加するか、既定の処理を呼び出します。

	return true;
}


void CWinOGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: ここにメッセージ ハンドラー コードを追加します。
	CClientDC clientDC(this);
	wglMakeCurrent(clientDC.m_hDC, m_hRC);
	glViewport(0, 0, cx, cy);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double hi;
	if (cx > cy) {
		hi = (double)cx / cy;
		glOrtho(-1 * hi, 1 * hi, -1, 1 , -100, 100); // 問 5.2 で考える内容
	}
	else {
		hi = (double)cy / cx;
		glOrtho(-1, 1, -1 * hi, 1 * hi , -100, 100); // 問 5.2 で考える内容
	}
	glMatrixMode(GL_MODELVIEW);
	RedrawWindow();
	wglMakeCurrent(clientDC.m_hDC, NULL);
}


void CWinOGLView::OnCursor()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (AC.cursorMode) {
		AC.cursorMode = false;
	}
	else {
		AC.cursorMode = true;
	}

	RedrawWindow();
}

void CWinOGLView::OnUpdateCursor(CCmdUI* pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	if (AC.cursorMode) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnEdit()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (AC.editMode) {
		AC.InitSelect();
		AC.editMode = false;
	}
	else if(AC.CanSelect()){
		AC.deleteMode = false;
		AC.viewMode = false;
		AC.InitSelect();
		InitViewMode();
		AC.editMode = true;
	}

	RedrawWindow();
}

void CWinOGLView::OnUpdateEdit(CCmdUI* pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	if (AC.editMode) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnSurface()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (AC.surfaceMode) {
		AC.surfaceMode = false;
	}
	else {
		AC.surfaceMode = true;
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateSurface(CCmdUI* pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	if (AC.surfaceMode) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnDelete()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (AC.deleteMode) {
		AC.InitSelect();
		AC.deleteMode = false;
	}
	else if(AC.CanSelect()){
		AC.editMode = false;
		AC.viewMode = false;
		AC.InitSelect();
		InitViewMode();
		AC.deleteMode = true;
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateDelete(CCmdUI* pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	if (AC.deleteMode) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnXyz()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (AC.axisMode) {
		AC.axisMode = false;
	}
	else {
		AC.axisMode = true;
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateXyz(CCmdUI* pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	if (AC.axisMode) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}


void CWinOGLView::OnView()
{
	// TODO: ここにコマンド ハンドラー コードを追加します。
	if (AC.viewMode) {
		InitViewMode();
		AC.viewMode = false;
	}
	else {
		AC.deleteMode = false;
		AC.editMode = false;
		AC.InitSelect();
		AC.viewMode = true;
	}

	RedrawWindow();
}


void CWinOGLView::OnUpdateView(CCmdUI* pCmdUI)
{
	// TODO:ここにコマンド更新 UI ハンドラー コードを追加します。
	if (AC.viewMode) {
		pCmdUI->SetCheck(true);
	}
	else {
		pCmdUI->SetCheck(false);
	}
}



