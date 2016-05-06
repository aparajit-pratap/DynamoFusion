// This is the main DLL file.


#include "stdafx.h"

#include "DynamoManagedWrapper.h"

#using<DynamoSandboxWrapper.dll>

using namespace System;
using namespace DynamoSandboxWrapper;

[STAThreadAttribute]
void DynamoManagedWrapper::LoadDynamo(const char* asm_location)
{
	String^ asmLocation = gcnew String(asm_location);
	DynamoWrapper::LoadDynamo(asmLocation);
}