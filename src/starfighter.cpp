/***************************************************************************
 *   Copyright (C) 2004 by Beau V.C. Bellamy                               *
 *   beau@borealisbroadband.net                                            *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <cstdlib>

#include <math.h>

#include <amethyst/vector.h>
#include <amethyst/orientation.h>
#include <amethyst/object.h>
#include <amethyst/physics.h>

#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_opengl.h"

#include "joystick.h"
#include "net.h"

#define WIDTH  1024
#define HEIGHT 768

#define TICK_INTERVAL    10

#define TODEG(x)    x = x * 180 / M_PI
#define TORAD(x)    x = x / 180 * M_PI

#define JOYAXISGET(axis, var)  


using namespace std;
using namespace amethyst;

class __global {

   public:
      Object ship;

      GLUquadric *quadratic;

};

__global Global;

void DrawShip(void);
static void setup_sdl(void);
static void setup_joystick(void);
Uint32 TimerFunction(Uint32 interval, void *param);

GLuint dlShip;          // Display list identifier for the ship

// Initial
Quaternion dir(1, 0, 0, 0);
Cartesian_Vector position(100,0,0), velocity, accel, thrust;
unsigned short joy_null = 1000;
short joy_max = 32767;
short joy_min = -32768;
float viewx = 0;
float viewy = 0;

//for debug refresh
int ref_count = 0;

SDL_Joystick *global_joy;

// Window geometry
GLfloat windowWidth;
GLfloat windowHeight;

Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v)
{
    Quaternion t;
    //Quaternion invq = q.Bar();

    t = (q*v)*q.Bar();
    return t.GetVector();
}

static Uint32 next_time;

Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(next_time <= now)
        return 0;
    else
        return next_time - now;
}


void print_vector(char *title, const Cartesian_Vector &vector) {
  
       printf("%s CV: x:%#1.16lE y:%#1.16lE z:%#1.16lE\n", title, vector.x, vector.y, vector.z);
       }

// Called to draw scene
void RenderScene(void)
{

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPushMatrix();

    //Camera
      //View
      Cartesian_Vector raw_pos (0,0,0);
      Cartesian_Vector raw_view(0,1e10,0);
      Cartesian_Vector raw_up  (0,0,1e10);

      Cartesian_Vector shipoffset(0,-20, 3);
      //Cartesian_Vector shipoffset(0,0, 0);

      //dir.Normalize();

      Cartesian_Vector real_pos   = (QVRotate(dir, (shipoffset + raw_pos ))) + position;
      Cartesian_Vector real_view  = (QVRotate(dir, (shipoffset + raw_view))) + position;
      Cartesian_Vector real_up    = (QVRotate(dir, (shipoffset + raw_up  ))) + position;

      //Cartesian_Vector real_pos   = QVRotate(dir, (shipoffset + raw_pos ));
      //Cartesian_Vector real_view  = QVRotate(dir, (shipoffset + raw_view));
      //Cartesian_Vector real_up    = QVRotate(dir, (shipoffset + raw_up  ));

      /*
      if (ref_count >= 15)
      {
        print_vector("ship position", position);
        print_vector("real_pos     ", real_pos);
        print_vector("real_view    ", real_view);
        print_vector("real_up      ", real_up);
        printf("RAW  SQUARE: %f \n", phys_distance(raw_view, raw_up));
        printf("REAL SQUARE: %f \n", phys_distance(real_view, real_up));
        ref_count = 0;
      } else ref_count++;
      */

      //Down tilt
      //Euler eul(0, -4, -0);

      //Camera tilt in
      

      // Apply Camera
      gluLookAt(real_pos.x,  real_pos.y,  real_pos.z,
                real_view.x, real_view.y, real_view.z,
                real_up.x,   real_up.y,   real_up.z);

      //gluLookAt(0,-20,0,
      //          0,0,0,
      //          0,-20,10);

    //glRotatef(viewx, 0.0f, 1.0f, 0.0f);
    //glRotatef(viewy, 1.0f, 0.0f, 0.0f);

    //Lights
    GLfloat lightPos[] = { 00.0f, 00.0f, 100.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


    //glTranslatef(0.0f, 0.0f, -15.0f);
    //glRotatef(-75.0f, 2.0f, 0.0f, 0.0f);

    //Draw Ship
    glPushMatrix();

      double theta = 2 * acos(dir.w);
      TODEG(theta);

      //float scale = sqrtf ((dir.x * dir.x) + (dir.y *  dir.y) + (dir.z * dir.z));

      //glRotatef(theta, dir.x/scale, dir.y/scale, dir.z/scale);

      //Move to ship position
      glTranslated(position.x, position.y, position.z);

      //Rotate Ship
      glRotatef(theta, dir.x, dir.y, dir.z);

      {
        GLfloat fDiffLight[] =  { 0.0f, 0.0f, 1.0f };  // BLUE!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      glCallList(dlShip);
gg:
    glPopMatrix();

    
    //Draw Ship 2
    glPushMatrix();

      //Move to ship position
      glTranslated(0, 40, 3);

      //Rotate Ship
      //glRotatef(theta, dir.x, dir.y, dir.z);
      {
        GLfloat fDiffLight[] =  { 0.0f, 1.0f, 0.0f };  // Green!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      glCallList(dlShip);

    glPopMatrix();
    

    //Draw view ref
    glPushMatrix();

      //Move to ref position
      //glTranslatef(real_view.x , real_view.y, real_view.z);

      //Rotate Ship
      //glRotatef(theta, dir.x, dir.y, dir.z);
      {
        GLfloat fDiffLight[] =  { 1.0f, 0.0f, 0.0f };  // Red!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      glDisable(GL_LIGHTING);
      gluSphere(Global.quadratic,1.392e6,32,32);
      glEnable(GL_LIGHTING);

    glPopMatrix();



    //glPopMatrix();  Camera
    // Do the buffer Swap
    SDL_GL_SwapBuffers();
}

void SetupRC(void)
{
    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f );
    
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
        
    // Lit texture environment
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Load the ship texture
    //GLint iWidth, iHeight,iComponents;
    //GLenum eFormat;
    //GLbyte *pBytes = gltLoadTGA("YellowSub.tga", &iWidth, &iHeight, &iComponents, &eFormat);    
    //glTexImage2D(GL_TEXTURE_2D, 0, iComponents, iWidth, iHeight, 0, eFormat, GL_UNSIGNED_BYTE, (void *)pBytes);
    //free(pBytes);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glEnable(GL_TEXTURE_2D);

    // Set up lighting
    GLfloat fAmbLight[] =   { 0.01f, 0.01f, 0.01f };
    GLfloat fDiffLight[] =  { 0.0f, 0.0f, 1.0f };  // BLUE!!
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
    
    // Build a single display list
    dlShip = glGenLists(1);
    glNewList(dlShip, GL_COMPILE);
    DrawShip();
    glEndList();

    // Set up quadratics
    Global.quadratic = gluNewQuadric();
    gluQuadricNormals(Global.quadratic, GLU_SMOOTH);  //Create Smooth Normals (NEW)
}

void ChangeSize(GLsizei w, GLsizei h)
{

	GLfloat fAspect;

	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
            h = 1;

        glViewport(0, 0, w, h);
        
        fAspect = (GLfloat)w / (GLfloat)h;

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	// Set the clipping volume
	gluPerspective(35.0f, fAspect, 1.0f, 50000000000.0f);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

static void setup_sdl() 
{
    const SDL_VideoInfo* video;

    cout << "Initializing SDL...";
    if ( SDL_Init(SDL_INIT_VIDEO) < 0 ) {
        fprintf(stderr,
                "Couldn't initialize SDL: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    atexit(SDL_Quit);

    cout << "Initializing Video...";
    video = SDL_GetVideoInfo( );

    if( !video ) {
        fprintf(stderr,
                "Couldn't get video information: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Success!" << endl;

    cout << "Initializing OpenGL...";

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    /* Note the SDL_DOUBLEBUF flag is not required to enable double 
     * buffering when setting an OpenGL video mode. 
     * Double buffering is enabled or disabled using the 
     * SDL_GL_DOUBLEBUFFER attribute.
     */
    if( SDL_SetVideoMode( WIDTH, HEIGHT, video->vfmt->BitsPerPixel, SDL_OPENGL |  SDL_RESIZABLE) == 0 ) {
        fprintf(stderr, 
                "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Sucess!" << endl;

}


static void setup_opengl()
{
    cout << "Initializing OpenGL...";

    ChangeSize(WIDTH, HEIGHT);

    SetupRC();

    cout << "YAY!" << endl;
}

static void setup_joystick()
{
    cout << "Initializing Joystick...";

    SDL_InitSubSystem(SDL_INIT_JOYSTICK); 

    
    // Check for joystick
    if(SDL_NumJoysticks()>0){
      // Open joystick
      global_joy=SDL_JoystickOpen(0);

      if(global_joy)
      {
        cout << "Yay!" << endl;
        printf("Opened Joystick 0\n");
        printf("Name: %s\n", SDL_JoystickName(0));
        printf("Number of Axes   : %d\n", SDL_JoystickNumAxes(global_joy));
        printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(global_joy));
        printf("Number of Hats   : %d\n", SDL_JoystickNumHats(global_joy));
        printf("Number of Balls  : %d\n", SDL_JoystickNumBalls(global_joy));
        printf("Number of Shafts : 0\n");
      }
      else
        cout << "Couldn't open Joystick 0" << endl;
      
      // Close if opened
      //if(SDL_JoystickOpened(0))
      //  SDL_JoystickClose(global_joy);
    }
    else
      cout << "No Joysticks found." << endl;


}

static void process_inputs()
{
    GLfloat yRot  = 0.0f;
    GLfloat xRot  = 0.0f;
    GLfloat zRot  = 0.0f;
    GLfloat speed = 0.0f;
    accel.Zeroize();
    thrust.Zeroize();

    float scaler = 1.0f / (float(joy_max - joy_null));

    // Get Inputs
    yRot = float(joystick_axis_norm(SDL_JoystickGetAxis(global_joy, 0), joy_null) * scaler);

    //if (SDL_JoystickGetAxis(global_joy, 0) > joy_null) yRot =
    //      (float(SDL_JoystickGetAxis(global_joy, 0) - joy_null)) * 1.0f / (float(joy_max - joy_null));
    //if (SDL_JoystickGetAxis(global_joy, 0) < -joy_null) yRot =
    //      (float(SDL_JoystickGetAxis(global_joy, 0) + joy_null)) * 1.0f / (float(joy_max - joy_null));

    xRot = float(joystick_axis_norm(SDL_JoystickGetAxis(global_joy, 1), joy_null) * scaler);

    //if (SDL_JoystickGetAxis(global_joy, 1) > joy_null) xRot =
    //      (float(SDL_JoystickGetAxis(global_joy, 1) - joy_null)) * 1.0f / (float(joy_max - joy_null));
    //if (SDL_JoystickGetAxis(global_joy, 1) < -joy_null) xRot =
    //      (float(SDL_JoystickGetAxis(global_joy, 1) + joy_null)) * 1.0f / (float(joy_max - joy_null));

    zRot = -float(joystick_axis_norm(SDL_JoystickGetAxis(global_joy, 3), joy_null) * scaler);

    //if (SDL_JoystickGetAxis(global_joy, 3) > joy_null) zRot =
    //      -(float(SDL_JoystickGetAxis(global_joy, 3) - joy_null)) * 1.0f / (float(joy_max - joy_null));
    //if (SDL_JoystickGetAxis(global_joy, 3) < -joy_null) zRot =
    //      -(float(SDL_JoystickGetAxis(global_joy, 3) + joy_null)) * 1.0f / (float(joy_max - joy_null));

    speed = -float(joystick_axis_norm(SDL_JoystickGetAxis(global_joy, 4), joy_null) * scaler);

    //if (joy_state.axis[4] > joy_null)  speed = (float(joy_state.axis[4] - joy_null)) * 1.0f / (float(joy_max - joy_null));
    //if (joy_state.axis[4] < -joy_null) speed = (float(joy_state.axis[4] + joy_null)) * 1.0f / (float(joy_max - joy_null));

    //if (SDL_JoystickGetAxis(global_joy, 4) > joy_null)  speed =
    //      (float(SDL_JoystickGetAxis(global_joy, 4) - joy_null)) * 1.0f / (float(joy_max - joy_null));
    //if (SDL_JoystickGetAxis(global_joy, 4) < -joy_null) speed =
    //      (float(SDL_JoystickGetAxis(global_joy, 4) + joy_null)) * 1.0f / (float(joy_max - joy_null));

    if (SDL_JoystickGetAxis(global_joy, 6) == joy_max) viewx += .1;
    if (SDL_JoystickGetAxis(global_joy, 6) == -joy_max) viewx -= .1;
    
    if (SDL_JoystickGetAxis(global_joy, 7) == joy_max) viewy += .1;
    if (SDL_JoystickGetAxis(global_joy, 7) == -joy_max) viewy -= .1;

    if (SDL_JoystickGetButton(global_joy, 15)) thrust.x += .001;
    if (SDL_JoystickGetButton(global_joy, 17)) thrust.x -= .001;
    
    if (SDL_JoystickGetButton(global_joy, 14)) thrust.z += .001;
    if (SDL_JoystickGetButton(global_joy, 16)) thrust.z -= .001;

    if (SDL_JoystickGetButton(global_joy, 0)) {
        velocity.Zeroize();
        };

    if (SDL_JoystickGetButton(global_joy, 7)) {
        dir.w = 1;
        dir.x = 0;
        dir.y = 0;
        dir.z = 0;
        viewx = 0;
        viewy = 0;
        position.Zeroize();
        velocity.Zeroize();
        };

    //Ship Rotation
    TORAD(xRot);
    TORAD(yRot);
    TORAD(zRot);
       
    Euler eul(xRot, yRot, zRot);
    Quaternion quat(eul);
    
    dir *= quat;
    dir.Normalize();
        
    
    //float scale = sqrtf ((dir.x * dir.x) + (dir.y *  dir.y) + (dir.z * dir.z));
    
    //Ship position
    if (speed != 0) {

        //thrust.y = -1.0f;
        thrust.y = speed/1000;
        //cout << speed << ":" << joy_state.axis[3] << "    ";
        //thrust.x = 0.1f;
        //thrust.z = 0.1f;

        //Quaternion new_dir = ~(dir);
        }

    // Rotate trust vector to match ship orientation 
    accel = QVRotate(dir, thrust);

    // Calculate new velocity from Acceleration vectors
    velocity += accel;

    // Calculate new position based on velocity
    position += velocity;


}



static void main_loop() 
{
    SDL_Event event;
//cout << "Yay!" << endl;
    next_time = SDL_GetTicks() + TICK_INTERVAL;

    while(1) {
        /* process pending events */
        while( SDL_PollEvent( &event ) ) {

            switch( event.type ) {
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                case SDLK_ESCAPE:
                    exit(0);
             
                case SDLK_KP_PLUS:
//                    level++;
//                    if (level > 5) level=5;
                    break;
            
                case SDLK_KP_MINUS:
//                    level--;
//                    if (level < 0) level=0;
                    break;
                } break;
            case SDL_MOUSEMOTION:
//                pitch += event.motion.yrel;
//                if (pitch < -70) pitch = -70;
//                if (pitch > 70) pitch = 70;
                break;

            case SDL_VIDEORESIZE:
                  SDL_SetVideoMode( event.resize.w, event.resize.h, 32, SDL_OPENGL |  SDL_RESIZABLE);
                  ChangeSize(event.resize.w, event.resize.h);
                break;

             case SDL_QUIT:
               exit (0);
            }
        }

    // Process Inputs
    process_inputs();

    /* update the screen */
    RenderScene();
    //SDL_Delay(time_left());
    next_time += TICK_INTERVAL;
    }
}


int main(int argc, char* argv[])
{

    setup_sdl();

    setup_joystick();

    setup_opengl();

    net_test();

    main_loop();

    glDeleteLists(dlShip,1);
    return EXIT_SUCCESS;
}


