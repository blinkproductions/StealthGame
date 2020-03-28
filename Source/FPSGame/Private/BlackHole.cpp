// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ABlackHole::ABlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetRootComponent(MeshComp);

	InnerCollision = CreateDefaultSubobject<USphereComponent>(TEXT("InnerCollision"));
	InnerCollision->SetSphereRadius(100);
	InnerCollision->AttachTo(MeshComp);

	// Bind to event
	InnerCollision->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	OuterCollision = CreateDefaultSubobject<USphereComponent>(TEXT("OuterCollision"));
	OuterCollision->SetSphereRadius(3000);
	OuterCollision->AttachTo(MeshComp);

	/*RadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForce"));
	RadialForce->Radius = 3000.f;*/
}

// Called when the game starts or when spawned
void ABlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent * OverlappendComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}

// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Find all overlapping components that can collids and may be physically simulating.
	TArray<UPrimitiveComponent*> OverlappingComps;
	OuterCollision->GetOverlappingComponents(OverlappingComps);

	for (size_t i = 0; i < OverlappingComps.Num(); i++)
	{
		UPrimitiveComponent* PrimComp = OverlappingComps[i];
		if (PrimComp && PrimComp->IsSimulatingPhysics())
		{
			// the component we are looking for! It needs to be simulating in order to apply forces.

			const float SphereRadius = OuterCollision->GetScaledSphereRadius();
			const float ForceStrength = -2000; // Negative value make it pull towards the origin instead of pushing away

			PrimComp->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);
		}
		
	}
}

