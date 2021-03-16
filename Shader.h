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

#include <stdexcept> // for exceptions


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
	/// To Implement
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="CPURequirement"></param>
	/// <param name="GPURequirement"></param>
	/// <param name="size"></param>
	/// <param name="GPUslot"></param>
	/// <param name="data"></param>
	/// <returns>0 if ok </returns>
	template <typename T> unsigned short addBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, size_t size, unsigned short GPUslot = 0, T* data = NULL);

	bool removeStructuredBuffer(unsigned short GPUslot);
	/// <summary>
	/// Add a buffer to this collector to semplify management (during waiting for the other addBuffer using templates)
	/// </summary>
	/// <param name="buf"></param>
	/// <returns>GPUSlot or 65535 if already in</returns>
	unsigned short addBuffer(Buffer* buf);

	/// <summary>
	/// Add a constant buffer (given a struct)
	/// </summary>
	/// <param name="structBuffer"> a buffer made by a struct</param>
	/// <returns>0 if ok</returns>
	template <typename T> unsigned short addConstantBuffer(T &structBuffer, unsigned short GPUslot = 0);

	bool removeConstantBuffer(unsigned short GPUslot);


	~Shader();
	/// <summary>
	/// Run shader
	/// </summary>
	/// <param name="X"></param>
	/// <param name="Y"></param>
	/// <param name="Z"></param>
	/// <returns></returns>
	int runShader(UINT X, UINT Y, UINT Z);

	// multiple dispatch
	int runShaderMD();


	/// <summary>
	/// Return the Device 
	/// </summary>
	/// <returns></returns>
	GPUDevice* const getGPUDevice() const;

	void getLastErrors(const char*& messagePointer) const;


	bool readyToRun = false;

	ID3D11ComputeShader* const getcomputeShaderPointer() const;

	bool isAvailable() const;

private:
	bool isLoaded = false;

	GPUDevice* device;
	/// <summary>
	/// GPU slots that are not free to use for tBuffer. (SRV) 
	/// </summary>
	std::map<unsigned short, Buffer*> tBufferGPUSlots;
	/// <summary>
	/// GPU slots that are not free to use for uBuffer. (UAV)
	/// </summary>
	std::map<unsigned short, Buffer*> uBufferGPUSlots;
	/// <summary>
	/// GPU slots that are not free to use for cBuffer (constant).  
	/// </summary>
	std::map<unsigned short, ID3D11Buffer*> cBufferGPUSlots;


	ID3D11ComputeShader* computeShaderPointer = NULL;
	/// compiled shader errors - if to compile
	ID3DBlob* errorsPointer = NULL;

	/// compiled shader code pointer - if to compile
	ID3DBlob* shaderCodePointer = NULL;

};

/// <summary>
/// Add a buffer to this shader... If it's not correct, I delete it! 
/// </summary>
/// <typeparam name="T"></typeparam>
/// <param name="CPURequirement"></param>
/// <param name="GPURequirement"></param>
/// <param name="size"></param>
/// <param name="GPUslot"></param>
/// <param name="data"></param>
/// <returns></returns>
template<class T>
unsigned short Shader::addBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, size_t size, unsigned short GPUslot, T* data)
{
	// add here to be found since template
	//IOBuffer<T>* bf = new IOBuffer<T>(CPURequirement, GPURequirement, this, size, GPUslot);
	/*bf->create(data);*/
	//T arr[] = new T[5];
	//TODO
	throw std::invalid_argument("Not Enabled At Time");
	return 0;
}

template<typename T>
inline unsigned short Shader::addConstantBuffer(T &structBuffer, unsigned short GPUslot)
{
	// GPUSlot too big
	if (GPUslot >= D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT) return 1;
	// GPUSlot not available
	if (cBufferGPUSlots.find(GPUslot)!=cBufferGPUSlots.end()) return 2;
	//if (sizeof T % 16 != 0) return -1; // multiple of 16 bytes! 
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.ByteWidth = ((sizeof T + 15) / 16) * 16; // multiple of 16 bytes! - sarebbe da controllare che #elementi < 	D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT

	D3D11_SUBRESOURCE_DATA initData;
	ZeroMemory(&initData, sizeof initData); 
	initData.pSysMem = &structBuffer; 

	ID3D11Buffer* cBufferPointer = NULL; 

	// create cBuffer
	auto res = this->getGPUDevice()->getDevice()->CreateBuffer(&bufferDesc, &initData, &cBufferPointer);

	if (res == S_OK) {
		// add cBuffer to Context
		cBufferGPUSlots.insert(std::pair<unsigned short, ID3D11Buffer*>(GPUslot, cBufferPointer));
		return 0;
	}

	return -1;

}

#endif