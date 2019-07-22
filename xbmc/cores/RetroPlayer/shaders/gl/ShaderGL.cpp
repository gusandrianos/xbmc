/*
 *  Copyright (C) 2019 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "ShaderGL.h"
#include "ShaderTextureGL.h"
#include "Application.h"
#include "cores/RetroPlayer/rendering/RenderContext.h"
#include "cores/RetroPlayer/rendering/RenderContext.h"
//#include "cores/RetroPlayer/shaders/gl/ShaderTypesGL.h"
#include "cores/RetroPlayer/shaders/IShaderLut.h"
#include "cores/RetroPlayer/shaders/ShaderUtils.h"
#include "rendering/gl/RenderSystemGL.h"
#include "utils/log.h"
#include "utils/URIUtils.h"
#include "system.h"

using namespace KODI;
using namespace SHADER;

CShaderGL::CShaderGL(RETRO::CRenderContext &context) :
        m_context(context)
{
}


bool CShaderGL::Create(const std::string& shaderSource, const std::string& shaderPath, ShaderParameterMap shaderParameters,
        IShaderSampler* sampler, ShaderLutVec luts,
        float2 viewPortSize, unsigned frameCountMod)
{
  if(shaderPath.empty())
  {
    CLog::Log(LOGERROR, "ShaderDX: Can't load empty shader path");
    return false;
  }

  m_shaderSource = shaderSource;
  m_shaderPath = shaderPath;
  m_shaderParameters = shaderParameters;
  m_luts = luts;
  m_viewportSize = viewPortSize;
  m_frameCountMod = frameCountMod;

  std::string defines = "";

  if(!this->LoadShaderSources(shaderSource.c_str(), defines))
  {
    CLog::Log(LOGERROR, "%s: failed to load video shader: %s", __func__, shaderPath.c_str());
    return false;
  }

  return true;
}

void CShaderGL::Render(IShaderTexture *source, IShaderTexture *target) {
  auto* sourceGL = static_cast<CShaderTextureGL*> (source);
}

void CShaderGL::SetSizes(const float2 &prevSize, const float2 &nextSize) {

}

void CShaderGL::PrepareParameters(CPoint *dest, bool isLastPass, uint64_t frameCount) {

}

void CShaderGL::UpdateMVP() {

}

bool CShaderGL::CreateInputBuffer() {
  return false;
}

void CShaderGL::UpdateInputBuffer(uint64_t frameCount) {

}

CShaderGL::cbInput CShaderGL::GetInputData(uint64_t frameCount) {
  return CShaderGL::cbInput();
}
