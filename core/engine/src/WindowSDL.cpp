/**
* @author Jeffrey Uong
* @version 0.5
* @copyright Copyright (C) Allan Deutsch & Jeff Uong. All rights reserved.
*
*/

#include "../../engine/headers/WindowSDL.h"
#include "../headers/Engine.h"
#include <iostream> //cout, endl
#include "../../../lib/math/glm/gtc/matrix_transform.hpp"

const int screenWidth = 1280;
const int screenHeight = 720;

namespace AlJeEngine
{
  extern Engine* ENGINE;
  namespace Systems
  {

    /*!
    * @brief Ctor for the SDL window class.
    *
    */
    WindowSDL::WindowSDL() : System(std::string("WindowSDLSystem"), ES_WindowSDL),
                             _fullScreen(false),
                             _window(nullptr),
                             _context(NULL),
                             _width(screenWidth),
                             _height(screenHeight)
    {
    }

    /*!
    * @brief Set all the values that the system needs when setting up SDL.
    *
    */
    void WindowSDL::Init()
    {
      RegisterComponent(MC_NOOBJECTS);

      SDL_Init(SDL_INIT_VIDEO);

      //Telling OpenGL to be compatible as version 4.0
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
      SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

      //Creating the screen and setting position and screen size
      _window = SDL_CreateWindow("AlJeEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        _width, _height, SDL_WINDOW_OPENGL);

      //Creating the context for OpenGL
      _context = SDL_GL_CreateContext(_window);

      //Force GLEW to use modern OpenGL methods if available
      glewExperimental = GL_TRUE;
      glewInit();
    }

    /*!
    * @brief Destruct the context for SDL.
    */
    void WindowSDL::Shutdown(void)
    {
      //Delete the context
      SDL_GL_DeleteContext(_context);
    }

    /*!
    * @brief Gets the mouse trigger during the pressed frame.
    * @return bool if the mouse was triggered or not
    */
    bool WindowSDL::GetMouseTrigger(void)
    {
      return _theMouse.Trigger;
    }

    /*!
    * @brief Gets the mouse release during the pressed frame.
    * @return bool if the mouse was released on click or not
    */
    bool WindowSDL::GetMouseReleased(void)
    {
      return _theMouse.Released;
    }

    glm::vec2 WindowSDL::GetMousePosition(void)
    {
      glm::vec2 currentCameraPos = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(Transform)->position;
      glm::vec2 spaceMousePos = { _theMouse.position.x, _theMouse.position.y };
      return spaceMousePos;
    }

    /*!
    * @brief Gets all the window events that SDL has
    *
    * @param currentEvent The window event SDL is trying to get.
    */
    void WindowSDL::PollWindowEvent(SDL_Event &currentEvent)
    {
      switch (currentEvent.type)
      {
      case SDL_WINDOWEVENT:
        switch (currentEvent.window.event)
        {
        case SDL_WINDOWEVENT_SIZE_CHANGED:
          SDL_GetWindowSize(_window, &_width, &_height);
                                                                                     //Aspect ratio
          glViewport(0, 0, _width, static_cast<GLsizei>(_width / (static_cast<float>(_width) / _height)));
          break;
        case SDL_WINDOWEVENT_CLOSE:
          break;
        case SDL_WINDOWEVENT_MOVED:
          break;
        case SDL_WINDOWEVENT_FOCUS_GAINED:
          break;
        case SDL_WINDOWEVENT_FOCUS_LOST:
          break;
        }
        break;
      case SDL_QUIT:
        ENGINE->Stop();
        break;
      default:
        break;
      }//switch
    }

    /*!
    * @brief Gets all the keyboard events that SDL has
    *
    * @param currentEvent The keyboard event SDL is trying to get.
    */
    void WindowSDL::PollKeyEvent(SDL_Event &currentEvent)
    {
      switch (currentEvent.type)
      {
      case SDL_KEYDOWN:
        //If the key that is down is the escape key
        if (currentEvent.key.keysym.sym == SDLK_ESCAPE)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_BackButton);
        
        if (currentEvent.key.keysym.sym == SDLK_DOWN)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_Down);
        
        if (currentEvent.key.keysym.sym == SDLK_UP)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_Up);
        
        if (currentEvent.key.keysym.sym == SDLK_LEFT)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_Left);
        
