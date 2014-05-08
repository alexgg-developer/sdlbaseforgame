#include "cGame.hpp"
#include "cTexture.hpp"
#include "vec3.hpp"
#include "cAnimation2D.hpp"
#include "cText.hpp"
#include "TypesDefined.hpp"
#include "SDL_ttf.h"
#include "SDL_mixer.h"
#include "cMusic.hpp"
#include "cSound.hpp"
#include <sstream>

Game::Game()
{}

int Game::init()
{
  int error = initSDL();
  if(error == 0) error = initGLEW();
  if(error == 0) error = initGL();

  return error;
}

int Game::initSDL()
{
  int error = 0;

  if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO ) < 0 )
  {
    std::cout << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
    error = 1;
  }

  error = mWindow.init();

  if(error == 0) {
    if( TTF_Init() == -1 ) {
        std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << std::endl;
        error = 4;
    }
    if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 ) {
       std::cout << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError();
       error = 6;
    }
  }

  return error;
}

int Game::initGLEW()
{
  int error = 0;
  glewExperimental = GL_TRUE;
  GLenum glewError = glewInit();
  if( glewError != GLEW_OK ) {
    std::cout << "Error initializing GLEW! " << glewGetErrorString( glewError ) << std::endl;
    error = 10;
  }

  return error;
}

int Game::initGL()
{
  int error = 0;
  if(!mRenderer.initGL()) {
    error = 20;
  }
  return error;
}

int Game::quit()
{
  mWindow.free();
  Mix_Quit();
  TTF_Quit();
  IMG_Quit();
  SDL_Quit();

  return 0;
}

int Game::main()
{
  int error = init();

  if(!error) {
    uint frame = 0;
    Text textTime;
    /*Text textLoopTime;
    textTime.loadFont("../resources/fonts/lazy.ttf", 28);
    textLoopTime.loadFont("../resources/fonts/lazy.ttf", 28);
    SDL_Color textColor = { 255, 0, 0, 255 };
    textTime.loadText("Such a text, much beauty HIGH", textColor, LOW );
    textTime.setPosition(vec3(0, 0));
    textLoopTime.loadText("Such a text, much beauty HIGH", textColor, HIGH );
    textLoopTime.setPosition(vec3(0, textTime.mHeight));*/
    Texture texture;
    if (!texture.load("../resources/img/nehe.png")) {
        std::cerr << "ERROR! shiet tex" << std::endl;
    }


    Timer timer;
    timer.start();
    mTimer.start();
    while(!mInput.check(Input::KESC)) {
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        if(event.type == SDL_WINDOWEVENT) {
          mWindow.handleEvent(event);
          if(mWindow.mMinimized) {
            mTimer.pause();
          }
          else {
            mTimer.resume();
          }
        }
        else {
          mInput.readWithScanCode(event);
        }
      }
      if(!mWindow.mMinimized) {        
        glClearColor( 0.f, 0.f, 0.f, 1.f );
        glClear( GL_COLOR_BUFFER_BIT );
        //textTime.draw();
        mRenderer.render();
        //texture.draw();
        SDL_GL_SwapWindow( mWindow.mWindow );
      }
    }

    texture.free();
    textTime.free();
    //textLoopTime.free();
    error = quit();
  }
  else quit();

  return error;
}

