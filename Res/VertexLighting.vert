/**
*@file VertexLighting.vert
*/
#version 410

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexCoord;
layout(location=3) in vec3 vNormal;
layout(location=0) out vec4 outColor;
layout(location=1) out vec2 outTexCoord;

//環境光.
struct AmbientLight{

vec3 color;
};

//指向性ライト.
struct DirectionLight{

vec3 direction;
vec3 color;
};

//ポイント・ライト.

struct PointLight{

vec3 position[8];
vec3 color[8];
};

uniform AmbientLight ambientLight;
uniform DirectionLight directionalLight;
uniform PointLight pointLight;
uniform mat4x4 matMVP;

/**
*頂点シェーダ.
*/

void main(){

//指向性ライトの明るさを計算.
float cosTheta = clamp(dot(vNormal,-directionalLight.direction),0.0,1.0);
outColor.rgb = (vColor.rgb * directionalLight.color * cosTheta);
vec3 lightColor = directionalLight.color * cosTheta;

//ポイント・ライトの明るさを計算.
for(int i = 0; i < 8; ++i){
if(dot(pointLight.color[i],pointLight.color[i]) != 0){

//フラグメントからライトへ向かうベクトルを計算.
vec3 lightVector = pointLight.position[i] - vPosition;
vec3 lightDir = normalize(lightVector);
//面の傾きよる明るさの変化量を計算.
float cosTheta = clamp(dot(vNormal,lightDir),0.0,1.0);
//距離による明るさの変化量を計算.
float intensity = 1.0 / (1.0 + dot(lightVector,lightVector));
//変化量を掛け合わせて明るさを求め、合計に加算.
lightColor += pointLight.color[i] * cosTheta * intensity;

}
}
outColor.rgb = vColor.rgb * lightColor;

//環境光の計算.
outColor.rgb += ambientLight.color;
outColor.a = vColor.a;

outTexCoord = vTexCoord;
gl_Position = matMVP * vec4(vPosition,1.0);
}