/*
 *  Copyright (C) 2019 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#include "ShaderPresetGL.h"
#include "cores/RetroPlayer/shaders/ShaderUtils.h"
#include "cores/RetroPlayer/shaders/gl/ShaderLutGL.h"
#include "cores/RetroPlayer/rendering/RenderContext.h"
#include "cores/RetroPlayer/shaders/ShaderPresetFactory.h"
#include "cores/RetroPlayer/shaders/gl/ShaderGL.h"
#include "ServiceBroker.h"

#include <regex>


using namespace KODI;
using namespace SHADER;


CShaderPresetGL::CShaderPresetGL(RETRO::CRenderContext &context, unsigned int videoWidth, unsigned int videoHeight)
  : m_context(context)
  , m_videoSize(videoWidth, videoHeight)
{
  m_textureSize = CShaderUtils::GetOptimalTextureSize(m_videoSize);

  CRect viewPort;
  m_context.GetViewPort(viewPort);
  m_outputSize = {viewPort.Width(), viewPort.Height()};
}

CShaderPresetGL::~CShaderPresetGL()
{
  DisposeShaders();
  // The gui is going to render after this, so apply the state required
  m_context.ApplyStateBlock();
}

ShaderParameterMap CShaderPresetGL::GetShaderParameters(const std::vector<ShaderParameter> &parameters, const std::string &sourceStr) const
{
  static const std::regex pragmaParamRegex("#pragma parameter ([a-zA-Z_][a-zA-Z0-9_]*)");
  std::smatch matches;

  std::vector<std::string> validParams;
  std::string::const_iterator searchStart(sourceStr.cbegin());
  while (regex_search(searchStart, sourceStr.cend(), matches, pragmaParamRegex))
  {
    validParams.push_back(matches[1].str());
    searchStart += matches.position() + matches.length();
  }

  ShaderParameterMap matchParams;
  for (const auto& match : validParams)   // for each param found in the source code
  {
    for (const auto& parameter : parameters)   // for each param found in the preset file
    {
      if (match == parameter.strId)  // if they match
      {
        // The add-on has already handled parsing and overwriting default
        // parameter values from the preset file. The final value we
        // should use is in the 'current' field.
        matchParams[match] = parameter.current;
        break;
      }
    }
  }

  return matchParams;
}

bool CShaderPresetGL::ReadPresetFile(const std::string &presetPath)
{
  return CServiceBroker::GetGameServices().VideoShaders().LoadPreset(presetPath, *this);
}

void CShaderPresetGL::SetSpeed(double speed)
{
  m_speed = speed;
}

ShaderPassVec &CShaderPresetGL::GetPasses()
{
  return m_passes;
}

bool CShaderPresetGL::RenderUpdate(const CPoint *dest, IShaderTexture *source, IShaderTexture *target)
{
  return false;
}

bool CShaderPresetGL::Update()
{
  return false;
}

void CShaderPresetGL::SetVideoSize(const unsigned videoWidth, const unsigned videoHeight)
{
  m_videoSize = {videoWidth, videoHeight};
  m_textureSize = CShaderUtils::GetOptimalTextureSize(m_videoSize);

}

bool CShaderPresetGL::SetShaderPreset(const std::string &shaderPresetPath)
{
  m_bPresetNeedsUpdate = true;
  m_presetPath = shaderPresetPath;
  return Update();
}

const std::string &CShaderPresetGL::GetShaderPreset() const
{
  return m_presetPath;
}

bool CShaderPresetGL::CreateShaderTextures()
{
  return false;
}

bool CShaderPresetGL::CreateShaders()
{
  auto numPasses = m_passes.size();
  m_textureSize = CShaderUtils::GetOptimalTextureSize(m_videoSize);
  ShaderLutVec passLUTsGL;
  for (unsigned shaderIdx = 0; shaderIdx < numPasses; ++shaderIdx)
  {
    const auto& pass = m_passes[shaderIdx];

    for (unsigned i = 0; i < pass.luts.size(); ++i)
    {
      auto& lutStruct = pass.luts[i];

      ShaderLutPtr passLut(new CShaderLutGL(lutStruct.strId, lutStruct.path));
      if (passLut->Create(m_context, lutStruct))
        passLUTsGL.emplace_back(std::move(passLut));
    }
  }

//  std::unique_ptr<CShaderGL> videoShader(new CShaderGL(m_context));

  return false;
}

bool CShaderPresetGL::CreateSamplers()
{
  return false;
}

bool CShaderPresetGL::CreateLayouts()
{
  return false;
}

bool CShaderPresetGL::CreateBuffers()
{
  return false;
}

void CShaderPresetGL::UpdateViewPort()
{

}

void CShaderPresetGL::UpdateViewPort(CRect viewPort)
{

}

void CShaderPresetGL::UpdateMVPs()
{

}

void CShaderPresetGL::DisposeShaders()
{

}

void CShaderPresetGL::PrepareParameters(const IShaderTexture *texture, const CPoint *dest)
{

}

void CShaderPresetGL::RenderShader(IShader *shader, IShaderTexture *source, IShaderTexture *target) const
{
  CRect newViewPort(0.f, 0.f, target->GetWidth(), target->GetHeight());
  m_context.SetViewPort(newViewPort);
  m_context.SetScissors(newViewPort);

  shader->Render(source, target);
}

bool CShaderPresetGL::HasPathFailed(const std::string &path) const
{
  return false;
}








