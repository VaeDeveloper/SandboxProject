#pragma once

#include "Modules/ModuleManager.h"



DECLARE_LOG_CATEGORY_EXTERN(VisualizerModule, All, All);

class FVisualizerModule : public IModuleInterface
{
	public:

	/* Called when the module is loaded */
	virtual void StartupModule() override;

	/* Called when the module is unloaded */
	virtual void ShutdownModule() override;
};