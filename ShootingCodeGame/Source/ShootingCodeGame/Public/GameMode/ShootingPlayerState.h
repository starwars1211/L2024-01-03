// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ShootingPlayerState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDele_UpdateHp, float, CurHp, float, MaxHp);

/**
 * 
 */
UCLASS()
class SHOOTINGCODEGAME_API AShootingPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void AddDamage(float Damage);

public:
	// replicate function
	UFUNCTION()
	void OnRep_CurHp();

public:
	UPROPERTY(ReplicatedUsing = OnRep_CurHp)
	float m_CurHp;

	UPROPERTY(BlueprintAssignable, VisibleAnywhere, BlueprintCallable)
	FDele_UpdateHp m_Dele_UpdateHp;
};
