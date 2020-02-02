#include "ErrorLoger.h"
#include <comdef.h>

void ErrorLoger::Log(std::string message)
{
	std::string error_message = "Error: " + message;
	MessageBoxA(NULL, error_message.c_str(), "Error", MB_ICONERROR);
}

void ErrorLoger::Log(HRESULT hr, std::string message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + StringHelper::StringToWide(message) + L"\n" + StringHelper::StringToWide(error.ErrorMessage());
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
void ErrorLoger::Log(HRESULT hr, std::wstring message)
{
	_com_error error(hr);
	std::wstring error_message = L"Error: " + message + L"\n" + StringHelper::StringToWide(error.ErrorMessage());
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}

void ErrorLoger::Log(COMException & exception)
{
	std::wstring error_message = exception.what();
	MessageBoxW(NULL, error_message.c_str(), L"Error", MB_ICONERROR);
}
