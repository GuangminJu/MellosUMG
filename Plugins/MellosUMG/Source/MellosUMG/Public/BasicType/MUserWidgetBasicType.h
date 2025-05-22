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
	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;

	virtual bool IsPropertySupported(const FProperty* InProperty) const
	{
		return false;
	}

	virtual TArray<FProperty*> GetProperties()
	{
		return {};
	}

	void SetProperty(FProperty* InProperty)
	{
		Property = InProperty;
		PropertyDisplayName = InProperty->GetDisplayNameText().ToString();
		OnSetProperty(Property);
	}

	void SetMemory(void* InMemory)
	{
		Memory = InMemory;
	}

	void* GetMemory() const
	{
		return Memory;
	}

	virtual void OnSetProperty(FProperty* InProperty)
	{
	}

	UFUNCTION(BlueprintPure)
	FText GetPropertyName() const
	{
		if (!Property)
		{
			return FText::FromString(PropertyDisplayName);
		}

		return Property->GetDisplayNameText();
	}

	bool TryInitFromParentWidget();

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
protected:
	UFUNCTION()
	TArray<FString> GetPropertyNameOptions() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MUserWidgetBasicType",
		meta = (GetOptions="GetPropertyNameOptions"), Setter="SetPropertyName")
	FString PropertyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MUserWidgetBasicType")
	FString PropertyDisplayName;

	UFUNCTION()
	void SetPropertyName(const FString& InPropertyName);

	UPROPERTY()
	UMUserWidgetBasicType* ParentMWidget = nullptr;

	FProperty* Property = nullptr;
	void* Memory = nullptr;
};
