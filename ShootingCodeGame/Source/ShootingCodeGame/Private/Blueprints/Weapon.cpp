// Fill out your copyright notice in the Description page of Project Settings.


#include "Blueprints/Weapon.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"// DOREPLIFETIME 사용을 위해 추가

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

void AWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AWeapon, m_pOwnChar);
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

	FVector CameraLoc = pPlayer0->PlayerCameraManager->GetCameraLocation();
	FVector CameraForward = pPlayer0->PlayerCameraManager->GetActorForwardVector();
	FVector vStart = (CameraForward * GetFireStartLength()) + CameraLoc;
	FVector vEnd = (CameraForward * 5000.0f) + CameraLoc;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("ReqShoot"));
	ReqShoot(vStart, vEnd);
}

void AWeapon::EventReload_Implementation()
{
	m_pOwnChar->PlayAnimMontage(m_ReloadMontage);
}

void AWeapon::ReqShoot_Implementation(FVector vStart, FVector vEnd)
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, TEXT("ReqShoot_Implementation"));

	FHitResult result;
	FCollisionObjectQueryParams collisionObjParams;
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
	collisionObjParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

	FCollisionQueryParams collisionParams;
	collisionParams.AddIgnoredActor(m_pOwnChar);

	bool isHit = GetWorld()->LineTraceSingleByObjectType(result, vStart, vEnd, collisionObjParams, collisionParams);
	DrawDebugLine(GetWorld(), vStart, vEnd, FColor::Red, false, 5.0f);

	if (false == isHit)
		return;

	ACharacter* pHitChar = Cast<ACharacter>(result.GetActor());
	if (false == IsValid(pHitChar))
		return;

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("HitChar = %s"), *pHitChar->GetName()));

	UGameplayStatics::ApplyDamage(pHitChar, 10.0f, m_pOwnChar->GetController(), this, UDamageType::StaticClass());
}

float AWeapon::GetFireStartLength()
{
	if (false == IsValid(m_pOwnChar))
		return 0.0f;

	USpringArmComponent* pArm = Cast<USpringArmComponent>(m_pOwnChar->GetComponentByClass(USpringArmComponent::StaticClass()));
	if (false == IsValid(pArm))
		return 0.0f;

	return pArm->TargetArmLength + 100;
}

