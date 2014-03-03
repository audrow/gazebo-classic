/*
 * Copyright (C) 2012-2014 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef _MATERIALGENERATOR_HH_
#define _MATERIALGENERATOR_HH_

#include <OgreMaterial.h>
#include <OgreStringVector.h>
#include <map>

#include <string>

namespace gazebo
{
  namespace rendering
  {
    /// \brief Caching, on-the-fly material generator. This is a class that
    /// automatically generates and stores different permutations of
    /// a material, and its shaders.  It can be used if you have a material
    /// that has lots of slightly different variations, like whether to use
    /// a specular light, skinning, normal mapping and other options. Writing
    /// all these out is a tedious job. Of course it is possible to always use
    /// the material with all features, but that might result in large, slow
    /// shader programs. This class provides an efficient solution to that.
    class GAZEBO_VISIBLE MaterialGenerator
    {
      /// Bitfield used to signify a material permutations
      public: typedef uint32_t Perm;

      /// \briefThe constructor is protected as this base class should never
      /// be constructed as-is. It is meant to be subclassed so that values
      /// can be assigned to the various fields controlling material
      /// generator, and most importantly, the mImpl field.
      protected: MaterialGenerator();

      public: virtual ~MaterialGenerator();

      public: const Ogre::MaterialPtr &GetMaterial(Perm permutation);

      /// \brief Implementation class that takes care of actual generation
      /// or lookup of the various constituent parts (template material,
      /// fragment shader and vertex shader). These methods are only called
      /// once for every permutation, after which the result is stored and
      /// re-used.
      public: class Impl
              {
                public: virtual ~Impl();
                public: virtual Ogre::GpuProgramPtr GenerateVertexShader(
                            Perm _permutation) = 0;
                public: virtual Ogre::GpuProgramPtr GenerateFragmentShader(
                            Perm _permutation) = 0;
                public: virtual Ogre::MaterialPtr GenerateTemplateMaterial(
                            Perm permutation) = 0;
              };


      protected: const Ogre::GpuProgramPtr &GetVertexShader(Perm _perm);
      protected: const Ogre::GpuProgramPtr &GetFragmentShader(Perm _perm);
      protected: const Ogre::MaterialPtr &GetTemplateMaterial(Perm _perm);

      protected: std::string schemeName;

      /// Base name of materials generated by this
      protected: Ogre::String materialBaseName;

      /// Mask of permutation bits that influence vertex shader choice
      protected: Perm vsMask;

      /// Mask of permutation bits that influence fragment shader choice
      protected: Perm fsMask;

      /// Mask of permutation bits that influence template material choice
      protected: Perm matMask;

      /// Generator
      protected: Impl *impl;

      protected: typedef std::map<Perm, Ogre::GpuProgramPtr> ProgramMap;
      protected: typedef std::map<Perm, Ogre::MaterialPtr> MaterialMap;

      protected: ProgramMap vs, fs;
      protected: MaterialMap templateMat, materials;
    };
  }
}
#endif
