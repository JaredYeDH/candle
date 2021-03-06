
#include "common.glsl"
#line 4

layout (location = 0) out vec4 FragColor;

BUFFER {
	sampler2D color;
} buf;

uniform int level;
uniform float alpha;

void main(void)
{
	/* FragColor = vec4(texcoord, 0, 1); */
	/* return; */
	vec4 tex;
	tex = textureLod(buf.color, pixel_pos(), float(level));
	/* if(tex.a == 0) discard; */
	tex.a = alpha;

	FragColor = tex;
}

// vim: set ft=c:
