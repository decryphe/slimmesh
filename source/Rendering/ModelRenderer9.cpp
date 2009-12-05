/*
* Copyright (c) 2007-2009 SlimDX Group
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/
#include "stdafx.h"
#include "../Model.h"
#include "../VertexLayout.h"

#include "ModelRenderer9.h"

using namespace SlimDX;
using namespace SlimDX::Direct3D9;
using namespace System;
using namespace System::Collections::Generic;

static char DefaultEffectSource[] = "asdf";

namespace SlimMesh
{
namespace Rendering
{
	void ModelRenderer9::Construct(SlimDX::Direct3D9::Device^ device, ModelEffect<Effect^>^ defaultEffect)
	{
		this->device = device;

		declarations = gcnew Dictionary<VertexLayout^, VertexDeclaration^>();
		TextureCache = gcnew SlimMesh::Rendering::TextureCache<Texture^>();
		EffectCache = gcnew SlimMesh::Rendering::EffectCache<Effect^>();
		DefaultEffect = defaultEffect;
		PreserveState = true;
	}

	ModelRenderer9::ModelRenderer9(SlimDX::Direct3D9::Device^ device)
	{
		Effect^ effect = Effect::FromString(device, gcnew String(DefaultEffectSource), ShaderFlags::None);

		Construct(device, gcnew ModelEffect<Effect^>("default", effect, nullptr));
	}

	ModelRenderer9::ModelRenderer9(SlimDX::Direct3D9::Device^ device, ModelEffect<Effect^>^ defaultEffect)
	{
		Construct(device, defaultEffect);
	}

	ModelRenderer9::~ModelRenderer9()
	{
		for each (VertexDeclaration^ declaration in VertexDeclarations)
		{
			if (!declaration->Disposed)
				delete declaration;
		}

		for each (Texture^ texture in TextureCache->Values)
		{
			if (!texture->Disposed)
				delete texture;
		}

		for each (ModelEffect<Effect^>^ effect in EffectCache)
		{
			if (!effect->Effect->Disposed)
				delete effect->Effect;
		}

		if (!DefaultEffect->Effect->Disposed)
			delete DefaultEffect->Effect;
	}

	void ModelRenderer9::Lost()
	{
		for each (ModelEffect<Effect^>^ effect in EffectCache)
			effect->Effect->OnLostDevice();

		DefaultEffect->Effect->OnLostDevice();
	}

	void ModelRenderer9::Reset()
	{
		for each (ModelEffect<Effect^>^ effect in EffectCache)
			effect->Effect->OnResetDevice();

		DefaultEffect->Effect->OnResetDevice();
	}

	void ModelRenderer9::RenderDefault(Model^ model, Matrix worldViewProjection)
	{
		DefaultEffect->Effect->SetValue("WorldViewProjection", worldViewProjection);
		Render(model, DefaultEffect->Effect);
	}

	void ModelRenderer9::RenderDefault(Model^ model)
	{
		Render(model, DefaultEffect->Effect);
	}

	void ModelRenderer9::Render(Model^ model)
	{
	}

	void ModelRenderer9::Render(Model^ model, Effect^ effect)
	{
	}

	void ModelRenderer9::Render(Model^ model, EffectParameterMap^ parameterMap)
	{
	}
}
}