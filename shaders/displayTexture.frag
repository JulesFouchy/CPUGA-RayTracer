#version 430

in vec2 vTexCoords;

uniform sampler2D u_TextureSlotGA;
uniform sampler2D u_TextureSlotLA;

void main() {
	if (vTexCoords.x  < 0.5) {
		gl_FragColor = texture(u_TextureSlotGA, vec2(
			vTexCoords.x * 2., 
			vTexCoords.y
		));
	}
	else {
		gl_FragColor = texture(u_TextureSlotLA, vec2(
			vTexCoords.x * 2. - 1., 
			vTexCoords.y
		));
	}
}