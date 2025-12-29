/**
 ******************************************************************************
 * @file 	sim_gl.c
 * @author 	Ahmet Can GULMEZ
 * @brief 	GL simulation area integration of AeroSONAR.
 * 
 ******************************************************************************
 * @attention
 * 
 * Copyright (c) 2025 Ahmet Can GULMEZ.
 * All rights reserved.
 * 
 * This software is licensed under the MIT License.
 * 
 *****************************************************************************
 */

#include "main.h"


/* Shared widgets and variables */

GLuint shaderProgram;
GLuint VAO, VBO, EBO;
GLuint lineVAO, lineVBO, lineEBO;
GLuint transformLoc, colorLoc, alphaLoc;

float rotationX = 0.0, rotationY = 0.0;
gdouble lastX = 0.0, lastY = 0.0;
gboolean isDragging = FALSE;

const char* vertexShaderSrc = 
	"#version 330 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"uniform mat4 transform;\n"
	"void main()\n"
	"{\n"
	"	gl_Position = transform * vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
	"}\n";

const char* fragmentShaderSrc = 
	"#version 330 core\n"
   "out vec4 FragColor;\n"
   "uniform vec3 ourColor;\n"
	"uniform float alpha;\n"
   "void main()\n"
   "{\n"
   "   FragColor = vec4(ourColor, alpha);\n"
   "}\n";


gboolean on_gesture_click_press(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data)
{
	if (n_press == TRUE) 
	{
		isDragging = TRUE;
		lastX = x;
		lastY = y;
		return TRUE;
	} 
	return FALSE;
}

gboolean on_gesture_click_release(GtkGestureClick *gesture, int n_press, double x, double y, gpointer user_data)
{
	isDragging = FALSE;
	return TRUE;
}


gboolean on_event_controller_motion(GtkEventControllerMotion *motion, double x, double y, gpointer user_data)
{
	if (isDragging) 
	{
		gdouble deltaX = x - lastX;
		gdouble deltaY = y - lastY;

		rotationY += deltaX * 0.2f;
		rotationX += deltaY * 0.2f;
		lastX = x;
		lastY = y;
		gtk_widget_queue_draw(gtk_event_controller_get_widget(
			GTK_EVENT_CONTROLLER(motion)));

		return TRUE;
	}
	return FALSE;
}

void on_sim_rotation_changed(float angleX, float angleY, float *matrix)
{
	float cx, sx, cy, sy;

	cx = cosf(angleX);
   sx = sinf(angleX);
   cy = cosf(angleY);
   sy = sinf(angleY);
   matrix[0] = cy;  	matrix[4] = sx * sy;	matrix[8] = cx * sy;  matrix[12] = 0.0f;
   matrix[1] = 0.0f;	matrix[5] = cx;     	matrix[9] = -sx;      matrix[13] = 0.0f;
   matrix[2] = -sy; 	matrix[6] = sx * cy;	matrix[10] = cx * cy; matrix[14] = 0.0f;
   matrix[3] = 0.0f;	matrix[7] = 0.0f;   	matrix[11] = 0.0f;    matrix[15] = 1.0f;
}

void on_ballistic_sim_realize(GtkGLArea *gl_area)
{
	GLuint vertexShader, fragmentShader;
	int success;
	char infoLog[512];

	gtk_gl_area_make_current(gl_area);
	print_log("%s", glGetString(GL_VERSION));
	print_log("%s", glGetString(GL_RENDERER));

	/* Set the background color of GtkGLArea context */
	// glClearColor(10.0f/255.0f, 10.0f/255.0f, 44.0f/255.0f, 1.0f);
	// glClearColor(25.0f/255.0f, 25.0f/255.0f, 112.0f/255.0f, 1.0f);
	// glClearColor(5.0f/255.0f, 5.0f/255.0f, 30.0f/255.0f, 1.0f);
	glClearColor(8.0f/255.0f, 8.0f/255.0f, 36.0f/255.0f, 1.0f);
	// glClearColor(3.0f/255.0f, 3.0f/255.0f, 20.0f/255.0f, 1.0f);

	/* Enable depth testing for 3D view */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	/* Enable blending for transparency */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Build, compile and check shader and vertex programs */
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSrc, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		g_error("Vertex shader compilation failed: %s\n", infoLog);
	}

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSrc, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		g_error("Fragment shader compilation failed: %s\n", infoLog);
	}

	/* Link the shaders and check these */
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) 
	{
      glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
      g_error("Shader program linking failed: %s", infoLog);
   }

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	/* Get uniform locations */
	transformLoc = glGetUniformLocation(shaderProgram, "transform");
	colorLoc = glGetUniformLocation(shaderProgram, "ourColor");
	alphaLoc = glGetUniformLocation(shaderProgram, "alpha");

	/* Set up vertex data (and buffer(s)) and configure vertex attribute */
	float vertices[] = {
		-0.8f, -0.8f, -0.5f,
       0.8f, -0.8f, -0.5f,
       0.8f,  0.8f, -0.5f,
      -0.8f,  0.8f, -0.5f,
      -0.8f, -0.8f,  0.5f,
       0.8f, -0.8f,  0.5f,
       0.8f,  0.8f,  0.5f,
      -0.8f,  0.8f,  0.5f
	};
	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,    // front face
      4, 5, 6, 6, 7, 4,    // back face
      3, 2, 6, 6, 7, 3,    // top face
      0, 1, 5, 5, 4, 0,    // bottom face
      0, 3, 7, 7, 4, 0,    // left face
      1, 2, 6, 6, 5, 1     // right face
	};
	unsigned int lineIndices[] = {
		0, 1, 1, 2, 2, 3, 3, 0, // front face
		4, 5, 5, 6, 6, 7, 7, 4, // back face
		0, 4, 1, 5, 2, 6, 3, 7  // connecting edges
	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	/* Create and set up the line VAO, VBO, and EBO */
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glGenBuffers(1, &lineEBO);

	glBindVertexArray(lineVAO);
	
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
	glEnableVertexAttribArray(0);
	
	/* Unbind the VAO */
	glBindVertexArray(0);
}

void on_ballistic_sim_render(GtkGLArea *gl_area)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Use the shader program */
	glUseProgram(shaderProgram);

	/* Create transformation matrix (rotation) */
	float transform[16];
	on_sim_rotation_changed(rotationX * (M_PI / 180.0f), 
		rotationY * (M_PI / 180.0f), transform);

	/* Pass transformation matrix to shader */
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform);

	/* Draw the semi-transparent cube faces */
	glUniform3f(colorLoc, 255.0f/255.0f, 255.0f/255.0f, 255.0f/255.0f);
	glUniform1f(alphaLoc, 0.2f); /* semi-transparent */

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

	/* Draw the edges with solid color */
	glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f); /* white edges */
	glUniform1f(alphaLoc, 1.0f); /* opaque */

	/* Make lines slightly thicker for better visibility */
	glLineWidth(1.5f);
	glBindVertexArray(lineVAO);
	glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
	
	/* Unbind */
	glBindVertexArray(0);
}

