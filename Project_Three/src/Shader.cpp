#include "Shader.h"
#include <iostream>
#include <fstream>

Shader::Shader( const std::string& fName)
{
    my_program = glCreateProgram();
    my_shaders[0] = createShader( loadShader(fName + ".vs.glsl"), GL_VERTEX_SHADER);
    my_shaders[1] = createShader( loadShader(fName + ".fs.glsl"), GL_FRAGMENT_SHADER);

    //attach shaders to OpenGL
    for( unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glAttachShader( my_program, my_shaders[i]);
    }

    //attribute config
    //glBindAttribLocation( my_program, 0, "position");
    //glBindAttribLocation( my_program, 1, "color");

    //link the OpenGL progam to our program
    glLinkProgram( my_program);
    checkShaderError( my_program, GL_LINK_STATUS, true, "Program failed linking: ");

    //validate.
    glValidateProgram( my_program);
    checkShaderError( my_program, GL_VALIDATE_STATUS, true, "Program is invalid lol: ");


}

Shader::~Shader()
{
    //delete shaders from OpenGL
    for( unsigned int i = 0; i < NUM_SHADERS; i++)
    {
        glDetachShader( my_program, my_shaders[i]);
        glDeleteShader( my_shaders[i]);
    }

    glDeleteProgram(my_program);
}


// BUREAUCRACY  CONT.D //
std::string Shader::loadShader(const std::string& fileName)
{
    std::ifstream file;
    file.open((fileName).c_str());

    std::string output;
    std::string line;

    if(file.is_open())
    {
        while(file.good())
        {
            getline(file, line);
			output.append(line + "\n");
        }
    }
    else
    {
		std::cerr << "Unable to load shader: " << fileName << std::endl;
    }

    return output;
}

void Shader::checkShaderError(GLuint shader, GLuint flag, bool isProgram, const std::string& errorMessage)
{
    GLint success = 0;
    GLchar error[1024] = { 0 };

    if(isProgram)
        glGetProgramiv(shader, flag, &success);
    else
        glGetShaderiv(shader, flag, &success);

    if(success == GL_FALSE)
    {
        if(isProgram)
            glGetProgramInfoLog(shader, sizeof(error), NULL, error);
        else
            glGetShaderInfoLog(shader, sizeof(error), NULL, error);

        std::cout << errorMessage << ": '" << error << "'" << std::endl;
    }
}

//


void Shader::Bind()
{
    glUseProgram( my_program);
}

GLuint Shader::createShader( const std::string& text, GLenum type)
{
    const GLchar* shaderSourceStrings[1];
    GLint shaderSourceStringLengths[1];

    GLuint myshader = glCreateShader( type);

    if( !myshader)
        std::cout << "Shader creation failure!" << std::endl;

    shaderSourceStrings[0] = text.c_str();
    shaderSourceStringLengths[0] = text.length();

    glShaderSource( myshader, 1, shaderSourceStrings, shaderSourceStringLengths);
    glCompileShader( myshader);

    //check err
    Shader::checkShaderError( myshader, GL_COMPILE_STATUS, false, "Program failed linking: ");


    return myshader;
}

