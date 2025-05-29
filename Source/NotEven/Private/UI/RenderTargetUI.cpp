// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderTargetUI.h"

#include "NotEvenPlayer.h"
#include "Components/Image.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"

void URenderTargetUI::SetTarget(class AActor* actor)
{
	auto* CaptureComp = NewObject<USceneCaptureComponent2D>(actor, USceneCaptureComponent2D::StaticClass());
	CaptureComp->RegisterComponent();
	CaptureComp->AttachToComponent(actor->GetRootComponent(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	auto* RenderTarget = NewObject<UTextureRenderTarget2D>(actor, UTextureRenderTarget2D::StaticClass());
	RenderTarget->InitAutoFormat(400, 400);
	RenderTarget->UpdateResourceImmediate(true);

	CaptureComp->ProjectionType = ECameraProjectionMode::Type::Orthographic;
	CaptureComp->OrthoWidth = 320;
	CaptureComp->TextureTarget = RenderTarget;
	CaptureComp->SetRelativeLocationAndRotation(FVector(500.f, 0.f, 40.f), FRotator(0.0f,-180.f, 0.0f));

	CaptureComp->CaptureScene();
	CaptureComp->ShowOnlyActorComponents(actor);
	
	if (DynamicMat)
	{
		DynamicMat->SetTextureParameterValue(FName("RenderTarget"), RenderTarget);
	}
	
	if (auto cast = Cast<ANotEvenPlayer>(actor))
	{
		cast->GetMesh()->PlayAnimation(Dance, true);
	}
}

void URenderTargetUI::NativeConstruct()
{
	Super::NativeConstruct();

	if (RenderMaterial)
	{
		DynamicMat = UMaterialInstanceDynamic::Create(RenderMaterial, this);
		RenderImage->SetBrushFromMaterial(DynamicMat);
	}
}
