

/**
 * @author Allan Deutsch
 * @version 0.5
 * @copyright Copyright (C) Allan Deutsch & Jeff Uong. All rights reserved.
 * 
 */

#pragma once
#include <memory>
namespace AlJeEngine
{

  typedef unsigned mask;
#pragma region metadataEnums
  enum EnumeratedComponent
  {
    EC_Transform      =  0,
    EC_Drawable       =  1,
    EC_RigidBody      =  2,
    EC_Sprite         =  3,
    EC_Collider       =  4,
    EC_Camera         =  5,
    
    // This will correspond to the maximum number of components that exist.
    EC_Max,

    // This is to be used for systems that do not wish to receive any objects automatically.
    EC_NOOBJECTS = -1
  };

  enum BitfieldComponent
  {
    MC_Alive           = 1,
    MC_Transform       = 1 <<  1,
    MC_Drawable        = 1 <<  2,
    MC_RigidBody       = 1 <<  3,
    MC_Sprite          = 1 <<  4,
    MC_Collider        = 1 <<  5,
    MC_Camera          = 1 <<  6,

  };
#pragma endregion


  struct Component
  {
    Component(EnumeratedComponent type, BitfieldComponent mask) : _type(type), _mask(mask) {}

    // These are const because once the component is created, it should never change types.
    const EnumeratedComponent _type;
    const BitfieldComponent   _mask;

    // this makes sure derived component types are deallocated properly.
    virtual ~Component() {}

  private:
    // no default construction is allowed.
    //Component();
  };

  typedef std::shared_ptr<Component> ComponentPtr;

}; // AlJeEngine
