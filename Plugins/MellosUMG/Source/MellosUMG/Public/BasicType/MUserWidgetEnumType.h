// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MUserWidgetBasicType.h"
#include "MUserWidgetEnumType.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MELLOSUMG_API UMUserWidgetEnumType : public UMUserWidgetBasicType
{
	GENERATED_BODY()

	virtual bool IsPropertySupported(const FProperty* InProperty) const override;
	virtual void OnSetProperty(FProperty* InProperty) override;

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	void SetValue(FString InValue);

	UFUNCTION(BlueprintCallable, Category = "MUserWidget")
	TArray<FString> GetOptions() const;

	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	FString GetValue() const;
	
	UFUNCTION(BlueprintPure, Category = "MUserWidget")
	bool GetClampedIndex(int32& OutMin, int32& OutMax) const;
public:
	// If no property is set, use this value
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MUserWidget")
	FString Value;

private:
	uint8 GetIndexFromEnumName(const FString& EnumName) const;
	
	TWeakObjectPtr<UEnum> Enum;
	TArray<FString> EnumNames;
};
