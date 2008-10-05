/***********************************************************************
 Amethyst-GL
  - OpenGL shaders setup and processing function implemenations

 Authors (c):
 2008 Beau V.C. Bellamy (beau@stellarnetservices.net)

 $Revision$
 $LastChangedDate$
 $LastChangedBy$
 ***********************************************************************/

#include "opengl.h"
#include "opengl_shader.h"
#include "global.h"

#include <fstream>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *textFileRead(const char *fn);
void printInfoLog(GLhandleARB obj);

void load_shader(const std::string &vname, const std::string &fname)
{

    std::string   vpath = Global.dir_shaders + vname;
    std::string   fpath = Global.dir_shaders + fname;

    GLhandleARB myVertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    GLhandleARB myFragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    char *vtxt = textFileRead(vpath.c_str());
    char *ftxt = textFileRead(fpath.c_str());

    glShaderSourceARB(myVertexShader, 1, const_cast<const char**>(&vtxt), NULL);
    glShaderSourceARB(myFragmentShader, 1, const_cast<const char**>(&ftxt), NULL);

    glCompileShaderARB(myVertexShader);

    GLint success;
    glGetObjectParameterivARB(myVertexShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(myVertexShader, 1000, NULL, infoLog);
        std::cout << "Error in vertex shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        return;
    }

    success = 0;
    glCompileShaderARB(myFragmentShader);
    glGetObjectParameterivARB(myFragmentShader, GL_OBJECT_COMPILE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(myFragmentShader, 1000, NULL, infoLog);
        std::cout << "Error in fragment shader compilation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        return;
    }


    Global.shaderProgram = glCreateProgramObjectARB();
    glAttachObjectARB(Global.shaderProgram, myFragmentShader);
    glAttachObjectARB(Global.shaderProgram, myVertexShader);

    success = 0;

    glLinkProgramARB(Global.shaderProgram);
    glGetObjectParameterivARB(Global.shaderProgram, GL_OBJECT_LINK_STATUS_ARB, &success);
    if (!success)
    {
        printInfoLog(Global.shaderProgram);
    }

    success = 0;

    glValidateProgramARB(Global.shaderProgram);
    glGetObjectParameterivARB(Global.shaderProgram, GL_OBJECT_VALIDATE_STATUS_ARB, &success);
    if (!success)
    {
        GLcharARB infoLog[1000];
        glGetInfoLogARB(Global.shaderProgram, 1000, NULL, infoLog);
        std::cout << "Error in program validation!" << std::endl;
        std::cout << "Info Log: " << infoLog << std::endl;
        //return;
    }

    free(vtxt);
    free(ftxt);
}


char *textFileRead(const char *fn) {


    FILE *fp;
    char *content = NULL;

    int count=0;

    if (fn != NULL) {
        fp = fopen(fn,"rt");

        if (fp != NULL) {

      fseek(fp, 0, SEEK_END);
      count = ftell(fp);
      rewind(fp);

            if (count > 0) {
                content = reinterpret_cast<char *>(malloc(sizeof(char) * (count+1)));
                count = fread(content,sizeof(char),count,fp);
                content[count] = '\0';
            }
            fclose(fp);
        }
    }
    return content;
}

void printInfoLog(GLhandleARB obj)
{
    int infologLength = 0;
    int charsWritten  = 0;
    char *infoLog;

    glGetObjectParameterivARB(obj, GL_OBJECT_INFO_LOG_LENGTH_ARB,
                                            &infologLength);

    if (infologLength > 0)
    {
        infoLog = reinterpret_cast<char *>(malloc(infologLength));
        glGetInfoLogARB(obj, infologLength, &charsWritten, infoLog);
                printf("%s\n",infoLog);
        free(infoLog);
    }
}

