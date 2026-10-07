#include <iostream>
#include <glad/glad.h> 
#include <GLFW/glfw3.h>

using namespace std;

int main()
{
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", glfwGetPrimaryMonitor(), NULL);
    cout << "Hello World !" << endl;
    return 0;
}