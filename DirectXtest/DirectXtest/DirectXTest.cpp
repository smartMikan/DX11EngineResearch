#include <Windows.h>
#include <d3d11.h>

//ウィンドウ関連
HWND* pHwnd;
//DirectX11ウィンドウ表示関連
ID3D11Device				*pDevice;
IDXGISwapChain				*pSwapChain;
ID3D11DeviceContext			*pDeviceContext;
ID3D11RasterizerState		*pRs;
ID3D11RenderTargetView		*pRtv;
ID3D11Texture2D				*pDepthStencil = nullptr;
ID3D11DepthStencilView		*pDepthStencilView = NULL;

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam);

int CALLBACK WinMain(_In_ HINSTANCE hInstance, _In_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = "Test";
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		return 1;
	}

	HWND hWnd = CreateWindow(
		"Test",
		"DirectXTest",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL,
		NULL,
		hInstance,
		NULL
	);

	if (!hWnd)
	{
		return 1;
	}

	ShowWindow(hWnd,nCmdShow);
	
	//DirectX
	//機能レベルの設定
	D3D_FEATURE_LEVEL featureLevels[4] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3
	};
	UINT featureLevelsNum = 4;
	D3D_FEATURE_LEVEL featureLevelSupported;

	//デバイスの作成
	D3D11CreateDevice(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		featureLevels,
		featureLevelsNum,
		D3D11_SDK_VERSION,
		&pDevice,
		&featureLevelSupported,
		&pDeviceContext
	);

	//ファクトリの取得
	IDXGIDevice* pDxgiDevice;
	pDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDxgiDevice);
	IDXGIAdapter* pDxgiAdapter;
	pDxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDxgiAdapter);
	IDXGIFactory* pIdxgiFactory;
	pDxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&pIdxgiFactory);

	//ウィンドウサイズの取得
	RECT rect;
	GetClientRect(hWnd, &rect);

	//HWNDの保存
	pHwnd = &hWnd;

	//スワップチェインの作成
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = rect.right - rect.left;
	sd.BufferDesc.Height = rect.bottom - rect.top;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	pIdxgiFactory->CreateSwapChain(pDevice, &sd, &pSwapChain);

	pDxgiDevice->Release();
	pDxgiAdapter->Release();
	pIdxgiFactory->Release();

	//レンダリングターゲットの作成
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_TEXTURE2D_DESC backBufferSurfaceDesc;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	pBackBuffer->GetDesc(&backBufferSurfaceDesc);
	pDevice->CreateRenderTargetView(pBackBuffer, NULL, &pRtv);

	//深度ステンシルテクスチャーの作成
	D3D11_TEXTURE2D_DESC descDepth;
	RECT rc;
	GetClientRect(hWnd, &rc);
	ZeroMemory(&descDepth, sizeof(descDepth));
	descDepth.Width = rc.right - rc.left;
	descDepth.Height = rc.bottom - rc.top;
	descDepth.MipLevels = 1;
	descDepth.ArraySize = 1;
	descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	descDepth.SampleDesc.Count = 1;
	descDepth.SampleDesc.Quality = 0;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	descDepth.CPUAccessFlags = 0;
	descDepth.MiscFlags = 0;
	pDevice->CreateTexture2D(&descDepth, NULL, &pDepthStencil);

	//深度ステンシルビューの作成
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
	ZeroMemory(&descDSV, sizeof(descDSV));
	descDSV.Format = descDepth.Format;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0;
	pDevice->CreateDepthStencilView(pDepthStencil, &descDSV, &pDepthStencilView);

	//レンダリングターゲット設定
	pDeviceContext->OMSetRenderTargets(1, &pRtv, pDepthStencilView);

	//ラスタライザの設定
	D3D11_RASTERIZER_DESC descRastrizer;
	ZeroMemory(&descRastrizer, sizeof(descRastrizer));
	descRastrizer.FillMode = D3D11_FILL_SOLID;
	descRastrizer.CullMode = D3D11_CULL_NONE;
	descRastrizer.FrontCounterClockwise = FALSE;
	descRastrizer.DepthClipEnable = TRUE;
	pDevice->CreateRasterizerState(&descRastrizer, &pRs);
	pDeviceContext->RSSetState(pRs);

	//ビューポートの設定
	D3D11_VIEWPORT vp;
	vp.Width = rc.right - rc.left;
	vp.Height = rc.bottom - rc.top;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	pDeviceContext->RSSetViewports(1, &vp);

	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		float f[4]{ 0.0f,1.0f,0.0f,1.0f };
		pDeviceContext->ClearRenderTargetView(pRtv, f);

		pDeviceContext->ClearDepthStencilView(pDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);

		pDeviceContext->RSSetState(pRs);
		pSwapChain->Present(0, 0);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;

	return 0;
}

LRESULT CALLBACK WndProc(_In_ HWND hWnd, _In_ UINT message, _In_ WPARAM wParam, _In_ LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}