#include "NativeDebugModule.h"

DEFINE_LOG_CATEGORY(NativeDebugModule);

#define LOCTEXT_NAMESPACE "FNativeDebugModule"

void FNativeDebugModule::StartupModule()
{
	UE_LOG(NativeDebugModule, Warning, TEXT("NativeDebugModule module has been loaded"));
}

void FNativeDebugModule::ShutdownModule()
{
	UE_LOG(NativeDebugModule, Warning, TEXT("NativeDebugModule module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FNativeDebugModule, NativeDebugModule)