#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/vector_float3.hpp"
#include "fwd.hpp"
#include "game/chunk.hpp"
#include "game/cube.hpp"
#include "game/player.hpp"
#include "game/textute-atlas.hpp"
#include "game/world.hpp"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer/camera.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/texture.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <array>
#include <iostream>
#include <string>
#include <vector>

void setWindowFPS(GLFWwindow* win);

const static inline std::string GameName = "Minecraft";

int main(void) {
    GLFWwindow* window;

    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(800, 800, "Minecraft", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(0);

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    GLCALL(glEnable(GL_DEPTH_TEST));

    // glEnable(GL_CULL_FACE);
    // glCullFace(GL_BACK);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    {
        Texture texture("res/textures/tileset.png");

        Shader basicShader("res/shaders/basic.glsl");
        Renderer renderer;

        auto transformMatrix = glm::mat4(1.0f);
        // transformMatrix      = glm::translate(transformMatrix, glm::vec3(-5, -5, -29));

        auto& camera = Camera::Get();
        Player player(glm::vec3(5, 13, 10));

        auto projectionMat = glm::perspective(70.0f, 1.0f, 0.1f, 120.0f);

        Chunk chunk(glm::vec3(0, 0, 0));

        World world(10, 10);

        const double fpsLimit   = 1.0 / 60.0;
        double lastUpdateTime   = 0; // number of seconds since the last loop
        double lastFrameTime    = 0; // number of seconds since the last frame
        double lastFramRateTime = 0;
        int frameCount          = 0;

        while (!glfwWindowShouldClose(window)) {
            double now       = glfwGetTime();
            double deltaTime = now - lastUpdateTime;

            ////////// TICK //////////
            glfwPollEvents();
            world.Tick(world);
            player.Tick(window, deltaTime, world);
            camera.SetPos(player.Pos());
            camera.SetRot(player.Rot());

            // camera.HandleInput(window, deltaTime);
            //////////////////////////////

            if ((now - lastFrameTime) >= fpsLimit) {
                ////////// RENDER //////////
                renderer.clear();

                basicShader.Bind();
                texture.Bind();

                basicShader.SetUniform1i("u_TextureSlot", 0);
                basicShader.SetUniformMat4f("u_MVP",
                                            projectionMat * camera.ViewMatrix() * transformMatrix);

                world.Render(renderer, basicShader);
                player.Render(renderer, basicShader);

                glfwSwapBuffers(window);
                frameCount++;
                //////////////////////////////
                lastFrameTime = now;
            }
            // set lastUpdateTime every iteration
            lastUpdateTime = now;

            if ((now - lastFramRateTime) >= 1.0) {
                std::string title = GameName + " | FPS: " + std::to_string(frameCount);
                glfwSetWindowTitle(window, title.c_str());
                lastFramRateTime = now;
                frameCount       = 0;
            }
        }
    }

    glfwTerminate();
    return 0;
}
