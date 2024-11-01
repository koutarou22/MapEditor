#include "Stage.h"
#include"Input.h"
#include"Camera.h"
#include"Direct3D.h"
#include "resource.h"
#include <iostream>

using std::string;

Stage::Stage() : pFbx(), Width(20), Height(20),SelectMode(0),SelectType(0)
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            table[x][z].height = 1;
            table[x][z].type = 0;
        }
    }
 /*   table[0][0].height = 5;
    table[3][3].height = 2;
    table[10][1].height = 3;

    table[0][0].type = 1;
    table[3][3].type = 2;
    table[10][1].type = 3;*/
}

Stage::~Stage()
{
}

void Stage::Initialize()
{
    string fileName[] = { "BoxDefault", "BoxBrick", "BoxGrass", "BoxSand", "BoxWater" };

    for (int i = 0; i < 5; i++)
    {
        pFbx[i] = new FBX;
        string path = "Assets/" + fileName[i] + ".fbx";
        pFbx[i]->Load(path);
    }
 
}

void Stage::Update()
{
    //RayCastDeta data;
    //data.start = XMFLOAT4(100, 5, 0, 0);
    //data.dir = XMFLOAT4(0, -1, 0, 0);

    //Transform trans;
    //trans.position_.x = 1;

    ////あくまでFBXのレイキャストなので
    //pFbx[0]->RayCast(data, trans);

    if (Input::IsMouseButtonDown(0))
    {
        float w = Direct3D::WINDOW_WIDTH / 2;
        float h = Direct3D::WINDOW_HEIGHT / 2;
        //Offsetx,y は0
        //minZ =0 maxZ = 1

        XMMATRIX vp =
        {
             w,  0,  0, 0,
             0, -h,  0, 0,
             0,  0,  1, 0,
             w,  h,  0, 1
        };

        //レイを飛ばす処理
        XMMATRIX matView = Camera::GetViewMatrix();//ビュー行列
        XMMATRIX matProj = Camera::GetProjectionMatrix();//プロジェクション行列

        //ビューポート
        XMMATRIX invVP = XMMatrixInverse(nullptr, vp);
        //プロジェクション変換
        XMMATRIX invProj = XMMatrixInverse(nullptr, matProj);
        //ビュー変換
        XMMATRIX invView = XMMatrixInverse(nullptr, matView);


        XMFLOAT3 mousePosBack = Input::GetMousePosition();
        mousePosBack.z = 0.0f;
        XMFLOAT3 mousePosFront = Input::GetMousePosition();
        mousePosFront.z = 1.0f;
        //①　mousePosFrontをベクトルに変換
        XMVECTOR vMouseFront = XMLoadFloat3(&mousePosFront); // カメラから見える範囲で一番後ろの面を取得
         
        //ここで変換する(前) 
        // ①にinvVP、invPrj、invViewをかける
        vMouseFront = XMVector3TransformCoord(vMouseFront, invVP * invProj * invView);
        //③　mousePosBackをベクトルに変換
        XMVECTOR vMouseBack = XMLoadFloat3(&mousePosBack);
        //④　③にinvVP、invPrj、invViewをかける
        vMouseBack = XMVector3TransformCoord(vMouseBack, invVP * invProj * invView);

        RayCastDeta data;
        XMStoreFloat3(&data.start, vMouseFront);
        XMStoreFloat3(&data.dir, vMouseBack - vMouseFront);
     
        for (int x = 0; x < Width; x++)
        {
            for (int z = 0; z < Height; z++)
            {
                for (int y = 0; y < table[x][z].height; y++)
                {
                    Transform trs;
                    trs.position_.x = x;
                    trs.position_.y = y;
                    trs.position_.z = z;


                    int type = table[x][z].type;
                    pFbx[type]->RayCast(data, trs);
                    if (data.hit == true)
                    {
                       // PostQuitMessage(0);
                        switch (SelectMode)
                        {
                        case 0:
                            table[x][z].height++;
                            break;
                        case 1:
                            if (table[x][z].height > 1)
                            {
                                table[x][z].height--;
                            }
                            break;
                        case 2:
                            table[x][z].type = SelectType;
                            break;
                        }

                        return;
                    }
                }
            }
        }
    }
}

