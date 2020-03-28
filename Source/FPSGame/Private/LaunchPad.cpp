// Fill out your copyright notice in the Description page of Project Settings.


#include "LaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "FPSCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ALaunchPad::ALaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollision->SetBoxExtent(FVector(75, 75, 75));
	/*BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	BoxCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);*/
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &ALaunchPad::OnOverlap);
	SetRootComponent(BoxCollision);
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComp->AttachTo(BoxCollision);

	LaunchStrength = 1500;
	LaunchPitchAngle = 35.f;
}

// Called when the game starts or when spawned
void ALaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALaunchPad::OnOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Make rotator with our specified 'pitch' and convert to a direction vector * intensity
	FRotator LaunchDirection = OtherActor->GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	AFPSCharacter* OtherCharacter = Cast<AFPSCharacter>(OtherActor);
	if (OtherActor)
	{
		// Launch Player! Both booleans give consistent launch velocity by ignoring the current player velocity
		OtherCharacter->LaunchPawn(LaunchVelocity, true, true);

		// Spawn FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ActivateLaunchEffect, GetActorLocation());
	}
}

// Called every frame
void ALaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

