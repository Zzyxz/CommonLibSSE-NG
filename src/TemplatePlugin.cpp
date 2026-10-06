#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>

#include <memory>

namespace logger = SKSE::log;

namespace
{
	void initialize_log()
	{
		auto path = logger::log_directory();
		if (!path) {
			SKSE::stl::report_and_fail("Could not locate the SKSE log directory.");
		}

		*path /= std::string(TEMPLATE_PLUGIN_NAME) + ".log";
		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
		auto log = std::make_shared<spdlog::logger>("global", std::move(sink));
		log->set_level(spdlog::level::info);
		log->flush_on(spdlog::level::info);
		spdlog::set_default_logger(std::move(log));
	}
}

#ifdef SKYRIM_SUPPORT_AE
extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData version;
	version.PluginVersion(1);
	version.PluginName(TEMPLATE_PLUGIN_NAME);
	version.AuthorName(TEMPLATE_PLUGIN_AUTHOR);
	version.UsesAddressLibrary();
	version.UsesAddressLibraryV5();
	version.UsesNoStructs();
	return version;
}();
#endif

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* skse, SKSE::PluginInfo* info)
{
	if (skse->IsEditor()) {
		return false;
	}

	info->infoVersion = SKSE::PluginInfo::kVersion;
	info->name = TEMPLATE_PLUGIN_NAME;
	info->version = 1;
	return true;
}

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* skse)
{
	initialize_log();
	SKSE::Init(skse);
	logger::info("{} loaded for runtime {}.", TEMPLATE_PLUGIN_NAME, skse->RuntimeVersion().string());
	return true;
}
