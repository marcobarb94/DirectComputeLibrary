#include "GPGPU.h"


ULONG GPGPU::releaseResource(IUnknown* resource)
{
	if (resource != NULL)
		return resource->Release();
	else return -1;
}

std::unique_ptr<TCHAR[]> GPGPU::resolveHRESULT(HRESULT result)
{

	/*LPTSTR errorText = NULL;
	/*FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// allocate buffer on local heap for error text
		| FORMAT_MESSAGE_ALLOCATE_BUFFER
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&errorText,  // output 
		0, // minimum size for output buffer
		NULL);   // arguments - see note */

	std::unique_ptr<TCHAR[]> errorText(new TCHAR[4096]);

	FormatMessage(
		// use system message tables to retrieve error text
		FORMAT_MESSAGE_FROM_SYSTEM
		// Important! will fail otherwise, since we're not 
		// (and CANNOT) pass insertion parameters
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,    // unused with FORMAT_MESSAGE_FROM_SYSTEM
		result,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)errorText.get(),  // output 
		4096, // size for output buffer
		NULL);   // arguments - see note 


	return errorText;
}
