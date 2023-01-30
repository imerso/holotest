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

		UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("Blueprint'/Game/Blueprints/BP_Explosion01.BP_Explosion01'")));
		ExplosionBP = Cast<UBlueprint>(SpawnActor);
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
	if (OtherActor->GetInstigator() == this->GetInstigator()) return;

	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		OtherComponent->AddImpulseAtLocation(MoveComponent->Velocity * 100.0f, Hit.ImpactPoint);
	}

	UE_LOG(LogTemp, Warning, TEXT("HIT ACTOR: %s"), *OtherActor->GetName());

	// Spawn explosion at location
	if (!HasAuthority())
	{
		// Explosion at Server
		ServerExplosion(Hit.ImpactPoint);
	}
	else
	{
		// Explosion at Client
		ExplosionAtPos(Hit.ImpactPoint);
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
		AActor* Explosion = World->SpawnActor<AActor>(ExplosionBP->GeneratedClass, Pos, FRotator(0, 0, 0), SpawnParams);
		if (Explosion)
		{
			// Explode
			Explosion->SetReplicates(true);
			Explosion->SetReplicateMovement(true);
		}
	}
}

// Server Fire RPC implementation
void AWeapon::ServerExplosion_Implementation(const FVector& Pos)
{
	ExplosionAtPos(Pos);
}
