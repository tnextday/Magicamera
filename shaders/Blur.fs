#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform sampler2D uImgTex;

varying vec2 g_vTexCoords11;
varying vec2 g_vTexCoords00;
varying vec2 g_vTexCoords02;
varying vec2 g_vTexCoords20;
varying vec2 g_vTexCoords22;

void main()
{
    vec4 vBlur = texture2D( uImgTex, g_vTexCoords11 ) +
                 texture2D( uImgTex, g_vTexCoords00 ) +
                 texture2D( uImgTex, g_vTexCoords02 ) +
                 texture2D( uImgTex, g_vTexCoords20 ) +
                 texture2D( uImgTex, g_vTexCoords22 );
    gl_FragColor = 0.2 * vBlur;
}
