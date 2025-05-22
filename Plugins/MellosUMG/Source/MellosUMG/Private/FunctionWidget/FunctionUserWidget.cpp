// Fill out your copyright notice in the Description page of Project Settings.


#include "FunctionWidget/FunctionUserWidget.h"

FText UFunctionUserWidget::GetFunctionName() const
{
	return Function ? Function->GetDisplayNameText() : FText::FromString(TEXT("No Function"));
}

void UFunctionUserWidget::Call()
{
	if (!Function || !Object)
	{
		ensureMsgf(false, TEXT("Function or Object is null"));
		return;
	}

	Object->ProcessEvent(Function, nullptr);
}
