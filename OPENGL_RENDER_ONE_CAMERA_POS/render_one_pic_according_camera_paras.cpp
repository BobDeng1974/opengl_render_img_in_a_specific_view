// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <sstream>
#include <algorithm>
#include <map>
using namespace std;
#include <GLFW/glfw3.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <SOIL.h>
#include "glm/glm.h"
#include "ObjToVAO.h"
#include <opencv\cv.h>  
#include <opencv\highgui.h>  
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void render_a_pic_according_camera_pos(char * extrinsics_file, char* intrinsics_file, char* transform_matrix_file, char* model_file_name, char* current_selected_image_file, int index, double scale, int WIDTH, int HEIGHT);
const GLuint WIDTH = 1500, HEIGHT = 1000;
int main() {

	char in[100] = ".//input//K.txt";
	char ex[100] = ".//input//E.txt";
	char model_file_name[100] = ".//input//2017_05_05_jiuping.obj";

	char current_selected_image_file[100] = ".";
	char tr[100] = ".//input//result_r_t.txt";

	double scale = 192.9;
	render_a_pic_according_camera_pos(ex,in,tr, model_file_name, current_selected_image_file, 4, scale, WIDTH, HEIGHT);
}

/*
index start with 1
*/
void render_a_pic_according_camera_pos(char * extrinsics_file, char* intrinsics_file, char* transform_matrix_file, char* model_file_name, char* current_selected_image_file,int index, double scale, int WIDTH, int HEIGHT) {
	
	//1. get the camerapos
	double extrinsic[12];
	double intrinsic[9];

	string str_intrin;
	string str_extrin;

	int i = 1;

	ifstream fin_intrinsic(intrinsics_file);
	ifstream fin_extrinsic(extrinsics_file);


	int flag = false;
	while (getline(fin_intrinsic, str_intrin)&& getline(fin_extrinsic, str_extrin))
	{
		if (i == index) {

			// get camear intrinsic
			stringstream sstream;
			sstream << str_intrin;
			sstream >> intrinsic[0] >> intrinsic[1] >> intrinsic[2] >> intrinsic[3] >> intrinsic[4] >> intrinsic[5] >> intrinsic[6] >> intrinsic[7] >> intrinsic[8];

			// get camera extrinsic
			sstream << str_extrin;
			sstream >> extrinsic[0] >> extrinsic[1] >> extrinsic[2] >> extrinsic[3] >> extrinsic[4] >> extrinsic[5] >> extrinsic[6] >> extrinsic[7] >> extrinsic[8] >> extrinsic[9] >> extrinsic[10] >> extrinsic[11];

			flag = true;
		}

		i++;
	}

	if (flag == false) {
		cout << "error in reading camear parameters!!!" << endl;
		fin_intrinsic.close();
		fin_extrinsic.close();
		return;
	}

	fin_intrinsic.close();
	fin_extrinsic.close();


	// 2. get the transform matrixs
	double transform_matrix[12];
	ifstream fin_transform(transform_matrix_file);

	string str_transform;
	int line_num = 0;
	while (getline(fin_transform, str_transform)&& line_num<3) {

		// get camear intrinsic
		stringstream sstream;
		sstream << str_transform;
		sstream >> transform_matrix[line_num * 4] >> transform_matrix[line_num * 4 + 1] >> transform_matrix[line_num * 4 + 2] >> transform_matrix[line_num * 4 + 3];

		line_num++;
	}
	fin_transform.close();

	// 3. render
	if (!glfwInit())
	{
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		return ;
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Hello", nullptr, nullptr);
	if (window == nullptr)
	{
		cout << "Fail to creat GLFW window" << endl;
		glfwTerminate();
		return ;
	}
	glfwMakeContextCurrent(window);

	glfwHideWindow(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		cout << "Fail to initialize GLFW" << endl;
		return ;
	}

	glViewport(0, 0, WIDTH, HEIGHT);
	glEnable(GL_DEPTH_TEST);


	//shader
	Shader transcelucentShader(".//shader//2_04shader_translucent.vs", ".//shader//2_04shader_translucent.frag");


	// translucent
	ObjToVAO translucent_obj(model_file_name);
	GLMmodel* model_object = translucent_obj.getModel();

	
	//model
	glRenderMode(GL_RENDER);
	glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	transcelucentShader.Use();


	glBindVertexArray(translucent_obj.get_vao());
	glm::mat4 model, view, projection;

	GLfloat fovy = 2 * (GLfloat)atan2(intrinsic[7], intrinsic[4]);

	projection = glm::perspective(fovy, (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 20000.0f);

	glm::mat4 CamSystemRelativeToModel = glm::mat4(
		glm::vec4(transform_matrix[0], transform_matrix[4], transform_matrix[8], 0),
		glm::vec4(transform_matrix[1], transform_matrix[5], transform_matrix[9], 0),
		glm::vec4(transform_matrix[2], transform_matrix[6], transform_matrix[10], 0),
		glm::vec4(transform_matrix[3], transform_matrix[7], transform_matrix[11], 1));

	glm::mat4 CamSystemRelativeToThisCamera = glm::mat4(
		glm::vec4(extrinsic[0], extrinsic[1], extrinsic[2], 0),
		glm::vec4(extrinsic[3], extrinsic[4], extrinsic[5], 0),
		glm::vec4(extrinsic[6], extrinsic[7], extrinsic[8], 0),
		glm::vec4(extrinsic[9], extrinsic[10], extrinsic[11], 1));

	model = CamSystemRelativeToThisCamera * glm::inverse(CamSystemRelativeToModel);

	view = glm::mat4(
		glm::vec4(1, 0, 0, 0),
		glm::vec4(0, -1, 0, 0),
		glm::vec4(0, 0, -1, 0),
		glm::vec4(0, 0, 0, 1));

	glUniformMatrix4fv(glGetUniformLocation(transcelucentShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(transcelucentShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(transcelucentShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	glUniform1f(glGetUniformLocation(transcelucentShader.Program, "scale"), scale);

	//glPolygonMode(GL_FRONT, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, translucent_obj.get_vertex_num());
	glBindVertexArray(0);

	// 4. saave the render image
	GLubyte *data = (GLubyte *)malloc(3 * WIDTH * HEIGHT);
	if (data) {
		glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	//create the sub image
	cv::Mat rendered_img(HEIGHT, WIDTH, CV_8UC3);
	memcpy(rendered_img.data, data, HEIGHT*WIDTH * 3);
	cv::flip(rendered_img, rendered_img, 0); // different image coordinate between opengl and opencv

	string str_saved_name;
	stringstream sstream;
	sstream << current_selected_image_file;
	sstream << "//output//rendered"  << ".png";
	sstream >> str_saved_name;
	cv::imwrite(str_saved_name, rendered_img);

	printf("rendered image %s\n", str_saved_name.c_str());


	system("pause");
}