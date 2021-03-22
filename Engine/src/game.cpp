#include "glad.h"

#include <cstddef>

struct Vertex
{
	float x, y;
	unsigned char r, g, b;
};

constexpr Vertex VERTEX_DATA[] =
{
	{-0.5f, -0.5f, 255,   0,   0},
	{ 0.5f, -0.5f,   0, 255,   0},
	{ 0.0f,  0.5f,   0,   0, 255}
};

constexpr const char* VERTEX_CODE = R"glsl(
#version 330 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

uniform float _Time;

out vec3 vertexColor;

void main()
{
	float c = cos(_Time);
	float s = sin(_Time);

	mat2 rot = mat2(c, s, -s, c);

	gl_Position = vec4(rot * aPos, 0.0, 1.0);
	vertexColor = aColor;
}
)glsl";

constexpr const char* FRAGMENT_CODE = R"glsl(
#version 330 core

in vec3 vertexColor;
out vec4 fragColor;

void main()
{
	fragColor = vec4(vertexColor, 1.0);
}
)glsl";

static GLuint vbo;
static GLuint vao;
static GLuint program;

static float time;

void init()
{
	// settings

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// glEnable(GL_CULL_FACE);
	// glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// model

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(VERTEX_DATA), VERTEX_DATA, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, false, sizeof(Vertex), (void*)offsetof(Vertex, x));
	glVertexAttribPointer(1, 3, GL_UNSIGNED_BYTE, true, sizeof(Vertex), (void*)offsetof(Vertex, r));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// program

	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex_shader, 1, &VERTEX_CODE, nullptr);
	glCompileShader(vertex_shader);

	GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment_shader, 1, &FRAGMENT_CODE, nullptr);
	glCompileShader(fragment_shader);

	program = glCreateProgram();

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);

	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);
}

void update(float dt)
{
	time += dt;
}

void draw()
{
	glClear(GL_COLOR_BUFFER_BIT /* | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT */);

	glUseProgram(program);

	GLint _Time = glGetUniformLocation(program, "_Time");
	glUniform1f(_Time, time);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, sizeof(VERTEX_DATA) / sizeof(VERTEX_DATA[0]));
}

void deinit()
{
	glDeleteProgram(program);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
