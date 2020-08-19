#version 460 core

uniform sampler2D U_MainTexture;

in vec2 T_texcoord;

void main()
{
    vec4 minValue = vec4(1.0);
    int coreSize = 2;
    int halfCore = coreSize/2;
    float texe10Offset = 1/100.0;
    for(int y = 0; y < coreSize; ++y)
    {
        for(int x = 0; x < coreSize; ++x)
        {
            vec4 currentColor = texture2D(U_MainTexture, T_texcoord+vec2((-halfCore+x)*texe10Offset,(-halfCore+y)*texe10Offset));  
            minValue = min(minValue, currentColor);
		}
	}
    gl_FragColor = vec4(minValue.rgb, minValue.a);
}