
#include <Common.h>
#include "KeyFrames.h"

#pragma region hidethis
static const struct
{
    float x, y;
    float r, g, b;
} vertices[3] =
        {
                { -0.6f, -0.4f, 1.f, 0.f, 0.f },
                {  0.6f, -0.4f, 0.f, 1.f, 0.f },
                {   0.f,  0.6f, 0.f, 0.f, 1.f }
        };

static const char* vertex_shader_text =
        "#version 330\n"
        "uniform mat4 MVP;\n"
        "attribute vec3 vCol;\n"
        "attribute vec2 vPos;\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
        "    color = vCol;\n"
        "}\n";

static const char* fragment_shader_text =
        "#version 330\n"
        "varying vec3 color;\n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = vec4(color, 1.0);\n"
        "}\n";

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
#pragma endregion

int main()
{
#pragma region Window n OpenGL stuff
    GLFWwindow* window;
    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location, vpos_location, vcol_location;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(1000, 1000, "Simple example", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glfwMakeContextCurrent(window);

    gladLoadGL();
    glfwSwapInterval(1);

    // NOTE: OpenGL error checks have been omitted for brevity

    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, nullptr);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, nullptr);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    mvp_location = glGetUniformLocation(program, "MVP");
    vpos_location = glGetAttribLocation(program, "vPos");
    vcol_location = glGetAttribLocation(program, "vCol");

    glEnableVertexAttribArray(vpos_location);
    glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), nullptr);
    glEnableVertexAttribArray(vcol_location);
    glVertexAttribPointer(vcol_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertices[0]), (void*) (sizeof(float) * 2));


    glClearColor(0.14f, 0.0f, 0.05f, 1.0f);

    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float zoom = 0.01f;

#pragma endregion

#pragma region keyframes
    KeyFrames kfs;
    kfs.pushVec3(make_pair(glm::vec3(0,4,0), 0), 't');
    kfs.pushVec3(make_pair(glm::vec3(0,-4,0), 1), 't');
    kfs.pushVec3(make_pair(glm::vec3(6,0,0), 3), 't');
    kfs.pushVec3(make_pair(glm::vec3(6,4,0), 5), 't');
    kfs.pushVec3(make_pair(glm::vec3(0,0,0), 6), 't');
    kfs.pushVec3(make_pair(glm::vec3(0,0, 0.0f), 0), 'r');
    kfs.pushVec3(make_pair(glm::vec3(0,0,45.0f), 2), 'r');
    kfs.pushVec3(make_pair(glm::vec3(0,0,90.0f), 3), 'r');
    kfs.pushVec3(make_pair(glm::vec3(0,0, 0.0f), 4), 'r');
    kfs.pushVec3(make_pair(glm::vec3(5), 0), 's');
    kfs.pushVec3(make_pair(glm::vec3(1), 1), 's');
    kfs.pushVec3(make_pair(glm::vec3(1), 3), 's');
    kfs.pushVec3(make_pair(glm::vec3(5), 4), 's');
    kfs.pushVec3(make_pair(glm::vec3(3), 2), 's');
 #pragma endregion

    while (!glfwWindowShouldClose(window))
    {
        glm::mat4 m, p, mvp;

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if(!kfs.IsPlaying())
            m = glm::scale(glm::mat4(1), glm::vec3(3));
        else m = kfs.getTransform();

        int w, h;
        glfwGetWindowSize(window, &w, &h);
        if(h != height || w != width) { string message = "The new canvas has ";  cout << message.append(to_string(h)).append(" h, ").append(to_string(w)).append(" w\n"); width = w; height = h; }
        p = glm::ortho(-(float)w * zoom/2.0f,(float)w * zoom/2.0f,-(float)h * zoom/2.0f,(float)h * zoom/2.0f);
        mvp = p * m;

        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, &mvp[0][0]);
        glDrawArrays(GL_TRIANGLES, 0, 3);



        {
            bool run_animation = false;
            ImGui::Begin("Hello, interpolations!");

            ImGui::Text("d0 - first keyframe derivative\nd1 - last keyframe derivative\nfor the keyframes in-between defaults are 0 and 0\n");
            ImGui::SliderFloat("d0t", &kfs.d0t, -1.0f, 1.0f);
            ImGui::SliderFloat("d1t", &kfs.d1t, -1.0f, 1.0f);
            ImGui::SliderFloat("d0r", &kfs.d0r, -1.0f, 1.0f);
            ImGui::SliderFloat("d1r", &kfs.d1r, -1.0f, 1.0f);
            ImGui::SliderFloat("d0s", &kfs.d0s, -1.0f, 1.0f);
            ImGui::SliderFloat("d1s", &kfs.d1s, -1.0f, 1.0f);

            ImGui::Text("m - attached mass\nk - spring stiffness\nc - damper coefficient\n");
            ImGui::SliderFloat("ks", &kfs.ks, 0.01f, 1000.0f);
            ImGui::SliderFloat("cs", &kfs.cs, -10.0f, 20.0f);
            ImGui::SliderFloat("kr", &kfs.kr, 0.01f, 1000.0f);
            ImGui::SliderFloat("cr", &kfs.cr, -10.0f, 20.0f);
            ImGui::SliderFloat("kt", &kfs.kt, 0.01f, 1000.0f);
            ImGui::SliderFloat("ct", &kfs.ct, -10.0f, 20.0f);

            ImGui::Text("Available modes\n 0 - linear interpolation\n 1 - cubic spline interpolation\n 2 - mass-spring-damper interpolation\n");
            ImGui::SliderInt("Translation Mode", &kfs.tmode, 0, 2);
            ImGui::SliderInt("Rotation Mode", &kfs.rmode, 0, 2);
            ImGui::SliderInt("Scaling Mode", &kfs.smode, 0, 2);



            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            //if(!kfs.IsPlaying())
            ImGui::Checkbox("Interpolate", &run_animation);
            if(run_animation)
                kfs.startPlaying();

            ImGui::End();
        }


        // Rendering
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    exit(EXIT_SUCCESS);
}
 