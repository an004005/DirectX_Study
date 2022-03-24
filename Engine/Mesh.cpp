#include "pch.h"
#include "Mesh.h"
#include "Engine.h"
#include "Material.h"

void Mesh::Init(const vector<Vertex>& vertexBuffer, const vector<uint32>& indexBuffer)
{
	CreateVertexBuffer(vertexBuffer);
	CreateIndexBuffer(indexBuffer);
}

// �׷��ִ� �Լ�, ������ �κ��� cmdList�� �о�־��ش�.
void Mesh::Render()
{
	// cmdList�� ������ �κ� ����
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	CMD_LIST->IASetIndexBuffer(&_indexBufferView);

	CONST_BUFFER(CONSTANT_BUFFER_TYPE::TRANSFORM)->PushData(&_transform, sizeof(_transform));
	_mat->Update();

	GEngine->GetTableDescHeap()->CommitTable();
		
	// CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
	CMD_LIST->DrawIndexedInstanced(_indexCount, 1, 0, 0, 0);
}

// Root Constant ����� ��
// 1) buffer(gpu��)�� ������ ����(Mesh:init���� ��)
// 2) buffer�� �ּҸ� register�� ����(Mesh:Render���� ��)
// buffer�� �ִ½����� register�� �����ϴ� ������ �ٸ��� ������ �̸� �� ����ϰ� cmdQ�� �������

// Table desc Heap ����� ��
// 1) buffer(gpu��)�� ������ ����(Mesh:init���� ��)
// 2) TableDescHeap�� CBV ����
// 3) ��� ������ ������ TableDescHeap�� commit

void Mesh::CreateVertexBuffer(const vector<Vertex>& buffer) // ���� ��� �ޱ�
{
	_vertexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// gpu �ʿ� ����(�޸�)�Ҵ�)(���� ����)
	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_vertexBuffer));

	// Copy the triangle data to the vertex buffer.
	void* vertexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);// �Ѳ� ����
	::memcpy(vertexDataBuffer, &buffer[0], bufferSize); // �޸� ����
	_vertexBuffer->Unmap(0, nullptr); // �Ѳ��ݱ�
	// gpu�� �޸𸮿� �����ϴ� ����


	// Initialize the vertex buffer view. �������� Ȱ���� view�� �̿�(���۸� �����ϴ� view)
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��

	// ����: ���� ������ ��ü?
	// view: �� �����͸� ����
}

void Mesh::CreateIndexBuffer(const vector<uint32>& buffer)
{
	_indexCount = static_cast<uint32>(buffer.size());
	uint32 bufferSize = _indexCount * sizeof(uint32);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	DEVICE->CreateCommittedResource(
		&heapProperty,
		D3D12_HEAP_FLAG_NONE,
		&desc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&_indexBuffer));

	void* indexDataBuffer = nullptr;
	CD3DX12_RANGE readRange(0, 0); // We do not intend to read from this resource on the CPU.
	_indexBuffer->Map(0, &readRange, &indexDataBuffer);
	::memcpy(indexDataBuffer, &buffer[0], bufferSize);
	_indexBuffer->Unmap(0, nullptr);

	_indexBufferView.BufferLocation = _indexBuffer->GetGPUVirtualAddress();
	_indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	_indexBufferView.SizeInBytes = bufferSize;
}