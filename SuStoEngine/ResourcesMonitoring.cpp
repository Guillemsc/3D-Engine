#include "ResourcesMonitoring.h"
#include "App.h"
#include "imgui.h"
#include "imgui_docking.h"
#include "ResourceManager.h"
#include "ResourceLoader.h"

ResourcesMonitoring::ResourcesMonitoring(bool start_enabled)
{
}

ResourcesMonitoring::~ResourcesMonitoring()
{
}

void ResourcesMonitoring::Start()
{
}

void ResourcesMonitoring::Draw(uint flags)
{
	ImGui::BeginDock("Resources monitoring", false, &visible, false, flags);

	std::vector<ResourceLoader*> loaders = App->resource_manager->GetLoaders();

	for (std::vector<ResourceLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it)
	{
		ResourceLoader* curr_loader = (*it);

		std::map<std::string, Resource*> resources = curr_loader->GetResources();

		for (std::map<std::string, Resource*>::iterator re = resources.begin(); re != resources.end(); ++re)
		{
			Resource* curr_resource = (*re).second;

			ImGui::Text(curr_resource->GetUniqueId().c_str());
		}
	}

	ImGui::EndDock();
}
