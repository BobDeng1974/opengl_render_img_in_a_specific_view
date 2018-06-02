#pragma once

#include <iostream>
#define GLEW_STATIC
#include <GL/glew.h>
using namespace std;
#include "glm\glm.h"

/*
read an obj file,and translate it to an VAO
*/
class ObjToVAO {

private:
	char* objfilename;
	int vertext_num;
	GLfloat* vertext_array;
	GLuint vbo, vao;
	GLMmodel* obj_file;
	GLfloat scale = 1.0;


	// get vertice 
	void get_vertices_from_obj_file(char* filename) {

		// read file 
		this->obj_file = glmReadOBJ(filename);


		//
		this->vertext_num = obj_file->numtriangles * 3;
		// allocate memory

		GLfloat * resulted = (GLfloat*)malloc(sizeof(GLfloat) * 3 * 8 * (obj_file->numtriangles));


		/**/
		for (int i = 0; i < obj_file->numtriangles; i++) {

			GLMtriangle tri = *(obj_file->triangles + i);
			// 1
			// vertex 1 position 
			*(resulted + 24 * i) = obj_file->vertices[3 * tri.vindices[0]]/scale;
			*(resulted + 24 * i + 1) = obj_file->vertices[3 * tri.vindices[0] + 1] / scale;
			*(resulted + 24 * i + 2) = obj_file->vertices[3 * tri.vindices[0] + 2] / scale;

			// vertex 1 texture coordinate
			*(resulted + 24 * i + 3) = obj_file->texcoords[2 * tri.tindices[0]];
			*(resulted + 24 * i + 4) = obj_file->texcoords[2 * tri.tindices[0] + 1];

			*(resulted + 24 * i + 5) = obj_file->normals[3 * tri.nindices[0]];
			*(resulted + 24 * i + 6) = obj_file->normals[3 * tri.nindices[0] + 1];
			*(resulted + 24 * i + 7) = obj_file->normals[3 * tri.nindices[0] + 2];

			//printf("%f %f %f %f %f \n", *(resulted + 15 * i), *(resulted + 15 * i + 1), *(resulted + 15 * i + 2), *(resulted + 15 * i + 3), *(resulted + 15 * i + 4));

			// 2
			// vertex 1 position 
			*(resulted + 24 * i + 8) = obj_file->vertices[3 * tri.vindices[1]] / scale;
			*(resulted + 24 * i + 9) = obj_file->vertices[3 * tri.vindices[1] + 1] / scale;
			*(resulted + 24 * i + 10) = obj_file->vertices[3 * tri.vindices[1] + 2] / scale;

			// vertex 1 texture coordinate
			*(resulted + 24 * i + 11) = obj_file->texcoords[2 * tri.tindices[1]];
			*(resulted + 24 * i + 12) = obj_file->texcoords[2 * tri.tindices[1] + 1];


			*(resulted + 24 * i + 13) = obj_file->normals[3 * tri.nindices[1]];
			*(resulted + 24 * i + 14) = obj_file->normals[3 * tri.nindices[1] + 1];
			*(resulted + 24 * i + 15) = obj_file->normals[3 * tri.nindices[1] + 2];

			//printf("%f %f %f %f %f \n", *(resulted + 15 * i+5), *(resulted + 15 * i + 6), *(resulted + 15 * i + 7), *(resulted + 15 * i + 8), *(resulted + 15 * i + 9));

			// 3
			// vertex 1 position 
			*(resulted + 24 * i + 16) = obj_file->vertices[3 * tri.vindices[2]] / scale;
			*(resulted + 24 * i + 17) = obj_file->vertices[3 * tri.vindices[2] + 1] / scale;
			*(resulted + 24 * i + 18) = obj_file->vertices[3 * tri.vindices[2] + 2] / scale;

			// vertex 1 texture coordinate
			*(resulted + 24 * i + 19) = obj_file->texcoords[2 * tri.tindices[2]];
			*(resulted + 24 * i + 20) = obj_file->texcoords[2 * tri.tindices[2] + 1];


			*(resulted + 24 * i + 21) = obj_file->normals[3 * tri.nindices[2]];
			*(resulted + 24 * i + 22) = obj_file->normals[3 * tri.nindices[2] + 1];
			*(resulted + 24 * i + 23) = obj_file->normals[3 * tri.nindices[2] + 2];

			//printf("%f %f %f %f %f \n", *(resulted + 15 * i+10), *(resulted + 15 * i + 11), *(resulted + 15 * i + 12), *(resulted + 15 * i + 13), *(resulted + 15 * i + 14));
		}

		this->vertext_array =  resulted;


		// print the vertex
		for (int i = 0; i < 3  * (obj_file->numtriangles); i++) {

			//cout << *(resulted + i * 8 + 0) << "  "<<*(resulted + i * 8 + 1) << "  " << *(resulted + i * 8 + 2) << "  / " << *(resulted + i * 8 + 3) << "  " << *(resulted + i * 8 + 4) << " / " << *(resulted + i * 8 + 5) << "  " << *(resulted + i * 8 + 6) << "  " << *(resulted + i * 8 + 7) << endl;
		}

	}


