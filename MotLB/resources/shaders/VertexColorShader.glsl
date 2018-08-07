/*
 * VertexColorShader.glsl
 *
 *  Created on: Jul 30, 2018
 *      Author: Samuel Tan
 */

#shader vertex -----------------------
#version 330 core

in vec4 position;
in vec4 color;
out vec4 vo_color;

void main()
{
	gl_Position = position;
	vo_color = color;
}

#shader fragment ---------------------
#version 330 core

in vec4 vo_color;
out vec4 color;

void main()
{
	color = vo_color;
}