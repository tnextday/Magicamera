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
    virtual float update(double t) = 0;
};

class CEaseOutCubic :public CEasing
{
public:
    virtual float update( double t );
};

class CEaseInCubic : public CEasing{
public:
    virtual float update( double t );
};

class CEaseShake :public CEasing{
    int m_round;
public:
    CEaseShake(int round = 6);
    virtual float update( double t );
};

class CEaseInOutCirc :public CEasing{
public:
    virtual float update( double t );
};

class CEaseOutBounce:public CEasing{
    float   m_amplitude;
public:
    CEaseOutBounce(double amplitude = 1.0);
    virtual float update( double t );
};

class CEaseOutQuart :public CEasing
{
public:
    virtual float update( double t );
};

class CEaseInQuart : public CEasing{
public:
    virtual float update( double t );
};

inline float easeShake(double t, int round = 6);
static float easeOutBounce_helper(double t, double c, double a);
#endif // _easing_h_
