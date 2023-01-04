/*---------------------------------------------------------*/
/* ----------------  Proyecto Final              ----------*/
/*-----------------    Equipo 4   -------------------------*/
/*---------------------------------------------------------*/

#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <math.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

#pragma comment(lib, "winmm.lib")

bool sound = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
//void my_input(GLFWwindow *window);
void my_input(GLFWwindow* window, int key, int scancode, int action, int mods);
void animate(void);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 600;
GLFWmonitor *monitors;

void getResolution(void);

// camera
Camera camera(glm::vec3(-400.0f, 50.0f, -500.0f));
float MovementSpeed = 0.1f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
		lastFrame = 0.0f;

//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(0.0f, -1.0f, -1.0f);

// posiciones
//float x = 0.0f;
//float y = 0.0f;
float	movAuto_x = 0.0f,
movAuto_z = 0.0f,
orienta = 0.0f,
orienta2 = 0.0f,
giroTorniquete_x = 0.0f,
giroTorniquete_y = 0.0f,
giroTorniquete_z = 0.0f;

bool	animacion = false,
		animacion2 = true, 
		recorrido1 = true,
		recorrido2 = false,
		recorrido3 = false,
		recorrido4 = false;

//Keyframes (Manipulación y dibujo)
float	posX = 0.0f,
		posY = 0.0f,
		posZ = 0.0f,
		rotRodIzq = 0.0f,
		giroMonito = 0.0f;
		
float	incX = 0.0f,
		incY = 0.0f,
		incZ = 0.0f,
		rotInc = 0.0f,
		giroMonitoInc = 0.0f;

//Variables para animación delfin
float pos_x;
float pos_y;
float pos_z;

#define MAX_FRAMES 9
int i_max_steps = 60;
int i_curr_steps = 0;
typedef struct _frame
{
	//Variables para GUARDAR Key Frames
	float posX;		//Variable para PosicionX
	float posY;		//Variable para PosicionY
	float posZ;		//Variable para PosicionZ
	float rotRodIzq;
	float giroMonito;

}FRAME;

FRAME KeyFrame[MAX_FRAMES];
int FrameIndex = 0;			//introducir datos
bool play = false;
int playIndex = 0;

void saveFrame(void){
	//printf("frameindex %d\n", FrameIndex);
	std::cout << "Frame Index = " << FrameIndex << std::endl;

	KeyFrame[FrameIndex].posX = posX;
	KeyFrame[FrameIndex].posY = posY;
	KeyFrame[FrameIndex].posZ = posZ;

	KeyFrame[FrameIndex].rotRodIzq = rotRodIzq;
	KeyFrame[FrameIndex].giroMonito = giroMonito;

	FrameIndex++;
}

void resetElements(void)
{
	posX = KeyFrame[0].posX;
	posY = KeyFrame[0].posY;
	posZ = KeyFrame[0].posZ;

	rotRodIzq = KeyFrame[0].rotRodIzq;
	giroMonito = KeyFrame[0].giroMonito;
	
}

void interpolation(void)
{
	incX = (KeyFrame[playIndex + 1].posX - KeyFrame[playIndex].posX) / i_max_steps;
	incY = (KeyFrame[playIndex + 1].posY - KeyFrame[playIndex].posY) / i_max_steps;
	incZ = (KeyFrame[playIndex + 1].posZ - KeyFrame[playIndex].posZ) / i_max_steps;

	rotInc = (KeyFrame[playIndex + 1].rotRodIzq - KeyFrame[playIndex].rotRodIzq) / i_max_steps;
	giroMonitoInc = (KeyFrame[playIndex + 1].giroMonito - KeyFrame[playIndex].giroMonito) / i_max_steps;


}

