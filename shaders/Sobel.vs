uniform float uTexWidth;
uniform float uTexHeight;

attribute vec4 aPosition;

varying vec2 g_vTexCoords0;
varying vec2 g_vTexCoords1;
varying vec2 g_vTexCoords2;
varying vec2 g_vTexCoords3;

void main()
{
    gl_Position  = vec4( aPosition.x, -aPosition.y, 0.0, 1.0 );
    vec2 vTexCoords = vec2( aPosition.z, aPosition.w );
    float stepX = 1.0/uTexWidth;
    float stepY = 1.0/uTexHeight;
    g_vTexCoords0 = vTexCoords + vec2( -stepX, -stepY );
    g_vTexCoords1 = vTexCoords + vec2(  stepX, -stepY );
    g_vTexCoords2 = vTexCoords + vec2( -stepX,  stepY );
    g_vTexCoords3 = vTexCoords + vec2(  stepX,  stepY );
}
