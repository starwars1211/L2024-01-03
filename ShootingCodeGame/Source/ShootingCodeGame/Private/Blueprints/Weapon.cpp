// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("Weapon");
	WeaponMesh->SetCollisionProfileName("OverlapAllDynamic");
	SetRootComponent(WeaponMesh);

	bReplicates = true;
	SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::EventTrigger_Implementation()
{
	m_pOwnChar->PlayAnimMontage(m_ShootMontage);
}

void AWeapon::EventShoot_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), m_FireEffect, 
		WeaponMesh->GetSocketLocation("muzzle"),
		WeaponMesh->GetSocketRotation("muzzle"),
		FVector(0.1f, 0.1f, 0.1f));

	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), m_SoundBase,
		WeaponMesh->GetSocketLocation("muzzle"));

	APlayerController* pPlayer0 = GetWorld()->GetFirstPlayerController();

	FHitResult result;
	FVector CameraLoc = pPlayer0->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = pPlayer0->PlayerCameraManager->GetActorForwardVector();
	FVector vEnd = (CameraForward * 1000.0f) + CameraLoc;

	FCollisionObjectQueryParams collisionObjParams;
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(m_pOwnChar);

	GetWorld()->LineTraceSingleByObjectType(result, CameraLoc, vEnd, collisionObjParams, collisionParams);
	DrawDebugLine(GetWorld(), CameraLoc, vEnd, FColor::Red, false, 5.0f);
}

void AWeapon::EventReload_Implementation()
{
	m_pOwnChar->PlayAnimMontage(m_ReloadMontage);
}

