#include "pch.h"
#include "Language.h"

LanguageOption Language::current = LanguageOption::French;

void Language::toggle()
{
	if (current == LanguageOption::French)
		current = LanguageOption::English;
	else
		current = LanguageOption::French;
}