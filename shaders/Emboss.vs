uniform float uTexWidth;
uniform float uTexHeight;

attribute vec4 aPosition;

varying vec2 g_vTexCoords0;
varying vec2 g_vTexCoords1;

void main()
{
    gl_Position  = vec4( aPosition.x, -aPosition.y, 0.0, 1.0 );
    vec2 vTexCoords = vec2( aPosition.z, aPosition.w ) + 2.0 * vec2( g_fHalfStepSizeX, g_fHalfStepSizeY );
    vec2 step = vec2(0.5/uTexWidth, 0.5/uTexHeight);
    g_vTexCoords0 = vTexCoords - step;
    g_vTexCoords1 = vTexCoords + step;
}
