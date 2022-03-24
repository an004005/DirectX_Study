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

void ConstantBuffer::Init(CBV_REGISTER reg, uint32 size, uint32 count)
{
	_reg = reg;

	// ��� ���۴� 256 ����Ʈ ����� ������ �Ѵ�(��Ģ)
	// 0 256 512 768 ... 
	_elementSize = (size + 255) & ~255;
	_elementCount = count;

	// 255  : 0000000011111111
	// ~255 : 1111111100000000  ����
	// ~255�� �ƹ� ���� & �����ϸ�, �ᱹ ���� 8��Ʈ�� ��� 0���� ����ٴ� �ǹ�
	// -> 256�� ����� ��(����� �غ�����)
	// size + 255�� �ø��� �ϱ� ����, ���� 256�� ����� size�� �Է��ϸ�, 255�� ���ؼ� �����ϱ� ����
	// 255�� ���� ���: size 299 �Է½� 256�� ��µ�(����)

	// ����: 296�� size�� �Է��ϸ�, _elementSize�� 512�� ��
	// ����: 555�� size�� �Է��ϸ�, _elementSize�� 768�� ��

	CreateBuffer();
	CreateView();
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
	// gpu�� �ش�κ� �� ���� �ǵ��ȵ�, (����� WaitSync�� gpu�����߿��� �׳� ��ٷ��� �������, ������)
}

void ConstantBuffer::CreateView()
{
	D3D12_DESCRIPTOR_HEAP_DESC cbvDesc = {};
	cbvDesc.NumDescriptors = _elementCount; // ������ ������ _elementCount��ŭ ����� ����
	cbvDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // cpu ram�� �����ϵ��� �ϴ� �÷���
	cbvDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DEVICE->CreateDescriptorHeap(&cbvDesc, IID_PPV_ARGS(&_cbvHeap));

	_cpuHandleBegin = _cbvHeap->GetCPUDescriptorHandleForHeapStart();
	// gpu���帶�� �ٸ��� �ֱ� ������ �� �Լ� ����Ѵ�.
	_handleIncrementSize = DEVICE->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	for (uint32 i = 0; i < _elementCount; ++i)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE cbvHandle = GetCpuHandle(i);

		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
		cbvDesc.BufferLocation = _cbvBuffer->GetGPUVirtualAddress() + static_cast<uint64>(_elementSize) * i;
		cbvDesc.SizeInBytes = _elementSize;   // CB size is required to be 256-byte aligned.

		DEVICE->CreateConstantBufferView(&cbvDesc, cbvHandle);
	}
}

void ConstantBuffer::Clear()
{
	_currentIndex = 0; // 0���� �ٲٰ�, ���� �� ���� �����(�� �����Ӹ��� ���� ���ֱ� ����)
}

void ConstantBuffer::PushData(void* buffer, uint32 size)
{
	assert(_currentIndex < _elementCount); // ���� ũ�� üũ, debug���� �����
	assert(_elementSize == ((size + 255) & ~255));

	::memcpy(&_mappedBuffer[_currentIndex * _elementSize], buffer, size);

	D3D12_GPU_VIRTUAL_ADDRESS address = GetGpuVirtualAddress(_currentIndex);

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = GetCpuHandle(_currentIndex);
	GEngine->GetTableDescHeap()->SetCBV(cpuHandle, _reg);
	_currentIndex++;
}

D3D12_GPU_VIRTUAL_ADDRESS ConstantBuffer::GetGpuVirtualAddress(uint32 index)
{
	D3D12_GPU_VIRTUAL_ADDRESS objCBAddress = _cbvBuffer->GetGPUVirtualAddress();
	objCBAddress += index * _elementSize;
	return objCBAddress;
}

D3D12_CPU_DESCRIPTOR_HANDLE ConstantBuffer::GetCpuHandle(uint32 index)
{
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(_cpuHandleBegin, index * _handleIncrementSize);
}