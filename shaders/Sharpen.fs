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
    vec4 vSharp11 = texture2D( uImgTex, g_vTexCoords11 );
    vec4 vSharp00 = texture2D( uImgTex, g_vTexCoords00 );
    vec4 vSharp02 = texture2D( uImgTex, g_vTexCoords02 );
    vec4 vSharp20 = texture2D( uImgTex, g_vTexCoords20 );
    vec4 vSharp22 = texture2D( uImgTex, g_vTexCoords22 );

    // Sharpen by increasing local contrast.
    // This is basically the image, plus the four differences between the pixel and it's neighbors.
    vec4 vSharp = 5.0 * vSharp11 - ( vSharp00 + vSharp02 + vSharp20 + vSharp22 );

    gl_FragColor = vSharp;
}
