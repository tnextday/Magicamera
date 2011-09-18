uniform float uTexWidth;
uniform float uTexHeight;

attribute vec4 aPosition;

varying vec2 g_vTexCoords11;
varying vec2 g_vTexCoords00;
varying vec2 g_vTexCoords02;
varying vec2 g_vTexCoords20;
varying vec2 g_vTexCoords22;


void main()
{
    gl_Position  = vec4( aPosition.x, -aPosition.y, 0.0, 1.0 );
    vec2 vTexCoords = vec2( aPosition.z, aPosition.w );
    float stepX = 0.5/uTexWidth;
    float stepY = 0.5/uTexHeight;
    g_vTexCoords11 = vTexCoords;
    g_vTexCoords00 = vTexCoords + vec2( -stepX, -stepY );
    g_vTexCoords02 = vTexCoords + vec2(  stepX, -stepY );
    g_vTexCoords20 = vTexCoords + vec2(  stepX,  stepY );
    g_vTexCoords22 = vTexCoords + vec2( -stepX,  stepY );
}
