// Basic weapon projectile
//
// Made by Vander R. N. Dias for Hologate test.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include <Runtime/Engine/Classes/Sound/SoundCue.h>
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

	// Spawn explosion
	void ExplosionAtPos(const FVector& Pos);

	// Server Fire RPC
	UFUNCTION(NetMulticast, Reliable)
	void ServerExplosion(const FVector& Pos);
	void ServerExplosion_Implementation(const FVector& Pos);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Fire
	void Fire(const FVector& Direction);

	// Weapon mesh
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	UStaticMeshComponent* WeaponMeshComponent;

	// Explosion sound
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	USoundBase* ExplosionAudio;

	// Explosion effect
	// Weapon class
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AActor> ExplosionClass;

	// Collision component
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon")
	USphereComponent* CollisionComponent;

	// Movement
	UPROPERTY(VisibleAnywhere, Category = "Movement")
	UProjectileMovementComponent* MoveComponent;

	// Collision detection
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
};
