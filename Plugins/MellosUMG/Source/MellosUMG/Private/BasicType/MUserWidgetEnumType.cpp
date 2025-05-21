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

	uint8 IndexFromEnumName = GetIndexFromEnumName(Value);

	if (Property->HasSetter())
	{
		Property->CallSetter(GetMemory(), &IndexFromEnumName);
	}
	else if (uint8* ContainerPtrToValuePtr = Property->ContainerPtrToValuePtr<uint8>(GetMemory()))
	{
		*ContainerPtrToValuePtr = IndexFromEnumName;

		uint8 EnumIndex = *ContainerPtrToValuePtr;
		ensureMsgf(EnumNames.Num() > EnumIndex, TEXT("Enum index out of range: %d"),
		           *ContainerPtrToValuePtr);
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
		uint8 Index;
		Property->CallGetter(GetMemory(), &Index);
		return EnumNames[Index];
	}

	if (uint8* ContainerPtrToValuePtr = Property->ContainerPtrToValuePtr<uint8>(GetMemory()))
	{
		uint8 EnumIndex = *ContainerPtrToValuePtr;
		if (EnumIndex >= EnumNames.Num())
		{
			UE_LOG(LogTemp, Warning, TEXT("Enum index out of range: %d"), EnumIndex);
			return FString();
		}
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
			FString Name = Enum->GetMetaData(TEXT("DisplayName"), i);
			if (!Name.IsEmpty())
			{
				EnumNames.Add(Name);
			}
		}
	}
}

bool UMUserWidgetEnumType::GetClampedIndex(int32& OutMin, int32& OutMax) const
{
	if (Enum.IsValid())
	{
		const FString* ClampMin = Enum->FindMetaData(TEXT("ClampMin"));
		const FString* ClampMax = Enum->FindMetaData(TEXT("ClampMax"));
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

uint8 UMUserWidgetEnumType::GetIndexFromEnumName(const FString& EnumName) const
{
	return EnumNames.IndexOfByPredicate([&](const FString& Name)
	{
		return Name.Equals(EnumName, ESearchCase::IgnoreCase);
	});
}
