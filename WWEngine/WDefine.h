#pragma once
#define HR(x) {hr = x;if(FAILED(hr)){return hr;}}
#define  SAFE_RELEASE(p) {if(p){ (p)->Release(); (p)=NULL; }}
#define	 SAFE_DELETE(p) { if(p){delete(p);(p)=NULL;} }

#define MAX_OBJNAME 50
#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768