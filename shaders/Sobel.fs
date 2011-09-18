#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform sampler2D uImgTex;

varying vec2 g_vTexCoords0;
varying vec2 g_vTexCoords1;
varying vec2 g_vTexCoords2;
varying vec2 g_vTexCoords3;

void main()
{
    vec4 s0 = texture2D( uImgTex, g_vTexCoords0 );
    vec4 s1 = texture2D( uImgTex, g_vTexCoords1 );
    vec4 s2 = texture2D( uImgTex, g_vTexCoords2 );
    vec4 s3 = texture2D( uImgTex, g_vTexCoords3 );

    vec4 vSobelX = 4.0 * ( ( s0 + s2 ) - ( s1 + s3 ) );
    vec4 vSobelY = 4.0 * ( ( s0 + s1 ) - ( s2 + s3 ) );

    // Compute the vector length
    gl_FragColor = sqrt( vSobelX * vSobelX + vSobelY * vSobelY );
}
