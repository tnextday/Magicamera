#pragma once
#include "curveeffect.h"

#define E6C41_Effect_Name "E6C41"

class E6C41 : public CurveEffect{
public:
    E6C41(){
        setCurveTex("res://maps/E6C41Curve.png");
    }
    virtual const char* getName(){
        return E6C41_Effect_Name;
    }
};

#define C41E6_Effect_Name "C41E6"

class C41E6 : public CurveEffect{
public:
    C41E6() {
        setCurveTex("res://maps/C41E6Curve.png");
    }
    virtual const char* getName(){
        return C41E6_Effect_Name;
    }
};

#define CROSS1_Effect_Name "CROSS1"

class CROSS1 : public CurveEffect{
public:
    CROSS1() {
        setCurveTex("res://maps/Cross1Curve.png");
    }
    virtual const char* getName(){
        return CROSS1_Effect_Name;
    }
};

#define CROSS2_Effect_Name "CROSS2"

class CROSS2 : public CurveEffect{
public:
    CROSS2() {
        setCurveTex("res://maps/Cross2Curve.png");
    }
    virtual const char* getName(){
        return CROSS2_Effect_Name;
    }
};

#define CROSS3_Effect_Name "CROSS3"

class CROSS3 : public CurveEffect{
public:
    CROSS3() {
        setCurveTex("res://maps/Cross3Curve.png");
    }
    virtual const char* getName(){
        return CROSS3_Effect_Name;
    }
};
