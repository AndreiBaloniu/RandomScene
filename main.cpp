#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "model.h"
#include "mesh.h"
#include "obiect.h"
#include "scena.h"

#define STB_IMAGE_IMPLEMENTATION
#include"stb/stb_image.h"

// functii ce creaza shaderele
GLuint createProgram(char* VSFile, char* FSFile);
std::string readFile(const char *filePath);

// functie ce creaza scena
// aceasta functie trebuie rescrisa de voi
Scena createScena(GLuint programShadere);


// functii ce initializeaza fereastra si
// proceseaza intrarile de la mouse si tastatura
GLFWwindow* initializari();
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// configurari
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 1200;

// camera
glm::vec3 cameraPos   = glm::vec3(5.0f, 5.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -1.0f, -1.0f);
glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

bool firstMouse = true;
float yaw   = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
float pitch =  0.0f;
float lastX =  1600.0f / 2.0;
float lastY =  1200.0 / 2.0;
float fov   =  90.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

int main()
{
    // initializam fereastra
    GLFWwindow* fereastra = initializari();

    // cream shaderele si folosim programul creat
    // (avand o scena simpla folosim un singur program)
    GLuint program = createProgram("obiect1.vert","obiect1.frag");
    glUseProgram(program);

    //cream scena
    Scena S = createScena(program);


    while(!glfwWindowShouldClose(fereastra))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // procesam intrarile ferestrei: din taste si/sau mouse
        processInput(fereastra);

        // trimitem matricea projection catre shadere
        glm::mat4 projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glUniformMatrix4fv(glGetUniformLocation(program,"projection"), 1, GL_FALSE,&projection[0][0]);

        // camera/view transformation
        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, &view[0][0]);


        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        S.DrawScene();

        glfwSwapBuffers(fereastra);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}

Scena createScena(GLuint program){

    Scena aux = Scena();
    // load models
    // -----------


    // TO DO: aici incarcati modelele voastre
    // fie luati datele dintr-un fisier, fie puneti datele aici


    // aici aveti un exemplu extrem de sumar cum puteti incarca si folosi doua obiecte.

    locatie l = {0.0, 0.0, 0.0};
    rotatie r = {0.0, 1.0, 1.0, 1.0};
    scalare s = {1.0f, 1.0f, 1.0f};

    aux.addObiect(Obiect(100, "resurse/Trabant.obj", {-15.0, 1.5, -3.0}, r, {0.25f, 0.25f, 0.25f}, program));
    aux.addObiect(Obiect(100, "resurse/stopsign.obj", {21.5, 0.0, -3.0}, {-90, 0, 1, 0}, {5.0f, 5.0f, 5.0f}, program));
    aux.addObiect(Obiect(110, "resurse/export3dcoat.obj", {-10.0, 30.0, 0.0}, r, {0.01f, 0.01f, 0.01f}, program));

    aux.addObiect(Obiect(0, "resurse/road.obj", l, r, {5.5,1,1.5f}, program));
    aux.addObiect(Obiect(120, "resurse/duba.obj", {-10.0, 2.5, 3.0}, r, {4, 4, 4}, program));


    aux.addObiect(Obiect(1, "resurse/Cub.obj", l, r, {39, 50, 20}, program));
    aux.addObiect(Obiect(2, "resurse/water.obj", {0.0, -0.01, 0.0}, r, {2, 1.5f, 1}, program));

    aux.addObiect(Obiect(3, "resurse/tree.obj", {22.0,0.0,18.0}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {11.0,0.0,18.0}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {0.0,0.0,18.0}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {-11.0,0.0,18.0}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {-22.0,0.0,18.0}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {-33.0,0.0,18.0}, r, {0.05f, 0.05f, 0.05f}, program));

    aux.addObiect(Obiect(3, "resurse/tree.obj", {22.0,0.0,-4.50}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {11.0,0.0,-4.50}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {0.0,0.0,-4.50}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {-11.0,0.0,-4.50}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {-22.0,0.0,-4.50}, r, {0.05f, 0.05f, 0.05f}, program));
    aux.addObiect(Obiect(3, "resurse/tree.obj", {-33.0,0.0,-4.50}, r, {0.05f, 0.05f, 0.05f}, program));

    aux.addObiect(Obiect(4, "resurse/dacia.obj", {10.0, 0.0, 3.0}, r, {0.02f, 0.02f, 0.02f}, program));
    aux.addObiect(Obiect(5, "resurse/BUS.obj", {13.0, 0.7, -3.0}, {-90.0, 0, 1, 0}, {2.0f, 2.7f, 2.0f}, program));
    aux.addObiect(Obiect(6, "resurse/crab.obj", {10.2, 1.7, 2.2}, r, {0.3, 0.3, 0.3}, program));
    aux.addRotatieObiect(6,{90, 0, 1, 0});
    aux.addObiect(Obiect(7, "resurse/crab.obj", {10.2, 1.5, 2.2}, r, {0.3, 0.3, 0.3}, program));
    aux.addRotatieObiect(7,{90, 0, 1, 0});

    aux.addObiect(Obiect(8, "resurse/treasure.obj", {9, 1.3 ,2.32}, {-180, 0, 1, 1}, {0.005, 0.005, 0.005}, program));


    return aux;
}


