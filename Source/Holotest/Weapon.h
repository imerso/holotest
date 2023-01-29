// Basic weapon projectile
//
// Made by Vander R. N. Dias for Hologate test.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Weapon.generated.h"

UCLASS()
class HOLOTEST_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Fire
	void Fire(const FVector& Direction);

	// Weapon mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMeshComponent;

	// Collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	// Movement
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* MoveComponent;
};
