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
#include <amethyst/utility.h>

#include <stdlib.h>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"
#include "SDL/SDL_opengl.h"

#include "global.h"
#include "joystick.h"
#include "net.h"
#include "texture.h"
#include "model.h"
//#include "debug.h"

#define WIDTH  1024
#define HEIGHT 768

#define TICK_INTERVAL    10

#define TODEG(x)    x = x * 180 / M_PI
#define TORAD(x)    x = x / 180 * M_PI

using namespace std;
using namespace amethyst;

static void setup_sdl(void);


//for debug refresh
//int ref_count = 0;

Cartesian_Vector QVRotate(Quaternion &q, const Cartesian_Vector &v)
{
    Quaternion t;
    //Quaternion invq = q.Bar();

    t = (q*v)*q.Bar();
    return t.GetVector();
}


Uint32 time_left(void)
{
    Uint32 now;

    now = SDL_GetTicks();
    if(Global.next_time <= now)
        return 0;
    else
        return Global.next_time - now;
}

// Called to draw scene
void RenderScene(void)
{
    // Get Gobal State
    Cartesian_Vector &position = Global.ship.location;
    Quaternion       &attitude = Global.ship.attitude;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Clear the window with current clearing color
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glPushMatrix();

    //Camera
      //Camera initial values
      Cartesian_Vector raw_pos (0,0,0);
      Cartesian_Vector raw_view(0,1e10,0);
      Cartesian_Vector raw_up  (0,0,1e10);

      //Camera location in relation to ship
      Cartesian_Vector shipoffset(0,-20, 0);

     {
        // Rotate Camera to ship's orientation
        Cartesian_Vector real_pos   = (QVRotate(attitude, (shipoffset + raw_pos ))) + position;
        Cartesian_Vector real_view  = (QVRotate(attitude, (shipoffset + raw_view))) + position;
        Cartesian_Vector real_up    = (QVRotate(attitude, (shipoffset + raw_up  ))) + position;

      /*
      if (ref_count >= 15)
      {
          print_camera(real_pos, real_view, real_up);
          ref_count = 0;
      } else ref_count++;
      */


      // Ring buffer (delays camera movement by a few frames)
      {
        Global.cam_num++;
        if(Global.cam_num > 8) Global.cam_num = 0;
        int cam_num = Global.cam_num;

        Global.cam_pos[cam_num] = real_pos;
        Global.cam_view[cam_num] = real_view;
        Global.cam_up[cam_num] = real_up;

        int view_num = cam_num - 7;
        if(view_num < 0) view_num = view_num + 9;
        real_pos  = Global.cam_pos[view_num];
        real_view = Global.cam_view[view_num];
        real_up   = Global.cam_up[view_num];
      }

      // Apply Camera
      gluLookAt(real_pos.x,  real_pos.y,  real_pos.z,
                real_view.x, real_view.y, real_view.z,
                real_up.x,   real_up.y,   real_up.z);
    }

    //Sky Box
    {
      glPushMatrix();
        glDisable(GL_LIGHTING);
         skybox();
         //glCallList(Global.starfield_mdl);
        glEnable(GL_LIGHTING);
      glPopMatrix();
    }

    //Lights
    GLfloat lightPos[] = { 00.0f, 00.0f, 1000.0f, 1.0f };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);


    //Draw Ship
    glPushMatrix();

      glDisable(GL_TEXTURE_2D);

      double theta = 2 * acos(attitude.w);
      TODEG(theta);

      //Move to ship position
      glTranslated(position.x, position.y, position.z);

      //Rotate Ship
      glRotatef(theta, attitude.x, attitude.y, attitude.z);

      { // Set light to blue
        GLfloat fDiffLight[] =  { 0.0f, 0.0f, 1.0f };
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      glCallList(Global.dlShip);

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
      glCallList(Global.dlShip);

    glPopMatrix();


    //Draw sphere at origin
    glPushMatrix();
      glEnable(GL_TEXTURE_2D);
      glDisable(GL_CULL_FACE);

      //Move to ref position
      glTranslatef(100 , 100, 100);

      //Rotate Ship
      //glRotatef(theta, dir.x, dir.y, dir.z);
      {
        GLfloat fDiffLight[] =  { 1.0f, 1.0f, 1.0f };  // Whiteness!!
        glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);
      }
      //gluSphere(Global.quadratic,2e1,32,32);\
      //glBindTexture(GL_TEXTURE_2D, Global.planet_tex);
      glCallList(Global.planet_mdl);

      glEnable(GL_CULL_FACE);

    glPopMatrix();

    // Do the buffer Swap
    SDL_GL_SwapBuffers();
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
        gluPerspective(Global.fov, fAspect, 1.0f, 50000000000.0f);

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

    cout << "Initializing OpenGL (Part 1)...";

    /* Set the minimum requirements for the OpenGL window */
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 5 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    if( SDL_SetVideoMode( WIDTH, HEIGHT, video->vfmt->BitsPerPixel, SDL_OPENGL |  SDL_RESIZABLE) == 0 ) {
        fprintf(stderr,
                "Couldn't set video mode: %s\n", SDL_GetError());
        exit(1);
    } else cout << "Sucess!" << endl;

}


