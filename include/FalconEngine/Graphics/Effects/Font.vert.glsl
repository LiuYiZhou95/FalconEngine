#version 330

layout (location = 0) in vec2  VertexCoord;
layout (location = 1) in vec2  TextureCoord;
layout (location = 2) in vec4  FontColor;
layout (location = 3) in float FontWidth;
layout (location = 4) in float FontEdge;

// NOTE(Wuxiang): You better not use integer here. The OpenGL conversion of 
// integer vertex attribute is so not lossless. The result would be unexpected.
layout (location = 5) in float FontPage;

uniform mat4 Projection;

out Vout
{
    vec2  TextureCoord;
    vec4  FontColor;
    float FontEdge;
    float FontWidth;
    flat int FontPage;
} vout;

void main(void)
{
	gl_Position = Projection * vec4(VertexCoord, 0.0, 1.0);

    vout.TextureCoord = TextureCoord;
    vout.FontColor    = FontColor;
    vout.FontEdge     = FontEdge;
    vout.FontWidth    = FontWidth;
    vout.FontPage     = int(FontPage);
}