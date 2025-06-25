// Fill out your copyright notice in the Description page of Project Settings.


#include "MetaPassthroughPlayerController.h"
#include "Kismet/GameplayStatics.h"
#if PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#include "Android/AndroidJNI.h"
#include "AndroidPermissionFunctionLibrary.h"
#endif

TQueue<FMetaYuvBuffer> AMetaPassthroughPlayerController::YuvBufferQueue;
FMetaYuvBuffer AMetaPassthroughPlayerController::YuvBuffer;

//#if PLATFORM_ANDROID
//extern "C" void CameraOpenedCallback(JNIEnv* Env, jobject CameraDevice)
//{
//    AMetaPassthroughPlayerController* PlayerController = Cast<AMetaPassthroughPlayerController>(UGameplayStatics::GetPlayerController(GEngine->GetWorld(), 0));
//    if (PlayerController)
//    {
//        // Call OnCameraOpened method in PlayerController
//        PlayerController->OnCameraOpened();
//    }
//}
//
//extern "C" void CameraDisconnectedCallback(JNIEnv* Env, jobject CameraDevice)
//{
//    AMetaPassthroughPlayerController* PlayerController = Cast<AMetaPassthroughPlayerController>(UGameplayStatics::GetPlayerController(GEngine->GetWorld(), 0));
//    if (PlayerController)
//    {
//        // Call OnCameraDisconnected method in PlayerController
//        PlayerController->OnCameraDisconnected();
//    }
//}
//
//extern "C" void CameraErrorCallback(JNIEnv* Env, jobject CameraDevice, jint ErrorCode)
//{
//    AMetaPassthroughPlayerController* PlayerController = Cast<AMetaPassthroughPlayerController>(UGameplayStatics::GetPlayerController(GEngine->GetWorld(), 0));
//    if (PlayerController)
//    {
//        // Call OnCameraError method in PlayerController
//        PlayerController->OnCameraError(ErrorCode);
//    }
//}
//#endif

#if PLATFORM_ANDROID
extern "C" JNIEXPORT void JNICALL Java_com_epicgames_unreal_GameActivity_onYuvFrame(JNIEnv* env, jclass clazz,jbyteArray y, jbyteArray u, jbyteArray v,
    jint width, jint height,
    jint yRowStride, jint uRowStride,jint vRowStride,
    jint uPixelStride, jint vPixelStride)
{
    // 拿到Y/U/V数据
    // jbyte* yPtr = env->GetByteArrayElements(y, nullptr);
    // jbyte* uPtr = env->GetByteArrayElements(u, nullptr);
    // jbyte* vPtr = env->GetByteArrayElements(v, nullptr);
    // // ... 在C++侧直接YUV转RGB/上传OpenGL/创建UTexture2D ...
    // env->ReleaseByteArrayElements(y, yPtr, JNI_ABORT);
    // env->ReleaseByteArrayElements(u, uPtr, JNI_ABORT);
    // env->ReleaseByteArrayElements(v, vPtr, JNI_ABORT);
    jsize yLen = env->GetArrayLength(y);
    jsize uLen = env->GetArrayLength(u);
    jsize vLen = env->GetArrayLength(v);

    TArray<uint8> YArr, UArr, VArr;
    YArr.SetNumUninitialized(yLen);
    UArr.SetNumUninitialized(uLen);
    VArr.SetNumUninitialized(vLen);

    env->GetByteArrayRegion(y, 0, yLen, reinterpret_cast<jbyte*>(YArr.GetData()));
    env->GetByteArrayRegion(u, 0, uLen, reinterpret_cast<jbyte*>(UArr.GetData()));
    env->GetByteArrayRegion(v, 0, vLen, reinterpret_cast<jbyte*>(VArr.GetData()));

    // 调用你的静态函数
    AMetaPassthroughPlayerController::ReceiveYuvFrame(
        YArr, UArr, VArr,
        width, height,
        yRowStride, uRowStride, vRowStride,
        uPixelStride, vPixelStride
    );
}
#endif

