#include "character.h"
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
	}
	else
	{
		primary = monitors[0];
	}

	try
	{
		mode = glfwGetVideoMode(secondMonitor);
	}
	catch (const std::exception &e)
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

	FastNoiseLite noise = FastNoiseLite(rand());

	for (int chunkX = -10; chunkX < 10; chunkX++)
	{
		for (int chunkZ = -10; chunkZ < 10; chunkZ++)
		{
			glm::vec3 chunkPos = glm::vec3((float)chunkX * (float)chunkSize * 0.5f, 0.0f, (float)chunkZ * (float)chunkSize * 0.5f);
			Chunk chunk(chunkSize, mainShader, chunkPos, noise, true, true);

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

	// Crosshair setup - simple + shape in screen space
	float crosshairSize = 0.02f; // Size relative to screen
	float crosshairVerts[] = {
		// Horizontal line
		-crosshairSize, 0.0f,
		crosshairSize, 0.0f,
		// Vertical line
		0.0f, -crosshairSize,
		0.0f, crosshairSize};

	Shader crosshairShader("crosshairVertex.glsl", "crosshairFrag.glsl");
	crosshairShader.use();

	unsigned int crosshairVBO, crosshairVAO;
	glGenBuffers(1, &crosshairVBO);
	glGenVertexArrays(1, &crosshairVAO);
	glBindVertexArray(crosshairVAO);
	glBindBuffer(GL_ARRAY_BUFFER, crosshairVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(crosshairVerts), crosshairVerts, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	while (!glfwWindowShouldClose(window))
	{

		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// rendering
		glClearColor(173.0f / 255.0f, 216.0f / 255.0f, 230.0f / 255.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		mainShader.use();

		mainShader.setMat4("projection", glm::value_ptr(projection));
		mainShader.setMat4("view", glm::value_ptr(view));
		mainShader.setVec3("viewPos", player.cameraPos.x, player.cameraPos.y, player.cameraPos.z);

		player.update(mainShader, window, deltaTime, false);

		for (Chunk &chunk : chunks)
		{
			chunk.update(mainShader, grassTexture, cobbleTexture);
			player.handleCollision(chunk.blockPositions, false);
			if (chunk.chunkPosition.x <= player.cameraPos.x + 16.0f && chunk.chunkPosition.x >= player.cameraPos.x - 16.0f && chunk.chunkPosition.z <= player.cameraPos.z + 16.0f && chunk.chunkPosition.z >= player.cameraPos.z - 16.0f)
			{
				player.checkRayCollision(chunk, window, mainShader);
			}
		}

		player.move(deltaTime);
		// std::cout<<chunks[0].blockPositions.size()<<std::endl;

		// Draw crosshair using blending (no depth changes)
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		crosshairShader.use();
		crosshairShader.setVec3("crosshairColor", 0.0f, 0.0f, 0.0f);

		glBindVertexArray(crosshairVAO);
		glDrawArrays(GL_LINES, 0, 4);

		glDisable(GL_BLEND);

		// check and call events and swap the buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}