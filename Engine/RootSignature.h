#pragma once

// 계약서/ 결재 단계에 해당함.

// cpu [ ] , gpu [ ]
// 한국 [ ] 베트남 [ ]
// cpu에 있는 정보를 gpu에 

class RootSignature
{
public:
	void Init();
	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	void CreateSamplerDesc();
	void CreateRootSignature();

private:
	ComPtr<ID3D12RootSignature> _signature;
	D3D12_STATIC_SAMPLER_DESC _samplerDesc;
};

