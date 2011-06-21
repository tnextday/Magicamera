#ifndef main_h__
#define main_h__

char* readFile(char* filename, int &size, char* preBuffer = NULL);
void updateNV21();
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
float FrmGetTime();

class WinCallBack: public SaveImageCallBack{
public:
    virtual bool SaveImage( char* buffer, int w, int h, int format );
};

#endif // main_h__