	// generate vao vbo
	void generate_vao_vbo() {
		//vertex buffer object VBO ���㻺�����
		//vertex array object VAO �����������
		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);

		//1.��
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);	//vbo�Ļ���������GL_ARRAY_BUFFER
											//�ɰ󶨶�����壬ֻҪ�ǲ�ͬ�Ļ�������
											//2.�Ѷ������鸴�Ƶ������й�OpenGLʹ��
		glBufferData(GL_ARRAY_BUFFER, this->vertext_num * 8*sizeof(GLfloat), this->vertext_array, GL_STATIC_DRAW);	//���û���������ݸ��Ƶ���ǰ�󶨵Ļ�����
																				//����1��Ŀ�껺������ͣ�����2���������ݵĴ�С������3��ʵ�����ݣ�����4���Կ���ι��������
																				//����4��static�������䣬dynamic�ı�ܶ࣬streamÿ�λ���ʱ���ı�


																				//3.���ö�������ָ��
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);	//����1��Ҫ���õĶ�������,vertexShader��position
																							//����2���������ԵĴ�С
																							//����3����������
																							//����4�������Ƿ񱻱�׼��
																							//����5��stride��������������������֮��ļ��:8*sizeof(GLfloat)����ֵ��������ʱ������Ϊ0��OpenGL�������岽��
																							//����6�������ڻ�������ʼλ�õ�ƫ����
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
		glEnableVertexAttribArray(0);	//���ö������ԣ���������������λ��ֵ
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);
		//4.���VAO
		glBindVertexArray(0);//���ú�OpenGL��������Է��������ط���������
	}



public:

	// constructor
	ObjToVAO(char* filename) {
		this->objfilename = filename;

		//1. get vertex array
		get_vertices_from_obj_file(this->objfilename);

		//2. 
		generate_vao_vbo();

	}

	// getters
	int get_vertex_num() {
		return this->vertext_num;
	}

	GLint get_vao() {

		return this->vao;
	}


	GLfloat* get_array() {
		return this->vertext_array;
	}

	GLMmodel* getModel() {
		return obj_file;
	}


	// get the max and min x,y,z
	GLvoid getMaxMinXYZ(GLfloat* dimensions)
	{
		GLuint i;
		GLfloat maxx, minx, maxy, miny, maxz, minz;

		/* get the max/mins */
		maxx = minx = this->obj_file->vertices[3 + 0];
		maxy = miny = this->obj_file->vertices[3 + 1];
		maxz = minz = this->obj_file->vertices[3 + 2];
		for (i = 1; i <= this->obj_file->numvertices; i++) {
			if (maxx < this->obj_file->vertices[3 * i + 0])
				maxx = this->obj_file->vertices[3 * i + 0];
			if (minx > this->obj_file->vertices[3 * i + 0])
				minx = this->obj_file->vertices[3 * i + 0];

			if (maxy < this->obj_file->vertices[3 * i + 1])
				maxy = this->obj_file->vertices[3 * i + 1];
			if (miny > this->obj_file->vertices[3 * i + 1])
				miny = this->obj_file->vertices[3 * i + 1];

			if (maxz < this->obj_file->vertices[3 * i + 2])
				maxz = this->obj_file->vertices[3 * i + 2];
			if (minz > this->obj_file->vertices[3 * i + 2])
				minz = this->obj_file->vertices[3 * i + 2];
		}

		/* calculate model width, height, and depth */
		dimensions[0] = (maxx);
		dimensions[1] = (minx);
		dimensions[2] = (maxy);
		dimensions[3] = (miny);
		dimensions[4] = (maxz);
		dimensions[5] = (minz);
	}

};