void Stage::Draw()
{
    for (int x = 0; x < Width; x++)
    {
        for (int z = 0; z < Height; z++)
        {
            for (int y = 0; y < table[x][z].height; y++)
            {
                Transform trs;
                trs.position_.x = x;
                trs.position_.y = y;
                trs.position_.z = z;

                int type = table[x][z].type;
                pFbx[type]->Draw(trs);

            }
        }
    }
}

void Stage::Release()
{
    for (int i = 0; i < 5; i++)
    {
        pFbx[i]->Release();
        SAFE_DELETE(pFbx[i]);
    }
}

void Stage::Save()
{
 /*   string s = "";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";
    s += "" + table[0][0].height;
    s += ",";*/

   // ///////////////////////////////////////
   // char d[256];
   // sprintf(d, "%d", table[0][0].height);
   /////////////////////////////////////////

    //ワイド文字列にすれば解決
    WCHAR fileName[MAX_PATH] = L"無題.txt";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.txt)\0*.txt\0")                         //─┬ファイルの種類を選ぶときに出てくるやつ
                      TEXT("すべてのファイル(*.*)\0*.*\0\0");//一番下は\0が二つ     //─┘※拡張子を変えても中身(.txt)は変わらないよ！
    ofn.lpstrFile = fileName;               	//ファイル名
    ofn.nMaxFile = MAX_PATH;               	    //パスの最大文字数// windowさんから260文字分用意されている
    ofn.Flags = OFN_OVERWRITEPROMPT;   		    //フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = L"txt";                  	//デフォルト拡張子

    //「ファイルを保存」ダイアログ
    BOOL selFile;
    selFile = GetSaveFileName(&ofn);//これが名前を付けて保存の画面を出す関数

    //キャンセルしたら中断
    if (selFile == FALSE) return;//もしキャンセルが押されたら



      //１.まっさらなノートを持ってくる(イメージ)
      //↓ファイルハンドル
      HANDLE hFile;            //ファイルのハンドル
      hFile = CreateFile(
      L"Stage.txt",            //ファイル名
      GENERIC_WRITE,           //アクセスモード（書き込み用）
      0,                       //共有（なし）
      NULL,                    //セキュリティ属性（継承しない）
      CREATE_ALWAYS,           //作成方法
      FILE_ATTRIBUTE_NORMAL,   //属性とフラグ（設定なし）
      NULL);                   //拡張属性（なし）
    
      //２.ノートに書き込む(イメージ)
      DWORD dwBytes = 0; //書き込み位置

      //☆ここにMapの情報を追加していく
      string MapData = "";
      for (int x = 0; x < Width; x++)
      {
          for (int z = 0; z < Height; z++)
          {
              MapData += std::to_string(table[x][z].height) + ",";
              MapData += std::to_string(table[x][z].type)   + ",";
          }
      }

      WriteFile(
      hFile,                   //ファイルハンドル
      MapData.c_str(),         //保存するデータ（文字列）
      MapData.size(),          //書き込む文字数
      &dwBytes,                //書き込んだサイズを入れる変数
      NULL);                   //オーバーラップド構造体（今回は使わない）
     
      //３.本棚に戻す しっかり見てる本(text)を閉じること(イメージ)
      CloseHandle(hFile);
}


