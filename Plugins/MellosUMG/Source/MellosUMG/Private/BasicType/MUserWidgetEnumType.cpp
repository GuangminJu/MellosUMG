#include "BasicType/MUserWidgetEnumType.h"

#include "Engine/UserDefinedEnum.h"

bool UMUserWidgetEnumType::IsPropertySupported(const FProperty* InProperty) const
{
	if (InProperty->IsA<FEnumProperty>())
	{
		return true;
	}

	if (InProperty->IsA<FByteProperty>())
	{
		const FByteProperty* ByteProperty = static_cast<const FByteProperty*>(InProperty);
		if (ByteProperty->IsEnum())
		{
			return true;
		}
	}

	return false;
}

void UMUserWidgetEnumType::SetValue(FString InValue)
{
	Value = InValue;

	if (Property->HasSetter())
	{
		Property->CallSetter(GetMemory(), &Value);
	}
	else if (uint8* ContainerPtrToValuePtr = Property->ContainerPtrToValuePtr<uint8>(GetMemory()))
	{
		*ContainerPtrToValuePtr = GetIndexFromEnumName(Value);
	}
}

TArray<FString> UMUserWidgetEnumType::GetOptions() const
{
	return EnumNames;
}

FString UMUserWidgetEnumType::GetValue() const
{
	if (Property->HasGetter())
	{
		int32 Index;
		Property->CallGetter(GetMemory(), (void*)&Index);
		return EnumNames[Index];
	}

	if (uint8* ContainerPtrToValuePtr = Property->ContainerPtrToValuePtr<uint8>(GetMemory()))
	{
		int32 EnumIndex = *ContainerPtrToValuePtr;
		return EnumNames[EnumIndex];
	}

	return Value;
}

void UMUserWidgetEnumType::OnSetProperty(FProperty* InProperty)
{
	if (InProperty->IsA<FEnumProperty>())
	{
		FEnumProperty* EnumProperty = CastField<FEnumProperty>(InProperty);
		Enum = EnumProperty->GetEnum();
	}
	else if (InProperty->IsA<FByteProperty>())
	{
		const FByteProperty* ByteProperty = static_cast<const FByteProperty*>(InProperty);
		if (ByteProperty->IsEnum())
		{
			Enum = ByteProperty->Enum;
		}
	}
	else
	{
		ensureMsgf(false, TEXT("Unsupported property type for Enum: %s"), *InProperty->GetName());
	}

	if (UUserDefinedEnum* UserDefinedEnum = Cast<UUserDefinedEnum>(Enum))
	{
		for (auto Name : UserDefinedEnum->DisplayNameMap)
		{
			EnumNames.Add(Name.Value.ToString());
		}
	}
	else
	{
		int32 NumEnums = Enum->NumEnums();
		for (int32 i = 0; i < NumEnums; ++i)
		{
			FString EnumName = Enum->GetNameStringByIndex(i);
			EnumNames.Add(EnumName);
		}
	}
}

int32 UMUserWidgetEnumType::GetIndexFromEnumName(const FString& EnumName) const
{
	return EnumNames.IndexOfByPredicate([&](const FString& Name)
	{
		return Name.Equals(EnumName, ESearchCase::IgnoreCase);
	});
}
