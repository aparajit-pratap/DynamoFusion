// DynamoManagedWrapper.h

#pragma once

//class DynamoManagedWrapperPrivate;

class DynamoManagedWrapper
{

public:
	static DynamoManagedWrapper* GetInstance();
	
	void LoadDynamo(const char* asm_location);
	void CreateSelectionNode();
	
//private:
//	DynamoManagedWrapperPrivate* dynamoAPI;
//	static DynamoManagedWrapper* wrapper;
//
//	DynamoManagedWrapper();
};


