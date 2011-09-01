#ifndef main_h__
#define main_h__

char* readFile(char* filename, int &size, char* preBuffer = NULL);
void updateNV21();
LRESULT CALLBACK WndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
float FrmGetTime();

class WinCallBack: public IOCallBack{
public:
    void swapRedAndBlue(char* buffer, int w, int h);
    virtual bool SaveImage( char* buffer, int w, int h, int format );
    virtual char* readRes( const char* resname, long* size );
};

#endif // main_h__
