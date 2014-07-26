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
  UINT width = rc.right - rc.left;           // �������� ������
  UINT height = rc.bottom - rc.top;          // � ������ ����
  UINT createDeviceFlags = 0;
  D3D_DRIVER_TYPE driverTypes[] =
  {
    D3D_DRIVER_TYPE_HARDWARE,
    D3D_DRIVER_TYPE_WARP,
    D3D_DRIVER_TYPE_REFERENCE,
  };
  UINT numDriverTypes = ARRAYSIZE( driverTypes );

 // ��� �� ������� ������ �������������� ������ DirectX
  D3D_FEATURE_LEVEL featureLevels[] =
  {
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
  };
  UINT numFeatureLevels = ARRAYSIZE( featureLevels );

  // ������ �� �������� ���������� DirectX. ��� ������ �������� ���������,
  // ������� ��������� �������� ��������� ������ � ����������� ��� � ������ ����.
  DXGI_SWAP_CHAIN_DESC sd;            // ���������, ����������� ���� ����� (Swap Chain)
  ZeroMemory( &sd, sizeof( sd ) );    // ������� ��
  sd.BufferCount = 1;                               // � ��� ���� ������ �����
  sd.BufferDesc.Width = width;                     // ������ ������
  sd.BufferDesc.Height = height;                          // ������ ������
  sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;      // ������ ������� � ������
  sd.BufferDesc.RefreshRate.Numerator = 75;         // ������� ���������� ������
  sd.BufferDesc.RefreshRate.Denominator = 1;
  sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ���������� ������ - ������ �����
  sd.OutputWindow = g_hWnd;                               // ����������� � ������ ����
  sd.SampleDesc.Count = 1;
  sd.SampleDesc.Quality = 0;
  sd.Windowed = TRUE;                               // �� ������������� �����

  for( UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++ )
  {
    g_driverType = driverTypes[driverTypeIndex];
    hr = D3D11CreateDeviceAndSwapChain ( NULL, g_driverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &g_featureLevel, &g_pImmediateContext );
    if (SUCCEEDED(hr)) // ���� ���������� ������� �������, �� ������� �� �����
      break;
  }
  if (FAILED(hr)) return hr;

  // ������ ������� ������ �����. �������� ��������, � SDK
  // RenderTargetOutput - ��� �������� �����, � RenderTargetView - ������.
  pBackBuffer = NULL;
  hr = g_pSwapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer );
  if (FAILED(hr)) return hr;

  // ��������� g_pd3dDevice ����� �������������� ��� �������� ��������� ��������
  hr = g_pd3dDevice->CreateRenderTargetView( pBackBuffer, NULL, &g_pRenderTargetView );
  pBackBuffer->Release();
  if (FAILED(hr)) return hr;

  // ���������� ������ ������� ������ � ��������� ����������
  g_pImmediateContext->OMSetRenderTargets( 1, &g_pRenderTargetView, NULL );

  // ��������� ��������
  D3D11_VIEWPORT vp;
  vp.Width = (FLOAT)width;
  vp.Height = (FLOAT)height;
  vp.MinDepth = 0.0f;
  vp.MaxDepth = 1.0f;
  vp.TopLeftX = 0;
  vp.TopLeftY = 0;

  // ���������� ������� � ��������� ����������
  g_pImmediateContext->RSSetViewports (1, &vp);

  return S_OK;
}

//--------------------------------------------------------------------------------------
// ������� ��� ��������� �������
//--------------------------------------------------------------------------------------
void D3D11::CleanupDevice()
{
  // ������� �������� �������� ����������, ����� �������� �������.
  if( g_pImmediateContext ) g_pImmediateContext->ClearState();

  //  ������� ������� � �������, �������� ����, � ������� ���������.
  if( g_pRenderTargetView ) g_pRenderTargetView->Release();

  if( g_pSwapChain ) g_pSwapChain->Release();

  if( g_pImmediateContext ) g_pImmediateContext->Release();

  if( g_pd3dDevice ) g_pd3dDevice->Release();
}

void Render()
{

}*/