void Stage::Open()
{
    ///
    //やってることはSave()とほぼ同じ
    ///
    WCHAR fileName[MAX_PATH] = L"無題.txt";  //ファイル名を入れる変数

    //「ファイルを保存」ダイアログの設定
    OPENFILENAME ofn;                         	//名前をつけて保存ダイアログの設定用構造体
    ZeroMemory(&ofn, sizeof(ofn));            	//構造体初期化
    ofn.lStructSize = sizeof(OPENFILENAME);   	//構造体のサイズ
    ofn.lpstrFilter = TEXT("マップデータ(*.txt)\0*.txt\0")                         //─┬ファイルの種類を選ぶときに出てくるやつ
                      TEXT("すべてのファイル(*.*)\0*.*\0\0");//一番下は\0が二つ     //─┘※拡張子を変えても中身(.txt)は変わらないよ！
    ofn.lpstrFile = fileName;               	//ファイル名
    ofn.nMaxFile = MAX_PATH;               	    //パスの最大文字数// windowさんから260文字分用意されている
    ofn.Flags = OFN_FILEMUSTEXIST;   		    //フラグ（同名ファイルが存在したら上書き確認）
    ofn.lpstrDefExt = L"txt";                  	//デフォルト拡張子

    //「ファイルを開く」ダイアログ
    BOOL selFile;
    selFile = GetOpenFileName(&ofn);//名前を付けて保存の画面を出す関数

    //キャンセルしたら中断
    if (selFile == FALSE) return;//もしキャンセルが押されたら



    HANDLE hFile;              //ファイルのハンドル
    hFile = CreateFile(
    L"Stage.txt",              //ファイル名
    GENERIC_READ,              //アクセスモード（読み込み用）textを読み込む
    0,                         //共有（なし）
    NULL,                      //セキュリティ属性（継承しない）
    OPEN_EXISTING,             //作成方法
    FILE_ATTRIBUTE_NORMAL,     //属性とフラグ（設定なし）
    NULL);                     //拡張属性（なし）

    //ファイルのサイズを取得
    DWORD fileSize = GetFileSize(hFile, NULL);//hFileが何バイトなのかを調べてくれる

    //ファイルのサイズ分メモリを確保
    char* MapData;//現在ポインタにしているため先頭の文字しか表示されない(配列なら見れる)
    MapData = new char[fileSize];

    DWORD dwBytes = 0; //読み込み位置　あんま関係ないけどないとエラーが起きるんだそう

    ReadFile(
        hFile,     //ファイルハンドル
        MapData,      //データを入れる変数
        fileSize,  //読み込むサイズ
        &dwBytes,  //読み込んだサイズ
        NULL);     //オーバーラップド構造体（今回は使わない）

    CloseHandle(hFile);//しっかり閉じる(大事)

    //☆ここを追加
    string data(MapData);//Charをstringに変換

    size_t pos = 0;
    
    //こっちでは情報を探す
    if (!data.empty())
    {
        for (int x = 0; x < Width; x++)
        {
            for (int z = 0; z < Height; z++)
            {
                //findで,の位置を探す
                size_t next = data.find(',', pos);//次のposから始まるコンマの位置を探す
              
                //ここでint型に変換しないと怒られるので『stoi』を使って変換
                table[x][z].height = std::stoi(data.substr(pos,next - pos));//『substr』で特定の文字の場所を特定する

                pos = next + 1;//次のdataに向かう

                next = data.find(',', pos);

                table[x][z].type = std::stoi(data.substr(pos, next - pos));

                pos = next + 1;

            }
        }
    }
 
}

//ウィンドウプロシージャの偽物さん
LRESULT Stage::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
    case WM_DESTROY:

        PostQuitMessage(0);  //プログラム終了
        return 0;

    case WM_MOUSEMOVE:
        Input::SetMousePosition(LOWORD(lParam), HIWORD(lParam));
        return 0;

    case WM_COMMAND:

        switch (LOWORD(wParam))
        {
        case IDC_MENU_NEW:
            break;

        case IDC_MENU_OPEN:
            Open();
            break;

        case IDC_MENU_SAVE:
            Save();
            break;
        }
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

//ダイアログプロシージャっぽい赤の他人
BOOL Stage::DialogProc(HWND hDlg, UINT msg, WPARAM wp, LPARAM lp)
{
    
    switch (msg)//何か処理をしたのならTRUEを返す
    {
    case WM_INITDIALOG:
        /*HWND h = GetDlgItem(hDlg, IDC_RADIO_UP);*/
        SendMessage(GetDlgItem(hDlg, IDC_RADIO_UP), BM_SETCHECK, BST_CHECKED, 0);
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"デフォルト");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"レンガ");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"草");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"砂");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM)L"水");
        SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_SETCURSEL, 0,0);
        break;

    case WM_COMMAND:

        switch (LOWORD(wp))
        {
        case IDC_RADIO_UP:
            SelectMode = 0;
            break;

        case IDC_RADIO_DOWN:
            SelectMode = 1;
            break;

        case IDC_RADIO_CHANGE:
            SelectMode = 2;
            break;
        case IDC_COMBO3:
            SelectType = (int)SendMessage(GetDlgItem(hDlg, IDC_COMBO3), CB_GETCURSEL, 0, 0);
            break;
        }
    }

    return FALSE;
}

