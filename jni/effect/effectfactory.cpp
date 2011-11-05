#include <string.h>
#include "effectfactory.h"
#include "glow.h"
#include "scanline.h"
#include "hdr.h"


static const char* g_str_effects = 
    Glow_Effect_Name "|"
    ScanLine_Effect_Name "|"
    HDR_Effect_Name "|"
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
    }
    return NULL;
}


const char* getEffectList()
{
    return g_str_effects;
}