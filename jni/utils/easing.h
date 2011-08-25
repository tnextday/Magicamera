#ifndef _easing_h_
#define _easing_h_

#define _USE_MATH_DEFINES

class CEasing
{
public:
    bool AutoFree;
    virtual ~CEasing(){};
    //! time a value between 0 and 1
    //! For example: 
    //! * 0 means that the action just started
    //! * 0.5 means that the action is in the middle
    //! * 1 means that the action is over
    virtual float update(float t) = 0;
};

class CEaseOutCubic :public CEasing
{
public:
    virtual float update( float t );
};

class CEaseInCubic : public CEasing{
public:
    virtual float update( float t );
};

class CEaseShake :public CEasing{
    int m_round;
public:
    CEaseShake(int round = 6);
    virtual float update( float t );
};

class CEaseInOutCirc :public CEasing{
public:
    virtual float update( float t );
};

class CEaseOutBounce:public CEasing{
    float   m_amplitude;
public:
    CEaseOutBounce(float amplitude = 1.0);
    virtual float update( float t );
};

class CEaseOutQuart :public CEasing
{
public:
    virtual float update( float t );
};

class CEaseInQuart : public CEasing{
public:
    virtual float update( float t );
};

inline float easeShake(float t, int round = 6);
static float easeOutBounce_helper(float t, float c, float a);
#endif // _easing_h_
