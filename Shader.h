#pragma once

#ifndef Shader_H
#define Shader_H

#include <d3d11_4.h>
#include <map>
#include "GPGPU.h"
// declare before to fix 
class GPUDevice;
class Buffer;

#include "GPUDevice.h"
#include "Buffer.h"
#include "IOBuffer.h"


class Shader
{

public:
	/// <summary>
	/// Compile a HLSL file and load
	/// </summary>
	/// <param name="fileName"></param>
	/// <param name="startupFunctionName"></param>
	/// <returns>is loaded? </returns>
	HRESULT compileFromFileAndLoad(LPCWSTR fileName, LPCSTR startupFunctionName);
	/// <summary>
	/// Load a compiled HLSL file - TODO
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns>is loaded?</returns>
	HRESULT loadCompiledShader(LPCWSTR fileName);

	/// <summary>
	/// 
	/// </summary>
	/// <param name="device"></param>
	Shader(GPUDevice* device);

	/// <summary>
	/// 
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="CPURequirement"></param>
	/// <param name="GPURequirement"></param>
	/// <param name="size"></param>
	/// <param name="GPUslot"></param>
	/// <param name="data"></param>
	/// <returns></returns>
	template <typename T> unsigned short addBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, size_t size, unsigned short GPUslot = 255, T* data = NULL);

	bool removeBuffer(unsigned short GPUslot);


	~Shader();

	int runShader(UINT X, UINT Y, UINT Z);

	/// <summary>
	/// Return the Device 
	/// </summary>
	/// <returns></returns>
	GPUDevice* getGPUDevice();

	void getLastErrors(const char*& messagePointer);

private:
	bool isLoaded = false;
	GPUDevice* device;
	/// <summary>
	/// GPU slots that are not free to use.  
	/// </summary>
	std::map<unsigned short, Buffer*> GPUSlots;

	ID3D11ComputeShader* computeShaderPointer = NULL;
	/// compiled shader errors - if to compile
	ID3DBlob* errorsPointer = NULL;

	/// compiled shader code pointer - if to compile
	ID3DBlob* shaderCodePointer = NULL;

};
// add here to be found since template
template<typename T>
unsigned short Shader::addBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, size_t size, unsigned short GPUslot, T* data)
{
	/*IOBuffer<T> bf = new IOBuffer<T>(CPURequirement, GPURequirement, this, size, GPUslot);
	bf->create(data);*/

	//TODO
	return 0;
}
#endif