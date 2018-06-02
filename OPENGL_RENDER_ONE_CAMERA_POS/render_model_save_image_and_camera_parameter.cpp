// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// Std. Includes
#include <string>
#include <algorithm>
#include <map>
using namespace std;
// GLFW
#include <GLFW/glfw3.h>

// GL includes
#include "Shader.h"
// GLM Mathemtics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
# include "glm/glm.h" 
#include "ObjToVAO.h"

// opencv used to save image
#include <cv.h>  
#include <highgui.h>  

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void do_movement();
bool keys[1024];

using namespace std;

//call back
const GLuint WIDTH = 1500, HEIGHT = 1000;

//light position

//x y axis rotate 
//GLfloat x_rotate = -acos(0);
GLfloat x_rotate = 0.0;
GLfloat y_rotate = 0.0;
GLfloat z_rotate = 0.0;
GLfloat rotate_step = 0.02;

//scale 
GLfloat scale = 1;
GLfloat scale_step = 0.1;

// transform matrix
glm::mat4 g_model, g_view, g_projection;


//x y  view transfer
GLfloat x_transfer = 0.0;
GLfloat y_transfer = 0.0;
GLfloat z_transfer = 0.0;
GLfloat transfer_step = 10.0;

// GLModel
GLMmodel* model_object;


// cursor position
double cursor_xpos, cursor_ypos;
double selected_xpos, selected_ypos;


