#include "GPUDevice.h"
#include <map>
#include <iostream>

//using namespace DCL;
GPUDevice::GPUDevice(const D3D_FEATURE_LEVEL featureLevel) {
	this->featureLevel = featureLevel;
	loadedShaders = std::map<int, Shader*>();
}

int GPUDevice::create() {
	if (enabled) return -2;
	//info @ https://docs.microsoft.com/en-us/windows/win32/api/d3d11/nf-d3d11-d3d11createdevice
	//FLAGS: https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ne-d3d11-d3d11_create_device_flag
	D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, flags, NULL, 0,
		D3D11_SDK_VERSION, &device, &featureLevel, &context);
	// if something wrong return -1 
	if (device == NULL || context == NULL) return -1;
	enabled = true; // set enabled 
	return 0;
}

GPUDevice::~GPUDevice()
{
	// remove all shaders 
	for (auto itv = loadedShaders.begin(); itv != loadedShaders.end(); itv++) {
		delete itv->second;
	}
	/*if(this->errorsPointer !=nullptr)
		delete (this->errorsPointer);
	std::cout << (this->errorsPointer == nullptr) << std::endl;*/
	GPGPU::releaseResource(this->context);
	GPGPU::releaseResource(this->device);
}

int GPUDevice::addCompiledShader(LPCWSTR fileName)
{
	//TODO
	return -1;
}

int GPUDevice::compileAndAddShader(const LPCWSTR fileName, const LPCSTR startupFunctionName)
{
	Shader* sh = new Shader(this);
	lastResult = sh->compileFromFileAndLoad(fileName, startupFunctionName);
	if (lastResult == S_OK) {
		auto resMap = this->loadedShaders.insert(std::pair<int, Shader*>(progressiveNumber, sh));
		progressiveNumber = progressiveNumber < INT_MAX ? progressiveNumber + 1 : 0;
		while (!resMap.second && loadedShaders.size() < INT_MAX) {
			resMap = this->loadedShaders.insert(std::pair<int, Shader*>(progressiveNumber, sh));
			progressiveNumber = progressiveNumber < INT_MAX ? progressiveNumber + 1 : 0;
		}
		return progressiveNumber - 1;
	}
	else {
		if (errorsPointer != NULL) {
			delete[] errorsPointer;
		}
		sh->getLastErrors(errorsPointer);
		delete sh;
		return -1;
	}
}

ID3D11Device* const GPUDevice::getDevice() const
{
	return device;
}

ID3D11DeviceContext* const GPUDevice::getContext() const
{
	return this->context;
}

HRESULT GPUDevice::getLastResult() const
{
	return lastResult;
}
std::unique_ptr<TCHAR[]> GPUDevice::getLastResultText() const
{
	return GPGPU::resolveHRESULT(this->lastResult);
}

Shader* const GPUDevice::getShader(int number) const
{
	try {
		return loadedShaders.at(number);//use this instead of [] to be const
	}
	catch (std::out_of_range e) {
		return nullptr;
	}
}

bool GPUDevice::setActiveShader(const Shader* shader)
{
	// remove if there is something
	setShaderOff();

	// find key
	auto findResult = std::find_if(std::begin(loadedShaders), std::end(loadedShaders), [&](const std::pair<int, Shader*>& pair)
		{
			return pair.second == shader;
		});
	int foundKey = -1; // You might want to initialise this to a value you know is invalid in your map
	Shader* foundValue = nullptr;
	if (findResult != std::end(loadedShaders))
	{
		foundKey = findResult->first;
		foundValue = findResult->second;

		context->CSSetShader(foundValue->getcomputeShaderPointer(), NULL, 0); // set active shader

		foundValue->readyToRun = true;
		enabledShader = foundKey;
		return true;
	}
	else
		return false;
}

bool GPUDevice::setShaderOff()
{
	if (enabledShader == -1) return false;
	context->CSSetShader(NULL, NULL, 0); // set active shader
	try {
		loadedShaders.at(this->enabledShader)->readyToRun = false;
	}
	catch(std::out_of_range){
		return false;
	}
	enabledShader = -1;
	return true;
}



const char* GPUDevice::getLastErrors() const
{
	if (this->errorsPointer != NULL)
		return this->errorsPointer;
	else {
		auto noMessage = "No errors!";
		return noMessage;
	}
}

bool GPUDevice::isEnabled() const
{
	return enabled;
}


