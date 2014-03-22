#include <glload/gl_4_0.hpp>
#include <glload/gl_load.hpp>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

#define NL "\n"

bool compileSuccessful(unsigned obj) {
    int status;
    gl::GetShaderiv(obj, gl::COMPILE_STATUS, &status);
    return status == gl::TRUE_;
}

bool linkSuccessful(unsigned program) {
    int status;
    gl::GetProgramiv(program, gl::LINK_STATUS, &status);
    return status == gl::TRUE_;
}

std::string getShaderLog(unsigned shader)
{
    GLsizei len;
    char log[2048];
    gl::GetShaderInfoLog(shader, sizeof(log), &len, log);
    return log;
}

std::string loadFileToString(std::string const& path) {
    std::ifstream file (path, std::ios::binary);
    return { std::istreambuf_iterator<char>(file),
             std::istreambuf_iterator<char>() };
}

unsigned loadProgram() {
    using std::string;

    const string vss = loadFileToString("../shaders/vert.glsl");
    const string tcss = loadFileToString("../shaders/tess_control.glsl");
    const string tess = loadFileToString("../shaders/tess_eval.glsl");
    const string fss = loadFileToString("../shaders/frag.glsl");

    auto vs = gl::CreateShader(gl::VERTEX_SHADER);
    auto fs = gl::CreateShader(gl::FRAGMENT_SHADER);
    auto tcs = gl::CreateShader(gl::TESS_CONTROL_SHADER);
    auto tes = gl::CreateShader(gl::TESS_EVALUATION_SHADER);

    auto shaderSource = [](unsigned shader, string const& source) {
        auto const ptr = source.data();
        int size = source.size();
        gl::ShaderSource(shader, 1, &ptr, &size);
    };

    shaderSource(vs, vss);
    shaderSource(fs, fss);
    shaderSource(tcs, tcss);
    shaderSource(tes, tess);

    gl::CompileShader(vs);
    gl::CompileShader(fs);
    gl::CompileShader(tcs);
    gl::CompileShader(tes);

    std::cerr << "TCS: " << getShaderLog(tcs);
    std::cerr << "TES: " << getShaderLog(tes);

    unsigned program = gl::CreateProgram();
    gl::AttachShader(program, vs);
    gl::AttachShader(program, fs);
    gl::AttachShader(program, tcs);
    gl::AttachShader(program, tes);

    gl::LinkProgram(program);
    if (!linkSuccessful(program))
        std::cerr << "LINK FAILED!\n";

    gl::DeleteShader(vs);
    gl::DeleteShader(fs);
    gl::DeleteShader(tcs);
    gl::DeleteShader(tes);

    return program;
}

struct Mesh { unsigned vbo, vao; };
Mesh loadMesh() {
    Mesh m;
    gl::GenVertexArrays(1, &m.vao);
    gl::GenBuffers(1, &m.vbo);

    gl::BindVertexArray(m.vao);

    float verts[] = {
        -0.5f, -0.5f,
        0.5f, -0.5f,
        -0.5f, 0.5f,
        0.5f, 0.5f
    };

    gl::BindBuffer(gl::ARRAY_BUFFER, m.vbo);
    gl::BufferData(gl::ARRAY_BUFFER, sizeof(verts), verts, gl::STATIC_DRAW);

    gl::EnableVertexAttribArray(0);
    gl::VertexAttribPointer(0, 2, gl::FLOAT, gl::FALSE_, 0, 0);

    return m;
}

int main(void) {
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, gl::TRUE_);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, gl::TRUE_);
    glfwWindowHint(GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glload::LoadFunctions();

    auto program = loadProgram();

    auto mesh = loadMesh();
    gl::BindVertexArray(mesh.vao);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        /* Render here */
        gl::UseProgram(program);
        gl::PolygonMode(gl::FRONT_AND_BACK, gl::LINE);

        gl::PatchParameteri(gl::PATCH_VERTICES, 4);       // tell OpenGL that every patch has 16 verts
        gl::DrawArrays(gl::PATCHES, 0, 4);
        //gl::DrawArrays(gl::TRIANGLE_STRIP, 0, 4);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}