#include "AProceduralTextureTest.h"
#include "Engine/Texture2D.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/AssetRegistryModule.h"

// 기본값 설정
AProceduralTextureTest::AProceduralTextureTest()
{
  // 이 액터를 모든 프레임에서 Tick() 호출 가능하도록 설정
  PrimaryActorTick.bCanEverTick = false;
}

// 게임 시작 또는 스폰될 때 호출
void AProceduralTextureTest::BeginPlay()
{
  Super::BeginPlay();

  // 여기에 필요한 코드 추가
}

// 블루프린트에서 호출 가능한 텍스처 생성 함수
void AProceduralTextureTest::ToTextureGodDamn(int32 TextureWidth, int32 TextureHeight, const TArray<float>& PixelValues)
{
  // 실제 텍스처 생성 함수 호출
  GenerateTexture(TextureWidth, TextureHeight, PixelValues);
}

// 텍스처 생성 함수
void AProceduralTextureTest::GenerateTexture(int32 TextureWidth, int32 TextureHeight, const TArray<float>& PixelValues)
{
  // 패키지 및 랜덤 UUID로 텍스처 이름 생성
  FString PackageName = TEXT("/Game/ProceduralTextures/");
  FString BaseTextureName = FString("ArrayTexture_") + FGuid::NewGuid().ToString();
  PackageName += BaseTextureName;

  // 패키지 생성
  UPackage* Package = CreatePackage(*PackageName);
  GLog->Log("프로젝트 디렉토리:" + FPaths::ProjectDir());

  // 유니크한 텍스처 이름 생성
  FName TextureName = MakeUniqueObjectName(Package, UTexture2D::StaticClass(), FName(*BaseTextureName));
  Package->FullyLoad();

  // UTexture2D 객체 생성
  UTexture2D* NewTexture = NewObject<UTexture2D>(Package, TextureName, RF_Public | RF_Standalone | RF_MarkAsRootSet);

  if (NewTexture)
  {
    // 루트에 추가
    NewTexture->AddToRoot();

    // 플랫폼 데이터 설정
    NewTexture->PlatformData = new FTexturePlatformData();
    NewTexture->PlatformData->SizeX = TextureWidth;
    NewTexture->PlatformData->SizeY = TextureHeight;
    NewTexture->PlatformData->PixelFormat = EPixelFormat::PF_B8G8R8A8;

    // 픽셀 데이터 할당
    uint8* Pixels = new uint8[TextureWidth * TextureHeight * 4];

    for (int32 y = 0; y < TextureHeight; y++)
    {
      for (int32 x = 0; x < TextureWidth; x++)
      {
        int32 CurrentPixelIndex = ((y * TextureWidth) + x);

        if (CurrentPixelIndex < PixelValues.Num())
        {
          // 0에서 1 사이의 float 값을 0에서 255로 변환
          float CurrentValue = FMath::Clamp(PixelValues[CurrentPixelIndex], 0.0f, 1.0f);
          uint8 ConvertedValue = FMath::RoundToInt(CurrentValue * 255.0f);

          // 픽셀 데이터에 값 할당
          Pixels[4 * CurrentPixelIndex] = ConvertedValue;
          Pixels[4 * CurrentPixelIndex + 1] = ConvertedValue;
          Pixels[4 * CurrentPixelIndex + 2] = ConvertedValue;
          Pixels[4 * CurrentPixelIndex + 3] = 255; // 알파 채널은 항상 255로 설정
        }
        else
        {
          // 배열 크기가 텍스처 크기보다 작을 경우 처리
          // 기본적으로 검정색으로 설정
          Pixels[4 * CurrentPixelIndex] = 0;
          Pixels[4 * CurrentPixelIndex + 1] = 0;
          Pixels[4 * CurrentPixelIndex + 2] = 0;
          Pixels[4 * CurrentPixelIndex + 3] = 255;
        }
      }
    }

    // 미맵 설정
    FTexture2DMipMap* Mip = new FTexture2DMipMap();
    NewTexture->PlatformData->Mips.Add(Mip);
    Mip->SizeX = TextureWidth;
    Mip->SizeY = TextureHeight;

    Mip->BulkData.Lock(LOCK_READ_WRITE);
    uint8* TextureData = (uint8*)Mip->BulkData.Realloc(TextureWidth * TextureHeight * 4);
    FMemory::Memcpy(TextureData, Pixels, sizeof(uint8) * TextureHeight * TextureWidth * 4);
    Mip->BulkData.Unlock();

    // 텍스처 데이터 업데이트
    NewTexture->UpdateResource();

    // 패키지 마킹 및 에디터에 알림
    Package->MarkPackageDirty();
    FAssetRegistryModule::AssetCreated(NewTexture);

    // 자동 저장
    FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());
    bool bSaved = UPackage::SavePackage(Package, NewTexture, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *PackageFileName, GError, nullptr, true, true, SAVE_NoError);

    // 픽셀 데이터 메모리 해제
    delete[] Pixels;
  }
}
