// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ShootingHUD.h"
#include "Blueprint/UserWidget.h"
#include "GameMode/ShootingPlayerState.h"

void AShootingHUD::BeginPlay()
{
	Super::BeginPlay();

	check(HudWidgetClass);

	HudWidget = CreateWidget<UUserWidget>(GetWorld(), HudWidgetClass);
	HudWidget->AddToViewport();
}

void AShootingHUD::BindMyPlayerState()
{
	APlayerController* pc = GetWorld()->GetFirstPlayerController();

	AShootingPlayerState* ps = Cast<AShootingPlayerState>(pc->PlayerState);

	ps->m_Dele_UpdateHp.AddDynamic(this, &AShootingHUD::OnUpdateMyHp);
}

void AShootingHUD::OnUpdateMyHp(float CurHp, float MaxHp)
{
}
