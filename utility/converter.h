#ifndef CONVERTER_H
#define CONVERTER_H

#include <string>
#include <string_view>

namespace Converter {
	std::wstring str_to_wstring(std::string_view str);
}

#endif // CONVERTER_H