static void setup_opengl()
{
    cout << "Initializing OpenGL (Part 2)...";

    ChangeSize(WIDTH, HEIGHT);

    // Black background
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f );

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);

    // Set up lighting
    GLfloat fAmbLight[] =   { 0.00f, 0.00f, 0.00f };
    GLfloat fDiffLight[] =  { 0.0f, 0.0f, 1.0f };  // BLUE!!
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glLightfv(GL_LIGHT0, GL_AMBIENT, fAmbLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, fDiffLight);

    // Set up quadratics
    Global.quadratic = gluNewQuadric();
    gluQuadricNormals(Global.quadratic, GLU_SMOOTH);  //Create Smooth Normals

    cout << "YAY!" << endl;
}

static void process_inputs()
{
    // Get Global State
    Cartesian_Vector &position = Global.ship.location;
    Cartesian_Vector &velocity = Global.ship.velocity;
    Cartesian_Vector &thrust   = Global.ship.force;
    Cartesian_Vector &accel    = Global.ship.acceleration;
    Quaternion       &attitude = Global.ship.attitude;

    unsigned short   &joy_null = Global.joy_null;
    short            &joy_max  = Global.joy_max;
    short            &joy_min  = Global.joy_min;

    Joy_Axis_Map     &joy_pitch    = Global.axis_pitch;
    Joy_Axis_Map     &joy_roll     = Global.axis_roll;
    Joy_Axis_Map     &joy_yaw      = Global.axis_yaw;
    Joy_Axis_Map     &joy_throttle = Global.axis_throttle;



    // Initialize Context Data
    GLfloat yRot     = 0.0f;
    GLfloat xRot     = 0.0f;
    GLfloat zRot     = 0.0f;
    GLfloat throttle = 0.0f;
    accel.Zeroize();
    thrust.Zeroize();

    float scaler = 1.0f / (float(joy_max - joy_null));

    // Get Inputs
    yRot     = float(joystick_axis_norm (SDL_JoystickGetAxis(
                  Global.joystick[joy_roll.joystick],     joy_roll.axis),     joy_null) * scaler);
    xRot     = float(joystick_axis_norm (SDL_JoystickGetAxis(
                  Global.joystick[joy_pitch.joystick],    joy_pitch.axis),    joy_null) * scaler);
    zRot     = -float(joystick_axis_norm(SDL_JoystickGetAxis(
                  Global.joystick[joy_yaw.joystick],      joy_yaw.axis),      joy_null) * scaler);
    throttle = -float(joystick_axis_norm(SDL_JoystickGetAxis(
                  Global.joystick[joy_throttle.joystick], joy_throttle.axis), joy_null) * scaler);


    if (SDL_JoystickGetButton(Global.joystick[0], 15)) thrust.x += .001;
    if (SDL_JoystickGetButton(Global.joystick[0], 17)) thrust.x -= .001;

    if (SDL_JoystickGetButton(Global.joystick[0], 14)) thrust.z += .001;
    if (SDL_JoystickGetButton(Global.joystick[0], 16)) thrust.z -= .001;

    if (SDL_JoystickGetButton(Global.joystick[0], 0)) {
        velocity.Zeroize();
        };

    if (SDL_JoystickGetButton(Global.joystick[0], 7)) {
        Global.ship.attitude.w = 1;
        Global.ship.attitude.x = 0;
        Global.ship.attitude.y = 0;
        Global.ship.attitude.z = 0;
        position.Zeroize();
        velocity.Zeroize();
        };


    //Ship Rotation
    TORAD(xRot);
    TORAD(yRot);
    TORAD(zRot);

    Euler eul(xRot, yRot, zRot);
    Quaternion quat(eul);

    attitude *= quat;
    attitude.Normalize();


    //Apply throttle state to thrust vector
    if (throttle != 0) {

        thrust.y = throttle/1000;
        }

    // Rotate trust vector to match ship orientation
    accel = QVRotate(attitude, thrust);

    // Calculate new velocity from Acceleration vectors
    velocity += accel;

    // Calculate new position based on velocity
    position += velocity;

}



static void main_loop()
{
    SDL_Event event;
    Global.next_time = SDL_GetTicks() + TICK_INTERVAL;

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
//               pitch += event.motion.yrel;
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

       //print_vector("Location: ", Global.ship.location);
       //print_vector("Velocity: ", Global.ship.velocity);
       //print_vector("Accel   : ", Global.ship.acceleration);

    // Send object to server
    //net_send_telemetry();

    /* update the screen */
    RenderScene();
    SDL_Delay(time_left());
    Global.next_time += TICK_INTERVAL;
    }
}


int main(int argc, char* argv[])
{
    SDL_Thread *net_thread;


    setup_sdl();

    setup_opengl();

    setup_joystick();

    net_test();

    load_models();

      load_skybox();

      net_thread = SDL_CreateThread((int (*)(void*))net_recieve_thread, NULL);
      if ( net_thread == NULL ) {
        fprintf(stderr, "Unable to create thread: %s\n", SDL_GetError());
        return 0;
      }

      main_loop();

    free_models();
    return EXIT_SUCCESS;
}
