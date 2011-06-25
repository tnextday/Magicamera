#ifndef _meshengine_h_
#define _meshengine_h_
#include "mesh.h"
#include <GLES2/gl2.h>

struct Vertex {
    GLfloat x;
    GLfloat y;
};

class MeshEngine : public Mesh{

private:
    //坐标差值
    GLfloat*    m_DeltaVertex;
    //目标坐标
    GLfloat*    m_DestVertex;
    //原始坐标，恢复用
    GLfloat*    m_OrgiVertex;
    int            m_BufferCount;
    
    bool        m_bAnimating;
    float        m_Duration;
    float        m_Elapsed;    //逝去时间

    bool        m_bMeshChanged;


public:
    MeshEngine(int width, int height);
    ~MeshEngine();

    void update(GLfloat delta);

    //设置完坐标后，调用此函数备份原始坐标
    void backupOrigVertex();

    //set destvertex then start animating
    //Animated duration in sec
    void startAnimating(float duration);
    void stopAnimating();

    //还原成原始图像
    void restore();

    void draw();


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

};
#endif // _meshengine_h_