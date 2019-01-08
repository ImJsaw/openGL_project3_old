#pragma once

#include <vgl.h>
#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"
#include <LoadShaders.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "glm/ext.hpp"
using namespace glm;
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

#define FACE_SIZE 5000
Tri_Mesh *mesh;
Tri_Mesh *patch; // selected patch

GLuint quadVAO;
GLuint quadVBO;
unsigned int framebuffer; 
unsigned int textureColorbuffer;
unsigned int programFrame;
unsigned int programUV;
glm::vec4 pixel;
int facesid[FACE_SIZE];
vector<int> facesid2;

float quadVertices1[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions   // texCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

bool isLoad = false;
std::vector<double> vertices;

std::vector<double> meshUV;
std::vector<double> verticesPatch; // patch的點，給vbo用
std::vector<double> patchUV; // patch的uv座標，給vbo用

unsigned int checkerBoardImg; // 貼圖
double rotateAngle = 0.0f;

float eyeAngleX = 0.0;
float eyeAngleY = 0.0;
float translateX = 0.0;
float translateY = 0.0;
float eyedistanceuv = 2.0;

float eyedistance = 2.0;
#define DOR(angle) (angle*3.1415/180);
int prevMouseX,prevMouseY;

mat4 MVPuv;
mat4 ProjectionUV;
mat4 ViewMatrixUV;

GLuint VBO;
GLuint VAO;
GLuint UBO;
GLuint VBOuv;
GLuint VAOuv;
int face;
int facePatch;

GLuint program;

GLuint mvpID;
GLuint ColorID;

mat4 Projection;
mat4 ViewMatrix;
mat4 Model;
mat4 MVP;

ShaderInfo meshShaders[] = {
	{ GL_VERTEX_SHADER, "meshShader.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "meshShader.fp" },//fragment shader
	{ GL_NONE, NULL } };

typedef struct _TextureData
{
	_TextureData() : width(0), height(0), data(0) {}
	int width;
	int height;
	unsigned char* data;
} TextureData;

TextureData Load_png(const char* path, bool mirroredY = true)
{
	TextureData texture;
	int n;
	stbi_uc *data = stbi_load(path, &texture.width, &texture.height, &n, 4);
	if (data != NULL)
	{
		texture.data = new unsigned char[texture.width * texture.height * 4 * sizeof(unsigned char)];
		memcpy(texture.data, data, texture.width * texture.height * 4 * sizeof(unsigned char));
		// vertical-mirror image data
		if (mirroredY)
		{
			for (size_t i = 0; i < texture.width; i++)
			{
				for (size_t j = 0; j < texture.height / 2; j++)
				{
					for (size_t k = 0; k < 4; k++) {
						std::swap(texture.data[(j * texture.width + i) * 4 + k], texture.data[((texture.height - j - 1) * texture.width + i) * 4 + k]);
					}
				}
			}
		}
		stbi_image_free(data);
		printf("texture load complete at path : %s\n", path);
	}
	return texture;
}

namespace OpenMesh_EX {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// MyForm 的摘要
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form{
		public:
			MyForm(void){
				//constructer
				InitializeComponent();
				std::cout << "construct" << std::endl;
				pixel.r = 0.0f;
				pixel.g = 0.0f;
				pixel.b = 0.0f;
				pixel.a = 0.0f;
				for (int i = 0; i < FACE_SIZE; i++) facesid[i] = -1;
				Projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
			}

		protected:
			/// <summary>
			/// 清除任何使用中的資源。
			/// </summary>
			~MyForm(){
				if (components){
					delete components;
				}
			}

		private: System::Windows::Forms::MenuStrip^  menuStrip1;
		private: System::Windows::Forms::ToolStripMenuItem^  fileToolStripMenuItem;
		private: System::Windows::Forms::ToolStripMenuItem^  loadModelToolStripMenuItem;
		private: System::Windows::Forms::OpenFileDialog^  openModelDialog;
		private: System::Windows::Forms::SaveFileDialog^  saveModelDialog;
		private: System::Windows::Forms::ToolStripMenuItem^  saveModelToolStripMenuItem;
		private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
		private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
		private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl2;
		private: System::Windows::Forms::Button^  button1;
		private: System::Windows::Forms::Button^  button2;
		protected:

		private:
			/// <summary>
			/// 設計工具所需的變數。
			/// </summary>
			System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
			/// <summary>
			/// 此為設計工具支援所需的方法 - 請勿使用程式碼編輯器修改
			/// 這個方法的內容。
			/// </summary>
			void InitializeComponent(void)
			{
				this->components = (gcnew System::ComponentModel::Container());
				HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
				HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
				HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting2 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
				HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat2 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
				this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
				this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
				this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
				this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
				this->hkoglPanelControl2 = (gcnew HKOGLPanel::HKOGLPanelControl());
				this->button1 = (gcnew System::Windows::Forms::Button());
				this->button2 = (gcnew System::Windows::Forms::Button());
				this->menuStrip1->SuspendLayout();
				this->tableLayoutPanel1->SuspendLayout();
				this->SuspendLayout();
				// 
				// menuStrip1
				// 
				this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
				this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
				this->menuStrip1->Location = System::Drawing::Point(0, 0);
				this->menuStrip1->Name = L"menuStrip1";
				this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
				this->menuStrip1->Size = System::Drawing::Size(1286, 28);
				this->menuStrip1->TabIndex = 1;
				this->menuStrip1->Text = L"menuStrip1";
				this->menuStrip1->ItemClicked += gcnew System::Windows::Forms::ToolStripItemClickedEventHandler(this, &MyForm::menuStrip1_ItemClicked);
				// 
				// fileToolStripMenuItem
				// 
				this->fileToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
					this->loadModelToolStripMenuItem,
						this->saveModelToolStripMenuItem
				});
				this->fileToolStripMenuItem->Name = L"fileToolStripMenuItem";
				this->fileToolStripMenuItem->Size = System::Drawing::Size(45, 24);
				this->fileToolStripMenuItem->Text = L"File";
				// 
				// loadModelToolStripMenuItem
				// 
				this->loadModelToolStripMenuItem->Name = L"loadModelToolStripMenuItem";
				this->loadModelToolStripMenuItem->Size = System::Drawing::Size(216, 26);
				this->loadModelToolStripMenuItem->Text = L"Load Model";
				this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
				// 
				// saveModelToolStripMenuItem
				// 
				this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
				this->saveModelToolStripMenuItem->Size = System::Drawing::Size(216, 26);
				this->saveModelToolStripMenuItem->Text = L"Save Model";
				this->saveModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::saveModelToolStripMenuItem_Click);
				// 
				// openModelDialog
				// 
				this->openModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::openModelDialog_FileOk);
				// 
				// saveModelDialog
				// 
				this->saveModelDialog->DefaultExt = L"obj";
				this->saveModelDialog->FileOk += gcnew System::ComponentModel::CancelEventHandler(this, &MyForm::saveModelDialog_FileOk);
				// 
				// hkoglPanelControl1
				// 
				hkcoglPanelCameraSetting1->Far = 1000;
				hkcoglPanelCameraSetting1->Fov = 45;
				hkcoglPanelCameraSetting1->Near = -1000;
				hkcoglPanelCameraSetting1->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
				this->hkoglPanelControl1->Camera_Setting = hkcoglPanelCameraSetting1;
				this->hkoglPanelControl1->Dock = System::Windows::Forms::DockStyle::Fill;
				this->hkoglPanelControl1->Location = System::Drawing::Point(4, 4);
				this->hkoglPanelControl1->Margin = System::Windows::Forms::Padding(4);
				this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
				hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
				this->hkoglPanelControl1->Size = System::Drawing::Size(709, 608);
				this->hkoglPanelControl1->TabIndex = 2;
				this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
				this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
				this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
				this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
				this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
				// 
				// tableLayoutPanel1
				// 
				this->tableLayoutPanel1->ColumnCount = 2;
				this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					61.05442F)));
				this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent,
					38.94558F)));
				this->tableLayoutPanel1->Controls->Add(this->hkoglPanelControl2, 1, 0);
				this->tableLayoutPanel1->Controls->Add(this->hkoglPanelControl1, 0, 0);
				this->tableLayoutPanel1->Location = System::Drawing::Point(0, 27);
				this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
				this->tableLayoutPanel1->RowCount = 1;
				this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
				this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Percent, 50)));
				this->tableLayoutPanel1->Size = System::Drawing::Size(1176, 616);
				this->tableLayoutPanel1->TabIndex = 3;
				this->tableLayoutPanel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::tableLayoutPanel1_Paint);
				// 
				// hkoglPanelControl2
				// 
				hkcoglPanelCameraSetting2->Far = 1000;
				hkcoglPanelCameraSetting2->Fov = 45;
				hkcoglPanelCameraSetting2->Near = -1000;
				hkcoglPanelCameraSetting2->Type = HKOGLPanel::HKCOGLPanelCameraSetting::CAMERATYPE::ORTHOGRAPHIC;
				this->hkoglPanelControl2->Camera_Setting = hkcoglPanelCameraSetting2;
				this->hkoglPanelControl2->Dock = System::Windows::Forms::DockStyle::Fill;
				this->hkoglPanelControl2->Location = System::Drawing::Point(721, 4);
				this->hkoglPanelControl2->Margin = System::Windows::Forms::Padding(4);
				this->hkoglPanelControl2->Name = L"hkoglPanelControl2";
				hkcoglPanelPixelFormat2->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat2->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat2->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				this->hkoglPanelControl2->Pixel_Format = hkcoglPanelPixelFormat2;
				this->hkoglPanelControl2->Size = System::Drawing::Size(451, 608);
				this->hkoglPanelControl2->TabIndex = 4;
				this->hkoglPanelControl2->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl2_Load);
				this->hkoglPanelControl2->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl2_Paint);
				this->hkoglPanelControl2->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl2_MouseWheel);
				// 
				// button1
				// 
				this->button1->Location = System::Drawing::Point(1182, 49);
				this->button1->Name = L"button1";
				this->button1->Size = System::Drawing::Size(92, 47);
				this->button1->TabIndex = 4;
				this->button1->Text = L"loadPatch";
				this->button1->UseVisualStyleBackColor = true;
				// 
				// button2
				// 
				this->button2->Location = System::Drawing::Point(1182, 114);
				this->button2->Name = L"button2";
				this->button2->Size = System::Drawing::Size(92, 46);
				this->button2->TabIndex = 5;
				this->button2->Text = L"loadTxt";
				this->button2->UseVisualStyleBackColor = true;
				// 
				// MyForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(1286, 645);
				this->Controls->Add(this->button2);
				this->Controls->Add(this->button1);
				this->Controls->Add(this->tableLayoutPanel1);
				this->Controls->Add(this->menuStrip1);
				this->MainMenuStrip = this->menuStrip1;
				this->Margin = System::Windows::Forms::Padding(4);
				this->Name = L"MyForm";
				this->Text = L"OpenMesh_EX";
				this->menuStrip1->ResumeLayout(false);
				this->menuStrip1->PerformLayout();
				this->tableLayoutPanel1->ResumeLayout(false);
				this->ResumeLayout(false);
				this->PerformLayout();

			}
