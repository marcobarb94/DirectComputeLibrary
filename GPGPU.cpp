#include "GPGPU.h"

ULONG GPGPU::releaseResource(IUnknown* resource)
{
	if (resource != NULL)
		return resource->Release();
	else return -1;
}