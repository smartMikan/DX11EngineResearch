#pragma once
#include <windows.h>

///SetObjectNameInGraphicDebug created by d3ddevice
template<UINT TNameLength>
inline void D3D11SetDebugObjectName(_In_ ID3D11DeviceChild* resource, _In_ const char(&name)[TNameLength])
{
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
}


///SetDXGIObjectNameInGraphicDebug
template<UINT TNameLength>
inline void DXGISetDebugObjectName(_In_ IDXGIObject* resource, _In_ const char(&name)[TNameLength])
{
	resource->SetPrivateData(WKPDID_D3DDebugObjectName, TNameLength - 1, name);
}