void animate(void)
{
	if (play)
	{
		if (i_curr_steps >= i_max_steps) //end of animation between frames?
		{
			playIndex++;
			if (playIndex > FrameIndex - 2)	//end of total animation?
			{
				std::cout << "Animation ended" << std::endl;
				//printf("termina anim\n");
				playIndex = 0;
				play = false;
			}
			else //Next frame interpolations
			{
				i_curr_steps = 0; //Reset counter
								  //Interpolation
				interpolation();
			}
		}
		else
		{
			//Draw animation
			posX += incX;
			posY += incY;
			posZ += incZ;

			rotRodIzq += rotInc;
			giroMonito += giroMonitoInc;

			i_curr_steps++;
		}
	}

	//Vehículo
	if (animacion)
	{
		movAuto_z += 3.0f;
	}
	
	//Delfin
	if (animacion2) {
		if (recorrido1) {
			pos_x += 0.1f;
			pos_y += 0.1f;
			orienta = 0.0f;
			if (pos_y >= 30.0f) {
				recorrido1 = false;
				recorrido2 = true;
			}
		}
		if (recorrido2) {
			pos_x += 0.1f;
			pos_y -= 0.001f;
			orienta = 0.0f;
			if (pos_x >= 60.0f) {
				recorrido2 = false;
				recorrido3 = true;
			}
		}
		if (recorrido3) {
			pos_x += 0.1f;
			pos_y -= 0.1f;
			orienta = 30.0f;
			if (pos_y <= 0.0f) {
				recorrido3 = false;
				recorrido4 = true;
			}
		}
		if (recorrido4) {
			pos_x -= 0.2f;
			pos_y = 0.0f;
			orienta = -180.0f;
			if (pos_x <= 0.0f) {
				recorrido4 = false;
				recorrido1 = true;
			}
		}
	}
		}

