#include <string.h>
#include "effectfactory.h"
#include "glow.h"
#include "scanline.h"
#include "hdr.h"
#include "shift.h"
#include "microspur.h"
#include "reversal.h"
#include "nostalgia.h"
#include "coloreffect.h"

static const char* g_str_effects = 
    Glow_Effect_Name ","
    ScanLine_Effect_Name ","
    HDR_Effect_Name ","
    Shift_Effect_Name ","
    Microspur_Effect_Name ","
    Reversal_Effect_Name ","
    Nostalgia_Effect_Name ","
    ColorEffect_Effect_Name ","
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
    }else if (strcmp(effectName, ColorEffect_Effect_Name) == 0){
        return new ColorEffect;
    }
    return NULL;
}


const char* getEffectList()
{
    return g_str_effects;
}