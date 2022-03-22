#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec) // ���� ��� �ޱ�
{
	_vertexCount = static_cast<uint32>(vec.size());
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
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // �޸� ����
	_vertexBuffer->Unmap(0, nullptr); // �Ѳ��ݱ�
	// gpu�� �޸𸮿� �����ϴ� ����


	// Initialize the vertex buffer view. �������� Ȱ���� view�� �̿�(���۸� �����ϴ� view)
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // ���� 1�� ũ��
	_vertexBufferView.SizeInBytes = bufferSize; // ������ ũ��

	// ����: ���� ������ ��ü?
	// view: �� �����͸� ����
}

// �׷��ִ� �Լ�, ������ �κ��� cmdList�� �о�־��ش�.
void Mesh::Render()
{
	// cmdList�� ������ �κ� ����
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}