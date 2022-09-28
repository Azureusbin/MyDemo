// Fill out your copyright notice in the Description page of Project Settings.


#include "MyAsyncHttpRequest.h"

#include "HttpManager.h"
#include "Navigation/PathFollowingComponent.h"

UMyAsyncHttpRequest* UMyAsyncHttpRequest::HttpRequest(const FString& URL)
{
	UMyAsyncHttpRequest* NewRequest = NewObject<UMyAsyncHttpRequest>();
	NewRequest->SendRequest(URL);
	return NewRequest;
}

void UMyAsyncHttpRequest::OnHttpResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bSuccessed)
{
	if (bSuccessed)
	{
		OnSuccess.Broadcast(Response->GetResponseCode(), Response->GetContentAsString());
	}
	else
	{
		OnFail.Broadcast(-1, TEXT(""));
	}

	RemoveFromRoot();
}

void UMyAsyncHttpRequest::SendRequest(const FString& URL)
{
	AddToRoot();

	FHttpModule& HttpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest> Request = HttpModule.CreateRequest();
	Request->SetURL(URL);
	Request->SetVerb("Get");
	Request->OnProcessRequestComplete().BindUObject(this, &UMyAsyncHttpRequest::OnHttpResponse);
	Request->ProcessRequest();
}
