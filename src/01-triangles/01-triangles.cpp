//////////////////////////////////////////////////////////////////////////////
//
//  Triangles.cpp
//
//////////////////////////////////////////////////////////////////////////////

#include "vgl.h"
#include "LoadShaders.h"

enum VAO_IDs { Triangles, NumVAOs };
enum Buffer_IDs { ArrayBuffer, NumBuffers };
enum Attrib_IDs { vPosition = 0 };

GLuint  VAOs[NumVAOs];
GLuint  Buffers[NumBuffers];

const GLuint  NumVertices = 6;

//----------------------------------------------------------------------------
//
// init
//

void
init( void )
{
	// 生成Vertex Array
	// 函数API上没有Object这个词汇
    // 而且是，可以生成多个，所以命名上挂s
    glGenVertexArrays(NumVAOs, VAOs );
    glBindVertexArray(VAOs[Triangles] );
    
    // -----------------------------------------
    //              传入网格数据
    // -----------------------------------------

	// 顶点数据
    GLfloat vertices[NumVertices][2] = {
        { -0.90f, -0.90f }, {  0.85f, -0.90f }, { -0.90f,  0.85f },  // Triangle 1
        {  0.90f, -0.85f }, {  0.90f,  0.90f }, { -0.85f,  0.90f }   // Triangle 2
    };

	// Array Buffer
    glGenBuffers(NumBuffers, Buffers );
    glBindBuffer(GL_ARRAY_BUFFER, Buffers[ArrayBuffer]);
    
	// glBufferStorage 会崩掉？
	// 8th书中，没有提到glBufferStorage
    // glBufferStorage( GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
	int sizeOfVertices = sizeof(vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ------------------------
    //      加载使用的Shader
    // ----------------------
    ShaderInfo  shaders[] =
    {
        { GL_VERTEX_SHADER, "media/shaders/triangles/triangles.vert" },
        { GL_FRAGMENT_SHADER, "media/shaders/triangles/triangles.frag" },
        { GL_NONE, NULL }
    };

    GLuint program = LoadShaders( shaders );
    glUseProgram( program );
    
    // -------------------------------------------
    //      指定，Shader中变量，应该如何从Buffer中获取
    // --------------------------------------------
    
    // 测试void*大小
    int voidStarSize = sizeof(void*);
    
    // 指定顶点属性position，应该如何从Array Buffer中去取
    glVertexAttribPointer(vPosition,
                          2,                // size: 每个顶点，有几个基本数据元素！这里是2，表示有2个float !
                          GL_FLOAT,         // type: 每一个数据元素的类型
                          GL_FALSE,         // normalized: 传入vertex shader的时候，是否需要normalize? 这里传true，会怎样？！
                          0,                // stride: 到下个顶点数据之间的间隔。这里是0，表示数据紧密的排列着！
                          BUFFER_OFFSET(0)  // pointer: void*的大小是8，因为是64位操作系统 8*8=64。这里pointer就是表示字节偏移？虽然不知道为什么要用void*类型！
    );
    
    glEnableVertexAttribArray( vPosition );
}

//----------------------------------------------------------------------------
//
// display
//

void
display( void )
{
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };

    glClearBufferfv(GL_COLOR, 0, black);

    glBindVertexArray( VAOs[Triangles] );
    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
}

//----------------------------------------------------------------------------
//
// main
//

#ifdef _WIN32
int CALLBACK WinMain(
  _In_ HINSTANCE hInstance,
  _In_ HINSTANCE hPrevInstance,
  _In_ LPSTR     lpCmdLine,
  _In_ int       nCmdShow
)
#else
int
main( int argc, char** argv )
#endif
{
    glfwInit();

    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangles", NULL, NULL);

    glfwMakeContextCurrent(window);
    gl3wInit();

    init();

    while (!glfwWindowShouldClose(window))
    {
        display();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);

    glfwTerminate();
}
