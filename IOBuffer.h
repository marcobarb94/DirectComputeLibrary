#pragma once
#ifndef IOBuffer_H
#define IOBuffer_H
#pragma message ("Figlio")
class Buffer;
#include "Buffer.h"

template <typename T>
class IOBuffer: public Buffer
{
	IOBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t size, unsigned short GPUslot);

	bool create(T* array = NULL);

	T* readBuffer();
};

template<typename T>
IOBuffer<T>::IOBuffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t size, unsigned short GPUslot)
{
	Buffer::Buffer(CPURequirement, GPURequirement, shader, size, sizeof T, GPUslot);
}

template<typename T>
T* IOBuffer<T>::readBuffer()
{
	//TODO
	return nullptr;
}
template<typename T>
bool IOBuffer<T>::create(T* array)
{
	HRESULT res = S_FALSE;
	switch (Buffer::CPURequirement)
	{
	case CPU_IO::READ:
		switch (Buffer::GPURequirement)
		{
		case GPU_IO::READ:
			res = Buffer::createStructuredBuffer();
			break;
		case GPU_IO::READ_N_WRITE:
			res = Buffer::createRWStructuredBuffer();
			break;
		default:
			break;
		}
		if (res == S_OK)
			res = Buffer::createShaderResourceView();
		break;
	case CPU_IO::READ_N_WRITE:
		switch (Buffer::GPURequirement)
		{
		case GPU_IO::READ:
			res = Buffer::createStructuredBuffer();
			break;
		case GPU_IO::READ_N_WRITE:
			res = Buffer::createRWStructuredBuffer();
			break;
		default:
			break;
		}
		break;
		if (res == S_OK)
			res = Buffer::createUnorderedAccessView();
	case CPU_IO::NONE:
		switch (Buffer::GPURequirement)
		{
		case GPU_IO::READ:
			res = Buffer::createStructuredBuffer();
			break;
		case GPU_IO::READ_N_WRITE:
			res = Buffer::createRWStructuredBuffer();
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}

	Buffer::enabled = res == S_OK;
	return Buffer::enabled;
}
#endif