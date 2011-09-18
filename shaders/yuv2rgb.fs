precision mediump float;
varying vec2 vTexCoord;
uniform sampler2D Ytex;
uniform sampler2D UVtex;
void main() {
    float r,g,b,y,u,v;
    y = texture2D(Ytex, vTexCoord).r;
    vec4 uv = texture2D(UVtex, vTexCoord);
    y = 1.1643*(y-0.0625);
    u = uv.a-0.5;
    v = uv.r-0.5;
    r = y+1.5958*v;
    g = y-0.39173*u-0.81290*v;
    b = y+2.017*u;
    gl_FragColor = vec4(r,g,b,1.0);
}