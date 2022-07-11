// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
// Include GLEW. Always include it before gl.h and glfw3.h, since it's a bit magic.
#include <GL/glew.h>
// Include GLFW
#include <GLFW/glfw3.h>
// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <glm/gtx/norm.hpp>

using namespace glm;
#include "common/shader.hpp"
#include "common/texture.hpp"

//define a Scene class
//declare a Scene class





class CameraParameters{
	public:
		GLFWwindow* window;
		GLfloat *Projection;
		GLfloat *View;
		GLfloat *Model;
		GLfloat *MVP;

		GLuint MatrixID;
		GLuint ViewMatrixID;
		GLuint ModelMatrixID;
		CameraParameters(GLFWwindow* window,
		GLfloat *Projection,
		GLfloat *View,
		GLfloat *Model,
		GLfloat *MVP,

		GLuint MatrixID,
		GLuint ViewMatrixID,
		GLuint ModelMatrixID){
			this->window = window;
			this->Projection = Projection;
			this->View = View;
			this->Model = Model;
			this->MVP = MVP;

			this->MatrixID = MatrixID;
			this->ViewMatrixID = ViewMatrixID;
			this->ModelMatrixID = ModelMatrixID;
		}
};
class Square{
	GLfloat g_vertex_buffer_data[18] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f,
		-1.0f,  -1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,
		1.0f, 1.0f, 0.0f
	};
	GLfloat g_normal_buffer_data[18] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};
	GLfloat g_uv_buffer_data[12] = {
		1.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f		
	};
		// This will identify our vertex buffer
	GLuint vertexbuffer;
	GLuint VertexArrayID;

	GLuint UVbuffer;
	GLuint UVArrayID;

	GLuint normalbuffer;
	
	GLuint shaderID;
	GLuint TextureID;
	

	GLuint scaleID;
	GLuint posID;
	GLuint rotID;

	GLuint scaleIDGlobal;
	GLuint posIDGlobal;
	GLuint rotIDGlobal;

	
	GLuint ColorID;

	GLuint programID;

	

	public:
		GLuint Texture;
		vec3 scaleVector = glm::vec3(1.0f,1.0f,1.0f);
		vec3 posVector = glm::vec3(0.0f,1.0f,-2.0f);
		glm::quat rotObject;

		vec3 scaleVectorGlobal = glm::vec3(1.0f,1.0f,1.0f);
		vec3 posVectorGlobal = glm::vec3(0.0f,1.0f,-2.0f);
		glm::quat rotObjectGlobal;

		float greenValue;

		void Set(GLuint programID, GLuint Texture){
			this->programID = programID;
			this->Texture = Texture;
			scaleID = glGetUniformLocation(programID, "scale");
			posID = glGetUniformLocation(programID, "pos");
			rotID = glGetUniformLocation(programID, "rot");
			scaleIDGlobal = glGetUniformLocation(programID, "scaleGlobal");
			posIDGlobal = glGetUniformLocation(programID, "posGlobal");
			rotIDGlobal = glGetUniformLocation(programID, "rotGlobal");
			TextureID  = glGetUniformLocation(programID, "myTextureSampler");
			ColorID  = glGetUniformLocation(programID, "green");

			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);		

			glGenBuffers(1, &UVbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);	
	
			// Generate 1 buffer, put the resulting identifier in vertexbuffer
			glGenBuffers(1, &vertexbuffer);
			// The following commands will talk about our 'vertexbuffer' buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			// Give our vertices to OpenGL.
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		
			glGenBuffers(1, &normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);
			rotObject = glm::quat(vec3(0,0,0));
			rotObjectGlobal = glm::quat(vec3(0,0,0));
		}
		void Draw(CameraParameters *cameraParams){
			
			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture);
			// Set our "myTextureSampler" sampler to use Texture Unit 0
			glUniform1i(TextureID, 0);

			// 1st attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
			glUseProgram(programID);
			glUniform1f(ColorID, greenValue);


			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
			glVertexAttribPointer(
				2,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

	
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			//rotQuat = glm::quat(glm::vec3(0,0.01,0))*rotQuat;
			
			glUniformMatrix4fv(cameraParams->MatrixID, 1, GL_FALSE, cameraParams->MVP);
			glUniformMatrix4fv(cameraParams->ViewMatrixID, 1, GL_FALSE, cameraParams->View);
			glUniformMatrix4fv(cameraParams->ModelMatrixID, 1, GL_FALSE, cameraParams->Model);
			
			glm::mat4 rotMatrix = mat4_cast(rotObject);
			glm::mat4 rotMatrixGlobal = mat4_cast(rotObjectGlobal);
			glUniform3fv(scaleID,1, glm::value_ptr(scaleVector));
			glUniform3fv(posID,1, glm::value_ptr(posVector));
			glUniformMatrix4fv(rotID, 1, GL_FALSE, &rotMatrix[0][0]);

			glUniform3fv(scaleIDGlobal,1, glm::value_ptr(scaleVectorGlobal));
			glUniform3fv(posIDGlobal,1, glm::value_ptr(posVectorGlobal));
			glUniformMatrix4fv(rotIDGlobal, 1, GL_FALSE, &rotMatrixGlobal[0][0]);
				
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 6 vertices total -> 2 triangle

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);	
		}
		void Clean(){
			glDeleteBuffers(1, &vertexbuffer);
			glDeleteBuffers(1, &UVbuffer);
			glDeleteBuffers(1, &normalbuffer);
				
			glDeleteTextures(1, &Texture);
			glDeleteVertexArrays(1, &VertexArrayID);
		}
};


