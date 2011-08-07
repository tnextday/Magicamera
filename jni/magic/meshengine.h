#ifndef _meshengine_h_
#define _meshengine_h_
#include <GLES2/gl2.h>
#include "glutils/mesh.h"
#include "magicengine.h"

struct Vertex {
    GLfloat x;
    GLfloat y;
};

class MeshEngine : public Mesh, MagicEngine{

private:
    //坐标差值
    GLfloat*        m_DeltaVertex;
    //目标坐标
    GLfloat*        m_DestVertex;
    //原始坐标，恢复用
    GLfloat*        m_OrgiVertex;
    int             m_BufferCount;
    
    bool            m_bAnimating;
    float           m_Duration;
    float           m_Elapsed;    //逝去时间

    bool            m_bMeshChanged;


    //上次鼠标坐标
    float    m_lastX;
    float    m_lastY;

private:
    //************************************
    // Method:    moveMesh
    // Description: 以ox,oy为圆心，在半径r范围内，进行mxy的大小的形变
    // Parameter: float ox 圆心x坐标
    // Parameter: float oy 圆心y坐标
    // Parameter: float mx x轴偏移距离
    // Parameter: float my y轴偏移距离
    // Parameter: float r 范围半径
    // Returns:   void
    //************************************
    void moveMesh(float ox, float oy, float mx, float my, float r);


    //设置完坐标后，调用此函数备份原始坐标
    void backupOrigVertex();

    //set destvertex then start animating
    //Animated duration in sec
    void startAnimating(float duration);
    void stopAnimating();

    void generateMesh( int w, int h );

public:
    MeshEngine();
    ~MeshEngine();

    void init(int width, int height);

    virtual void update(GLfloat delta);

    //还原成原始图像
    void restore();

    virtual void onDraw(Texture *texutre);

    virtual bool onTouchDown(float x, float y);
    virtual bool onTouchDrag(float x, float y);
    virtual bool onTouchUp(float x, float y);

    virtual void finish();

    virtual bool onInit();

    virtual void start();

    virtual bool isFinished();

    virtual EngineType type();
};
#endif // _meshengine_h_