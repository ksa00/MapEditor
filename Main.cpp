//インクルード
#include <Windows.h>
#include "Direct3D.h"
#include"Quad.h"
#include"Dice.h"
#include"Sprite.h"
#include"Camera.h"
#include"Fbx.h"
#include"Stage.h"
#include"Input.h"
#include"Controller.h"
#include"resource.h"

//リンカ
#pragma comment(lib, "d3d11.lib")
Stage* pStage;
Controller* control;
//定数宣言
const char* WIN_CLASS_NAME = "SampleGame";  //ウィンドウクラス名
const LPCSTR APP_NAME = "サンプルゲーム";  //ウィンドウクラス名
const int WINDOW_WIDTH = 800;  //ウィンドウの幅
const int WINDOW_HEIGHT = 600; //ウィンドウの高さ

//プロトタイプ宣言
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp);

//エントリーポイント
 int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow)
{
//ウィンドウクラス（設計図）を作成
    
    WNDCLASSEX wc;
    wc.cbSize = sizeof(WNDCLASSEX);             //この構造体のサイズ
    wc.hInstance = hInstance;                   //インスタンスハンドル
    wc.lpszClassName = WIN_CLASS_NAME;            //ウィンドウクラス名
    wc.lpfnWndProc = WndProc;                   //ウィンドウプロシージャ
    wc.style = CS_VREDRAW | CS_HREDRAW;         //スタイル（デフォルト）
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); //アイコン
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO);   //小さいアイコン
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);   //マウスカーソル
    wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);                  //メニュー（なし）
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); //背景（白）
    RegisterClassEx(&wc);  //クラスを登録
    //ウィンドウサイズの計算
    RECT winRect = { 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
   AdjustWindowRect(&winRect, WS_OVERLAPPEDWINDOW, TRUE);
    int winW = winRect.right - winRect.left;     //ウィンドウ幅
    int winH = winRect.bottom - winRect.top;     //ウィンドウ高さ

 //ウィンドウを作成
 HWND hWnd = CreateWindow(
       WIN_CLASS_NAME,         //ウィンドウクラス名
     APP_NAME,     //タイトルバーに表示する内容
        WS_OVERLAPPEDWINDOW, //スタイル（普通のウィンドウ）
        CW_USEDEFAULT,       //表示位置左（おまかせ）
        CW_USEDEFAULT,       //表示位置上（おまかせ）
     winW,               //ウィンドウ幅
     winH,               //ウィンドウ高さ
        NULL,                //親ウインドウ（なし）
        NULL,                //メニュー（なし）
        hInstance,           //インスタンス
        NULL                 //パラメータ（なし）
    );

 //ウィンドウを表示
 ShowWindow(hWnd, nCmdShow);
Camera::Initialize();
 //DirectInputの初期化
 Input::Initialize(hWnd);

 control = new Controller;
 control->Initialize();
 //Direct3D初期化
 HRESULT hr= Direct3D::Initialize(WINDOW_WIDTH, WINDOW_HEIGHT, hWnd);
 if (FAILED(hr))
 {
     MessageBox(NULL, "DirectXの初期化に失敗", NULL, MB_OK);
     return hr;
 }


 
 pStage = new Stage;
 pStage->Initialize();
 Fbx* fbx = new Fbx;
 fbx->Load("Asset/BoxDefault.fbx");

 HWND hDlg = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, (DLGPROC)DialogProc);
 //メッセージループ（何か起きるのを待つ）
 MSG msg;
 ZeroMemory(&msg, sizeof(msg));
 while (msg.message != WM_QUIT)
 {
     //メッセージあり
     if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
     {
         TranslateMessage(&msg);
         DispatchMessage(&msg);
     }


     //メッセージなし
     else
     {


         //カメラを更新
         Camera::Update();
         Input::Update();
         pStage->Update();
         //ゲームの処理
         //入力の処理
     
         control->Update();
         Direct3D::BeginDraw();
         pStage->Draw();
        

      
        
        
    




         Direct3D::EndDraw();
        
     }

 }
 //解放処理
 pStage->Release();
 
 SAFE_DELETE(pStage);
 Input::Release();
SAFE_DELETE(control);
 Direct3D::Release();

 
	return 0;
}
//ウィンドウプロシージャ（何かあった時によばれる関数）
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    
	switch (msg)
	{
	case WM_MOUSEMOVE:
		Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_DESTROY:
        if (pStage->HasUnsavedChanges()) { // Assume you create this method
            if (MessageBox(hWnd, TEXT("You have unsaved changes. Do you want to save them?"), TEXT("Unsaved Changes"), MB_YESNO | MB_ICONQUESTION) == IDYES) {
                pStage->Save(); // Save the current file
            }
        }
		PostQuitMessage(0);  //プログラム終了
		return 0;
    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
        case ID_MENU_NEW:
       
            pStage->NewFile();
            return 0;
        case ID_MENU_OPEN:
            pStage -> Open();
            return 0;
        case ID_MENU_SAVE:
            pStage->Save();
            return 0;
        }
	
	}
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
BOOL CALLBACK DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    return pStage->DialogProc(hDlg, msg, wp, lp);
}
