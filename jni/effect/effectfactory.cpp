#include <string.h>
#include "effectfactory.h"
#include "glow.h"
#include "scanline.h"
#include "hdr.h"
#include "shift.h"
#include "microspur.h"
#include "reversal.h"
#include "nostalgia.h"
#include "lemo.h"
#include "infrared.h"
#include "purple.h"
#include "colorcurves.h"

static const char* g_str_effects = 
    Glow_Effect_Name ","
    ScanLine_Effect_Name ","
    HDR_Effect_Name ","
    Shift_Effect_Name ","
    Microspur_Effect_Name ","
    Reversal_Effect_Name ","
    //Nostalgia_Effect_Name ","
    LEMO_Effect_Name ","
    InfraRed_Effect_Name ","
    Purple_Effect_Name ","
    E6C41_Effect_Name ","
    C41E6_Effect_Name ","
    CROSS1_Effect_Name  ","
    CROSS2_Effect_Name  ","
    CROSS3_Effect_Name  ","
    ;

Effect* createEffect( const char* effectName )
{
    if (!effectName)
        return NULL;
    if (strcmp(effectName, Glow_Effect_Name) == 0){
        return new Glow;
    }else if (strcmp(effectName, ScanLine_Effect_Name) == 0){
        return new ScanLine;
    }else if (strcmp(effectName, HDR_Effect_Name) == 0){
        return new HDR;
    }else if (strcmp(effectName, Shift_Effect_Name) == 0){
        return new Shift;
    }else if (strcmp(effectName, Microspur_Effect_Name) == 0){
        return new Microspur;
    }else if (strcmp(effectName, Reversal_Effect_Name) == 0){
        return new Reversal;
    }else if (strcmp(effectName, Nostalgia_Effect_Name) == 0){
        return new Nostalgia;
    }else if (strcmp(effectName, LEMO_Effect_Name) == 0){
        return new LEMO;
    }else if (strcmp(effectName, InfraRed_Effect_Name) == 0){
        return new InfraRed;
    }else if (strcmp(effectName, Purple_Effect_Name) == 0){
        return new Purple;
    }else if (strcmp(effectName, E6C41_Effect_Name) == 0){
        return new E6C41;
    }else if (strcmp(effectName, C41E6_Effect_Name) == 0){
        return new C41E6;
    }else if (strcmp(effectName, CROSS1_Effect_Name) == 0){
        return new CROSS1;
    }else if (strcmp(effectName, CROSS2_Effect_Name) == 0){
        return new CROSS2;
    }else if (strcmp(effectName, CROSS3_Effect_Name) == 0){
        return new CROSS3;
    }
    return NULL;
}


const char* getEffectList()
{
    return g_str_effects;
}