#pragma once

#include <vgl.h>
#include "DotNetUtilities.h"
#include "Mesh/GUA_OM.h"
#include "Mesh/DP.h"
#include <LoadShaders.h>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

Tri_Mesh *mesh;

bool isLoad = false;
std::vector<double> vertices;

xform xf;
GLCamera camera;
float fov = 0.7f;

float eyeAngley = 0.0;
float eyedistance = 30.0;
#define DOR(angle) (angle*3.1415/180);

GLuint VBO;
GLuint VAO;
GLuint UBO;
int face;

GLint posX, posY, posZ;

GLuint program;

GLint MatricesIdx;
GLuint ModelID;

mat4 Projection;
mat4 ViewMatrix;
mat4 Model;

ShaderInfo shaders_robot[] = {
	{ GL_VERTEX_SHADER, "robotShader.vp" },//vertex shader
	{ GL_FRAGMENT_SHADER, "robotShader.fp" },//fragment shader
	{ GL_NONE, NULL } };


static const Mouse::button physical_to_logical_map[] = {
	Mouse::NONE, Mouse::ROTATE, Mouse::MOVEXY, Mouse::MOVEZ,
	Mouse::MOVEZ, Mouse::MOVEXY, Mouse::MOVEXY, Mouse::MOVEXY,
};
Mouse::button Mouse_State = Mouse::ROTATE;

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
				posX = 0;
				posY = 0;
				posZ = 0;
				std::cout << "construct" << std::endl;
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
		private: HKOGLPanel::HKOGLPanelControl^  hkoglPanelControl1;
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
				HKOGLPanel::HKCOGLPanelCameraSetting^  hkcoglPanelCameraSetting1 = (gcnew HKOGLPanel::HKCOGLPanelCameraSetting());
				HKOGLPanel::HKCOGLPanelPixelFormat^  hkcoglPanelPixelFormat1 = (gcnew HKOGLPanel::HKCOGLPanelPixelFormat());
				this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
				this->fileToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->loadModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->saveModelToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
				this->openModelDialog = (gcnew System::Windows::Forms::OpenFileDialog());
				this->saveModelDialog = (gcnew System::Windows::Forms::SaveFileDialog());
				this->hkoglPanelControl1 = (gcnew HKOGLPanel::HKOGLPanelControl());
				this->menuStrip1->SuspendLayout();
				this->SuspendLayout();
				// 
				// menuStrip1
				// 
				this->menuStrip1->ImageScalingSize = System::Drawing::Size(20, 20);
				this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(1) { this->fileToolStripMenuItem });
				this->menuStrip1->Location = System::Drawing::Point(0, 0);
				this->menuStrip1->Name = L"menuStrip1";
				this->menuStrip1->Padding = System::Windows::Forms::Padding(8, 2, 0, 2);
				this->menuStrip1->Size = System::Drawing::Size(817, 28);
				this->menuStrip1->TabIndex = 1;
				this->menuStrip1->Text = L"menuStrip1";
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
				this->loadModelToolStripMenuItem->Size = System::Drawing::Size(168, 26);
				this->loadModelToolStripMenuItem->Text = L"Load Model";
				this->loadModelToolStripMenuItem->Click += gcnew System::EventHandler(this, &MyForm::loadModelToolStripMenuItem_Click);
				// 
				// saveModelToolStripMenuItem
				// 
				this->saveModelToolStripMenuItem->Name = L"saveModelToolStripMenuItem";
				this->saveModelToolStripMenuItem->Size = System::Drawing::Size(168, 26);
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
				this->hkoglPanelControl1->Location = System::Drawing::Point(0, 28);
				this->hkoglPanelControl1->Margin = System::Windows::Forms::Padding(4);
				this->hkoglPanelControl1->Name = L"hkoglPanelControl1";
				hkcoglPanelPixelFormat1->Accumu_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat1->Alpha_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				hkcoglPanelPixelFormat1->Stencil_Buffer_Bits = HKOGLPanel::HKCOGLPanelPixelFormat::PIXELBITS::BITS_0;
				this->hkoglPanelControl1->Pixel_Format = hkcoglPanelPixelFormat1;
				this->hkoglPanelControl1->Size = System::Drawing::Size(817, 540);
				this->hkoglPanelControl1->TabIndex = 2;
				this->hkoglPanelControl1->Load += gcnew System::EventHandler(this, &MyForm::hkoglPanelControl1_Load);
				this->hkoglPanelControl1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MyForm::hkoglPanelControl1_Paint);
				this->hkoglPanelControl1->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseDown);
				this->hkoglPanelControl1->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseMove);
				this->hkoglPanelControl1->MouseWheel += gcnew System::Windows::Forms::MouseEventHandler(this, &MyForm::hkoglPanelControl1_MouseWheel);
				// 
				// MyForm
				// 
				this->AutoScaleDimensions = System::Drawing::SizeF(8, 15);
				this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
				this->ClientSize = System::Drawing::Size(817, 568);
				this->Controls->Add(this->hkoglPanelControl1);
				this->Controls->Add(this->menuStrip1);
				this->MainMenuStrip = this->menuStrip1;
				this->Margin = System::Windows::Forms::Padding(4);
				this->Name = L"MyForm";
				this->Text = L"OpenMesh_EX";
				this->menuStrip1->ResumeLayout(false);
				this->menuStrip1->PerformLayout();
				this->ResumeLayout(false);
				this->PerformLayout();

			}
	#pragma endregion

		//init
		private: System::Void hkoglPanelControl1_Load(System::Object^  sender, System::EventArgs^  e){
			//std::cout << "init" << std::endl;
			glutInitContextVersion(4, 3);//以OpenGL version4.3版本為基準
			glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);//是否向下相容,GLUT_FORWARD_COMPATIBLE不支援(?
			glutInitContextProfile(GLUT_CORE_PROFILE);

			glewExperimental = GL_TRUE; //置於glewInit()之前
			if (glewInit()) {
				std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;//c error
				exit(EXIT_FAILURE);
			}
			else std::cout << "initialize GLEW success" << std::endl;//c error

			glEnable(GL_DEPTH_TEST);
			//glDepthFunc(GL_LESS);
			glCullFace(GL_BACK);
			glEnable(GL_CULL_FACE);

			//VAO
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			program = LoadShaders(shaders_robot);//讀取shader

			glUseProgram(program);//uniform參數數值前必須先use shader
			MatricesIdx = glGetUniformBlockIndex(program, "MatVP");
			ModelID = glGetUniformLocation(program, "Model");
			Projection = glm::perspective(80.0f, 4.0f / 3.0f, 0.1f, 100.0f);
			ViewMatrix = glm::lookAt(
				glm::vec3(0, 10, 25), // Camera is at (0,10,25), in World Space
				glm::vec3(0, 0, 0), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,1,0 to look upside-down)
			);

			//UBO
			glGenBuffers(1, &UBO);
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferData(GL_UNIFORM_BUFFER, sizeof(mat4) * 2, NULL, GL_DYNAMIC_DRAW);
			//get uniform struct size
			int UBOsize = 0;
			glGetActiveUniformBlockiv(program, MatricesIdx, GL_UNIFORM_BLOCK_DATA_SIZE, &UBOsize);
			//bind UBO to its idx
			glBindBufferRange(GL_UNIFORM_BUFFER, 0, UBO, 0, UBOsize);
			glUniformBlockBinding(program, MatricesIdx, 0);

			glClearColor(0.0, 0.0, 0.0, 1);//black screen

		}
		//display
		private: System::Void hkoglPanelControl1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e){
			std::cout << "refresh" << std::endl;
			glEnable(GL_COLOR_MATERIAL);
			glClearColor(1.0, 1.0, 1.0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			point center;
			center[0] = 0.0;
			center[1] = 0.0;
			center[2] = 0.0;
			camera.setupGL(xf * center, 1.0);
			camera.autospin(xf);

			glPushMatrix();
			glMatrixMode(GL_MODELVIEW);
			glMultMatrixd((double *)xf);
			if (mesh != NULL) {
				std::cout << "refresh mesh not null" << std::endl;
				//mesh->Render_SolidWireframe();
				glGenBuffers(1, &VBO);
				glBindBuffer(GL_ARRAY_BUFFER, VBO);
				glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(double)* 2, &vertices[0], GL_STATIC_DRAW);

			}

			glEnable(GL_DEPTH_TEST);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);//draw to frame buffer

			glBindVertexArray(VAO);
			glUseProgram(program);//uniform參數數值前必須先use shader
			float eyey = DOR(eyeAngley);
			ViewMatrix = lookAt(
				glm::vec3(eyedistance*sin(eyey), 2, eyedistance*cos(eyey)), // Camera is at (0,0,20), in World Space
				glm::vec3(posX, posY, posZ), // and looks at the origin
				glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
			);
			//update data to UBO for MVP
			glBindBuffer(GL_UNIFORM_BUFFER, UBO);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(mat4), &ViewMatrix);
			glBufferSubData(GL_UNIFORM_BUFFER, sizeof(mat4), sizeof(mat4), &Projection);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			glUniformMatrix4fv(ModelID, 1, GL_FALSE, &Model[0][0]);
			glBindBuffer(GL_ARRAY_BUFFER, VBO);
			// 1rst attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0,				//location
				3,				//vec3
				GL_FLOAT,			//type
				GL_FALSE,			//not normalized
				0,				//strip
				0);//buffer offset
			if(isLoad) glDrawArrays(GL_TRIANGLES, 0, face*3);
			glPopMatrix();
		}

				 //mouseClick
		private: System::Void hkoglPanelControl1_MouseDown(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Button == System::Windows::Forms::MouseButtons::Left){
				//leftClick Or wheelClick
				point center;
				Mouse_State = Mouse::NONE;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);

				point depth;
				camera.read_depth(e->X, e->Y, depth);
				std::cout << "point : " << depth << std::endl;
			}
		}

				 //mouseDrag
		private: System::Void hkoglPanelControl1_MouseMove(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Button == System::Windows::Forms::MouseButtons::Left){
				point center;
				Mouse_State = Mouse::ROTATE;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State,
					xf * center,
					1.0, xf);
				

				hkoglPanelControl1->Invalidate();
			}

			if (e->Button == System::Windows::Forms::MouseButtons::Middle){
				point center;
				Mouse_State = Mouse::MOVEXY;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);
				hkoglPanelControl1->Invalidate();
			}
		}

		//mouseWheel
		private: System::Void hkoglPanelControl1_MouseWheel(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e){
			if (e->Delta < 0){
				point center;
				Mouse_State = Mouse::WHEELDOWN;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);
				posZ++;
				hkoglPanelControl1->Invalidate();
			}
			else{
				point center;
				Mouse_State = Mouse::WHEELUP;
				center[0] = 0.0;
				center[1] = 0.0;
				center[2] = 0.0;
				camera.mouse(e->X, e->Y, Mouse_State, xf * center, 1.0, xf);
				posZ--;
				hkoglPanelControl1->Invalidate();
			}
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
	};
}
