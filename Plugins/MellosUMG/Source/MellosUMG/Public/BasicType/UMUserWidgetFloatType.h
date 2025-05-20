// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "UMUserWidgetFloatType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetFloatType : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual FFieldClass* GetSupportedFieldClass() const override;

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	void SetValue(float InValue);

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	float GetValue() const;

	virtual void OnSetProperty(FProperty* InProperty) override;

public:
	// If no property is set, use this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUserWidget")
	float Value;

private:
	FFloatProperty* FloatProperty;
};
