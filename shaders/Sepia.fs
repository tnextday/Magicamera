#ifdef GL_FRAGMENT_PRECISION_HIGH
   precision highp float;
#else
   precision mediump float;
#endif

uniform sampler2D uImgTex;
uniform sampler2D uLookupTex;

varying vec2 g_vTexCoords;


void main()
{
    vec3  vSample    = texture2D( uImgTex, g_vTexCoords ).rgb;
    float fLuminance = dot( vSample, vec3( 0.3, 0.59, 0.11 ) );
    gl_FragColor     = texture2D( uLookupTex, vec2( fLuminance, 0.0 ) );
}