class Triangle{
	GLfloat g_vertex_buffer_data[9] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		1.0f,  1.0f, 0.0f
	};
	GLfloat g_normal_buffer_data[9] = {
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f
	};
	GLfloat g_uv_buffer_data[6] = {
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f,  1.0f
	};
		// This will identify our vertex buffer
	GLuint vertexbuffer;
	GLuint VertexArrayID;

	GLuint UVbuffer;
	GLuint UVArrayID;

	GLuint normalbuffer;
	
	GLuint shaderID;
	GLuint TextureID;
	

	GLuint scaleID;
	GLuint posID;
	GLuint rotID;

	GLuint scaleIDGlobal;
	GLuint posIDGlobal;
	GLuint rotIDGlobal;

	
	GLuint ColorID;

	GLuint programID;

	

	public:
		GLuint Texture;
		vec3 scaleVector = glm::vec3(1.0f,1.0f,1.0f);
		vec3 posVector = glm::vec3(0.0f,1.0f,0.0f);
		glm::quat rotObject;

		vec3 scaleVectorGlobal = glm::vec3(1.0f,1.0f,1.0f);
		vec3 posVectorGlobal = glm::vec3(0.0f,1.0f,0.0f);
		glm::quat rotObjectGlobal;

		float greenValue;

		void Set(GLuint programID, GLuint Texture){
			this->programID = programID;
			this->Texture = Texture;
			scaleID = glGetUniformLocation(programID, "scale");
			posID = glGetUniformLocation(programID, "pos");
			rotID = glGetUniformLocation(programID, "rot");
			scaleIDGlobal = glGetUniformLocation(programID, "scaleGlobal");
			posIDGlobal = glGetUniformLocation(programID, "posGlobal");
			rotIDGlobal = glGetUniformLocation(programID, "rotGlobal");
			TextureID  = glGetUniformLocation(programID, "myTextureSampler");
			ColorID  = glGetUniformLocation(programID, "green");

			glGenVertexArrays(1, &VertexArrayID);
			glBindVertexArray(VertexArrayID);		

			glGenBuffers(1, &UVbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);	
	
			// Generate 1 buffer, put the resulting identifier in vertexbuffer
			glGenBuffers(1, &vertexbuffer);
			// The following commands will talk about our 'vertexbuffer' buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			// Give our vertices to OpenGL.
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		
			glGenBuffers(1, &normalbuffer);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_normal_buffer_data), g_normal_buffer_data, GL_STATIC_DRAW);
			rotObject = glm::quat(vec3(0,0,0));
			rotObjectGlobal = glm::quat(vec3(0,0,0));
		}
		void Draw(CameraParameters *cameraParams){
			
			// Bind our texture in Texture Unit 0
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, Texture);
			// Set our "myTextureSampler" sampler to use Texture Unit 0
			glUniform1i(TextureID, 0);

			// 1st attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
			);
			glUseProgram(programID);
			glUniform1f(ColorID, greenValue);


			// 2nd attribute buffer : UVs
			glEnableVertexAttribArray(1);
			glBindBuffer(GL_ARRAY_BUFFER, UVbuffer);
			glVertexAttribPointer(
				1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
				2,                                // size : U+V => 2
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

			// 3rd attribute buffer : normals
			glEnableVertexAttribArray(2);
			glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
			glVertexAttribPointer(
				2,                                // attribute
				3,                                // size
				GL_FLOAT,                         // type
				GL_FALSE,                         // normalized?
				0,                                // stride
				(void*)0                          // array buffer offset
			);

	
			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			//rotQuat = glm::quat(glm::vec3(0,0.01,0))*rotQuat;
			
			glUniformMatrix4fv(cameraParams->MatrixID, 1, GL_FALSE, cameraParams->MVP);
			glUniformMatrix4fv(cameraParams->ViewMatrixID, 1, GL_FALSE, cameraParams->View);
			glUniformMatrix4fv(cameraParams->ModelMatrixID, 1, GL_FALSE, cameraParams->Model);
			
			glm::mat4 rotMatrix = mat4_cast(rotObject);
			glm::mat4 rotMatrixGlobal = mat4_cast(rotObjectGlobal);
			glUniform3fv(scaleID,1, glm::value_ptr(scaleVector));
			glUniform3fv(posID,1, glm::value_ptr(posVector));
			glUniformMatrix4fv(rotID, 1, GL_FALSE, &rotMatrix[0][0]);

			glUniform3fv(scaleIDGlobal,1, glm::value_ptr(scaleVectorGlobal));
			glUniform3fv(posIDGlobal,1, glm::value_ptr(posVectorGlobal));
			glUniformMatrix4fv(rotIDGlobal, 1, GL_FALSE, &rotMatrixGlobal[0][0]);
				
			// Draw the triangles !
			glDrawArrays(GL_TRIANGLES, 0, 12); // Starting from vertex 0; 6 vertices total -> 2 triangle

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);	
		}
		void Clean(){
			glDeleteBuffers(1, &vertexbuffer);
			glDeleteBuffers(1, &UVbuffer);
			glDeleteBuffers(1, &normalbuffer);
				
			glDeleteTextures(1, &Texture);
			glDeleteVertexArrays(1, &VertexArrayID);
		}
};

