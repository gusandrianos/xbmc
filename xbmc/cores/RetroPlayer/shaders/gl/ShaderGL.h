/*
 *  Copyright (C) 2019 Team Kodi
 *  This file is part of Kodi - https://kodi.tv
 *
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  See LICENSES/README.md for more information.
 */

#pragma once

#include "ShaderTextureGL.h"
#include "cores/RetroPlayer/shaders/IShader.h"
#include "rendering/gl/GLShader.h"
#include "guilib/TextureGL.h"

#include <stdint.h>

namespace KODI
{
namespace RETRO
{

class CRenderContext;

}

namespace SHADER
{
class CShaderGL : public CGLShader, public IShader
{
public:
  CShaderGL(RETRO::CRenderContext &context);
//  ~CShaderGL() override;

  // implementation of IShader
  bool Create(const std::string& shaderSource, const std::string& shaderPath, ShaderParameterMap shaderParameters,
         IShaderSampler* sampler, ShaderLutVec luts, float2 viewPortSize, unsigned frameCountMod = 0) override;
  void Render(IShaderTexture* source, IShaderTexture* target) override;
  void SetSizes(const float2& prevSize, const float2& nextSize) override;
  void PrepareParameters(CPoint dest[4], bool isLastPass, uint64_t frameCount) override;
//  CD3DEffect& GetEffect();
  void UpdateMVP() override;
  bool CreateInputBuffer() override;
  void UpdateInputBuffer(uint64_t frameCount);

  // expose these from CWinShader
//  bool CreateVertexBuffer(unsigned vertCount, unsigned vertSize) override;

protected:
//  void SetShaderParameters(CD3DTexture& sourceTexture);

private:
  struct cbInput
  {
    float2 video_size;
    float2 texture_size;
    float2 output_size;
    float frame_count;
    float frame_direction;
  };

  // Currently loaded shader's source code
  std::string m_shaderSource;

  // Currently loaded shader's relative path
  std::string m_shaderPath;

  // Array of shader parameters
  ShaderParameterMap m_shaderParameters;


  // Look-up textures that the shader uses
  ShaderLutVec m_luts; // todo: back to DX maybe

  // Resolution of the input of the shader
  float2 m_inputSize;

  // Resolution of the output of the shader
  float2 m_outputSize;

  // Resolution of the viewport/window
  float2 m_viewportSize;

  // Resolution of the texture that holds the input
  //float2 m_textureSize;

  // Holds the data bount to the input cbuffer (cbInput here)
//  ID3D11Buffer* m_pInputBuffer = nullptr;

  // Projection matrix
//  XMFLOAT4X4 m_MVP;

  // Value to modulo (%) frame count with
  // Unused if 0
  unsigned m_frameCountMod = 0;

private:
  cbInput GetInputData(uint64_t frameCount = 0);

  // Construction parameters
  RETRO::CRenderContext &m_context;
};
}
}
