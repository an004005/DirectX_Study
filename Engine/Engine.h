#pragma once

// header에 header를 넣는건 안좋은 습관이지만,
// engine에 다 한번에 넣어도 되는 코드를 관리차원으로 분리했기 때문에
// 이렇게 넣는다. 매크로 쉽게 쓰기 위해서 추가(EnginePch)
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "RootSignature.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "ConstantBuffer.h"
#include "TableDescriptorHeap.h"
#include "DepthStencilBuffer.h"

#include "Timer.h"
#include "Input.h"

class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

	void Update();

	// cmd q에 요청사항일 넣는 함수
	void RenderBegin();

	// cmd q 에 넣은 요청을 gpu에 넘겨주는 함수
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

	shared_ptr<Device> GetDevice() { return _device;  }
	shared_ptr<CommandQueue> GetCmdQueue() { return _cmdQueue;  }
	shared_ptr<SwapChain> GetSwapChain() { return _swapChain;  }
	shared_ptr<RootSignature> GetRootSignature() { return _rootSignature;  }
	shared_ptr<TableDescriptorHeap> GetTableDescHeap() { return _tableDescHeap;  }
	shared_ptr<DepthStencilBuffer> GetDepthStencilBuffer() { return _depthStencilBuffer;  }

	shared_ptr<Input> GetInput() { return _input; }
	shared_ptr<Timer> GetTimer() { return _timer; }

	shared_ptr<ConstantBuffer> GetConstantBuffer(CONSTANT_BUFFER_TYPE type) { return _constantBuffers[static_cast<uint8>(type)]; }

private:
	void ShowFps();
	void CreateConstantBuffer(CBV_REGISTER reg, uint32 bufferSize, uint32 count);


private:
	// 그려질 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	shared_ptr<Device> _device = make_shared<Device>();
	shared_ptr<CommandQueue> _cmdQueue = make_shared<CommandQueue>();
	shared_ptr<SwapChain> _swapChain = make_shared<SwapChain>();
	shared_ptr<RootSignature> _rootSignature = make_shared<RootSignature>();
	shared_ptr<TableDescriptorHeap> _tableDescHeap = make_shared<TableDescriptorHeap>();
	shared_ptr<DepthStencilBuffer> _depthStencilBuffer = make_shared<DepthStencilBuffer>();

	shared_ptr<Input> _input = make_shared<Input>();
	shared_ptr<Timer> _timer = make_shared<Timer>();

	vector<shared_ptr<ConstantBuffer>> _constantBuffers;
};