void AMetaPassthroughPlayerController::BeginPlay()
{
    Super::BeginPlay();
    
}

void AMetaPassthroughPlayerController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AMetaPassthroughPlayerController::StartCamera()
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 获取GameActivity类
        jclass GameActivityClass = FAndroidApplication::FindJavaClass("com/epicgames/unreal/GameActivity");
        if (GameActivityClass != nullptr)
        {
            // 获取startCamera方法
            jmethodID StartCameraMethod = Env->GetMethodID(GameActivityClass, "startCamera", "()V");
            if (StartCameraMethod != nullptr)
            {
                // 获取GameActivity实例
                jobject GameActivityInstance = FAndroidApplication::GetGameActivityThis();
                if (GameActivityInstance != nullptr)
                {
                    // 调用startCamera方法
                    Env->CallVoidMethod(GameActivityInstance, StartCameraMethod);
                    
                    UE_LOG(LogTemp, Log, TEXT("摄像头启动命令已发送"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("无法获取GameActivity实例"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("无法找到startCamera方法"));
            }
            
            Env->DeleteLocalRef(GameActivityClass);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("无法找到GameActivity类"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("无法获取JNI环境"));
    }
#endif
}

void AMetaPassthroughPlayerController::StopCamera()
{
#if PLATFORM_ANDROID
    if (JNIEnv* Env = FAndroidApplication::GetJavaEnv())
    {
        // 获取GameActivity类
        jclass GameActivityClass = FAndroidApplication::FindJavaClass("com/epicgames/unreal/GameActivity");
        if (GameActivityClass != nullptr)
        {
            // 获取stopCamera方法
            jmethodID StopCameraMethod = Env->GetMethodID(GameActivityClass, "stopCamera", "()V");
            if (StopCameraMethod != nullptr)
            {
                // 获取GameActivity实例
                jobject GameActivityInstance = FAndroidApplication::GetGameActivityThis();
                if (GameActivityInstance != nullptr)
                {
                    // 调用stopCamera方法
                    Env->CallVoidMethod(GameActivityInstance, StopCameraMethod);
                    
                    UE_LOG(LogTemp, Log, TEXT("摄像头停止命令已发送"));
                }
                else
                {
                    UE_LOG(LogTemp, Error, TEXT("无法获取GameActivity实例"));
                }
            }
            else
            {
                UE_LOG(LogTemp, Error, TEXT("无法找到stopCamera方法"));
            }
            
            Env->DeleteLocalRef(GameActivityClass);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("无法找到GameActivity类"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("无法获取JNI环境"));
    }
#endif
}

void AMetaPassthroughPlayerController::ReceiveYuvFrame(const TArray<uint8>& Y, const TArray<uint8>& U,
    const TArray<uint8>& V, int32 Width, int32 Height, int32 YRowStride, int32 URowStride, int32 VRowStride,
    int32 UPixelStride, int32 VPixelStride)
{
    FScopeLock Lock(&YuvBuffer.Mutex);
    YuvBuffer.Y = Y;
    YuvBuffer.U = U;
    YuvBuffer.V = V;
    YuvBuffer.Width = Width;
    YuvBuffer.Height = Height;
    YuvBuffer.YRowStride = YRowStride;
    YuvBuffer.URowStride = URowStride;
    YuvBuffer.VRowStride = VRowStride;
    YuvBuffer.UPixelStride = UPixelStride;
    YuvBuffer.VPixelStride = VPixelStride;
}

UTexture2D* AMetaPassthroughPlayerController::GetCapturedTexture()
{
    FMetaYuvBuffer CopyYuvBuffer;
    {
        FScopeLock Lock(&YuvBuffer.Mutex);
        if (YuvBuffer.Y.Num() == 0 || YuvBuffer.U.Num() == 0 || YuvBuffer.V.Num() == 0)
        {
            return nullptr;
        }
        CopyYuvBuffer.Y = YuvBuffer.Y;
        CopyYuvBuffer.U = YuvBuffer.U;
        CopyYuvBuffer.V = YuvBuffer.V;
        CopyYuvBuffer.Width = YuvBuffer.Width;
        CopyYuvBuffer.Height = YuvBuffer.Height;
        CopyYuvBuffer.YRowStride = YuvBuffer.YRowStride;
        CopyYuvBuffer.URowStride = YuvBuffer.URowStride;
        CopyYuvBuffer.VRowStride = YuvBuffer.VRowStride;
        CopyYuvBuffer.UPixelStride = YuvBuffer.UPixelStride;
        CopyYuvBuffer.VPixelStride = YuvBuffer.VPixelStride;
    }

    // 将YUV数据转换为RGBA格式
    FYuvFrameData YuvData;
    YuvData.YData = CopyYuvBuffer.Y;
    YuvData.UData = CopyYuvBuffer.U;
    YuvData.VData = CopyYuvBuffer.V;
    YuvData.Width = CopyYuvBuffer.Width;
    YuvData.Height = CopyYuvBuffer.Height;
    YuvData.YRowStride = CopyYuvBuffer.YRowStride;
    YuvData.URowStride = CopyYuvBuffer.URowStride;
    YuvData.VRowStride = CopyYuvBuffer.VRowStride;
    YuvData.UPixelStride = CopyYuvBuffer.UPixelStride;
    YuvData.VPixelStride = CopyYuvBuffer.VPixelStride;
    TArray<uint8> RgbaData;
    RgbaData.SetNumUninitialized(YuvData.Width * YuvData.Height * 4); // RGBA格式，每个像素4字节
    // YUV转RGBA
    for (int32 YIndex = 0; YIndex < YuvData.Height; ++YIndex)
    {
        for (int32 XIndex = 0; XIndex < YuvData.Width; ++XIndex)
        {
            int32 YValue = YuvData.YData[YIndex * YuvData.YRowStride + XIndex];
            int32 UValue = YuvData.UData[(YIndex / 2) * YuvData.URowStride + (XIndex / 2) * YuvData.UPixelStride];
            int32 VValue = YuvData.VData[(YIndex / 2) * YuvData.VRowStride + (XIndex / 2) * YuvData.VPixelStride];

            // 转换公式
            int32 R = FMath::Clamp((YValue + 1.402 * (VValue - 128)), 0, 255);
            int32 G = FMath::Clamp((YValue - 0.344136 * (UValue - 128) - 0.714136 * (VValue - 128)), 0, 255);
            int32 B = FMath::Clamp((YValue + 1.772 * (UValue - 128)), 0, 255);

            // 设置RGBA数据
            RgbaData[(YIndex * YuvData.Width + XIndex) * 4] = R;
            RgbaData[(YIndex * YuvData.Width + XIndex) * 4 + 1] = G;
            RgbaData[(YIndex * YuvData.Width + XIndex) * 4 + 2] = B;
            RgbaData[(YIndex * YuvData.Width + XIndex) * 4 + 3] = 255; // Alpha通道设为255
        }
    }
    
    UTexture2D *OutputTexture = UTexture2D::CreateTransient(YuvData.Width, YuvData.Height, PF_R8G8B8A8);
    if (!OutputTexture)
    {
        UE_LOG(LogTemp, Error, TEXT("无法创建UTexture2D对象！"));
        return nullptr;
    }
    
    // 设置贴图属性
#if WITH_EDITORONLY_DATA
    OutputTexture->MipGenSettings = TMGS_NoMipmaps;
#endif
    OutputTexture->SRGB = true;
    OutputTexture->AddToRoot(); // 防止被GC回收

    // 将RGBA数据拷贝到Texture2D
    void* TextureData = OutputTexture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
    FMemory::Memcpy(TextureData, RgbaData.GetData(), RgbaData.Num());
    OutputTexture->GetPlatformData()->Mips[0].BulkData.Unlock();

    // 更新贴图资源
    OutputTexture->UpdateResource();

    // 返回生成的Texture2D
    return OutputTexture;
}

