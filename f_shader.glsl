#version 330

uniform sampler2D tex;
out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

in vec4 n;
in vec4 v;
in vec4 l1;
in vec4 l2;
in vec4 l3;
in vec4 l4;
in vec4 l5;
in vec2 iTexCoord;

void main(void) {
	// znormalizowane interpolowane wektory 
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);
	// normalizacja swiatla
	vec4 ml1 = normalize(l1);
	vec4 ml2 = normalize(l2);
	vec4 ml3 = normalize(l3);
	vec4 ml4 = normalize(l4);
	vec4 ml5 = normalize(l5);
	// odbicie
	vec4 mr1 = reflect(-ml1, mn);
	vec4 mr2 = reflect(-ml2, mn);
	vec4 mr3 = reflect(-ml3, mn);
	vec4 mr4 = reflect(-ml4, mn);
	vec4 mr5 = reflect(-ml5, mn);
	// Parametry powierzchni
	vec4 kd = texture(tex, iTexCoord);
	vec4 ks = texture(tex, iTexCoord);
	// iloczyny skalarne wektorow mn, ml
	float nl1 = pow(clamp(dot(mn, ml1), 0, 1), 2);
	float nl2 = pow(clamp(dot(mn, ml2), 0, 1), 3);
	float nl3 = pow(clamp(dot(mn, ml3), 0, 1), 3);
	float nl4 = pow(clamp(dot(mn, ml4), 0, 1), 3);
	float nl5 = pow(clamp(dot(mn, ml5), 0, 1), 3);
	// iloczyny skalarne wektorow mr, mv
	float rv1 = pow(clamp(dot(mr1, mv), 0, 1), 4);
	float rv2 = pow(clamp(dot(mr2, mv), 0, 1), 40);
	float rv3 = pow(clamp(dot(mr3, mv), 0, 1), 50);
	float rv4 = pow(clamp(dot(mr4, mv), 0, 1), 50);
	float rv5 = pow(clamp(dot(mr5, mv), 0, 1), 60);
	// sumaryczne kolory pikseli
	pixelColor = vec4(kd.rgb * nl1, kd.a) + vec4(ks.rgb * rv1, 0);
	pixelColor += vec4(kd.rgb * nl2, kd.a) + vec4(ks.rgb * rv2, 0);
	pixelColor += vec4(kd.rgb * nl3, kd.a) + vec4(ks.rgb * rv3, 0);
	pixelColor += vec4(kd.rgb * nl4, kd.a) + vec4(ks.rgb * rv4, 0);
	pixelColor += vec4(kd.rgb * nl5, kd.a) + vec4(ks.rgb * rv5, 0);
}
