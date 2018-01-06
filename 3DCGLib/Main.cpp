#include <Windows.h>
#include <sstream>
#include <locale>
#include "Window.h"
#include "DirectX11.h"
#include "Model.h"
#include "Matrix.h"
#include "MyMath.h"
#include "Time.h"

using namespace Lib;

const float FPS   = 60.0f;   // 実行したいfps
const float SPEED = 1800.0f; // モデルの移動速度

int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hInstance);
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
    UNREFERENCED_PARAMETER(nCmdShow);

    // ウィンドウの作成
    auto w = std::make_shared<Lib::Window>(L"3DCGLib", 1026, 768);

    // DirectXのインスタンスの取得
    auto& directX = Lib::DirectX11::getInstance();
    directX.initDevice(w);

    // ViewMatrixの初期化
    Vector3 eye = Vector3(0.0f, 1.0f, -5.0f); // カメラの座標
    Vector3 at  = Vector3(0.0f, 1.0f,  0.0f); // 注視対象
    Vector3 up  = Vector3::UP;                // 現在のワールド座標の上方向
    auto view   = Matrix::LookAtLH(eye, at, up);
    directX.setViewMatrix(view);

    // ProjectionMatrixの初期化
    LONG windowWidth  = w->getWindowRect().right  - w->getWindowRect().left;
    LONG windowHeight = w->getWindowRect().bottom - w->getWindowRect().top;
    auto projection   = Matrix::perspectiveFovLH(MyMath::PIDIV2, windowWidth / static_cast<float>(windowHeight), 0.01f, 100.0f);
    directX.setProjectionMatrix(projection);
    
    // モデルの作成
    Model model = Model(36);
    Matrix world;
    world = Matrix::Identify;

    float rotX = 0.0f;
    float rotY = 0.0f;

    // 説明
    MessageBox(w->getHWND(), L"「W」「A」「S」「D」でモデルの回転", L"操作説明", MB_OK | MB_ICONINFORMATION);

    // 更新処理
    Time time = Time();
    int fps = 0;
    float countTime = 0.0f;
    float deltaTime = 0.0f;
    std::ostringstream oss;
    WCHAR wcstr[50];
    size_t size = 0;
    setlocale(LC_ALL, "japanese"); // 後のmbstowcs_sの為の処理
    
    while (w->Update().message != WM_QUIT) {
        directX.begineFrame();

        // FPSの固定
        if (!time.timeOver(1000.0f / FPS)) {
            continue;
        }

        deltaTime = time.getDeltaTime();
        countTime += deltaTime;

        // 1秒に１回行う処理
        if (countTime > 1000.0f) {
            // fpsをデバッガに出力
            oss.str("");
            oss << "fps: " << fps << std::endl;
            mbstowcs_s(&size, wcstr, 20, oss.str().c_str(), _TRUNCATE);
            OutputDebugString(wcstr);
            // 変数のリセット
            fps = 0;
            countTime = 0.0f;
        }
        time.reset();
        ++fps;

        // 回転
        if (w->getKeyDown('W')) {
            rotX += MyMath::PIDIV2 / SPEED * deltaTime;
        }
        if (w->getKeyDown('S')) {
            rotX -= MyMath::PIDIV2 / SPEED * deltaTime;
        }
        if (w->getKeyDown('A')) {
            rotY += MyMath::PIDIV2 / SPEED * deltaTime;
        }
        if (w->getKeyDown('D')) {
            rotY -= MyMath::PIDIV2 / SPEED * deltaTime;
        }

        // オーバーフローの制御
        rotX = MyMath::rollup<float>(rotX, MyMath::PI2);
        rotY = MyMath::rollup<float>(rotY, MyMath::PI2);

        // モデルの制御
        auto mtrX = Matrix::rotateX(rotX);
        auto mtrY = Matrix::rotateY(rotY);
        auto mtt  = Matrix::translate(Vector3(0.0f, -1.0f, 0.0f));
        world = mtrX * mtrY * mtt;
        model.setWorldMatrix(world);

        // 描画
        model.render(Lib::Color(Lib::Color::BLUE));

        directX.endFrame();
    }

    return 0;
}