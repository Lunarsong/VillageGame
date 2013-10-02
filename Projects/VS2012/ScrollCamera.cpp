#include "ScrollCamera.h"
#include <Core/Input/InputManager.h>


ScrollCamera::ScrollCamera(void)
{
	m_fSpeed = 500.0f;
	m_fZoomModifier = 1.0f;

	m_vMinBounds = Vector3( -4096.0f, -4096.0f );
	m_vMaxBounds = Vector3( 4096.0f, 4096.0f );

	Matrix matProjection;
	matProjection.BuildOrthoLH( (float)IRenderer::Get()->VGetScreenWidth(), (float)IRenderer::Get()->VGetScreenHeight(), -1.0f, 1.0f );
	SetProjection( matProjection );

	m_bMouseDrag = false;
	InputManager::Get()->AddKeyboardHandler( this );
	InputManager::Get()->AddMouseHandler( this );
	InputManager::Get()->AddTouchHandler( this );
}



ScrollCamera::~ScrollCamera(void)
{
	InputManager::Get()->RemoveKeyboardHandler( this );
	InputManager::Get()->RemoveMouseHandler( this );
	InputManager::Get()->RemoveTouchHandler( this );
}

bool ScrollCamera::VOnKeyDown( InputKeyCodes eKeyCode )
{
	switch ( eKeyCode )
	{
	case KEY_LEFT:
		m_vDirection.x -= 1.0f;
		break;

	case KEY_RIGHT:
		m_vDirection.x += 1.0f;
		break;
		
	case KEY_UP:
		m_vDirection.y += 1.0f;
		break;

	case KEY_DOWN:
		m_vDirection.y -= 1.0f;
		break;

	default:
		break;
	}

	return false;
}

bool ScrollCamera::VOnKeyUp( InputKeyCodes eKeyCode )
{
	switch ( eKeyCode )
	{
	case KEY_LEFT:
		m_vDirection.x += 1.0f;
		break;

	case KEY_RIGHT:
		m_vDirection.x -= 1.0f;
		break;

	case KEY_UP:
		m_vDirection.y -= 1.0f;
		break;

	case KEY_DOWN:
		m_vDirection.y += 1.0f;
		break;

	default:
		break;
	}

	return false;
}

bool ScrollCamera::VOnTouchStart( const int iTouchIndex, const Vector2& vPosition, const int iPressure )
{
	return false;
}

bool ScrollCamera::VOnTouchEnd( const int iTouchIndex, const Vector2& vPosition, const int iPressure )
{
	return false;
}

bool ScrollCamera::VOnTouchCancel( const int iTouchIndex, const Vector2& vPosition, const int iPressure )
{
	return false;
}

bool ScrollCamera::VOnTouchMove( const int iTouchIndex, const Vector2& vPosition, const Vector2& vDeltaPosition, const int iPressure )
{
	return false;
}

bool ScrollCamera::VOnMouseMove( const Vector3& vPosition, const Vector3& vDeltaPosition )
{
	if ( m_bMouseDrag )
	{
		Move( Vector4( -vDeltaPosition.x * m_fZoomModifier, vDeltaPosition.y * m_fZoomModifier, 0.0f, 0.0f ) );
	}

	return false;
}

bool ScrollCamera::VOnMouseButtonDown( const int iButtonIndex, const Vector3& vPosition )
{
	if ( iButtonIndex == 2 )
	{
		m_bMouseDrag = true;
	}

	return false;
}

bool ScrollCamera::VOnMouseButtonUp( const int iButtonIndex, const Vector3& vPosition )
{
	if ( iButtonIndex == 2 )
	{
		m_bMouseDrag = false;
	}

	return false;
}

bool ScrollCamera::VOnMouseButtonDClick( const int iButtonIndex, const Vector3& vPosition )
{
	return false;
}

bool ScrollCamera::VOnMouseWheel( const Vector3& vPosition, const Vector3& vDelta )
{
	Matrix matScale;
	if ( vDelta.y > 0.0f )
	{
		matScale.BuildScale( 2.0f, 2.0f, 2.0f );
		m_fZoomModifier *= 0.5f;
	}
	else
	{
		m_fZoomModifier *= 2.0f;
		matScale.BuildScale( 0.5f, 0.5f, 0.5f );
	}

	SetProjection( GetProjection() * matScale );

	return false;
}

void ScrollCamera::VUpdate( float fDeltaSeconds )
{
	Move( m_vDirection * m_fSpeed * fDeltaSeconds );

	Vector4 vPosition = GetPosition();
	if ( vPosition.x > m_vMaxBounds.x )
	{
		vPosition.x = m_vMaxBounds.x;
	}

	else if ( vPosition.x < m_vMinBounds.x )
	{
		vPosition.x = m_vMinBounds.x;
	}

	if ( vPosition.y > m_vMaxBounds.y )
	{
		vPosition.y = m_vMaxBounds.y;
	}

	else if ( vPosition.y < m_vMinBounds.y )
	{
		vPosition.y = m_vMinBounds.y;
	}

	SetPosition( vPosition );
}