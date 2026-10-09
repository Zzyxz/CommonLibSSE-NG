#include <RE/Skyrim.h>
#include <SKSE/SKSE.h>

#include <spdlog/sinks/basic_file_sink.h>

#include <filesystem>
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

// One DLL for SE and AE: AE SKSE reads SKSEPlugin_Version, SE SKSE calls SKSEPlugin_Query.
// NG selects every layout that differs between SE and AE at runtime, so no struct restriction is needed.
extern "C" __declspec(dllexport) constinit auto SKSEPlugin_Version = []() {
	SKSE::PluginVersionData version;
	version.PluginVersion(REL::Version{ 1, 0, 0, 0 });
	version.PluginName(TEMPLATE_PLUGIN_NAME);
	version.AuthorName(TEMPLATE_PLUGIN_AUTHOR);
	version.UsesAddressLibrary();
	version.UsesAddressLibraryV5();
	version.UsesNoStructs();
	return version;
}();

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Query(const SKSE::QueryInterface* a_skse, SKSE::PluginInfo* a_info)
{
	a_info->infoVersion = SKSE::PluginInfo::kVersion;
	a_info->name = TEMPLATE_PLUGIN_NAME;
	a_info->version = 1;
	if (a_skse->IsEditor()) {
		return false;
	}
	// SE SKSE and SKSE VR call this. Accept any version of the runtimes this build supports (SKSE VR reports
	// 1.4.15.1, not 1.4.15.0); SKSEPlugin_Load then looks for the matching Address Library file.
	[[maybe_unused]] const auto family = REL::Module::RuntimeFor(a_skse->RuntimeVersion());
#ifdef ENABLE_SKYRIM_SE
	if (family == REL::Module::Runtime::SE) {
		return true;
	}
#endif
#ifdef ENABLE_SKYRIM_VR
	if (family == REL::Module::Runtime::VR) {
		return true;
	}
#endif
	return false;
}

extern "C" __declspec(dllexport) bool SKSEAPI SKSEPlugin_Load(const SKSE::LoadInterface* a_skse)
{
	initialize_log();

	// Checked before SKSE::Init, which ends the game when the Address Library file is missing.
	// FindAddressLibrary looks for the file named after the executable's version, the one Init loads.
	const auto runtime = a_skse->RuntimeVersion();
	if (REL::Module::FindAddressLibrary().empty()) {
		logger::warn("Address Library for SKSE Plugins is missing for {}; plugin inactive.", runtime.string());
		return true;
	}

	SKSE::Init(a_skse);
	logger::info("{} loaded for runtime {}.", TEMPLATE_PLUGIN_NAME, runtime.string());
	return true;
}
