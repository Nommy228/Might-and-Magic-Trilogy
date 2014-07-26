/*#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN

#include "DirectX11.h"
#include "RenderStruct.h"

HRESULT D3D11::InitDevice()
{
  HRESULT hr = S_OK;
  RECT rc;
  GetClientRect( g_hWnd, &rc );
  UINT width = rc.right - rc.left;           // получаем ширину
  UINT height = rc.bottom - rc.top;          // и высоту окна
  UINT createDeviceFlags = 0;
  D3D_DRIVER_TYPE driverTypes[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE( driverTypes );

 // Тут мы создаем список поддерживаемых версий DirectX
  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );

  // Сейчас мы создадим устройства DirectX. Для начала заполним структуру,
  // которая описывает свойства переднего буфера и привязывает его к нашему окну.
  DXGI_SWAP_CHAIN_DESC sd;            // Структура, описывающая цепь связи (Swap Chain)
  ZeroMemory( &sd, sizeof( sd ) );    // очищаем ее
  sd.BufferCount = 1;                               // у нас один задний буфер
  sd.BufferDesc.Width = width;                     // ширина буфера
  sd.BufferDesc.Height = height;                          // высота буфера
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // формат пикселя в буфере
  sd.BufferDesc.RefreshRate.Numerator = 75;         // частота обновления экрана
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // назначение буфера - задний буфер
  sd.OutputWindow = g_hWnd;                               // привязываем к нашему окну
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;                               // не полноэкранный режим

  for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
  {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain ( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
    if (SUCCEEDED(hr)) // Если устройства созданы успешно, то выходим из цикла
      break;
  }
  if (FAILED(hr)) return hr;

  // Теперь создаем задний буфер. Обратите внимание, в SDK
  // RenderTargetOutput - это передний буфер, а RenderTargetView - задний.
  pBackBuffer = NULL;
  hr = g_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer );
  if (FAILED(hr)) return hr;

  // интерфейс g_pd3dDevice будет использоваться для создания остальных объектов
  hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
  pBackBuffer->Release();
  if (FAILED(hr)) return hr;

  // Подключаем объект заднего буфера к контексту устройства
  g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

  // Настройка вьюпорта
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  // Подключаем вьюпорт к контексту устройства
  g_pImmediateContext->RSSetViewports (1, &vp);

  return S_OK;
}

//--------------------------------------------------------------------------------------
// Удалить все созданные объекты
//--------------------------------------------------------------------------------------
void D3D11::CleanupDevice()
{
  // Сначала отключим контекст устройства, потом отпустим объекты.
  if( g_pImmediateContext ) g_pImmediateContext->ClearState();

  //  удалеям объекты в порядке, обратном тому, в котором создавали.
  if( g_pRenderTargetView ) g_pRenderTargetView->Release();

  if( g_pSwapChain ) g_pSwapChain->Release();

  if( g_pImmediateContext ) g_pImmediateContext->Release();

  if( g_pd3dDevice ) g_pd3dDevice->Release();
}

void Render()
{

}*/