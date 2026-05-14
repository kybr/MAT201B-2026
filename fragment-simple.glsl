#version 400
layout(location = 0) out vec4 fragmentColor;

uniform vec3 iResolution;

void mainImage( out vec4 fragColor, in vec2 fragCoord )
{
	vec2 uv = fragCoord.xy / iResolution.xy;
  if (distance(vec2(0.5), uv) < 0.1) {
    fragColor = vec4(vec3(1, 1, 1), 1);
  }
  else {
    fragColor = vec4(vec3(1, 0, 0), 1);
  }
}

void main() {
  mainImage(fragmentColor, gl_FragCoord.xy);
}