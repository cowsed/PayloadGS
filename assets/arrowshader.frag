VARYING vec3 pos;


void MAIN()
{
    vec3 col = arrowColor.rgb;
    FRAGCOLOR = vec4(col, 1);
}
