#include "pch.h"
#include "Mesh.h"
#include "Engine.h"

void Mesh::Init(vector<Vertex>& vec) // 정점 목록 받기
{
	_vertexCount = static_cast<uint32>(vec.size());
	uint32 bufferSize = _vertexCount * sizeof(Vertex);

	D3D12_HEAP_PROPERTIES heapProperty = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC desc = CD3DX12_RESOURCE_DESC::Buffer(bufferSize);

	// gpu 쪽에 공간(메모리)할당)(버퍼 생성)
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
	_vertexBuffer->Map(0, &readRange, &vertexDataBuffer);// 뚜껑 열기
	::memcpy(vertexDataBuffer, &vec[0], bufferSize); // 메모리 복사
	_vertexBuffer->Unmap(0, nullptr); // 뚜껑닫기
	// gpu쪽 메모리에 복사하는 과정


	// Initialize the vertex buffer view. 실절적인 활용은 view를 이용(버퍼를 설명하는 view)
	_vertexBufferView.BufferLocation = _vertexBuffer->GetGPUVirtualAddress();
	_vertexBufferView.StrideInBytes = sizeof(Vertex); // 정점 1개 크기
	_vertexBufferView.SizeInBytes = bufferSize; // 버퍼의 크기

	// 버퍼: 실질 데이터 자체?
	// view: 이 데이터를 묘사
}

// 그려주는 함수, 랜더할 부분을 cmdList에 밀어넣어준다.
void Mesh::Render()
{
	// cmdList에 랜더할 부분 예약
	CMD_LIST->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CMD_LIST->IASetVertexBuffers(0, 1, &_vertexBufferView); // Slot: (0~15)

	D3D12_CPU_DESCRIPTOR_HANDLE handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
	GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b0);


	handle = GEngine->GetCB()->PushData(0, &_transform, sizeof(_transform));
	GEngine->GetTableDescHeap()->SetCBV(handle, CBV_REGISTER::b1);

	GEngine->GetTableDescHeap()->CommitTable();
		
	CMD_LIST->DrawInstanced(_vertexCount, 1, 0, 0);
}

// Root Constant 사용할 때
// 1) buffer(gpu램)에 데이터 셋팅(Mesh:init에서 함)
// 2) buffer의 주소를 register에 전송(Mesh:Render에서 함)
// buffer를 넣는시점과 register에 전송하는 시점이 다르기 때문에 이를 잘 고려하고 cmdQ를 사용하자

// Table desc Heap 사용할 때
// 1) buffer(gpu램)에 데이터 셋팅(Mesh:init에서 함)
// 2) TableDescHeap에 CBV 전달
// 3) 모든 세팅이 끝나면 TableDescHeap을 commit

