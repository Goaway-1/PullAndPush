
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "GameData/CharacterStat.h"
#include "CharacterStatMaterialTable.generated.h"

/** Stat�� ���� Material�� ������ ����ü */
USTRUCT(BlueprintType)
struct FCharacterStatMaterialTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	ECharacterStat TargetStat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	TObjectPtr<class UMaterialInterface> MaterialUI;
};