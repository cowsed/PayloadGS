VARYING vec3 pos;

void MAIN()
{
    pos = VERTEX;
    // pos.x += sin(time * 4.0 + pos.y) * amplitude;
    // texcoord = UV0;
    POSITION = MODELVIEWPROJECTION_MATRIX * vec4(pos, 1.0);
    POSITION.z = 0.01;
}
