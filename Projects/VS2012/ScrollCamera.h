#pragma once

#include <Game/Entities/Components/Rendering/CameraComponent.h>
#include <Core/Input/InputHandlers.h>

using namespace Engine;

class ScrollCamera : 
	public CameraComponent,
	public IMouseHandler,
	public ITouchHandler,
	public IKeyboardHandler
{
public:
	enum TouchScroll
	{
		One,
		Two,

		TouchScrollCount
	};

	ScrollCamera(void);
	~ScrollCamera(void);

	virtual void VUpdate( float fDeltaSeconds );

	// Keyboard Handler
	virtual bool VOnKeyDown( InputKeyCodes eKeyCode );
	virtual bool VOnKeyUp( InputKeyCodes eKeyCode );
	virtual bool VOnStringInput( const std::string& strInput ) { return false; }

	virtual bool VOnTouchStart( const int iTouchIndex, const Vector2& vPosition, const int iPressure );
	virtual bool VOnTouchEnd( const int iTouchIndex, const Vector2& vPosition, const int iPressure );
	virtual bool VOnTouchCancel( const int iTouchIndex, const Vector2& vPosition, const int iPressure );
	virtual bool VOnTouchMove( const int iTouchIndex, const Vector2& vPosition, const Vector2& vDeltaPosition, const int iPressure );

	virtual bool VOnMouseMove( const Vector3& vPosition, const Vector3& vDeltaPosition );
	virtual bool VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition );
	virtual bool VOnMouseButtonUp( const int iButtonIndex, const Vector3& vPosition );
	virtual bool VOnMouseButtonDClick( const int iButtonIndex, const Vector3& vPosition );
	virtual bool VOnMouseWheel( const Vector3& vPosition, const Vector3& vDelta );

private:
	float	m_fSpeed;
	Vector4 m_vDirection;

	Vector3 m_vMinBounds;
	Vector3 m_vMaxBounds;

	bool m_bMouseDrag;
};

	

