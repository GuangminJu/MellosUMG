// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionWidget/FunctionUserWidget.h"

FText UFunctionUserWidget::GetFunctionName() const
{
	if (!FunctionNameOverride.IsEmpty())
	{
		return FunctionNameOverride;
	}

	return Function ? Function->GetDisplayNameText() : FText::FromString(TEXT("No Function"));
}

void UFunctionUserWidget::Call()
{
	PreCall.Broadcast(this);

	if (!Function || !Object)
	{
		return;
	}

	Object->ProcessEvent(Function, nullptr);
}
