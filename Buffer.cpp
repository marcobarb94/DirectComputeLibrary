#include "IOBuffer.h"

Buffer::Buffer(CPU_IO CPURequirement, GPU_IO GPURequirement, Shader* shader, size_t length, size_t byteDimension, unsigned short GPUslot)
{
	this->shader = shader;
	this->CPURequirement = CPURequirement;
	this->GPURequirement = GPURequirement;
	this->shader = shader;
	this->length = length;
	this->elementDimension = byteDimension;
	this->GPUslot = GPUslot;
}


Buffer::~Buffer()
{
	removeView();
	// TODO
	if (SRVpointer != nullptr) {
		GPGPU::releaseResource(SRVpointer);
	}
	if (UAVpointer != nullptr) {
		GPGPU::releaseResource(UAVpointer);
	}
	GPGPU::releaseResource(bufferPointer);
}

unsigned short Buffer::getGPUslot() const
{
	return GPUslot;
}

bool Buffer::isEnabled() const
{
	return enabled;
}

BUFFER_VIEW_TYPE Buffer::getViewType() const
{
	return type;
}

void Buffer::setView()
{
	if (type == BUFFER_VIEW_TYPE::SRV && SRVpointer != nullptr) {
		this->shader->getGPUDevice()->getContext()->CSSetShaderResources(GPUslot, 1, &SRVpointer);
	}else if(type == BUFFER_VIEW_TYPE::UAV && UAVpointer != nullptr) {
		this->shader->getGPUDevice()->getContext()->CSSetUnorderedAccessViews(GPUslot, 1, &UAVpointer, (UINT*)&UAVpointer);
	}
	viewLoaded = true;
}

void Buffer::removeView()
{
	// Specifically, if the only reference to an object is if you use set it on the device context 
	// (a.k.a.bind it to the rendering pipeline), that's not enough to keep it alive because those methods 
	// don't increment / decrement the reference count.
	if (type == BUFFER_VIEW_TYPE::SRV && SRVpointer != nullptr) {
		ID3D11ShaderResourceView* ppSRViewNULL[1] = { NULL }; // shader resource view need a pointer to an array! 
		this->shader->getGPUDevice()->getContext()->CSSetShaderResources(GPUslot, 1, ppSRViewNULL);
	}
	else if (type == BUFFER_VIEW_TYPE::UAV && UAVpointer != nullptr) {
		ID3D11UnorderedAccessView* ppUAViewNULL[1] = { NULL };
		this->shader->getGPUDevice()->getContext()->CSSetUnorderedAccessViews(GPUslot, 1, ppUAViewNULL, (UINT*)(&ppUAViewNULL));
	}
	viewLoaded = false;
}

HRESULT Buffer::createShaderResourceView()
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	bufferPointer->GetDesc(&descBuf);

	D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	desc.BufferEx.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		// Raw Buffer case

		desc.Format = DXGI_FORMAT_R32_TYPELESS;
		desc.BufferEx.Flags = D3D11_BUFFEREX_SRV_FLAG_RAW;
		desc.BufferEx.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		// Structured Buffer case

		desc.Format = DXGI_FORMAT_UNKNOWN;
		desc.BufferEx.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		return E_INVALIDARG;
	}

	auto res =  this->shader->getGPUDevice()->getDevice()->CreateShaderResourceView(bufferPointer, &desc, &SRVpointer);

	if (res == S_OK) {
		type = BUFFER_VIEW_TYPE::SRV;
	}

	return res;
}

HRESULT Buffer::createUnorderedAccessView()
{
	D3D11_BUFFER_DESC descBuf;
	ZeroMemory(&descBuf, sizeof(descBuf));
	bufferPointer->GetDesc(&descBuf);

	D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
	desc.Buffer.FirstElement = 0;

	if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_ALLOW_RAW_VIEWS)
	{
		// This is a Raw Buffer

		desc.Format = DXGI_FORMAT_R32_TYPELESS; // Format must be DXGI_FORMAT_R32_TYPELESS, when creating Raw Unordered Access View
		desc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_RAW;
		desc.Buffer.NumElements = descBuf.ByteWidth / 4;
	}
	else if (descBuf.MiscFlags & D3D11_RESOURCE_MISC_BUFFER_STRUCTURED)
	{
		// This is a Structured Buffer

		desc.Format = DXGI_FORMAT_UNKNOWN; // Format must be must be DXGI_FORMAT_UNKNOWN, when creating a View of a Structured Buffer
		desc.Buffer.NumElements = descBuf.ByteWidth / descBuf.StructureByteStride;
	}
	else
	{
		return E_INVALIDARG;
	}

	auto res =  this->shader->getGPUDevice()->getDevice()->CreateUnorderedAccessView(bufferPointer, &desc, &UAVpointer);

	if (res == S_OK) {
		type = BUFFER_VIEW_TYPE::UAV;
	}

	return res;
}

HRESULT Buffer::createStructuredBuffer(void* dataPtr)
{
	// D3D11_USAGE_IMMUTABLE flag on D3D11_USAGE ?
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.Usage = D3D11_USAGE_DEFAULT;
	/*if ((this->CPURequirement == CPU_IO::NONE)) {
		desc.Usage = D3D11_USAGE_IMMUTABLE;
	}
	if ((this->CPURequirement == CPU_IO::WRITE))
	{
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.Usage = D3D11_USAGE_DYNAMIC;
	}*/
	desc.ByteWidth = this->length * this->elementDimension; // size in byte
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = this->elementDimension; // number of elements

	if (dataPtr)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = dataPtr;
		return this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, &InitData, &bufferPointer);
	}
	else
		return this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, NULL, &bufferPointer);

}

HRESULT Buffer::createRWStructuredBuffer(void* dataPtr)
{
	// it's same of createStructuredBuffer since it doesn't use D3D11_USAGE_IMMUTABLE flag on D3D11_USAGE - fixed now
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	desc.ByteWidth = this->length * this->elementDimension; // size in byte
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	desc.StructureByteStride = this->elementDimension; // number of elements

	if (dataPtr)
	{
		D3D11_SUBRESOURCE_DATA InitData;
		InitData.pSysMem = dataPtr;
		return this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, &InitData, &bufferPointer);
	}
	else
		return this->shader->getGPUDevice()->getDevice()->CreateBuffer(&desc, NULL, &bufferPointer);

}