int main()
{
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Fail to creat GLFW window" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	//GLFW管理OpenGL的函数指针，调用任何OpenGL的函数前，先初始化GLFW
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Fail to initialize GLFW" << endl;
		return -1;
	}

	//视口
	glViewport(0, 0, WIDTH, HEIGHT);

	//深度测试
	glEnable(GL_DEPTH_TEST);
	//glDepthMask(GL_FALSE);	//禁用深度缓冲区写入
	glDepthFunc(GL_LESS);		//默认GL_LESS，深度测试使用的比较运算符

								//模板测试
								//glEnable(GL_STENCIL_TEST);
								//glStencilMask(0xFF);	//设置位掩码，默认为1，若设置为0x00，则类似深度测试的GL_FALSE
								//glStencilFunc(GL_ALWAYS, 1, 0xFF);	//参数1：设置模板测试操作；参数2：参考值；参数3：掩码值
								//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);	//参数1：模板测试失败采取的动作；参数2：模板测试通过但深度测试失败采取的动作；参数3：都通过采取的动作

								//混合
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//shader
	Shader transcelucentShader(".//shader//2_04shader_translucent.vs", ".//shader//2_04shader_translucent.frag");


	// translucent
	ObjToVAO translucent_obj(".//input//output_model.obj");
	model_object = translucent_obj.getModel();


	int width3, height3;


	unsigned char* image3 = SOIL_load_image(".//input//output_model.png", &width3, &height3, 0, SOIL_LOAD_RGBA);


	GLuint  translucentMap;

	//纹理3
	glGenTextures(1, &translucentMap);
	glBindTexture(GL_TEXTURE_2D, translucentMap);
	//纹理环绕方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	//纹理过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width3, height3, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);


	SOIL_free_image_data(image3);



	//Game loop
	while (!glfwWindowShouldClose(window)) {


		//检查事件
		glfwPollEvents();
		do_movement();

		int result = glRenderMode(GL_SELECT);
		if (result == 1) {

			printf("yes\n");
		}


		glRenderMode(GL_RENDER);


		//渲染指令
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


		// render translucent
		/**/
		transcelucentShader.Use();
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, translucentMap);
		glUniform1i(glGetUniformLocation(transcelucentShader.Program, "translucenttexture"), 6);


		glBindVertexArray(translucent_obj.get_vao());
		glm::mat4 model, view, projection;

		//model





		GLfloat fov = (GLfloat)atan2(29947, 48466);
		// projection
		// GLfloat)114.64*
		projection = glm::perspective(2 * (GLfloat)atan2(29947, 48466), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 20000.0f);

		//projection = glm::perspective((GLfloat)acos(0.707), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 20000.0f);

	

		

		// inverse rt
		model = glm::rotate(model, z_rotate, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, y_rotate, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, x_rotate, glm::vec3(1.0f, 0.0f, 0.0f));

		//scale
		model = glm::scale(model, glm::vec3(scale, scale, scale));

		//view = camera.GetViewMatrix();
		view = glm::translate(view, glm::vec3(x_transfer, y_transfer, z_transfer));
		//view = rt;


		g_model = model;
		g_view = view;

		glUniformMatrix4fv(glGetUniformLocation(transcelucentShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(transcelucentShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(transcelucentShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glPolygonMode(GL_FRONT, GL_LINE);
		glDrawArrays(GL_TRIANGLES, 0, translucent_obj.get_vertex_num());//参数1：图元类型；参数2：顶点数组的起始索引；参数3：顶点个数
		glBindVertexArray(0);

		/**/
		//交换缓冲
		glfwSwapBuffers(window);
	}

	//ourShader.Delete();
	glfwTerminate();
	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}



void do_movement()
{
	// Camera controls
	if (keys[GLFW_KEY_UP]) {
		y_transfer += transfer_step;
	}
	if (keys[GLFW_KEY_DOWN]) {
		y_transfer -= transfer_step;
	}
	if (keys[GLFW_KEY_LEFT]) {
		x_transfer -= transfer_step;
	}
	if (keys[GLFW_KEY_RIGHT]) {
		x_transfer += transfer_step;
	}

	if (keys[GLFW_KEY_I]) {
		z_transfer += transfer_step;
	}
	if (keys[GLFW_KEY_K]) {
		z_transfer -= transfer_step;
	}
	// Camera controls
	if (keys[GLFW_KEY_J]) {
		scale += scale_step;
	}
	if (keys[GLFW_KEY_L]) {
		scale -= scale_step;
	}


	if (keys[GLFW_KEY_W]) {
		y_rotate += rotate_step;
	}
	if (keys[GLFW_KEY_S]) {
		y_rotate -= rotate_step;
	}
	if (keys[GLFW_KEY_A]) {
		x_rotate -= rotate_step;
	}
	if (keys[GLFW_KEY_D]) {
		x_rotate += rotate_step;
	}

	if (keys[GLFW_KEY_Q]) {
		z_rotate += rotate_step;
	}
	if (keys[GLFW_KEY_E]) {
		z_rotate -= rotate_step;
	}


	// save photo
	if (keys[GLFW_KEY_F]) {
		// get data
		GLubyte *data = (GLubyte *)malloc(3 * WIDTH * HEIGHT);
		if (data) {
			glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data);
		}

		// create the sub image
		IplImage* sub_image = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);

		// get the sub image
		for (int i = 0; i < WIDTH; i++) {

			//
			for (int j = 0; j < HEIGHT; j++) {
				uchar *sub_img_point = (uchar*)(sub_image->imageData + j* sub_image->widthStep + i * 3);

				// write to the panorama
				sub_img_point[0] = *(data + 3 * (HEIGHT - 1 - j)*WIDTH + 3 * i + 2);
				sub_img_point[1] = *(data + 3 * (HEIGHT - 1 - j)*WIDTH + 3 * i + 1);
				sub_img_point[2] = *(data + 3 * (HEIGHT - 1 - j)*WIDTH + 3 * i);
			}

		}

		//1. save the image
		//cvSaveImage(".//output//rendered.png", sub_image);

		// resize image
		// resize the iamge to get a 2 order n resolution image
		IplImage * img_with_2_order_resolution = cvCreateImage(cvSize(6000, 4000), IPL_DEPTH_8U, 3);
		// resize
		cvResize(sub_image, img_with_2_order_resolution, CV_INTER_CUBIC);
		cvSaveImage(".//output//rendered.png", img_with_2_order_resolution);

		//2. save camear pose
		glm::mat4 rt = glm::inverse(g_model*g_view);
		ofstream out(".//output//camear_pose.txt");
		if (out.is_open())
		{
			out << rt[0][0] << " " << -rt[1][0] << " " << -rt[2][0] << " " << rt[3][0] << endl;
			out << rt[0][1] << " " << -rt[1][1] << " " << -rt[2][1] << " " << rt[3][1] << endl;
			out << rt[0][2] << " " << -rt[1][2] << " " << -rt[2][2] << " " << rt[3][2] << endl;
			out << rt[0][3] << " " << -rt[1][3] << " " << -rt[2][3] << " " << rt[3][3] << endl;
			out.close();
		}
	}
}
