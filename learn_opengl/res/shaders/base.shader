#shader vertex
#version 430 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aColor;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(aPosition.x, aPosition.y, aPosition.z, 1.0f);
	ourColor = aColor;
}

#shader fragment
#version 460 core

out vec4 fragColor;
in vec3 ourColor;

void main()
{
	fragColor = vec4(ourColor, 1.0f)
}