void getResolution() {
	const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void music() {
	if (sound) {
		bool played = PlaySound(L"acuario.wav", NULL, SND_LOOP | SND_ASYNC);
		sound = false;
	}
}


int main() {
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	// --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CGeIHC", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetKeyCallback(window, my_input);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights_mod.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");
	Shader animShader("Shaders/anim.vs", "Shaders/anim.fs");

	vector<std::string> faces
	{
		"resources/skybox/right.jpg",
		"resources/skybox/left.jpg",
		"resources/skybox/top.jpg",
		"resources/skybox/bottom.jpg",
		"resources/skybox/front.jpg",
		"resources/skybox/back.jpg"

	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model piso("resources/objects/piso/piso.obj");
	Model botaDer("resources/objects/Personaje/bota.obj");
	Model piernaDer("resources/objects/Personaje/piernader.obj");
	Model piernaIzq("resources/objects/Personaje/piernader.obj");
	Model torso("resources/objects/Personaje/torso.obj");
	Model brazoDer("resources/objects/Personaje/brazoder.obj");
	Model brazoIzq("resources/objects/Personaje/brazoizq.obj");
	Model cabeza("resources/objects/Personaje/cabeza.obj");
	Model carro("resources/objects/lambo/carroceria.obj");
	Model llanta("resources/objects/lambo/Wheel.obj");
	Model casaVieja("resources/objects/casa/OldHouse.obj");
	//Model cubo("resources/objects/cubo/cube02.obj");
	Model casaDoll("resources/objects/casa/DollHouse.obj");


	//MODELOS PROYECTO
	Model igloo("resources/objects/Igloo/igloo.obj");
	Model grada("resources/objects/Grada/grada.obj");
	Model torniquete_base("resources/objects/Torniquete/torniquete_base.obj");
	Model torniquete_tubo("resources/objects/Torniquete/torniquete_tubo.obj");
	Model taquilla("resources/objects/Taquilla/taquilla.obj");
	Model delfin("resources/objects/Delfin/delfin.obj");
	Model colaD("resources/objects/Delfin/colaDelfin.obj");
	Model valla("resources/objects/Valla/valla.obj");
	Model Jardinera("resources/objects/Jardinera/jardinera.obj");
	Model arbol("resources/objects/arbol/tree_in_OBJ.obj");
	Model fuente("resources/objects/reloj/town clock.obj");
	Model contorno("resources/objects/fuente/contorno.obj");
	Model cafeteria("resources/objects/cafeteria/estructura.obj");
	Model cafeteriaB("resources/objects/cafeteria/estructuraB.obj");
	Model cafeteriaC("resources/objects/cafeteria/estructuraC.obj");
	Model piscina("resources/objects/piscina/avika-curved_pool_ver1.obj");
	Model helado("resources/objects/helado/Icecreambooth.obj");
	Model res("resources/objects/swing set/res.obj");
	Model ba("resources/objects/swing set/ba.obj");
	Model la("resources/objects/swing set/la.obj");
	Model di("resources/objects/swing set/di.obj");


	Model banio("resources/objects/Banio/banio.obj");
	Model cerca_afuera("resources/objects/Cerca/cerca2.obj");
	Model cerca_afuera_sola("resources/objects/Cerca/cerca_sola.obj");

	

	/*ModelAnim animacionPersonaje("resources/objects/Personaje1/PersonajeBrazo.dae");
	animacionPersonaje.initShaders(animShader.ID);

	ModelAnim ninja("resources/objects/ZombieWalk/ZombieWalk.dae");
	ninja.initShaders(animShader.ID);*/

	//Inicialización de KeyFrames
	for (int i = 0; i < MAX_FRAMES; i++)
	{
		KeyFrame[i].posX = 0;
		KeyFrame[i].posY = 0;
		KeyFrame[i].posZ = 0;
		KeyFrame[i].rotRodIzq = 0;
		KeyFrame[i].giroMonito = 0;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		//my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.5f, 0.5f, 0.5f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.08f);
		staticShader.setFloat("pointLight[0].linear", 0.009f);
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);

		staticShader.setVec3("pointLight[1].position", glm::vec3(-80.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setVec3("spotLight[0].position", glm::vec3(0.0f, 20.0f, 10.0f));
		staticShader.setVec3("spotLight[0].direction", glm::vec3(0.0f, -1.0f, 0.0f));
		staticShader.setVec3("spotLight[0].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("spotLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("spotLight[0].cutOff", glm::cos(glm::radians(10.0f)));
		staticShader.setFloat("spotLight[0].outerCutOff", glm::cos(glm::radians(60.0f)));
		staticShader.setFloat("spotLight[0].constant", 1.0f);
		staticShader.setFloat("spotLight[0].linear", 0.0009f);
		staticShader.setFloat("spotLight[0].quadratic", 0.0005f);

		staticShader.setFloat("material_shininess", 32.0f);

		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 tmp = glm::mat4(1.0f);
		glm::mat4 tmp2 = glm::mat4(1.0f);
		glm::mat4 tmp3 = glm::mat4(1.0f);

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 10000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(0.6f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------
		//Remember to activate the shader with the animation
		animShader.use();
		animShader.setMat4("projection", projection);
		animShader.setMat4("view", view);

		animShader.setVec3("material.specular", glm::vec3(0.5f));
		animShader.setFloat("material.shininess", 32.0f);
		animShader.setVec3("light.ambient", ambientColor);
		animShader.setVec3("light.diffuse", diffuseColor);
		animShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		animShader.setVec3("light.direction", lightDirection);
		animShader.setVec3("viewPos", camera.Position);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(1.2f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//animacionPersonaje.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Segundo Personaje Animacion
		// -------------------------------------------------------------------------------------------------------------------------

		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.3f, 1.75f, 0.3f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.5f));	// it's a bit too big for our scene, so scale it down
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		animShader.setMat4("model", model);
		//ninja.Draw(animShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.use();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(250.0f, 0.0f, -10.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//casaDoll.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		piso.Draw(staticShader);

		//Zona de animales de clima frio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(190.0f, 0.0f, 275.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.8f));
		staticShader.setMat4("model", model);
		igloo.Draw(staticShader);

		//Area de Delfines
		model = glm::translate(glm::mat4(1.0f), glm::vec3(40.0f, -2.0f, 210.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.023f));
		staticShader.setMat4("model", model);
		grada.Draw(staticShader);


		//Banio
		model = glm::translate(glm::mat4(1.0f), glm::vec3(300.0f, 0.0f, -260.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00015f));
		staticShader.setMat4("model", model);
		banio.Draw(staticShader);


		//Piscina
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-115.0f, 12.0f, 315.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		piscina.Draw(staticShader);

		//Reloj
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 0.0f));
		model = glm::scale(model, glm::vec3(17.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		fuente.Draw(staticShader);

		//GRADA 2
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-230.0f, -2.0f, 210.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.023f));
		staticShader.setMat4("model", model);
		grada.Draw(staticShader);

		//Taquilla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-322.5f, 25.0f, -342.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(9.0f));
		staticShader.setMat4("model", model);
		taquilla.Draw(staticShader);


		//CercadeFuera_Esquina_Frente
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-425.5f, -2.0f, -372.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera.Draw(staticShader);

		//CercasdeFuera_Frente
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-360.0f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-148.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-87.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-26.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(35.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(96.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(157.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(218.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(279.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(340.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		//CercadeFuera_Esquina_Frente
		model = glm::translate(glm::mat4(1.0f), glm::vec3(401.5f, -2.0f, -370.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera.Draw(staticShader);

		//CercasdeFuera_izquierda
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-478.0f, -2.0f, -298.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-478.0f, -2.0f, -237.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-478.0f, -2.0f, -176.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.25f));
		staticShader.setMat4("model", model);
		cerca_afuera_sola.Draw(staticShader);









		/*Area de torniquetes Entrada
		Torniquete base*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-300.0f, 6.0f, -325.0f));
		tmp2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-286.0f, 6.0f, -325.0f));
		tmp2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-272.0f, 6.0f, -325.0f));
		tmp2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-258.0f, 6.0f, -325.0f));
		tmp2 = model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);


		/*Area de torniquetes Salida
		Torniquete base*/
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-234.0f, 6.0f, -344.0f));
		tmp2 = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(-0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(-giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-220.0f, 6.0f, -344.0f));
		tmp2 = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(-0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(-giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-206.0f, 6.0f, -344.0f));
		tmp2 = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(-0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(-giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//Torniquete base
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-192.0f, 6.0f, -344.0f));
		tmp2 = model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_base.Draw(staticShader);

		//Torniquete tubo
		model = glm::translate(tmp2, glm::vec3(-0.2f, 3.5f, -9.0f));
		model = glm::rotate(model, glm::radians(-giroTorniquete_x), glm::vec3(0.2f, -0.1f, 0.0f));
		model = glm::scale(model, glm::vec3(0.00013f));
		staticShader.setMat4("model", model);
		torniquete_tubo.Draw(staticShader);

		//DELFIN
		model = glm::translate(glm::mat4(1.0f), glm::vec3(pos_x, pos_y, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		tmp3 = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 0.0f, 1.0f));

		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		delfin.Draw(staticShader);

		model = glm::translate(tmp3, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f));
		staticShader.setMat4("model", model);
		colaD.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-104.7f, -1.70f, -335.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(42.0f, -1.80f, -335.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(187.0f, -1.70f, -335.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(333.0f, -1.80f, -335.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(402.0f, -1.70f, -258.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(402.0f, -1.80f, -111.5f));
		model = glm::scale(model, glm::vec3(11.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(402.0f, -1.70f, 33.5f));
		model = glm::scale(model, glm::vec3(16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(402.0f, -1.80f, 180.5f));
		model = glm::scale(model, glm::vec3(11.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(402.0f, -1.70f, 326.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(333.0f, -1.80f, 403.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(187.0f, -1.70f, 403.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(42.0f, -1.80f, 403.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-104.7f, -1.70f, 403.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-250.0f, -1.80f, 403.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-396.5f, -1.70f, 403.0f));
		model = glm::scale(model, glm::vec3(16.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-473.0f, -1.80f, 334.5f));
		model = glm::scale(model, glm::vec3(11.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-473.0f, -1.70f, 188.2f));
		model = glm::scale(model, glm::vec3(16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-473.0f, -1.80f, 42.7f));
		model = glm::scale(model, glm::vec3(11.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Valla
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-473.0f, -1.70f, -104.7f));
		model = glm::scale(model, glm::vec3(16.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		valla.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-473.0f, -1.80f, -250.2f));
		model = glm::scale(model, glm::vec3(11.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Jardinera
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-403.8f, -1.70f, -335.0f));
		model = glm::scale(model, glm::vec3(11.0f));
		staticShader.setMat4("model", model);
		Jardinera.Draw(staticShader);

		//Arbol
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-465.0f, 0.0f, -325.0f));
		model = glm::scale(model, glm::vec3(6.5f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		arbol.Draw(staticShader);

		//Cafeteria estructura
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-445.0f, 0.0f, -250.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cafeteria.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-445.0f, 0.0f, -250.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cafeteriaB.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-445.0f, 0.0f, -250.0f));
		model = glm::scale(model, glm::vec3(20.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		cafeteriaC.Draw(staticShader);

		//Puesto de helado
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-430.0f, 0.0f, 310.0f));
		model = glm::rotate(model, glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(23.0f));
		staticShader.setMat4("model", model);
		helado.Draw(staticShader);

		//Juegos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		res.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		ba.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		la.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-400.0f, 0.0f, 130.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		di.Draw(staticShader);

		//Juegos
		model = glm::translate(glm::mat4(1.0f), glm::vec3(-450.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		res.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-450.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		ba.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-450.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		la.Draw(staticShader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-450.0f, 0.0f, -20.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		di.Draw(staticShader);

		// -------------------------------------------------------------------------------------------------------------------------
		// Carro
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(15.0f + movAuto_x, -1.0f, movAuto_z));
		tmp = model = glm::rotate(model, glm::radians(orienta), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//carro.Draw(staticShader);

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, 12.9f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der delantera

		model = glm::translate(tmp, glm::vec3(-8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Der trasera

		model = glm::translate(tmp, glm::vec3(8.5f, 2.5f, -14.5f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));
		staticShader.setMat4("model", model);
		//llanta.Draw(staticShader);	//Izq trase
		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje
		// -------------------------------------------------------------------------------------------------------------------------
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		model = glm::translate(model, glm::vec3(posX, posY, posZ));
		tmp = model = glm::rotate(model, glm::radians(giroMonito), glm::vec3(0.0f, 1.0f, 0.0));
		staticShader.setMat4("model", model);
		//torso.Draw(staticShader);

		//Pierna Der
		model = glm::translate(tmp, glm::vec3(-0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::rotate(model, glm::radians(-rotRodIzq), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaDer.Draw(staticShader);

		//Pie Der
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);

		//Pierna Izq
		model = glm::translate(tmp, glm::vec3(0.5f, 0.0f, -0.1f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//piernaIzq.Draw(staticShader);

		//Pie Iz
		model = glm::translate(model, glm::vec3(0, -0.9f, -0.2f));
		staticShader.setMat4("model", model);
		//botaDer.Draw(staticShader);	//Izq trase

		//Brazo derecho
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(-0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoDer.Draw(staticShader);

		//Brazo izquierdo
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::translate(model, glm::vec3(0.75f, 2.5f, 0));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		staticShader.setMat4("model", model);
		//brazoIzq.Draw(staticShader);

		//Cabeza
		model = glm::translate(tmp, glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0));
		model = glm::translate(model, glm::vec3(0.0f, 2.5f, 0));
		staticShader.setMat4("model", model);
		//cabeza.Draw(staticShader);
		// -------------------------------------------------------------------------------------------------------------------------
		// Caja Transparente --- Siguiente Práctica
		// -------------------------------------------------------------------------------------------------------------------------
		/*glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -70.0f));
		model = glm::scale(model, glm::vec3(5.0f));
		staticShader.setMat4("model", model);
		cubo.Draw(staticShader);
		glEnable(GL_BLEND);*/
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Limitar el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window, int key, int scancode, int action, int mode)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, (float)deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, (float)deltaTime);
	//To Configure Model
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		posZ++;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		posZ--;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		posX--;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		posX++;
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
		rotRodIzq--;
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
		rotRodIzq++;
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		giroMonito--;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		giroMonito++;
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
		lightPosition.x++;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		lightPosition.x--;
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
		giroTorniquete_x++;
	if (glfwGetKey(window, GLFW_KEY_2 )== GLFW_PRESS)
		giroTorniquete_x--;
	//Car animation
	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
		animacion ^= true;

	if (glfwGetKey(window, GLFW_KEY_3 )== GLFW_PRESS)
		animacion2 ^= true;

	if (key == GLFW_KEY_O && action == GLFW_PRESS)
		music();

	//To play KeyFrame animation 
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		if (play == false && (FrameIndex > 1))
		{
			std::cout << "Play animation" << std::endl;
			resetElements();
			//First Interpolation				
			interpolation();

			play = true;
			playIndex = 0;
			i_curr_steps = 0;
		}
		else
		{
			play = false;
			std::cout << "Not enough Key Frames" << std::endl;
		}
	}

	//To Save a KeyFrame
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		if (FrameIndex < MAX_FRAMES)
		{
			saveFrame();
		}
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}
