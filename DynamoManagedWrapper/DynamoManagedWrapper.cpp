// This is the main DLL file.


#include "stdafx.h"

#include "DynamoManagedWrapper.h"

#using<DynamoSandboxWrapper.dll>

using namespace System;
using namespace DynamoSandboxWrapper;

[STAThreadAttribute]
void DynamoManagedWrapper::LoadDynamo()
{
	String^ asmLocation = 
		gcnew String("C:\\Users\pratapa.ADS\\AppData\\Local\\Autodesk\\webdeploy\\production\\c076e205c02e6f8f9c6eac7820ccca6953c0c53a");
	DynamoWrapper::LoadDynamo(asmLocation);
}