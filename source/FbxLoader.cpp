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
#include "FbxLoader.h"
#include "ModelLoadException.h"

using std::tr1::shared_ptr;
using namespace System;
using namespace System::IO;
using namespace System::Text;

namespace SlimMesh
{
	FbxLoader::FbxLoader()
	{
	}

	Model^ FbxLoader::LoadModel(String^ fileName)
	{
		if (!File::Exists(fileName))
			throw gcnew FileNotFoundException(fileName);

		shared_ptr<KFbxSdkManager> manager(KFbxSdkManager::Create(), destroyer<KFbxSdkManager>());
		if (!manager)
			throw gcnew ModelLoadException("Could not initialize FBX loader.");

		manager->LoadPluginsDirectory(KFbxGetApplicationDirectory().Buffer(), "dll");

		KFbxScene *scene = KFbxScene::Create(manager.get(), "");
		KFbxImporter *importer = KFbxImporter::Create(manager.get(), "");

		int format = -1;
		pin_ptr<unsigned char> pinnedName = &Encoding::ASCII->GetBytes(fileName)[0];
		const char *pFileName = reinterpret_cast<const char*>(pinnedName);

		if (!manager->GetIOPluginRegistry()->DetectFileFormat(pFileName, format))
			format = manager->GetIOPluginRegistry()->GetNativeReaderFormat();

		if (!importer->Initialize(pFileName, format))
			throw gcnew ModelLoadException("Could not initialize FBX file: " + gcnew String(importer->GetLastErrorString()));

		if (!importer->Import(scene))
			throw gcnew ModelLoadException("Could not import FBX scene: " + gcnew String(importer->GetLastErrorString()));

		NormalizeScene(scene);
		ProcessNode(scene->GetRootNode());

		return nullptr;
	}

	void FbxLoader::NormalizeScene(KFbxScene *scene)
	{
		KFbxAxisSystem axis(KFbxAxisSystem::YAxis, KFbxAxisSystem::ParityOdd, KFbxAxisSystem::RightHanded);
		if (axis != scene->GetGlobalSettings().GetAxisSystem())
			axis.ConvertScene(scene);

		if (scene->GetGlobalSettings().GetSystemUnit().GetScaleFactor() != 1.0)
			KFbxSystemUnit(1.0).ConvertScene(scene);
	}

	void FbxLoader::ProcessNode(KFbxNode *node)
	{
	}

	Version^ FbxLoader::FileVersion::get()
	{
		int major, minor, revision;
		KFbxSdkManager::GetFileFormatVersion(major, minor, revision);

		return gcnew Version(major, minor, revision);
	}
}