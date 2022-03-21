#pragma once
class Engine
{
public:
	void Init(const WindowInfo& window);
	void Render();

	// cmd q에 요청사항일 넣는 함수
	void RenderBegin();

	// cmd q 에 넣은 요청을 gpu에 넘겨주는 함수
	void RenderEnd();

	void ResizeWindow(int32 width, int32 height);

private:
	// 그려질 화면 크기 관련
	WindowInfo		_window;
	D3D12_VIEWPORT	_viewport = {};
	D3D12_RECT		_scissorRect = {};

	shared_ptr<class Device> _device;
	shared_ptr<class CommandQueue> _cmdQueue;
	shared_ptr<class SwapChain> _swapChain;
	shared_ptr<class DescriptorHeap> _descHeap;
};

