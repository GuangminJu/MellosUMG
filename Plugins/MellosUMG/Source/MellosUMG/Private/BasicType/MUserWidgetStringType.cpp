// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicType/MUserWidgetStringType.h"

bool UMUserWidgetStringType::IsPropertySupported(const FProperty* InProperty) const
{
	return InProperty->IsA<FStrProperty>();
}

void UMUserWidgetStringType::SetValue(FString InValue)
{
	Value = InValue;
	
	if (StringProperty)
	{
		StringProperty->SetPropertyValue_InContainer(Memory, Value);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("StringProperty is null"));
	}
}

FString UMUserWidgetStringType::GetValue() const
{
	if (StringProperty)
	{
		return StringProperty->GetPropertyValue_InContainer(Memory);
	}

	return Value;
}

void UMUserWidgetStringType::OnSetProperty(FProperty* InProperty)
{
	Super::OnSetProperty(InProperty);

	StringProperty = CastField<FStrProperty>(InProperty);
}
