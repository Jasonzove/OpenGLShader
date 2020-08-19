#version 460 core

uniform sampler2D U_MainTexture;

in vec2 T_texcoord;

void main()
{
    //1 2 1
    //2 4 2
    //1 2 1
    vec4 color = vec4(0.0);
    int coreSize = 3;
    float texe10Offset = 1/200.0;
    float kernal[9] = {1,2,1,2,4,2,1,2,1}; //从左下角开始扫描
    //kernal[6] = 1;kernal[7] = 2;kernal[8] = 1;
    //kernal[3] = 2;kernal[4] = 4;kernal[5] = 2;
    //kernal[0] = 1;kernal[1] = 2;kernal[2] = 1;

    int index = 0;
    for(int y = 0; y < coreSize; ++y)
    {
        for(int x = 0; x < coreSize; ++x)
        {
            vec4 currentColor = texture2D(U_MainTexture, T_texcoord+vec2((-1+x)*texe10Offset,(-1+y)*texe10Offset));  
            color += currentColor*kernal[index++];
		}
	}
    color /= 16.0;
    gl_FragColor = color;
}