#include "ext/matrix_clip_space.hpp"
#include "ext/matrix_transform.hpp"
#include "ext/vector_float3.hpp"
#include "fwd.hpp"
#include "glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "renderer/camera.hpp"
#include "renderer/index-buffer.hpp"
#include "renderer/renderer.hpp"
#include "renderer/vertex-array.hpp"
#include "renderer/vertex-buffer-layout.hpp"
#include "renderer/vertex-buffer.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/OpenGL.h>
#include <iostream>

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

    window = glfwCreateWindow(640, 480, "Minecraft", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    std::cout << glGetString(GL_VERSION) << std::endl;

    GLCALL(glEnable(GL_BLEND));
    GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    float positions[] = {
        -0.5f,
        -0.5f,
        0.5f,
        -0.5f,
        0.5f,
        0.5f,
        -0.5f,
        0.5f,
    };

    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    {
        VertexArray vao;
        vao.Bind();

        VertexBuffer vbo(positions, 4 * 2 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        vao.AddBuffer(vbo, layout);

        IndexBuffer ibo(indices, 6);

        Shader basicShader("res/shaders/basic.glsl");

        Renderer renderer;

        auto transformMatrix = glm::mat4(1.0f);
        transformMatrix =
            glm::translate(transformMatrix, glm::vec3(0.25f, -0.25f, 0));

        auto& camera = Camera::Get();

        auto projectionMat = glm::perspective(35.0f, 1.0f, 0.1f, 100.0f);

        while (!glfwWindowShouldClose(window)) {
            renderer.clear();

            camera.HandleInput(window);

            basicShader.Bind();
            basicShader.SetUniformMat4f("u_MVP",
                                        projectionMat * camera.ViewMatrix() *
                                            transformMatrix);
            renderer.draw(vao, ibo, basicShader);

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}