class WindowDrawer{
	protected:
	bool LPress = false;
	CameraParameters *cameraParams;
	glm::vec3 lightPos = glm::vec3(3,3,-4);
	Triangle *triangle;
	Triangle *triangle2;
	Square *square;
	

	//actual texture address on the Graphic card
	GLuint Texture;
	GLuint Texture2;
	GLuint Texture3;
	GLuint Texture4;
	//texture variable id on the shader
	GLuint TextureID;
	
	
	GLFWwindow* window;
	glm::mat4 Projection;
	glm::mat4 View;
	glm::mat4 Model;
	glm::mat4 MVP;	
	
	
	glm::quat rotQuat;
	glm::quat upDownQuat;

	GLuint MatrixID;
	GLuint ViewMatrixID;
	GLuint ModelMatrixID;
	GLuint LightID;
	
	vec3 CameraPos = glm::vec3(0,0,-10);



	// This will identify the shaders compiled for communication
	GLuint programID;
	double deltaTime = 0.0;
	double lastTime = 0.0;

	bool closed = false;	
	public:
		bool isClosed(){
			return closed;
		}
		int Init(int width = 1024, int height = 768){
			// Initialise GLFW
			glewExperimental = true; // Needed for core profile
			if( !glfwInit() )
			{
				fprintf( stderr, "Failed to initialize GLFW\n" );
				return -1;
			}
			glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
			//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL 

			// Open a window and create its OpenGL context
			window = glfwCreateWindow( 1024, 768, "Tutorial 01", NULL, NULL);
			if( window == NULL ){
				fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
				glfwTerminate();
				return -1;
			}
			glfwMakeContextCurrent(window); // Initialize GLEW
			//glewExperimental=true; // Needed in core profile
			if (glewInit() != GLEW_OK) {
				fprintf(stderr, "Failed to initialize GLEW\n");
				return -1;
			}
			glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);



			

