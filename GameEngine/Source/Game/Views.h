#pragma once

class Camera;
class IMouseHandler;
class IKeyboardHandler;
class MovementController;
class Widget;
struct SystemMessage;
class Renderer;
class IView
{
public:
	virtual ~IView() {}

	virtual bool VInit() = 0;
	virtual bool VRestore() = 0;
	virtual void VUpdate(unsigned int deltaMilliseconds) = 0;
	virtual void VRender(Renderer* pRenderer, double time, double elapsedTime) = 0;
	virtual bool VMsgProc(const SystemMessage * msg) = 0;

	virtual	void VSetCamera(shared_ptr<Camera> pCamera) = 0;
	virtual	shared_ptr<Camera> VGetCamera() const = 0;
	virtual	bool VHasCamera() const = 0;
};

class PlayerView : public IView
{
protected:
	shared_ptr<IMouseHandler> m_pMouseHandler;
	shared_ptr<IKeyboardHandler> m_pKeyboardHandler;
	shared_ptr<MovementController> m_pController;

public:
	PlayerView();

	virtual bool VInit() override;
	virtual bool VRestore() override;
	virtual void VUpdate(unsigned int deltaMilliseconds) override;
	virtual void VRender(Renderer* pRenderer, double time, double elapsedTime) override;
	virtual bool VMsgProc(const SystemMessage * msg) override;

protected:
	/* ==================================
	//	Widgets
	=================================== */
	vector<shared_ptr<Widget>> m_widgets;

	/* ==================================
	// Game Rendering
	=================================== */
	shared_ptr<Camera> m_pCamera;
	bool m_bHasCamera;

	float bgColor[4];

public:

	//Widgets
	virtual void AddWidget(shared_ptr<Widget> pWidget);
	virtual bool RemoveWidget(shared_ptr<Widget> pWidget);

	//Game Rendering
	virtual void VSetCamera(shared_ptr<Camera> pCamera);
	virtual shared_ptr<Camera> VGetCamera() const;
	virtual bool VHasCamera() const;

	void SetMouseHandler(shared_ptr<IMouseHandler> pHandler);
	void SetKeyboardHandler(shared_ptr<IKeyboardHandler> pHandler);
	void SetController(shared_ptr<MovementController> pController);

	//Accessors
	shared_ptr<MovementController> GetController() const;
};

__forceinline PlayerView::PlayerView()
{}

__forceinline void PlayerView::VSetCamera(shared_ptr<Camera> pCamera)
{
	m_pCamera = pCamera;
	m_bHasCamera = true;
}

__forceinline shared_ptr<Camera> PlayerView::VGetCamera() const 
{
	return m_pCamera; 
}

__forceinline bool PlayerView::VHasCamera() const
{
	return m_bHasCamera; 
}

__forceinline void PlayerView::SetMouseHandler(shared_ptr<IMouseHandler> pHandler)
{ 
	m_pMouseHandler = pHandler; 
}

__forceinline void PlayerView::SetKeyboardHandler(shared_ptr<IKeyboardHandler> pHandler)
{
	m_pKeyboardHandler = pHandler; 
}

__forceinline void PlayerView::SetController(shared_ptr<MovementController> pController)
{
	m_pController = pController; 
}

__forceinline shared_ptr<MovementController> PlayerView::GetController() const
{ 
	return m_pController; 
}