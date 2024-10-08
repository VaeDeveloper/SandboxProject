#include "DelegateModule.h"

DEFINE_LOG_CATEGORY(DelegateModule);

#define LOCTEXT_NAMESPACE "FDelegateModule"

void FDelegateModule::StartupModule()
{
	UE_LOG(DelegateModule, Warning, TEXT("DelegateModule module has been loaded"));
}

void FDelegateModule::ShutdownModule()
{
	UE_LOG(DelegateModule, Warning, TEXT("DelegateModule module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FDelegateModule, DelegateModule)