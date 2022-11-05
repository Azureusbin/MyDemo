#pragma once

#include "CoreMinimal.h"

class FMyLogListener : public FOutputDevice
{
public:
	FMyLogListener();
	~FMyLogListener();
	
	virtual void Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category) override;
};
