#include "GPUDevice.h"

//using namespace DCL;
DCL::GPUDevice::GPUDevice(D3D_FEATURE_LEVEL featureLevel) {
	this->featureLevel = featureLevel;
}

int DCL::GPUDevice::create() {
	if (isEnabled) return -2;
	//info @ https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	//FLAGS: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag
	D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0,
		D3D11_SDK_VERSION, &device, &featureLevel, &context);
	// if something wrong return -1 
	if (device == NULL || context == NULL) return -1; 
	isEnabled = true; // set enabled 
	return 0;
}

DCL::GPUDevice::~GPUDevice()
{
	releaseResource(this->context);
	releaseResource(this->device);
}

unsigned short DCL::GPUDevice::addCompiledShader(LPCWSTR fileName)
{
	return 0;
}

unsigned short DCL::GPUDevice::compileAndAddShader(LPCWSTR fileName, LPCSTR startupFunctionName)
{
	return 0;
}

ID3D11Device* DCL::GPUDevice::getDevice()
{
	return device;
}

ULONG DCL::GPUDevice::releaseResource(IUnknown* resource)
{
	if (resource != NULL)
		return resource->Release();
	else return - 1;
}
