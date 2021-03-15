#pragma once

#ifndef GPUDevice_H
#define GPUDevice_H

#include <d3d11_4.h>
#include <d3dcompiler.h>
#include <map>
class Shader;
#include "Shader.h"
#include <memory>


//#include <d3dx11.h> // non c'è più in W10 - how to survive: https://walbourn.github.io/living-without-d3dx/
class GPUDevice
{
public:
	D3D11_CREATE_DEVICE_FLAG flags = D3D11_CREATE_DEVICE_DEBUG;
	/// <summary>
	/// Constructor - set feature(s) level
	/// </summary>
	/// <param name="featureLevel"></param>
	GPUDevice(const D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_1);
	/// <summary>
	/// Create a device to run something on
	/// </summary>
	/// <returns>
	/// 	 0->OK
	/// 	 -1->error on create
	/// 	 -2->already created on this object
	/// 	 </returns>
	int create();
	/// <summary>
	/// a safe way to destroy the device - destructor
	/// </summary>
	~GPUDevice();
	/// <summary>
	/// Add a compiled shader to this device - TODO
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns>-1 if error otherwise the number associated</returns>
	int addCompiledShader(LPCWSTR fileName);
	/// <summary>
	/// Compile and add a shader to this device 
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="startupFunctionName"></param>
	/// <returns>-1 if error otherwise the number associated</returns>
	int compileAndAddShader(LPCWSTR fileName, LPCSTR startupFunctionName);
	/// <summary>
	/// Return the device 
	/// </summary>
	/// <returns></returns>
	ID3D11Device* const getDevice ()const;

	/// <summary>
	/// Returns the context, for example to add buffers! 
	/// </summary>
	/// <returns></returns>
	ID3D11DeviceContext* const getContext()const;

	HRESULT getLastResult() const;

	std::unique_ptr<TCHAR[]> getLastResultText() const;

	const char* getLastErrors() const;

	bool isEnabled() const;

	Shader* const getShader(int number) const;

	bool setActiveShader(const Shader* shader);

	bool setShaderOff();

private:
	ID3D11Device* device = NULL;
	ID3D11DeviceContext* context = NULL;
	D3D_FEATURE_LEVEL featureLevel;
	bool enabled = false; // double name
	HRESULT lastResult = 0;

	/// compiled shader errors - if to compile
	const char* errorsPointer = NULL;

	std::map <int, Shader*> loadedShaders;

	int progressiveNumber = 0;
	int enabledShader = -1; // which shader is enabled on this device?


};


#endif
