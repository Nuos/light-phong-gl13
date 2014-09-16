#include <stdio.h>
#include <OpenGL/gl3.h>
#define GLFW_INCLUDE_GLCOREARB
#include <GLFW/glfw3.h>
#include "ShaderLoader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>


GLuint vao;
GLuint vbo[3];
GLuint indexBufferID;

ShaderLoader shaderLoader;

GLint matview_fun;
GLint matproj_fun;
GLint matrot_fun;

glm::mat4 Model;
glm::mat4 View;
glm::mat4 Projection;

GLint rot=0;

//设置三角形
void initTriangle(void)
{
    //视图矩阵
    View = glm::lookAt(
                       glm::vec3(0.0f,2.0f,3.0f),
                       glm::vec3(0,0,0),
                       glm::vec3(0,1,0)
                       );
    
    //透视投影矩阵
    Projection = glm::perspective(45.0f, 800.0f/600.0f, 2.0f, 1000.0f);
    
    Model = glm::mat4(1.0f);
    Model = glm::rotate(Model, 45.0f, glm::vec3(0,1,0));

    //顶点是从上往下，从左往右的顺序排列。
    GLfloat vertex_data[24];
    vertex_data[0] = -0.35f;   vertex_data[1] =  0.30f; vertex_data[2] = 0.5f;
    vertex_data[3] = 0.35f;     vertex_data[4] = 0.30f;  vertex_data[5] = 0.5f;
    vertex_data[6] = -0.35f;    vertex_data[7] = -0.30f; vertex_data[8] = 0.5f;
    vertex_data[9] = 0.35f;     vertex_data[10] = -0.30f;  vertex_data[11] = 0.5f;
    vertex_data[12] = -0.35f;   vertex_data[13] =  0.30f; vertex_data[14] = -0.5f;
    vertex_data[15] = 0.35f;     vertex_data[16] = 0.30f;  vertex_data[17] = -0.5f;
    vertex_data[18] = -0.35f;    vertex_data[19] = -0.30f; vertex_data[20] = -0.5f;
    vertex_data[21] = 0.35f;     vertex_data[22] = -0.30f;  vertex_data[23] = -0.5f;
    
    GLfloat vertex_normal[24];
    vertex_normal[0] = -1.0f;   vertex_normal[1] = 1.0f;    vertex_normal[2] = 1.0f;
    vertex_normal[3] = 1.0f;   vertex_normal[4] = 1.0f;    vertex_normal[5] = 1.0f;
    vertex_normal[6] = -1.0f;   vertex_normal[7] = -1.0f;    vertex_normal[8] = 1.0f;
    vertex_normal[9] = 1.0f;   vertex_normal[10] = -1.0f;    vertex_normal[11] = 1.0f;
    vertex_normal[12] = -1.0f;   vertex_normal[13] = 1.0f;    vertex_normal[14] = -1.0f;
    vertex_normal[15] = 1.0f;   vertex_normal[16] = 1.0f;    vertex_normal[17] = -1.0f;
    vertex_normal[18] = -1.0f;   vertex_normal[19] = -1.0f;    vertex_normal[20] = -1.0f;
    vertex_normal[21] = 1.0f;   vertex_normal[22] = -1.0f;    vertex_normal[23] = -1.0f;
 
    
    GLfloat vertex_color[24];
    vertex_color[0] = 0;    vertex_color[1] = 1;    vertex_color[2] = 0;
    vertex_color[3] = 0;    vertex_color[4] = 1;    vertex_color[5] = 0;
    vertex_color[6] = 0;    vertex_color[7] = 1;    vertex_color[8] = 0;
    vertex_color[9] = 0;    vertex_color[10] = 1;    vertex_color[11] = 0;
    vertex_color[12] = 1;    vertex_color[13] = 1;    vertex_color[14] = 0;
    vertex_color[15] = 1;    vertex_color[16] = 1;    vertex_color[17] = 0;
    vertex_color[18] = 1;    vertex_color[19] = 1;    vertex_color[20] = 0;
    vertex_color[21] = 1;    vertex_color[22] = 1;    vertex_color[23] = 0;
    
    
    //索引顺序，从上而下，从右往左.逆时针
    GLuint vertex_indices[36];
    //前
    vertex_indices[0] = 0;    vertex_indices[1] = 1;    vertex_indices[2] = 2;
    vertex_indices[3] = 2;    vertex_indices[4] = 1;    vertex_indices[5] = 3;
    
    //后
    vertex_indices[6] = 4;    vertex_indices[7] = 5;    vertex_indices[8] = 6;
    vertex_indices[9] = 6;    vertex_indices[10] = 5;    vertex_indices[11] = 7;
    
    //左
    vertex_indices[12] = 4;    vertex_indices[13] = 0;    vertex_indices[14] = 6;
    vertex_indices[15] = 6;    vertex_indices[16] = 0;    vertex_indices[17] = 2;
    
    //右
    vertex_indices[18] = 1;    vertex_indices[19] = 5;    vertex_indices[20] = 3;
    vertex_indices[21] = 3;    vertex_indices[22] = 5;    vertex_indices[23] = 7;
    
    //上
    vertex_indices[24] = 4;    vertex_indices[25] = 5;    vertex_indices[26] = 0;
    vertex_indices[27] = 0;    vertex_indices[28] = 5;    vertex_indices[29] = 1;
    
    //下
    vertex_indices[30] = 6;    vertex_indices[31] = 7;    vertex_indices[32] = 2;
    vertex_indices[33] = 2;    vertex_indices[34] = 7;    vertex_indices[35] = 3;
    

    //1，生成指定的VBO信息
    //顶点信息
    glGenBuffers(3, vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_data), vertex_data, GL_STATIC_DRAW);
    
    //颜色信息
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_color), vertex_color, GL_STATIC_DRAW);
    
    //绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_normal), vertex_normal, GL_STATIC_DRAW);
    
    
    glGenBuffers(1, &indexBufferID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(vertex_indices), vertex_indices, GL_STATIC_DRAW);
    
    //2，生成VAO，并绑定VBO信息
    glGenVertexArrays(1,&vao);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,indexBufferID);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

}

//设置并使用着色器
void initShader(void)
{
    shaderLoader.load("/Users/wistoneqqx/Documents/opengl/opengl-study-records/light-phong-gl13/triangel_index/triangel_index/simpleShader.vert",
                      "/Users/wistoneqqx/Documents/opengl/opengl-study-records/light-phong-gl13/triangel_index/triangel_index/simpleShader.frag");
    shaderLoader.bind();
    
    //获取着色文件中的矩阵指针
    matview_fun = glGetUniformLocation(shaderLoader.getProgramID(), "mat_view");
    matproj_fun = glGetUniformLocation(shaderLoader.getProgramID(), "mat_proj");
    matrot_fun = glGetUniformLocation(shaderLoader.getProgramID(), "mat_rot");
    
    glUniformMatrix4fv(matview_fun,1,GL_FALSE,&View[0][0]);
    glUniformMatrix4fv(matproj_fun,1,GL_FALSE,&Projection[0][0]);
    glUniformMatrix4fv(matrot_fun,1,GL_FALSE,&Model[0][0]);
}

//画三角形
void drawVBO(void)
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}

int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    //使用OpenGL 3.2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
    initTriangle();
    initShader();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f,0.0f,0.0f,0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_BLEND);
        //深度测试，这个很重要
        glEnable(GL_DEPTH_TEST);
        
        drawVBO();
        
        /* Swap front and back buffers */
        glfwSwapBuffers(window);
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}

