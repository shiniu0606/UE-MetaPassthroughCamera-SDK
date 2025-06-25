// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Texture2D.h"
#include "MetaPassthroughPlayerController.generated.h"

// YUV数据结构体，用于蓝图
USTRUCT(BlueprintType)
struct METAPASSTHROUGH_API FYuvFrameData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	TArray<uint8> YData;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	TArray<uint8> UData;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	TArray<uint8> VData;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 Width = 0;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 Height = 0;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 YRowStride = 0;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 URowStride = 0;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 VRowStride = 0;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 UPixelStride = 0;

	UPROPERTY(BlueprintReadOnly, Category = "YUV Data")
	int32 VPixelStride = 0;

	FYuvFrameData()
	{
		Width = Height = 0;
		YRowStride = URowStride = VRowStride = 0;
		UPixelStride = VPixelStride = 0;
	}
};

struct FMetaYuvBuffer
{
	TArray<uint8> Y, U, V;
	int32 Width = 0, Height = 0;
	int32 YRowStride = 0, URowStride = 0, VRowStride = 0;
	int32 UPixelStride = 0, VPixelStride = 0;
	FCriticalSection Mutex;
};

// 声明YUV帧数据接收委托
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnYuvFrameReceived, const FYuvFrameData&, YuvData);

/**
 * Meta Passthrough 播放器控制器，支持相机数据接收和处理
 */
UCLASS()
class METAPASSTHROUGH_API AMetaPassthroughPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    // Override BeginPlay to initialize camera
    virtual void BeginPlay() override;
	// Override Tick to process YUV data queue
	virtual void Tick(float DeltaTime) override;
	
	// 摄像头控制方法 - 蓝图可调用
	UFUNCTION(BlueprintCallable, Category = "Meta Passthrough Camera")
	void StartCamera();
	
	UFUNCTION(BlueprintCallable, Category = "Meta Passthrough Camera")
	void StopCamera();
	
	UFUNCTION(BlueprintCallable, Category = "Meta Passthrough Camera")
	UTexture2D* GetCapturedTexture();
	
	// 静态方法用于接收JNI回调
	static void ReceiveYuvFrame(
		const TArray<uint8>& Y, const TArray<uint8>& U, const TArray<uint8>& V,
		int32 Width, int32 Height,
		int32 YRowStride, int32 URowStride, int32 VRowStride,
		int32 UPixelStride, int32 VPixelStride);

	// YUV帧数据接收事件
	UPROPERTY(BlueprintAssignable, Category = "Meta Passthrough Camera")
	FOnYuvFrameReceived OnYuvFrameReceived;

private:
	// 用队列来存储YUV数据
	static TQueue<FMetaYuvBuffer> YuvBufferQueue;
	static FMetaYuvBuffer YuvBuffer;
	static AMetaPassthroughPlayerController* ActiveInstance;
};
