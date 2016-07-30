#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <experimental/string_view>

using string_view = std::experimental::string_view;

namespace Converter {
	std::wstring str_to_wstring(string_view str);
}

#endif // CONVERTER_H
