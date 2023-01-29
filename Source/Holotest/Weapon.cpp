// Basic weapon Weapon
//
// Made by Vander R. N. Dias for Hologate test.


#include "Weapon.h"

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

	InitialLifeSpan = 1.0f;
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

void AWeapon::Fire(const FVector& Direction)
{
	MoveComponent->Velocity = Direction * MoveComponent->InitialSpeed;
}
