
#include "BasicType/MUserWidgetIntType.h"

bool UMUserWidgetIntType::IsPropertySupported(const FProperty* InProperty) const
{
	return InProperty->IsA(FIntProperty::StaticClass());
}

void UMUserWidgetIntType::SetValue(int32 InValue)
{
	Value = InValue;

	if (IntProperty)
	{
		IntProperty->SetPropertyValue_InContainer(GetMemory(), Value);
	}
}

int32 UMUserWidgetIntType::GetValue() const
{
	if (IntProperty)
	{
		return IntProperty->GetPropertyValue_InContainer(GetMemory());
	}

	return Value;
}

void UMUserWidgetIntType::OnSetProperty(FProperty* InProperty)
{
	IntProperty = CastField<FIntProperty>(InProperty);
}

bool UMUserWidgetIntType::GetClampedValue(int32& OutMin, int32& OutMax) const
{
	if (IntProperty)
	{
		const FString* ClampMin = IntProperty->FindMetaData(TEXT("ClampMin"));
		const FString* ClampMax = IntProperty->FindMetaData(TEXT("ClampMax"));
		if (ClampMin)
		{
			OutMin = FCString::Atoi(**ClampMin);
		}
		if (ClampMax)
		{
			OutMax = FCString::Atoi(**ClampMax);
		}
			
		return ClampMin && ClampMax;
	}
		
	return false;
}
