// This is the main DLL file.

#include "stdafx.h"

#using<DynamoSandboxWrapper.dll>


using namespace System;
using namespace DynamoSandboxWrapper;

#include <msclr\auto_gcroot.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DYNAMO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DYNAMO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DYNAMO_EXPORTS
#define DYNAMO_API __declspec(dllexport)
#else
#define DYNAMO_API __declspec(dllimport)
#endif


class DynamoManagedWrapperPrivate
{
public: msclr::auto_gcroot<DynamoWrapper^> dynamoWrapper;
};

class DYNAMO_API DynamoManagedWrapper
{
private:
	DynamoManagedWrapper()
	{
		dynamoAPI = new DynamoManagedWrapperPrivate();
		dynamoAPI->dynamoWrapper = gcnew DynamoWrapper();
	}

	static DynamoManagedWrapper* wrapper;
	DynamoManagedWrapperPrivate* dynamoAPI;

public: 
	static DynamoManagedWrapper* GetInstance()
	{
		if (wrapper == nullptr)
		{
			wrapper = new DynamoManagedWrapper();
		}
		return wrapper;
	}

	[STAThreadAttribute]
	void LoadDynamo(const char* asm_location)
	{
		String^ asmLocation = gcnew String(asm_location);
		dynamoAPI->dynamoWrapper->LoadDynamo(asmLocation);
	}

	void CreateSelectionNode()
	{
		dynamoAPI->dynamoWrapper->CreateSelectionNode();
	}
};


DynamoManagedWrapper* DynamoManagedWrapper::wrapper = nullptr;