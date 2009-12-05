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
#pragma once

#include "ModelEffect.h"
#include "TextureCache.h"
#include "EffectCache.h"
#include "EffectParameterMap.h"

namespace SlimMesh
{
	ref class Model;

	namespace Rendering
	{
		public ref class ModelRenderer9 : System::IDisposable
		{
		private:
			System::Collections::Generic::Dictionary<VertexLayout^, SlimDX::Direct3D9::VertexDeclaration^>^ declarations;
			SlimDX::Direct3D9::Device^ device;

			void Construct(SlimDX::Direct3D9::Device^ device, ModelEffect<SlimDX::Direct3D9::Effect^>^ defaultEffect);

		public:
			ModelRenderer9(SlimDX::Direct3D9::Device^ device);
			ModelRenderer9(SlimDX::Direct3D9::Device^ device, ModelEffect<SlimDX::Direct3D9::Effect^>^ defaultEffect);
			~ModelRenderer9();

			void Render(Model^ model);
			void Render(Model^ model, SlimDX::Direct3D9::Effect^ effect);
			void Render(Model^ model, EffectParameterMap^ parameterMap);

			void RenderDefault(Model^ model);
			void RenderDefault(Model^ model, SlimDX::Matrix worldViewProjection);

			void Lost();
			void Reset();

			property TextureCache<SlimDX::Direct3D9::Texture^>^ TextureCache;
			property EffectCache<SlimDX::Direct3D9::Effect^>^ EffectCache;
			property ModelEffect<SlimDX::Direct3D9::Effect^>^ DefaultEffect;
			property bool PreserveState;

			property SlimDX::Direct3D9::Device^ Device
			{
				SlimDX::Direct3D9::Device^ get() { return device; }
			}

			property System::Collections::Generic::IEnumerable<SlimDX::Direct3D9::VertexDeclaration^>^ VertexDeclarations
			{
				System::Collections::Generic::IEnumerable<SlimDX::Direct3D9::VertexDeclaration^>^ get() { return declarations->Values; }
			}
		};
	}
}