#pragma once

// ��༭/ ���� �ܰ迡 �ش���.

// cpu [ ] , gpu [ ]
// �ѱ� [ ] ��Ʈ�� [ ]
// cpu�� �ִ� ������ gpu�� 

class RootSignature
{
public:
	void Init(ComPtr<ID3D12Device> device);
	ComPtr<ID3D12RootSignature>	GetSignature() { return _signature; }

private:
	ComPtr<ID3D12RootSignature> _signature;
};