GLFWwindow* initializari()
{
    /*
        functia initializeaza bibliotecile glfw si glad, creaza o fereastra si
            o ataseaza  unui context OpenGL
    */

    //initializam glfw
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //cream fereastra
    GLFWwindow * window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "scena_01", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    // facem ca aceasta fereastra sa fie contextul curent


    //atasam fereastra contextului opengl
    //glfwMakeContextCurrent(window);

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout<<" nu s-a initializat biblioteca GLAD!";
    }


    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);


    return window;

}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    float cameraSpeed = static_cast<float>(4 * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;

            if(cameraPos.y > 1)
            {
                cameraPos += cameraSpeed * cameraUp;
            }
            else
                cameraPos.y = 1.00001;
        }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        {
            std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;

            if(cameraPos.y > 1)
            {
                cameraPos -= cameraSpeed * cameraUp;
            }
            else
                cameraPos.y = 1.00001;
        }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        {
            std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;

            if(cameraPos.y > 1)
            {
                cameraPos += cameraSpeed * cameraFront;
            }
            else
                cameraPos.y = 1.00001;
        }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;

        if(cameraPos.y > 1)
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        else
            cameraPos.y = 1.00001;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;

        if(cameraPos.y > 1)
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        else
            cameraPos.y = 1.00001;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        std::cout<<cameraPos.x<<" "<<cameraPos.y<<" "<<cameraPos.z<<std::endl;

        if(cameraPos.y > 1)
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        else
            cameraPos.y = 1.00001;
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
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

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

    float sensitivity = 0.1f; // change this value to your liking
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}


GLuint createProgram(char* VSFile, char* FSFile)
{

    /*
        functia creaza un program shader folosind ca fisiere sursa VSFile si FSFile
        IN: numele fisierelor sursa
        OUT: aliasul programului shader
    */

    GLuint vertexShader, fragmentShader, shaderProgram;
    int success;
    char infoLog[512];


    vertexShader = glCreateShader(GL_VERTEX_SHADER);

    if( 0 == vertexShader )
    {
        std::cout << "Error creating vertex shader." << std::endl;
        exit(1);
    }

    std::string shaderCode = readFile(VSFile);
    const char *codeArray = shaderCode.c_str();
    glShaderSource( vertexShader, 1, &codeArray, NULL );

    glCompileShader(vertexShader);

    // verficam daca s-a reusit compilarea codului

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    if( 0 == fragmentShader )
    {
        std::cout << "Error creating fragment shader." << std::endl;
        exit(1);
    }


    shaderCode = readFile(FSFile);
    codeArray = shaderCode.c_str();
    glShaderSource( fragmentShader, 1, &codeArray, NULL );


    glCompileShader(fragmentShader);

    // se verifica compilarea codului

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    //cream programul

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // se verifica procesul de link

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}


std::string readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);

    if(!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath;
        std::cerr << ". File does not exist." << std::endl;
        return "";
    }

    std::string line = "";
    while(!fileStream.eof())
    {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }

    fileStream.close();
    return content;
}
