#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Texture2D.h"
#include "AProceduralTextureTest.generated.h"

UCLASS()
class UIEXTENSION_API AProceduralTextureTest : public AActor
{
  GENERATED_BODY()

public:
  // 기본값 설정
  AProceduralTextureTest();

  // 블루프린트에서 호출 가능한 텍스처 생성 함수
  UFUNCTION(BlueprintCallable, Category = "ProjectMF")
  void ToTextureGodDamn(int32 TextureWidth, int32 TextureHeight, const TArray<float>& PixelValues);

protected:
  // 게임 시작 또는 스폰될 때 호출
  virtual void BeginPlay() override;

private:
  // 실제 텍스처 생성 함수
  UFUNCTION()
  void GenerateTexture(int32 TextureWidth, int32 TextureHeight, const TArray<float>& PixelValues);
};
