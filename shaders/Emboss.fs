#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform sampler2D uImgTex;

varying vec2 g_vTexCoords0;
varying vec2 g_vTexCoords1;

void main()
{
    vec4 vEmboss = 4.0 * ( texture2D( uImgTex, g_vTexCoords0 ) - texture2D( uImgTex, g_vTexCoords1 ) );

    // Offset with 0.5 to show both positive and negative response
    gl_FragColor = vEmboss + 0.5;
}
