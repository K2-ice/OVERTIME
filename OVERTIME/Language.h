#pragma once

enum class LanguageOption
{
	French,
	English
};

class Language
{
public:
	static LanguageOption current;

	static void toggle();
};