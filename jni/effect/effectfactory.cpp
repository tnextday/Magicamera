#include "effectfactory.h"
#include "glow.h"
#include <string.h>


static const char* g_str_effects = 
    Glow_Effect_Name "|";

Effect* createEffect( const char* effectName )
{
    if (!effectName)
        return NULL;
    if (strcmp(effectName, Glow_Effect_Name) == 0){
        return new Glow;
    }
    return NULL;
}


const char* getEffectList()
{
    return g_str_effects;
}