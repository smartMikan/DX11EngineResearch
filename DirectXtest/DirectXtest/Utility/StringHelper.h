#pragma once
#include <string>
#include <algorithm>
class StringHelper
{
public:
	static std::wstring StringToWide(std::string str);
	static std::string WideToString(std::wstring wstr);

	static std::string GetDirectoryFromPath(const std::string& filepath);
	static std::string GetFileExtension(const std::string& filename);

};

