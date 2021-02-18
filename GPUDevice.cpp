#include "GPUDevice.h"
#include <map>
#include <iostream>

//using namespace DCL;
GPUDevice::GPUDevice(D3D_FEATURE_LEVEL featureLevel) {
	this->featureLevel = featureLevel;
	loadedShaders = std::map<int, Shader*>();
}

int GPUDevice::create() {
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

int GPUDevice::compileAndAddShader(LPCWSTR fileName, LPCSTR startupFunctionName)
{
	Shader* sh = new Shader(this);
	lastResult = sh->compileFromFileAndLoad(fileName, startupFunctionName);
	if (lastResult==S_OK) {
		auto resMap = this->loadedShaders.insert(std::pair<int, Shader*>(progressiveNumber,sh));
		progressiveNumber = progressiveNumber < INT_MAX ? progressiveNumber + 1 : 0;
		while (!resMap.second && loadedShaders.size()< INT_MAX) {
			resMap = this->loadedShaders.insert(std::pair<int, Shader*>(progressiveNumber, sh));
			progressiveNumber = progressiveNumber < INT_MAX ? progressiveNumber + 1 : 0;
		}
		return progressiveNumber-1; 
	}
	else {
		if (errorsPointer != NULL) {
			delete [] errorsPointer;
		}
		sh->getLastErrors(errorsPointer);
		delete sh; 
		return -1;
	}
}

ID3D11Device* GPUDevice::getDevice()
{
	return device;
}

HRESULT GPUDevice::getLastResult()
{
	return lastResult;
}

Shader* GPUDevice::getShader(int number)
{
	return loadedShaders[number];
}

const char* GPUDevice::getLastErrors()
{
	if(this->errorsPointer!=NULL)
		return this->errorsPointer;
	else {
		auto noMessage = "No errors!";
		return noMessage;
	}
}