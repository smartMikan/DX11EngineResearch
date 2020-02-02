#include "StringHelper.h"

std::wstring StringHelper::StringToWide(std::string str)
{
	return 	std::wstring(str.begin(), str.end());

}

std::string StringHelper::WideToString(std::wstring wstr)
{
	return std::string(wstr.begin(), wstr.end());
}

std::string StringHelper::GetDirectoryFromPath(const std::string& filepath)
{
	size_t off1 = filepath.find_last_of('\\');
	size_t off2 = filepath.find_last_of('/');
	if (off1 == std::string::npos && off2 == std::string::npos) 
	{
		return "";
	}
	if (off1 == std::string::npos)
	{
		return filepath.substr(0,off2);
	}
	if (off2 == std::string::npos)
	{
		return filepath.substr(0, off1);
	}

	//use greater offset if both exist
	return filepath.substr(0, std::max(off1,off2));
}

std::string StringHelper::GetFileExtension(const std::string& filename)
{
	size_t off = filename.find_last_of('.');
	if (off == std::string::npos) 
	{
		return {};
	}
	return std::string(filename.substr(off + 1));
}
