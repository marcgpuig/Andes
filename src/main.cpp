#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const char *vertexShaderSource = 
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
const char *fragmentShaderSource = 
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main(int argc, char* argv[])
{
    /// Initializations ----------------------------------------------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    /// GLFW window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    
    /// Callbacks ----------------------------------------------------------------------------------
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    /// Shaders ------------------------------------------------------------------------------------
    /// Error variables init
    int  success;
    char infoLog[512];

    /// Vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    /// Attach the shader source code to the shader object
    glShaderSource(
        vertexShader,
        1,                   /// How many strings we're passing as source code  
        &vertexShaderSource, /// The actual source code of the vertex shader
        NULL
    );
    /// Compile the shader
    glCompileShader(vertexShader);

    /// Check for vertex shader errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /// Fragment shader
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    /// Attach the shader source code to the shader object
    glShaderSource(
        fragmentShader,
        1,                     /// How many strings we're passing as source code  
        &fragmentShaderSource, /// The actual source code of the vertex shader
        NULL
    );
    /// Compile the shader
    glCompileShader(fragmentShader);

    /// Check for fragment shader errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    /// Crating the shader program
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    /// Attach the previously compiled shaders to the program object
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    /// Link them into a final program
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    /// Check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }

    /// Geometry -----------------------------------------------------------------------------------
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    }; 

    /// Vertex Buffer Object
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    /// Generate 1 buffer and store its ID in VBO.
    glGenBuffers(1, &VBO);
    /// Generate 1 buffer and store its ID in EBO.
    glGenBuffers(1, &EBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    /// Bind the VBO to the GL_ARRAY_BUFFER target, so any buffer 
    /// call we make on GL_ARRAY_BUFFER, it will modify the VBO buffer.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    /// Now we call to glBufferData function that copies the previously
    /// defined vertex data (vertices) into the buffer's memory.
    glBufferData(
        GL_ARRAY_BUFFER,  /// Buffer type
        sizeof(vertices), /// Specifies the size of the data (in bytes) we want to pass to the buffer
        vertices,         /// The actual data we want to send
        GL_STATIC_DRAW    /// How we want the graphics card to manage the given data, this can be:
    );                    /// - GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
                          /// - GL_DYNAMIC_DRAW: the data is likely to change a lot.
                          /// - GL_STREAM_DRAW: the data will change every time it is drawn.

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

    /// https://learnopengl.com/#!Getting-started/Hello-Triangle
    glVertexAttribPointer(
        0,                 /// Look in vertex shader (location = 0)
        3,                 /// Size of the vertex attribute
        GL_FLOAT,          /// Type of the data for 'vec' in shader
        GL_FALSE,          /// If we want the data to be normalized
        3 * sizeof(float), /// Space between consecutive vertex attribute sets
        (void*)0           /// 
    );
    /// Enable attribute 0 of our shader
    glEnableVertexAttribArray(0);

    /// Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /// Draw in wireframe polygons.
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    /// Main Loop ----------------------------------------------------------------------------------
    while(!glfwWindowShouldClose(window))
    {
        processInput(window);

        /// Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    /// Delete Resources ---------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}