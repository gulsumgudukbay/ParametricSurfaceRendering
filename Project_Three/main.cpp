#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <GL/glew.h>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Shader.h>
#include <math.h>
#include <vector>

#define NUM_SHADERS 2
#define WSIZE 512
#define NUM_BUFFERS 1

bool is_closed;
long loop_size;

SDL_Window* window;
SDL_GLContext GLcontext;

std::vector<glm::vec3> vertices;
std::vector<unsigned int> indices;

int init_window()
{
    //regular bureaucracy
    SDL_Init( SDL_INIT_EVERYTHING);
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute( SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1);

    //window properties and other bureacuracy
    window = SDL_CreateWindow("Window Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WSIZE, WSIZE, SDL_WINDOW_OPENGL);
    GLcontext = SDL_GL_CreateContext(window);

    //bureaucracy continued...
    GLenum status = glewInit();

    if( status != GLEW_OK)
    {
        printf("Fail.\n");
    }

    is_closed = false;

    return 0;
}

void clear_window()
{
    glClearColor( 0.5f, 0.5f, 1.0f, 1.0f);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

}

void handle_events()
{
    SDL_Event event;

    SDL_GL_SwapWindow(window);
    while( SDL_PollEvent( &event))
    {
        if(event.type == SDL_QUIT)
            is_closed = true;
    }
}

void generate_vertices( float n1, float n2)
{
    float rx = 0.5f;
    float ry = 0.5f;
    float rz = 0.5f;

    vertices.clear();

    loop_size = 0;
    for( float u = -1.0f; u < 1.0f; u += 0.01)
    {
        for( float v = -1.0f; v < 1.0f; v += 0.01)
        {
            float x = rx*pow( cos( u*(M_PI/2)), n1)*pow( cos( v*M_PI), n2);
            float y = ry*pow( cos( u*(M_PI/2)), n1)*pow( sin( v*M_PI), n2);
            float z = rz*pow( sin( u*(M_PI/2)), n1);

            vertices.push_back( glm::vec3(x,y,z));
        }
        loop_size++;
    }
}

void generate_indices()
{
    indices.clear();


    for( long i = 0; i < loop_size-1; i++)
    {
        for( long j = 0; j < loop_size; j++)
        {
            indices.push_back(i*loop_size + j);
            indices.push_back((i+1)*loop_size + j);
        }
    }
}

unsigned int draw_count;
GLuint elementbuffer;
GLuint vertex_arr_obj;
GLuint vertex_arr_buffers[NUM_BUFFERS];

void send_vertices()
{
    draw_count = indices.size();

    glGenVertexArrays(1, &vertex_arr_obj);
    glBindVertexArray(vertex_arr_obj);

    glGenBuffers(NUM_BUFFERS, vertex_arr_buffers);

    glBindBuffer( GL_ARRAY_BUFFER, vertex_arr_buffers[0]);
    glBufferData( GL_ARRAY_BUFFER, vertices.size() * sizeof( vertices[0]), &vertices[0], GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    glGenBuffers(1, &elementbuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_DYNAMIC_DRAW);

    glBindVertexArray(0);
}

void draw_vertices()
{
    glBindVertexArray( vertex_arr_obj);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
     // Draw the triangles !
     glDrawElements(
         GL_TRIANGLE_STRIP,      // mode
         indices.size(),    // count
         GL_UNSIGNED_INT,   // type
         (void*)0           // element array buffer offset
     );

    glBindVertexArray(0);
}

float theta = 0;
void project( Shader* sh)
{
    glm::mat4 projectionMatrix;
    glm::mat4 cameraMatrix;
    glm::mat4 perspective;

    cameraMatrix = glm::lookAt(glm::vec3(0,0.3,-1.5), glm::vec3(0,0,0), glm::vec3(0,1.0,0) );
    perspective = glm::perspective( 0.8f, 1.0f, 0.01f, 10.0f);

    projectionMatrix = perspective*cameraMatrix;
    projectionMatrix = glm::rotate( projectionMatrix, theta, glm::vec3(1,1,0));

    GLint loc = glGetUniformLocation( sh->my_program, "projectionMatrix");
    if (loc != -1)
    {
       glUniformMatrix4fv(loc, 1, GL_FALSE, &projectionMatrix[0][0]);
    }

    theta += 0.01;
}

int main()
{
    init_window();
    Shader sh("./res/shader");

    sh.Bind();
    generate_vertices(1.0f,5.0f);
    generate_indices();
    send_vertices();

    glEnable(GL_DEPTH_TEST);
    while( ! is_closed)
    {
        clear_window();
        //do stuff here
        project(&sh);
        draw_vertices();
        //
        handle_events();
    }

}
