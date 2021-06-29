#version 330 core
out vec4 FragColor;

in vec4 outColor;
in vec2 outUV;
//取出纹理RGB
uniform sampler2D ourTexture;

void main(){
    FragColor=texture(ourTexture,outUV)*outColor;
	//FragColor=vec4(1.0f,0.5f,0.2f,1.0f);
	//最后一个alpha值为1.0代表完全不透明
};
