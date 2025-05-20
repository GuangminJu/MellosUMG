// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MUserWidgetBasicType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetBasicType : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FFieldClass* GetSupportedFieldClass() const
	{
		return nullptr;
	}

	void SetProperty(FProperty* InProperty)
	{
		Property = InProperty;
		OnSetProperty(Property);
	}

	void SetMemory(void* InMemory)
	{
		Memory = InMemory;
	}

	virtual void OnSetProperty(FProperty* InProperty)
	{
		
	}
protected:
	FProperty* Property = nullptr;
	void* Memory = nullptr;
};