#pragma endregion

		//init
		private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e){
			glewExperimental = GL_TRUE; //置於glewInit()之前
			if (glewInit()) {
				std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
				exit(EXIT_FAILURE);
			}
			else std::cout << "initialize GLEW success" << std::endl;//c error

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			/*
			*/
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			//VAO
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			program = LoadShaders(meshShaders);//讀取shader

			glUseProgram(program);//uniform參數數值前必須先use shader
			mvpID = glGetUniformLocation(program, "MVP");
			ColorID = glGetUniformLocation(program, "color");

			ViewMatrix = glm::lookAt(
				glm::vec3(0, 5, 5), // Camera is at (0,10,25), in World Space
				glm::vec3(0, 0, 0), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,1,0 to look upside-down)
			);

			const std::string ProjectName = "heightmap.jpg";
			TextureData tdata = Load_png((ProjectName).c_str(), true);

			//Generate empty texture
			glGenTextures(1, &checkerBoardImg);
			glBindTexture(GL_TEXTURE_2D, checkerBoardImg);
			//Do texture setting
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, tdata.width, tdata.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tdata.data);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glBindTexture(GL_TEXTURE_2D, 0);
			//-----------------------------------------------------------
			glUniform1i(glGetUniformLocation(program, "sprite"), 0);


			glClearColor(0.0, 0.0, 0.0, 1);//black screen

			//use frameBuffer to store face id
			ShaderInfo shaderframe[] = {
			{ GL_VERTEX_SHADER, "framebuffer.vp" },//vertex shader
			{ GL_FRAGMENT_SHADER, "framebuffer.fp" },//fragment shader
			{ GL_NONE, NULL } };
			programFrame = LoadShaders(shaderframe);
			glUseProgram(programFrame);//uniform參數數值前必須先use shader

			// screen quad VAO
			glGenVertexArrays(1, &quadVAO);
			glGenBuffers(1, &quadVBO);

			glUseProgram(programFrame);
			glUniform1i(glGetUniformLocation(programFrame, "screenTexture"), 0);

			// framebuffer configuration
			// -------------------------
			glGenFramebuffers(1, &framebuffer);
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
			// create a color attachment texture

			glGenTextures(1, &textureColorbuffer);
			glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
			//GL_RGBA32F for store value > 1
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, hkoglPanelControl1->Width, hkoglPanelControl1->Height, 0, GL_RGBA, GL_FLOAT, NULL);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);


			GLenum dr[2] = { GL_COLOR_ATTACHMENT0 ,GL_DEPTH_ATTACHMENT };
			glDrawBuffers(2, dr);

			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
				cout << "Framebuffer is not complete!" << endl;
			if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT)
				cout << "Framebuffer is not complete attach!" << endl;

			//bind to normal
			glBindTexture(GL_TEXTURE_2D, 0);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

		}
		//display
		private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e){
			//std::cout << "refresh" << std::endl;
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
			glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)
			glClearDepth(1.0);
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			if (mesh != NULL) {
				//std::cout << "refresh mesh not null" << std::endl;
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double), &vertices[0], GL_STATIC_DRAW);
			}

			glBindVertexArray(VAO);
			glUseProgram(program);//uniform參數數值前必須先use shader

			float horizonAngle = DOR(eyeAngleX);
			float verticleAngle = DOR(eyeAngleY);
			ViewMatrix = lookAt(
				glm::vec3(eyedistance*cos(horizonAngle)*cos(verticleAngle), eyedistance*sin(verticleAngle), eyedistance*sin(horizonAngle)*cos(verticleAngle)), 
				glm::vec3(0, 0, 0), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);

			mat4 Model = translate(translateX, translateY,0.0f);
			MVP = Model * Projection * ViewMatrix;

			glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,				//location
				3,				//vec3
				GL_DOUBLE,			//type
				GL_FALSE,			//not normalized
				3 * sizeof(double),		//strip
				0);//buffer offset
			if (isLoad) {//draw faceID to frameBuffer
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::vec3 color = glm::vec3(-1.0, 0.0, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, checkerBoardImg);
				glDrawArrays(GL_TRIANGLES, 0, face * 3);
			}
			glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
			if (isLoad) {//draw to screen
				//face
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::vec3 color = glm::vec3(1.0, 0.85, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glDrawArrays(GL_TRIANGLES, 0, face * 3);
				//line
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				color = glm::vec3(0.0, 0.0, 0.0);
				glUniform3fv(ColorID, 1, &color[0]);
				glDrawArrays(GL_TRIANGLES, 0, face * 3);
			}

			//----------------------------
			//畫出所選到的面(紅色)
			//---------------------------
			//glDeleteVertexArrays(1, &VAO);
			//glDeleteBuffers(1, &VBO);

			if (patch != NULL) {
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				//glBufferData(GL_ARRAY_BUFFER, verticesPatch.size() * sizeof(double), &verticesPatch[0], GL_STATIC_DRAW);
				glBufferData(GL_ARRAY_BUFFER, verticesPatch.size() * sizeof(double) + patchUV.size() * sizeof(double), &verticesPatch[0], GL_STATIC_DRAW);
				glBufferSubData(GL_ARRAY_BUFFER, verticesPatch.size() * sizeof(double), patchUV.size() * sizeof(double), &patchUV[0]);
				//debug1，把VAO重訂的部分拉上來
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0,				//location
					3,				//vec3
					GL_DOUBLE,			//type
					GL_FALSE,			//not normalized
					0,				//strip
					0);//buffer offset
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1,				//location
					2,				//vec3
					GL_DOUBLE,			//type
					GL_FALSE,			//not normalized
					0,				//strip
					(void *)(verticesPatch.size() * sizeof(double))
				);//buffer offset
			}

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LEQUAL);
			glUseProgram(program);//uniform參數數值前必須先use shader
			glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVP[0][0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			if (facesid2.size() != 0) {
				//draw red patch
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glm::vec3 color = glm::vec3(1.0, 0.0, 0.5);
				glUniform3fv(ColorID, 1, &color[0]);
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, checkerBoardImg);
				glDrawArrays(GL_TRIANGLES, 0, facePatch * 3);
			}
		}

		//mouseClick
		private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Button == System::Windows::Forms::MouseButtons::Left){
				//leftClick	
				//record mouse position for drag event
				prevMouseX = e->X;
				prevMouseY = e->Y;
				//read face
				glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
				glReadBuffer(GL_COLOR_ATTACHMENT0);
				glReadPixels(e->X, hkoglPanelControl1->Height - e->Y, 1, 1, GL_RGBA, GL_FLOAT, &pixel);
				cout << "face id : " << pixel.r << endl;
				//printf("mouse x = %d mouse y = %d\n", e->X, hkoglPanelControl1->Height - e->Y);
				if (isLoad) {
					//detect same face already stored
					for (int i = 0; i < facesid2.size(); i++) {
						if (facesid2[i] == int(pixel.r) - 1) break;
						if (pixel.r != 0 && ( facesid2[i] > int(pixel.r) - 1 || i == facesid2.size()-1 ) ) { 
							//when id > curID mean no repeat 'cause vector sorted
							facesid2.push_back(int(pixel.r) - 1);
							break;
						}
					}
					//first value
					if (pixel.r != 0 && facesid2.size() == 0) facesid2.push_back(int(pixel.r) - 1);
					std::sort(facesid2.begin(), facesid2.end());

					cout << "selected faceID: ";
					for (int i = 0; i < facesid2.size(); i++) cout << facesid2[i] << " ";
					cout << endl;
					//cout << endl << "selected face count : " << facesid2.size() << endl;
				}

				//----------------------------------
				//load face into vector
				//----------------------------------
				if (facesid2.size() != 0) {
					//del old mesh on screen
					if (patch != NULL) delete patch;
					patch = new Tri_Mesh;
					//clear vertices and face to null
					verticesPatch.clear();
					patchUV.clear();
					//verticesPatch.resize(0);
					//std::cout << "verticesPatch.resize(0)" << verticesPatch.size() << std::endl;
					facePatch = 0;
					// ReadFile(filename, patch); // change form here
					//patch->loadToBufferPatch(verticesPatch, facePatch, facesid, facesptr);
					mesh->loadToBufferPatch(verticesPatch, facePatch, facesid2, *patch);
					patch->getUV(patchUV, *patch, rotateAngle);
					//std::cout << "facePatch" << facePatch << std::endl;
					//std::cout << "verticesPatch.size()" << verticesPatch.size() << std::endl;
				}

				glReadBuffer(GL_NONE);
				glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);

			}
			if (e->Button == System::Windows::Forms::MouseButtons::Middle) {
				//record mouse position for drag event
				prevMouseX = e->X;
				prevMouseY = e->Y;
			}
			hkoglPanelControl1->Invalidate();
		}

		//mouseDrag
		private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Button == System::Windows::Forms::MouseButtons::Left){
				//std::cout << "left" << std::endl;
				eyeAngleX += (e->X - prevMouseX)*0.05;
				eyeAngleY += (e->Y - prevMouseY)*0.05;
				//record mouse position for drag event
				prevMouseX = e->X;
				prevMouseY = e->Y;
			}
			if (e->Button == System::Windows::Forms::MouseButtons::Middle){
				//std::cout << "middle" << std::endl;
				translateX += (e->X - prevMouseX)*0.002;
				translateY -= (e->Y - prevMouseY)*0.002;
				//record mouse position for drag event
				prevMouseX = e->X;
				prevMouseY = e->Y;
			}
			hkoglPanelControl1->Invalidate();
		}

		//mouseWheel
		private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Delta < 0) eyedistance += 0.1;
			else{
				eyedistance -= 0.1;
				if (eyedistance < 0.4) eyedistance = 0.4;
				//std::cout << "wheel up, distance : "  << eyedistance << std::endl;
			}
			hkoglPanelControl1->Invalidate();
		}

		//click "openModel"
		private: System::Void loadModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e){
			openModelDialog->Filter = "Model(*.obj)|*obj";
			openModelDialog->Multiselect = false;
			openModelDialog->ShowDialog();
		}

		//check openModel dialog
		private: System::Void openModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e){
			std::string filename;
			MarshalString(openModelDialog->FileName, filename);
			//del old mesh on screen
			if (mesh != NULL) delete mesh;
			mesh = new Tri_Mesh;
			if (ReadFile(filename, mesh)) std::cout << filename << std::endl;
			isLoad = true;
			mesh->loadToBuffer(vertices,face);
			for (int i = 0; i < vertices.size(); i += 3) {
				meshUV.push_back(0.0f);
				meshUV.push_back(0.0f);
			}
			std::cout << "meshUV.size() : " << meshUV.size() << "vertices.size()" << vertices.size() << endl;
			std::cout << "face" << face << std::endl;
			hkoglPanelControl1->Invalidate();
		}

		//saveObj menu open
		private: System::Void saveModelToolStripMenuItem_Click(System::Object^  sender, System::EventArgs^  e){
			saveModelDialog->Filter = "Model(*.obj)|*obj";
			saveModelDialog->ShowDialog();
		}

		//check saveObj in dialog 
		private: System::Void saveModelDialog_FileOk(System::Object^  sender, System::ComponentModel::CancelEventArgs^  e){
			std::string filename;
			MarshalString(saveModelDialog->FileName, filename);
			if (SaveFile(filename, mesh)) std::cout << filename << std::endl;
		}
				 private: System::Void hkoglPanelControl2_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {

					 glClearColor(0.5, 0.5, 0.5, 1.0);
					 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					 glEnable(GL_DEPTH_TEST);

					 std::cout << "un-paint uv ， facesid2.size() = " << facesid2.size() << endl;
					 std::cout << "facePatch = " << facePatch << endl;
					 std::cout << "patchUV.size() = " << patchUV.size() << endl;
					 for (int i = 0; i < patchUV.size(); i += 2) {
						 std::cout << "s = " << patchUV[i] << " " << "t = " << patchUV[i + 1] << endl;
					 }
					 if (facesid2.size() != 0) {
						 std::cout << "Drawing the uv texcoord..." << endl;
						 glGenBuffers(1, &VBOuv);
						 glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
						 glBufferData(GL_ARRAY_BUFFER, patchUV.size() * sizeof(double), &patchUV[0], GL_STATIC_DRAW);

						 glBindVertexArray(VAOuv);
						 glUseProgram(programUV);

						 float horizonAngle = DOR(eyeAngleX);
						 float verticleAngle = DOR(eyeAngleY);
						 ViewMatrixUV = lookAt(
							 glm::vec3(eyedistanceuv*cos(horizonAngle)*cos(verticleAngle), eyedistanceuv*sin(verticleAngle), eyedistanceuv*sin(horizonAngle)*cos(verticleAngle)),
							 glm::vec3(0, 0, 0), // and looks at the origin
							 glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
						 );

						 mat4 Modeluv = glm::translate(translateX, translateY, 0.0f);
						 //MVPuv = Modeluv * ProjectionUV * ViewMatrixUV;
						 MVPuv = ProjectionUV * ViewMatrixUV * Modeluv;
						 glUniformMatrix4fv(mvpID, 1, GL_FALSE, &MVPuv[0][0]);

						 glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
						 glEnableVertexAttribArray(0);
						 //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(double), (void*)0);
						 glVertexAttribPointer(0, 2, GL_DOUBLE, GL_FALSE, 2 * sizeof(double), (void*)0);

						 glUseProgram(programUV);

						 //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						 //glDrawArrays(GL_TRIANGLES, 0, facePatch * 3);
						 glDrawArrays(GL_TRIANGLES, 0, facePatch * 3);
					 }


					 //hkoglPanelControl2->Invalidate();
				 }
		private: System::Void hkoglPanelControl2_Load(System::Object^  sender, System::EventArgs^  e) {

			glewExperimental = GL_TRUE; //置於glewInit()之前
			if (glewInit()) {
				std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
				exit(EXIT_FAILURE);
			}
			else std::cout << "initialize GLEW success" << std::endl;//c error

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);
			glDisable(GL_CULL_FACE);

			ProjectionUV = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 1000.0f);

			glGenVertexArrays(1, &VAOuv);
			glBindVertexArray(VAOuv);

			ShaderInfo UVShader[] = {
			{ GL_VERTEX_SHADER, "uv.vp" },//vertex shader
			{ GL_FRAGMENT_SHADER, "uv.fp" },//fragment shader
			{ GL_NONE, NULL } };
			programUV = LoadShaders(UVShader);//讀取shader

			glUseProgram(programUV);//uniform參數數值前必須先use shader


			mvpID = glGetUniformLocation(programUV, "MVP");
			//glGenVertexArrays(1, &VAOuv);
			//glGenBuffers(1, &VBOuv);

			//glBindVertexArray(VAOuv);

			//glBindBuffer(GL_ARRAY_BUFFER, VBOuv);
			//glBufferData(GL_ARRAY_BUFFER, patchUV.size() * sizeof(double), &patchUV[0], GL_STATIC_DRAW);

			// position attribute
			//glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			//glEnableVertexAttribArray(0);

			//glUseProgram(programUV);
		}
		private: System::Void hkoglPanelControl2_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
			if (e->Delta < 0) {
				eyedistanceuv += 0.1;
				std::cout << "wheel down, distance : " << eyedistanceuv << std::endl;
			}
			else {
				eyedistanceuv -= 0.1;
				if (eyedistanceuv < 0.4) eyedistanceuv = 0.4;
				std::cout << "wheel up, distance : " << eyedistanceuv << std::endl;
			}
			hkoglPanelControl1->Invalidate();
		}
	private: System::Void tableLayoutPanel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {}
	private: System::Void menuStrip1_ItemClicked(System::Object^  sender, System::Windows::Forms::ToolStripItemClickedEventArgs^  e) {}
};
}
