#pragma once

// 계약서/ 결재 단계에 해당함.

// cpu [ ] , gpu [ ]
// 한국 [ ] 베트남 [ ]
// cpu에 있는 정보를 gpu에 

class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);
	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature> _signature;
};

