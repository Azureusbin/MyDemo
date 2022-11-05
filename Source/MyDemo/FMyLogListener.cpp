#include "FMyLogListener.h"

FMyLogListener::FMyLogListener()
{
	check(GLog)
	GLog->AddOutputDevice(this);
}

FMyLogListener::~FMyLogListener()
{
	if (GLog)
	{
		GLog->RemoveOutputDevice(this);
	}
}

void FMyLogListener::Serialize(const TCHAR* V, ELogVerbosity::Type Verbosity, const FName& Category)
{
	// 对日志进行一些处理。。。
}
