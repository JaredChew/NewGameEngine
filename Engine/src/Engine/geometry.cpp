#include "geometry.h"

void DefaultGeometry::generateSquare() {

	static GLuint squareVAO = 0, squareVBO = 0, squareEBO = 0;

start:

	if (squareVAO != 0) {

		glBindVertexArray(squareVAO);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

	}
	else {

		GLuint squareIndices[] = {

			0, 1, 2,
			2, 3, 0

		};

		GLfloat squareVertices[] = {

			-1.0f,  1.0f, 0.0f, //top left
			-1.0f, -1.0f, 0.0f, //bottom left
			 1.0f, -1.0f, 0.0f, //bottom right
			 1.0f,  1.0f, 0.0f, //top right

		};

		GLfloat squareColors[] = {

			1.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 1.0f, 1.0f,

			0.0f, 0.0f, 1.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f, 0.0f, 1.0f

		};

		GLfloat squareNormals[] = {

			0.0f, 0.0f, 1.0f

		};

		GLfloat squareTexCoords[] = {

			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,
			1.0f, 1.0f,

		};

		glGenVertexArrays(1, &squareVAO);
		glBindVertexArray(squareVAO);

		if (squareVBO == 0) {
			
			glGenBuffers(1, &squareVBO);
			glBindBuffer(GL_ARRAY_BUFFER, squareVBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(squareVertices) + sizeof(squareColors) + sizeof(squareTexCoords), squareVertices, GL_STATIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(squareVertices), squareVertices);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices), sizeof(squareColors), squareColors);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(squareVertices) + sizeof(squareColors), sizeof(squareTexCoords), squareTexCoords);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
			glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, (void*)sizeof(squareVertices));
			glVertexAttribPointer(2, 2, GL_FLOAT, false, 0, (void*)(sizeof(squareVertices) + sizeof(squareColors)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);
		
		}

		if (squareEBO == 0) {
			
			glGenBuffers(1, &squareEBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(squareIndices), squareIndices, GL_STATIC_DRAW);
		
		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		goto start;

	}
	
}

void DefaultGeometry::generateCube() {
	
	static GLuint cubeVAO = 0, cubeVBO = 0, cubeEBO = 0;

start:

	if (cubeVAO != 0) {

		glBindVertexArray(cubeVAO);

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);

	}
	else {

		int cubeIndices[] = {

			// Right
			1, 2, 6,
			6, 5, 1,

			// Left
			0, 4, 7,
			7, 3, 0,

			// Top
			4, 5, 6,
			6, 7, 4,

			// Bottom
			0, 3, 2,
			2, 1, 0,

			// Back
			0, 1, 5,
			5, 4, 0,

			// Front
			3, 7, 6,
			6, 2, 3

		};

		float cubeVertices[] = {

			//   Coordinates
			-1.0f, -1.0f,  1.0f,//        7--------6
			 1.0f, -1.0f,  1.0f,//       /|       /|
			 1.0f, -1.0f, -1.0f,//      4--------5 |
			-1.0f, -1.0f, -1.0f,//      | |      | |
			-1.0f,  1.0f,  1.0f,//      | 3------|-2
			 1.0f,  1.0f,  1.0f,//      |/       |/
			 1.0f,  1.0f, -1.0f,//      0--------1
			-1.0f,  1.0f, -1.0f

		};

		float cubeColour[] = {

			// r, g, b, a
			0.0, 0.0, 0.0, 1.0,
			1.0, 0.0, 0.0, 1.0,
			1.0, 1.0, 0.0, 1.0,
			0.0, 1.0, 0.0, 1.0,
			0.0, 0.0, 1.0, 1.0,
			1.0, 0.0, 1.0, 1.0,
			1.0, 1.0, 1.0, 1.0,
			0.0, 1.0, 1.0, 1.0

		};

		int cubeNormals[] = {

			 1.0f,  0.0f,  0.0f, //right
			-1.0f,  0.0f,  0.0f, //left
			 0.0f,  1.0f,  0.0f, //top
			 0.0f, -1.0f,  0.0f, //bottom
			 0.0f,  0.0f, -1.0f, //back
			 0.0f,  0.0f,  1.0f  //front

		};

		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		if (cubeVBO == 0) {

			glGenBuffers(1, &cubeVBO);
			glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(cubeColour) + sizeof(cubeNormals), cubeVertices, GL_STATIC_DRAW);

			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(cubeVertices), cubeVertices);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices), sizeof(cubeColour), cubeColour);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(cubeVertices) + sizeof(cubeColour), sizeof(cubeNormals), cubeNormals);

			glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0);
			glVertexAttribPointer(1, 4, GL_FLOAT, false, 0, (void*)sizeof(cubeVertices));
			glVertexAttribPointer(2, 3, GL_FLOAT, false, 0, (void*)(sizeof(cubeVertices) + sizeof(cubeColour)));

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);

		}

		if (cubeEBO == 0) {

			glGenBuffers(1, &cubeEBO);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

		}

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		goto start;

	}

}