			programID = LoadShaders( "myVertex.shader", "myFragment.shader" );

			//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
			Texture = loadDDS("uvtemplate.DDS");
			Texture2 = loadBMP_custom("uvtemplate.bmp");
			Texture4 = loadBMP_custom("uvtemplate.bmp");
			// Get a handle for our "myTextureSampler" uniform
			
			
			MatrixID = glGetUniformLocation(programID, "MVP");
			ViewMatrixID = glGetUniformLocation(programID, "V");
			ModelMatrixID = glGetUniformLocation(programID, "M");

			LightID = glGetUniformLocation(programID, "LightPosition_worldspace");			
			triangle = new Triangle();
			triangle2 = new Triangle();
			square = new Square();
			
			square->Set(programID,Texture4);
			triangle->Set(programID, Texture);
			triangle2->Set(programID, Texture);
			vec3 EulerAngles(0, 0, 0);
			upDownQuat = rotQuat = glm::quat(EulerAngles);			

			// Enable depth test
			glEnable(GL_DEPTH_TEST);
			// Accept fragment if it closer to the camera than the former one
			glDepthFunc(GL_LESS);
			UpdateCamera();
			cameraParams = new CameraParameters(window, &Projection[0][0],
													&View[0][0],
													&Model[0][0],
													&MVP[0][0],

													MatrixID,
													ViewMatrixID,
													ModelMatrixID);
									

