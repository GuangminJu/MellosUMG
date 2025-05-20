// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "MUserWidgetStringType.generated.h"

/**
 * 
 */
UCLASS()
class MELLOSUMG_API UMUserWidgetStringType : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	void SetValue(FString InValue);

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	FString GetValue() const;

	virtual void OnSetProperty(FProperty* InProperty) override;

public:
	// If no property is set, use this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUserWidget")
	FString Value;

private:
	FStrProperty* StringProperty;
};
