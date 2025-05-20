// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicType/MUserWidgetFloatType.h"

bool UMUserWidgetFloatType::IsPropertySupported(const FProperty* InProperty) const
{
	return InProperty->IsA<FFloatProperty>();	
}

void UMUserWidgetFloatType::SetValue(float InValue)
{
	Value = InValue;

	if (FloatProperty)
	{
		FloatProperty->SetPropertyValue_InContainer(Memory, Value);
	}
}

float UMUserWidgetFloatType::GetValue() const
{
	if (FloatProperty)
	{
		return FloatProperty->GetPropertyValue_InContainer(Memory);
	}

	return Value;
}

void UMUserWidgetFloatType::OnSetProperty(FProperty* InProperty)
{
	FloatProperty = CastField<FFloatProperty>(InProperty);
}
