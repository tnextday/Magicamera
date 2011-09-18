attribute vec4 aPosition;

varying vec2 g_vTexCoords;


void main()
{
    gl_Position  = vec4( aPosition.x, -aPosition.y, 0.0, 1.0 );
    g_vTexCoords = vec2( aPosition.z, aPosition.w );
}
