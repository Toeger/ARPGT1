#include "converter.h"

#include <codecvt>
#include <locale>

std::wstring Converter::str_to_wstring(std::string_view str) {
	static std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str.data());
}
