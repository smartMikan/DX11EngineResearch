#include "ErrorLoger.h"
void ErrorLoger::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLoger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + StringConverter::StringToWide(message) + L"\n" + StringConverter::StringToWide(error.ErrorMessage());
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
void ErrorLoger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + message + L"\n" + StringConverter::StringToWide(error.ErrorMessage());
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
