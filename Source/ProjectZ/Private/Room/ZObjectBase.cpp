// Fill out your copyright notice in the Description page of Project Settings.

#include "Room/ZObjectBase.h"
#include <string>
#include "ProjectZ/ProjectZ.h"

// Sets default values
AZObjectBase::AZObjectBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AZObjectBase::BeginPlay()
{
	Super::BeginPlay();
	GetComponents<UStaticMeshComponent>(MeshComponents);
	ZCharacter = Cast<AZCharacterBase>(GetWorld()->GetFirstPlayerController()->GetCharacter());
}

// Called every frame
void AZObjectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AZObjectBase::NotifyActorBeginCursorOver()
{
	Super::NotifyActorBeginCursorOver();
	if(!Interactable) return;

	// Latter code only applies when Interactable is True
	if(FVector::Dist(ZCharacter->GetActorLocation(), GetActorLocation()) > 200) return;
	
	// Latter code only applies when Player is close to the Object
	for(auto Mesh : MeshComponents)
	{
		Mesh->SetRenderCustomDepth(true);
		Mesh->SetCustomDepthStencilValue(CUSTOM_DEPTH_BLUE);
	}
}

void AZObjectBase::NotifyActorEndCursorOver()
{
	Super::NotifyActorEndCursorOver();
	if(!Interactable) return;

	// Latter code only applies when Interactable is True
	if(FVector::Dist(ZCharacter->GetActorLocation(), GetActorLocation()) > 200) return;
	
	// Latter code only applies when Player is close to the Object
	for(auto Mesh:MeshComponents)
	{
		Mesh->SetRenderCustomDepth(false);
	}
}
