#pragma once

#define HR(X) {if(FAILED(X)) { MessageBoxW(0,L"Create Failed",0,0); return false;}}
#define ReleaseCOM(x) { if (x) { x->Release(); x = 0; } }
#define ReleaseShader(x) { if (x) { x->Shutdown();delete x; x = 0; } }

