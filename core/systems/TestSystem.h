
/**
* @author Allan Deutsch
* @version 0.5
* @copyright Copyright (C) Allan Deutsch & Jeff Uong. All rights reserved.
*
*/

#pragma once
#include <memory>

#include "../engine/headers/System.h"
#include "../components/Components.h"
namespace AlJeEngine
{
  namespace Systems
  {

    class Test : public System
    {
    public:
      Test() : System(std::string("Test System"), ES_Test) {/* the engine calls init manually, which should take the place of most of the constructor. */}

      void Init();

      void Update(float);
      
      void Shutdown();



    };

    typedef std::shared_ptr<Test> TestPtr;

  }; // Systems

}; // AlJeEngine

