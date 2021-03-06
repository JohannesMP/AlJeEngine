/**
* @author Jeffrey Uong
* @version 0.5
* @copyright Copyright (C) Allan Deutsch & Jeff Uong. All rights reserved.
*
*/
#pragma once
#include <memory>

#include "../../../engine/headers/System.h"
namespace AlJeEngine
{
  namespace Systems
  {
    class Render : public System
    {
    public:
      Render();

      void Init();
      void Update(float);
      void Shutdown();



    private:

    };

    typedef std::shared_ptr<Render> RenderPtr;

  } // Systems

} // AlJeEngine

