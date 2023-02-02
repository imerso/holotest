// Basic weapon Weapon
//
// Made by Vander R. N. Dias for Hologate test.


#include "Weapon.h"
#include <Holotest/PlayerChar.h>
#include <Holotest/HolotestGameModeBase.h>
#include <Holotest/HolotestPlayerState.h>
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		// Create the root component
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

		// Simple collision detection using sphere
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
		CollisionComponent->InitSphereRadius(10);
		CollisionComponent->OnComponentHit.AddDynamic(this, &AWeapon::OnHit);
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("Projectile"));
		RootComponent = CollisionComponent;

		// Use this component to drive this Weapon's movement.
		MoveComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("WeaponMovementComponent"));
		MoveComponent->SetUpdatedComponent(CollisionComponent);
		MoveComponent->InitialSpeed = 1000.0f;
		MoveComponent->MaxSpeed = 1000.0f;
		MoveComponent->bRotationFollowsVelocity = true;
		MoveComponent->bShouldBounce = true;
		MoveComponent->Bounciness = 0.3f;
		MoveComponent->ProjectileGravityScale = 0.0f;

		// Load weapon mesh
		WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMeshComponent"));
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("StaticMesh'/Game/Meshes/projectile.projectile'"));
		WeaponMeshComponent->SetStaticMesh(Mesh.Object);
		WeaponMeshComponent->SetupAttachment(CollisionComponent);
	}

	InitialLifeSpan = 2.0f;
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

// Fire
void AWeapon::Fire(const FVector& Direction)
{
	MoveComponent->Velocity = Direction * MoveComponent->InitialSpeed;
}

// Collision detection
void AWeapon::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	// Ignore self-collisions
	if (OtherActor == this || OtherActor->GetInstigator() == this->GetInstigator()) return;

	if (OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(MoveComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}
	else if (HasAuthority())
	{
		// If the hit object is a player, damage it
		APlayerChar* PDamaged = Cast<APlayerChar>(OtherActor);
		if (PDamaged)
		{
			AHolotestGameModeBase* GMode = GetWorld()->GetAuthGameMode<AHolotestGameModeBase>();
			if (GMode)
			{
				// Add to hits count
				GMode->PlayerHit();

				// Damage player
				AHolotestPlayerState* PDamagedState = PDamaged->GetPlayerState<AHolotestPlayerState>();

				// Firing player scores
				APlayerChar* POwner = Cast<APlayerChar>(GetOwner());
				AHolotestPlayerState* POwnerState = POwner->GetPlayerState<AHolotestPlayerState>();

				// Calculate the amount of damage and score by distance
				// more distance is more score because it's harder,
				// but less damage
				float Dist = FVector::Distance(PDamaged->GetActorLocation(), POwner->GetActorLocation());
				uint16 DamageAmount = (uint16)(FMath::Clamp<float>(10 - Dist / 100, 0, 10));
				uint32 ScoreAmount = (uint32)(Dist / 10);

				PDamagedState->PlayerDamage(DamageAmount);
				POwnerState->PlayerAddScore(ScoreAmount);
				
				UE_LOG(LogTemp, Warning, TEXT("%s ==> %s, Dam %u, Sco %u (Dist: %f)"), *POwner->GetName() , *PDamaged->GetName(), DamageAmount, ScoreAmount, Dist);
			}
		}
	}

	if (HasAuthority())
	{
		// Local Explosion
		ExplosionAtPos(Hit.ImpactPoint);
	}
	else
	{
		// Explosion at Server
		ServerExplosion(Hit.ImpactPoint);
	}

	Destroy();
}

// Explosion at pos
void AWeapon::ExplosionAtPos(const FVector& Pos)
{
	UWorld* World = GetWorld();

	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();

		// Spawn new explosion
		AActor* Explosion = World->SpawnActor<AActor>(ExplosionClass, Pos, FRotator(0, 0, 0), SpawnParams);
		if (Explosion)
		{
			// Explode
			Explosion->SetReplicates(true);
			Explosion->SetReplicateMovement(true);
			Explosion->SetAutoDestroyWhenFinished(true);

			// Play explosion audio
			UGameplayStatics::PlaySoundAtLocation(World, ExplosionAudio, Pos);
		}
	}
}

// Server Fire RPC implementation
void AWeapon::ServerExplosion_Implementation(const FVector& Pos)
{
	ExplosionAtPos(Pos);
}
