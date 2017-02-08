//
// フォント描画用
//
$version$

uniform mat4 ciModelViewProjection;

in vec4 ciPosition;
in vec4 ciColor;
in vec2 ciTexCoord0;

out vec2 TexCoord0;
out vec4 Color;


void main(void) {
  vec4 position = ciModelViewProjection * ciPosition;

  gl_Position = position;
  TexCoord0   = ciTexCoord0;
  Color       = ciColor;
}