			return 1;
		}
		void UpdateCamera(){
			// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
			Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
			// Or, for an ortho camera :
			//glm::mat4 Projection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.0f,100.0f); // In world coordinates
			
			// Camera matrix
			View       = glm::lookAt(
										CameraPos, // Camera is at (4,3,3), in World Space
										CameraPos+rotQuat*upDownQuat*glm::vec3(0,0,1), // and looks "forward"
										glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
								);
			// Model matrix : an identity matrix (model will be at the origin)
			Model      = glm::mat4(1.0f);
			
			// Our ModelViewProjection : multiplication of our 3 matrices
			MVP        = Projection * View * Model; // Remember, matrix multiplication is the other way around
		}
		bool Loop(){
			lastTime = glfwGetTime();
			if(closed){
				return false;
			}
			glfwMakeContextCurrent(window);

			
			// Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			UpdateCamera();
			//We send all the necessary info to draw
			glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);
			//Draw all triangles			

			triangle->Draw(cameraParams);
			triangle2->Draw(cameraParams);
			square->Draw(cameraParams);
			// Swap buffers
			glfwSwapBuffers(window);
			glfwPollEvents();
			if(glfwGetKey(window, GLFW_KEY_SPACE ) == GLFW_PRESS){
				triangle->greenValue+=0.007;
				if(triangle->greenValue>1){
					triangle->greenValue=1;
				}
			}
			if(glfwGetKey(window, GLFW_KEY_G ) == GLFW_PRESS){
				triangle->greenValue -= 0.007;
				if(triangle->greenValue<0){
					triangle->greenValue=0;
				}
			}
			if(glfwGetKey(window, GLFW_KEY_I ) == GLFW_PRESS){
				triangle->posVector.y+=0.007;
				if(triangle->posVector.y >3){
					triangle->posVector.y=3;
				}
			}
			if(glfwGetKey(window, GLFW_KEY_K ) == GLFW_PRESS){
				triangle->posVector.y-=0.007;
				if(triangle->posVector.y<-1){
					triangle->posVector.y=-1;
				}
				
			}
			if(glfwGetKey(window, GLFW_KEY_7 ) == GLFW_PRESS){
				triangle->posVectorGlobal.z-=0.007;
				if(triangle->posVectorGlobal.z<-1){
					triangle->posVectorGlobal.z=-1;
				}
				
			}

			if(glfwGetKey(window, GLFW_KEY_8 ) == GLFW_PRESS){
				triangle->rotObject = glm::quat(glm::vec3(0,-0.01,0))*triangle->rotObject;
			}
			if(glfwGetKey(window, GLFW_KEY_9 ) == GLFW_PRESS){
				triangle->rotObjectGlobal = glm::quat(glm::vec3(0.01,0,0))*triangle->rotObjectGlobal;
			}
			if(glfwGetKey(window, GLFW_KEY_0 ) == GLFW_PRESS){
				triangle->scaleVectorGlobal.z += 0.001;
			}

			if(glfwGetKey(window, GLFW_KEY_P ) == GLFW_PRESS){
				triangle->scaleVector.y += 0.001;
			}


			if(glfwGetKey(window, GLFW_KEY_L ) == GLFW_PRESS){
				if(!LPress){
					LPress = true;
					if(triangle->Texture == Texture){
						triangle->Texture = Texture2;
					}
					else{
						triangle->Texture = Texture;
					}
				}
			}
			else{
				LPress = false;
			}
			
			if(glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
				//Ahora hacia adelante es relativo, debe rotarse por el rotQuat
				CameraPos += rotQuat*vec3(0,0,1);

			}
			if(glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS){
				//Ahora hacia atrás es relativo, debe rotarse por el rotQuat
				CameraPos -= rotQuat*vec3(0,0,1);

			}

			if(glfwGetKey(window, GLFW_KEY_RIGHT ) == GLFW_PRESS)
			{
				//paso de euler una rotación en y a cuaternion para rotar
				//rotQuat sólo guarda rotación en Y
				rotQuat = glm::quat(vec3(0,-0.007,0))*rotQuat;
			}
			if(glfwGetKey(window, GLFW_KEY_LEFT ) == GLFW_PRESS)
			{
				rotQuat = glm::quat(vec3(0,0.007,0))*rotQuat;
			}
			if(glfwGetKey(window, GLFW_KEY_UP ) == GLFW_PRESS)
			{
				//paso de euler una rotación en x a cuaternion para rotar
				//upDownQuat sólo guarda rotación en X
				upDownQuat = glm::quat(vec3(-0.007,0,0))*upDownQuat;
			}
			if(glfwGetKey(window, GLFW_KEY_DOWN ) == GLFW_PRESS)
			{
				upDownQuat = glm::quat(vec3(0.007,0,0))*upDownQuat;
			}
			deltaTime = glfwGetTime() - lastTime;
			std::cout << "DeltaTime: " << deltaTime << std::endl;
			
			if(glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS && glfwWindowShouldClose(window) == 0)
			{
				return true;
			}
			else{
				closed = true;
				glfwDestroyWindow(window);
				// Cleanup VBO and shader
				triangle->Clean();
				triangle2->Clean();
				square->Clean();
				free(triangle);
				free(triangle2);
				free(square);
				glDeleteProgram(programID);
				free(cameraParams);
				return false;				
			}
			
		}

};


int main(){
	//Variable puntero de la memoría Heap, de uso explícito
    WindowDrawer *windowDrawerHeap = new WindowDrawer();
	if(windowDrawerHeap->Init() != 1){
		printf("Error en inicialización\n");
		return 0;
	}
	
    while(!(windowDrawerHeap->isClosed())){
		if(!windowDrawerHeap->isClosed()){
			windowDrawerHeap->Loop();
		}
	}
	delete(windowDrawerHeap);
}
