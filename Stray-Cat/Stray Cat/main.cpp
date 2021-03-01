#include "common.h"
#include "Image.h"
#include "Player.h"
#include "castle.hpp"

#define GLFW_DLL
#include <GLFW/glfw3.h>

constexpr GLsizei WINDOW_WIDTH = 768, WINDOW_HEIGHT = 768;

struct InputState
{
    bool keys[1024]{}; //массив состояний кнопок - нажата/не нажата
    GLfloat lastX = 400, lastY = 300; //исходное положение мыши
    bool firstMouse = true;
    bool captureMouse         = true;  // Мышка захвачена нашим приложением или нет?
    bool capturedMouseJustNow = false;
} static Input;


GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
GLfloat deltaTimeTMP = 0.0f;
GLfloat deltaTimeTMP1 = 0.0f;


void OnKeyboardPressed(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    switch (key)
    {
        case GLFW_KEY_ESCAPE:
            if (action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_1:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case GLFW_KEY_2:
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            if (action == GLFW_PRESS)
                Input.keys[key] = true;
            else if (action == GLFW_RELEASE)
                Input.keys[key] = false;
    }
}

void processPlayerMovement(Player &player)
{
    if (Input.keys[GLFW_KEY_W])
        player.ProcessInput(MovementDir::UP);
    else if (Input.keys[GLFW_KEY_S])
        player.ProcessInput(MovementDir::DOWN);
    if (Input.keys[GLFW_KEY_A])
        player.ProcessInput(MovementDir::LEFT);
    else if (Input.keys[GLFW_KEY_D])
        player.ProcessInput(MovementDir::RIGHT);
}

void OnMouseButtonClicked(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        Input.captureMouse = !Input.captureMouse;
    
    if (Input.captureMouse)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        Input.capturedMouseJustNow = true;
    }
    else
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    
}

void OnMouseMove(GLFWwindow* window, double xpos, double ypos)
{
    if (Input.firstMouse)
    {
        Input.lastX = float(xpos);
        Input.lastY = float(ypos);
        Input.firstMouse = false;
    }
    
    GLfloat xoffset = float(xpos) - Input.lastX;
    GLfloat yoffset = Input.lastY - float(ypos);
    
    Input.lastX = float(xpos);
    Input.lastY = float(ypos);
}


void OnMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
    // ...
}


int initGL()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize OpenGL context" << std::endl;
        return -1;
    }
    
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    
    std::cout << "Controls: "<< std::endl;
    std::cout << "press right mouse button to capture/release mouse cursor  "<< std::endl;
    std::cout << "W, A, S, D - movement  "<< std::endl;
    std::cout << "press ESC to exit" << std::endl;
    
    return 0;
}

int UpdatePhase(GLfloat &currentFrame) {
    GLfloat tmp = currentFrame * 4;
    if (tmp < 1) {
        return 0;
    }
    if (tmp < 2) {
        return 1;
    }
    if (tmp < 3) {
        return 2;
    }
    currentFrame = 0;
    return 0;
}

//void SetBlackScreen(std::shared_ptr<Image> screen, GLfloat &current)
//{
//    Pixel pix;
//    GLfloat currentFrame = glfwGetTime();
//    while (currentFrame < 20) {
//        currentFrame = glfwGetTime();
//        std::cout << "LOL" <<  currentFrame << std::endl;
//    }
//    std::cout << currentFrame << std::endl;
//    for (double p = 0.9; p >= 0; p -= 0.5) {
//        for(int y = 0; y < 768; ++y) {
//            for(int x = 0; x < 768; ++x) {
//                pix = screen->GetPixel(x, y);
//                pix.r *= p;
//                pix.g *= p;
//                pix.b *= p;
//                pix.a *= p;
//                screen->PutPixel(x, y, pix);
//            }
//        }
//
//    }
//}

int main(int argc, char** argv)
{
    if(!glfwInit())
        return -1;
    
    //    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    //    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    
    GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "task1 base project", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    
    glfwMakeContextCurrent(window);
    
    glfwSetKeyCallback        (window, OnKeyboardPressed);
    glfwSetCursorPosCallback  (window, OnMouseMove);
    glfwSetMouseButtonCallback(window, OnMouseButtonClicked);
    glfwSetScrollCallback     (window, OnMouseScroll);
    
    if(initGL() != 0)
        return -1;
    
    //Reset any OpenGL errors which could be present for some reason
    GLenum gl_error = glGetError();
    while (gl_error != GL_NO_ERROR)
        gl_error = glGetError();
    
    Point starting_pos{.x = WINDOW_WIDTH / 2, .y = WINDOW_HEIGHT / 2};
    Point floor{.x = 0, .y = 0};
    
    Castle cast{floor};
    
    Player player{starting_pos};
    
    Image screenBuffer(WINDOW_WIDTH, WINDOW_HEIGHT, 4);
    
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);  GL_CHECK_ERRORS;
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); GL_CHECK_ERRORS;
    
    cast.SetScreen(screenBuffer);
    cast.DrawBackground();
    
    cast.GetScreen()->ScreenSave();
    player.SetCastle(std::make_shared<Castle>(cast));
    double p = 0.9;
    Pixel pix;
    bool not_black = true;
    //game loop
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glfwPollEvents();
        deltaTimeTMP += deltaTime;
        
        if (player.GetState() == PlayerState::CHANGING_ROOM) {
            cast.SaveScreen();
            if (not_black) {
                for(int y = 0; y < WINDOW_HEIGHT; ++y) {
                    for(int x = 0; x < WINDOW_HEIGHT; ++x) {
                        pix = cast.GetScreen()->GetPixel(x, y);
                        pix.r *= p;
                        pix.g *= p;
                        pix.b *= p;
                        pix.a *= p;
                        cast.GetScreen()->PutPixel(x, y, pix);
                    }
                }
                p -= 0.05;
                if (p < 0) {
                    not_black = false;
                }
            }
            if (!not_black) {
                player.SetCoords(cast.DrawNewRoom());
                cast.SaveNewRoom();
                player.SetState(PlayerState::ALIVE);
                player.TurnOnPlayer();
            }
        }
        
        if (player.GetState() == PlayerState::ALIVE) {
            player.SetPhase(UpdatePhase(deltaTimeTMP));
            processPlayerMovement(player);
            player.Draw(cast.GetScreen());
        }
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); GL_CHECK_ERRORS;
        glDrawPixels (WINDOW_WIDTH, WINDOW_HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, screenBuffer.Data()); GL_CHECK_ERRORS;
        
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    return 0;
}
