#include "AsyncActions.h"

DEFINE_LOG_CATEGORY(AsyncActions);

#define LOCTEXT_NAMESPACE "FAsyncActions"

void FAsyncActions::StartupModule()
{
	UE_LOG(AsyncActions, Warning, TEXT("AsyncActions module has been loaded"));
}

void FAsyncActions::ShutdownModule()
{
	UE_LOG(AsyncActions, Warning, TEXT("AsyncActions module has been unloaded"));
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FAsyncActions, AsyncActions)