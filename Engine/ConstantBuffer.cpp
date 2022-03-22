#include "pch.h"
#include "ConstantBuffer.h"
#include "Engine.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
	if (_cbvBuffer)
	{
		if (_cbvBuffer != nullptr)
			_cbvBuffer->Unmap(0, nullptr);

		_cbvBuffer = nullptr;
	}
}

void ConstantBuffer::Init(uint32 size, uint32 count)
{
	// 상수 버퍼는 256 바이트 배수로 만들어야 한다(원칙)
	// 0 256 512 768 ... 
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	// 255  : 0000000011111111
	// ~255 : 1111111100000000  반전
	// ~255와 아무 숫자 & 연산하면, 결국 하위 8비트를 모두 0으로 만든다는 의미
	// -> 256의 배수가 됨(계산기로 해봐도됨)
	// size + 255는 올림을 하기 위함, 또한 256의 배수인 size를 입력하면, 255를 더해서 내림하기 위함
	// 255안 덜할 경우: size 299 입력시 256이 출력됨(내림)

	// 예시: 296을 size로 입력하면, _elementSize는 512가 됨
	// 예시: 555를 size로 입력하면, _elementSize는 768이 됨

	CreateBuffer();
}

void ConstantBuffer::CreateBuffer()
{
	uint32 bufferSize = _elementSize * _elementCount;
	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_cbvBuffer));

	_cbvBuffer->Map(0, nullptr, reinterpret_cast<void**>(&_mappedBuffer));
	// We do not need to unmap until we are done with the resource.  However, we must not write to
	// the resource while it is in use by the GPU (so we must use synchronization techniques).
	// gpu가 해당부분 쓸 때는 건들면안됨, (현재는 WaitSync로 gpu동작중에는 그냥 기다려서 상관없음, 원시적)
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0; // 0으로 바꾸고, 다음 쓸 때는 덮어쓰기(매 프레임마다 새로 써주기 위함)
}

void ConstantBuffer::PushData(int32 rootParamIndex, void* buffer, uint32 size)
{
	assert(_currentIndex < _elementSize); // 버퍼 크기 체크

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);
	CMD_LIST->SetGraphicsRootConstantBufferView(rootParamIndex, address); // 레지스터에 버퍼주소 입력
	_currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}