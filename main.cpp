#include "character.h"

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

const float cameraSpeed = 5.0f;

Character player(0.25f, 1.0f, 0.0f, 5.0f, 0.1f, 5.0f, true);

bool mouseMove = false;

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void handleMouse(GLFWwindow *window, double xpos, double ypos)
{
	mouseMove = true;
	player.mouse_Handler(window, xpos, ypos);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWmonitor *primary = glfwGetPrimaryMonitor();
	const GLFWvidmode *mode = glfwGetVideoMode(primary);
	int width = mode->width;
	int height = mode->height;

	GLFWwindow *window = glfwCreateWindow(width, height, "Window", primary, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create window";
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// hide cursor and capture it
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, handleMouse);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Shader mainShader("vertexShader.glsl", "fragShader.glsl");

	std::vector<Cube> cubes;

	unsigned int grassTexture = textureHandler::appendTexture("assets/grassTop.jpg");

	for (int i = 0; i < 20; i++)
	{
		for (int j = 0; j < 20; j++)
		{
			for (size_t k = 0; k < 15; k++)
			{
				cubes.emplace_back();
				cubes.back().init(glm::vec3((float)i / 2, -(float)k / 2, (float)j / 2), grassTexture);
				cubes.back().bufferize();
			}
		}
	}

	unsigned int cobbleTexture = textureHandler::appendTexture("assets/cobbleTex.jpg");

	Cube stoneCube;
	stoneCube.init(glm::vec3(2.0f, 1.0f, 2.0f), cobbleTexture);
	stoneCube.bufferize();

	cubes.push_back(stoneCube);

	mainShader.use();

	glm::mat4 view = glm::mat4(1.0f);

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);

	const float radius = 10.0f;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glfwSwapInterval(1);

	// wireframe mode
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// rendering
		glClearColor(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mainShader.setMat4("projection", glm::value_ptr(projection));

		mainShader.setMat4("view", glm::value_ptr(view));

		player.update(mainShader, window, cubes.data(), cubes.size(), deltaTime, false);

		for (size_t i = 0; i < cubes.size(); i++)
		{
			cubes[i].draw(mainShader, cubes[i].CubeID.y < 0.0f, cubes[i].CubeID.y > -7.0f, cubes[i].CubeID.x > 0.0f, cubes[i].CubeID.x < 9.5f, cubes[i].CubeID.z > 0.0f, cubes[i].CubeID.z < 9.5f);
		}

		glCullFace(GL_FRONT);
		glFrontFace(GL_CW);

		// inputs

		// model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

		// std::cout << timeValue << std::endl;

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}