        if (currentEvent.key.keysym.sym == SDLK_RIGHT)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_Right);
        
        if (currentEvent.key.keysym.sym == SDLK_F11)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_ToggleFullScreen);
        
        if (currentEvent.key.keysym.sym == SDLK_SPACE)
          ENGINE->SendMsg(nullptr, nullptr, Message::MV_Space);

        break;
      case SDL_KEYUP:
        break;
      }

    }

    /*!
    * @brief Gets all the mouse events that SDL has
    *
    * @param currentEvent The window mouse event SDL is trying to get.
    */
    void WindowSDL::PollMouseEvent(SDL_Event &currentEvent)
    {

      switch (currentEvent.type)
      {
      case SDL_MOUSEMOTION:
      {
        //1 maps to near plane, -1 far plane
        std::pair<int, int> windowDimensions;
        windowDimensions = GetWindowDimensions();
        CameraPtr camera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(Camera);
        glm::vec3 mousescreen(_event.motion.x, _event.motion.y, 1);

        // We have to do this in order to make the bottom left corner 0,0 instead of the top left. 
        // By default the top left corner is 0,0, and increases along the y axis as you move down the window.
        // This flips that so it will work correctly.
        mousescreen.y = windowDimensions.second - mousescreen.y;

        glm::vec4 viewport(0, 0, windowDimensions.first, windowDimensions.second);
        glm::vec3 mousepoint = glm::unProject(mousescreen,  camera->_viewMatrix, camera->_ortho, viewport);
        _theMouse.position.x = mousepoint.x;
        _theMouse.position.y = mousepoint.y;
        //std::cout << "Mouse X: " << _theMouse.position.x << std::endl;
        //std::cout << "Mouse Y: " << _theMouse.position.y << std::endl;
      }
        break;

      case SDL_MOUSEBUTTONDOWN:
        if (currentEvent.button.button == SDL_BUTTON_LEFT)
        {
          if (_theMouse.Pressed == false)
          {
            _theMouse.Trigger = true;
          }
          _theMouse.Released = false;
          _theMouse.Pressed = true;
        }
        break;

      case SDL_MOUSEBUTTONUP:
        if (currentEvent.button.button == SDL_BUTTON_LEFT)
        {
          if (_theMouse.Pressed == true)
          {
            _theMouse.Released = true;
          }
          else
            _theMouse.Released = false;

          _theMouse.Pressed = false;
          _theMouse.Trigger = false;
        }
        break;

      default:
        break;
      }
    }

    /*!
    * @brief Gets all the events that SDL has
    */
    void WindowSDL::PollEvents(void)
    {
      // We only get a single mouse down event, so we can reset triggered to false every frame.
      _theMouse.Trigger = false;

      while(SDL_PollEvent(&_event))
      {
        PollWindowEvent(_event);
        PollKeyEvent(_event);
        PollMouseEvent(_event);
      }
    }

    int WindowSDL::GetWindowHeight(void)
    {
      return _height;
    }

    int WindowSDL::GetWindowWidth(void)
    {
      return _width;
    }

    std::pair<int, int> WindowSDL::GetWindowDimensions(void)
    {
      std::pair<int, int> dimensions(_width, _height);
      SDL_GetWindowSize(_window, &dimensions.first, &dimensions.second);
      return dimensions;
    }

    /*!
    * @brief Updates everything for SDL
    *
    * @param dt time if it needs to be used.
    */
    void WindowSDL::Update(float dt)
    {
      CameraPtr camera = ENGINE->GetActiveSpace()->GetCamera()->GET_COMPONENT(Camera);
      glm::vec2 mousepos= GetMousePosition();
      PollEvents();


    }

    /*!
    * @brief clears the color to start a new frame
    */
    void WindowSDL::newFrame()
    {
       // Clear the color buffer so the screen can be re-drawn.
       glClear(GL_COLOR_BUFFER_BIT);
    }

    /*!
    * @brief Swap the back buffer and front buffer
    */
    void WindowSDL::endFrame()
    {
      // This effectively the end of the frame.
      SDL_GL_SwapWindow(_window);
    }

    void WindowSDL::ToggleFullScreen()
    {
      if (_fullScreen)
      {
        SDL_SetWindowFullscreen(_window, SDL_WINDOW_FULLSCREEN_DESKTOP);
      }
      else
      {
        SDL_SetWindowFullscreen(_window, 0);
      }

      _fullScreen = !_fullScreen;   
    }

    void WindowSDL::SendMsg(EntityPtr, EntityPtr, Message::Message message)
    {
      switch (message)
      {
      case Message::MV_ToggleFullScreen:
        ToggleFullScreen();
        break;
      }

    }

  } // Systems
} // AlJeEngine
