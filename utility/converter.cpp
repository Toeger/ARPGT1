#include "converter.h"

#include <codecvt>
#include <locale>

std::wstring Converter::str_to_wstring(const std::string &str) {
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
}
