#version 450
in vec3 vertexColor;
in vec2 texCoord;
in vec3 scaledNormal;
in vec3 fragPos;

out vec4 color;

uniform sampler2D tex_buffer;

// Propriedades da superfície
uniform float ka;
uniform float kd;
uniform float ks;
uniform float q;

// Propriedades da fonte de luz
uniform vec3 lightColor;
uniform vec3 lightPos;

// Posição da camera
uniform vec3 cameraPos;

void main() {
	// Cálculo da parcela de iluminação ambiente
	vec3 ambient = ka * lightColor;

	// Cálculo da parcela de iluminação difusa
	vec3 N = normalize(scaledNormal);
	vec3 L = normalize(lightPos - fragPos);
	float diff = max(dot(N, L), 0.0f);
	vec3 diffuse = kd * diff * lightColor;

	// Cálculo da parcela de iluminação especular
	vec3 V = normalize(cameraPos - fragPos);
	vec3 R = normalize(reflect(-L,N));
	float spec = max(dot(R, V), 0.0f);
	spec = pow(spec, q);
	vec3 specular = ks * spec * lightColor;

	vec3 texColor = texture(tex_buffer, texCoord).rgb;

	vec3 result = (ambient + diffuse) * texColor + specular;

	color = vec4(result, 1.0f);
}