#include "character.h"
#include "chunk.h"
#include <algorithm>

float deltaTime = 0.0f; // Time between current frame and last frame
float lastFrame = 0.0f; // Time of last frame

const float cameraSpeed = 5.0f;

Character player(0.25f, 1.0f, -10.0f, 5.0f, 0.1f, 5.0f, false, glm::vec3(1.0f, 10.0f, 1.0f));

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
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_DEPTH_BITS, 24);

	int count;
	GLFWmonitor **monitors = glfwGetMonitors(&count);
	GLFWmonitor *primary;
	GLFWmonitor *secondMonitor;
	const GLFWvidmode *mode;
	if (count > 1)
	{
		secondMonitor = monitors[1];
		// Use secondMonitor for glfwCreateWindow, etc.
	}else{
		primary = monitors[0];
	}

	try
	{
		mode = glfwGetVideoMode(secondMonitor);
	}
	catch(const std::exception& e)
	{
		mode = glfwGetVideoMode(primary);
		std::cerr << "Error" << &e << std::endl;
	}
	
	
	// int width = mode->width;
	// int height = mode->height;
	int width = 1200;
	int height = 800;

	GLFWwindow *window = glfwCreateWindow(width, height, "Window", NULL, NULL);
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
	unsigned int grassTexture = textureHandler::appendTexture("assets/grassTop.jpg");

	Shader mainShader("vertexShader.glsl", "fragShader.glsl");

	unsigned int cobbleTexture = textureHandler::appendTexture("assets/cobbleTex.jpg");

	mainShader.use();
	mainShader.setInt("grassTexture", 0);
	mainShader.setInt("cobbleTexture", 1);

	const unsigned int chunkSize = 16;

	std::vector<Chunk> chunks;

	FastNoiseLite noise = FastNoiseLite( rand() );

	for (int chunkX = 0; chunkX < 10; chunkX++)
	{
		for (int chunkZ = 0; chunkZ < 10; chunkZ++)
		{

			Chunk chunk(chunkSize, mainShader, glm::vec3(chunkX * chunkSize * 0.5f, 0.0f, chunkZ * chunkSize * 0.5f), noise);

			chunks.push_back(chunk);
		}
	}

	// Chunk chunk(chunkSize, mainShader, glm::vec3(-1 * chunkSize * 0.5f, 0.0f, -1 * chunkSize * 0.5f), noise);
	// chunks.push_back(chunk);

	// 1, -1, 1 is this cube's position

	// float checkVal = 5.0f * 2.0f;
	// std::cout << chunk[checkVal*20.0f*20.0f + checkVal*20.0f + checkVal].x << " " << chunk[checkVal*20.0f*20.0f + checkVal*20.0f + checkVal].y << " " << chunk[checkVal*20.0f*20.0f + checkVal*20.0f + checkVal].z << " " << chunk[checkVal*20.0f*20.0f + checkVal*20.0f + checkVal].w << std::endl;

	glViewport(0, 0, width, height);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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
		
		player.update(mainShader, window, deltaTime, true);

		for (Chunk &chunk : chunks)
		{
			chunk.update(mainShader, grassTexture, cobbleTexture);
			player.handleCollision(chunk.blockPositions, true);
		}

		player.move(deltaTime);
		// std::cout<<chunks[0].blockPositions.size()<<std::endl;

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}