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
#include "ModelLoader.h"
#include "UnknownFormatException.h"

using namespace System;
using namespace System::IO;
using namespace System::Collections::Generic;

namespace SlimMesh
{
	ModelLoader::ModelLoader()
	{
		loaderMap = gcnew Dictionary<String^, IModelLoader^>();
	}

	void ModelLoader::RegisterLoader(IModelLoader^ loader, String^ extension)
	{
		if (!loaderMap->ContainsKey(extension))
			loaderMap->Add(extension, loader);
	}

	void ModelLoader::UnregisterLoader(String^ extension)
	{
		loaderMap->Remove(extension);
	}

	Model^ ModelLoader::LoadModel(System::String^ fileName)
	{
		IModelLoader^ loader = nullptr;
		String^ extension = Path::GetExtension(fileName);

		if (!loaderMap->TryGetValue(extension, loader))
			throw gcnew UnknownFormatException("The extension '" + extension + "' is not a registered model format.");

		return loader->LoadModel(fileName);
	}

	ModelLoader^ ModelLoader::Default::get()
	{
		if (defaultLoader == nullptr)
		{
			defaultLoader = gcnew ModelLoader();
		}

		return defaultLoader;
	}
}