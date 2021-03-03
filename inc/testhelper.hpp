#pragma once

#include "types.hpp"
#include <catch.hpp>

namespace WhipseeySaveManager::Test
{
	std::string stringify(float value);

	namespace Data
	{
		const std::filesystem::path missing(L"data/__MISSING_FILE__.NONE");
		const std::filesystem::path settingsDefault(L"data/settings-default.ini");
		const std::filesystem::path settingsValid(L"data/settings-valid.ini");
		const std::filesystem::path settingsInvalid(L"data/settings-invalid.ini");
		const std::filesystem::path settingsMissingSection(L"data/settings-missing-section.ini");
		const std::filesystem::path settingsMissingKey(L"data/settings-missing-key.ini");
		const std::filesystem::path optionsDefault(L"data/options-default.ini");
		const std::filesystem::path optionsValid(L"data/options-valid.ini");
		const std::filesystem::path optionsInvalid(L"data/options-invalid.ini");
		const std::filesystem::path optionsMissingSection(L"data/options-missing-section.ini");
		const std::filesystem::path optionsMissingKey(L"data/options-missing-key.ini");
	} // namespace Data

	//TODO display path not name
	#define REQUIRE_EXISTS(path) if(std::filesystem::exists(path) == false) FAIL(#path" file missing!")

	template<typename T>
	class EqualsMatcher : public Catch::Matchers::Impl::MatcherUntypedBase {
		T mRhs;
		std::string mDescription;
	public:
		EqualsMatcher(T&& rhs, std::string&& desc) :
			mRhs{std::move(rhs)}, mDescription{std::move(desc).insert(0, "== ")}
		{ }
		template<typename U>
		bool match(U const& lhs) const
		{
			return lhs == mRhs;
		}
		std::string describe() const override
		{
			return mDescription;
		}
	};

	#define EQUALS(rhs) EqualsMatcher(rhs, #rhs) 
} // namespace WhipseeySaveManager::Test
