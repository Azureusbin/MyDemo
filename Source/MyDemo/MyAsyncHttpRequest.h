// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "GenericPlatform/HttpRequestImpl.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "MyAsyncHttpRequest.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHttpResponseDelegate, int32, Code, FString, Data);

/**
 * 
 */
UCLASS(meta=(HideThen=true))
class MYDEMO_API UMyAsyncHttpRequest : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly = true))
	static UMyAsyncHttpRequest* HttpRequest(const FString& URL);

	UPROPERTY(BlueprintAssignable)
	FHttpResponseDelegate OnSuccess;

	UPROPERTY(BlueprintAssignable)
	FHttpResponseDelegate OnFail;

private:
	void OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessed);

	void SendRequest(const FString& URL);
};
