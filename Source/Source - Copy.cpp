#include "Engine.h"
#include <Rendering/Types/ColorF.h>

using namespace Engine;

const char vShaderStr[] = 
	"\n\
	uniform mat4 mWorldViewProjection;										\n\
																			\n\
	attribute vec4 vPosition;												\n\
	void main()																\n\
	{																		\n\
		gl_Position = mWorldViewProjection * vPosition;						\n\
	}																		\n\
	";

const char fShaderStr[] = 
	"precision mediump float; \n"
    "uniform vec4 u_Color;"
	"void main() \n"
	"{ \n"
	" gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * u_Color; \n"
	"} \n";

IShaderProgram* pShaderProgram = NULL;
IVertexBuffer* pVertexBuffer = NULL;
IBuffer*		pBuffer = NULL;
IBuffer*		pPixelConstantBuffer = NULL;
void Start()
{
	IVertexShader* pVertexShader = IRenderer::CreateVertexShader();
	IPixelShader* pPixelShader = IRenderer::CreatePixelShader();
	pShaderProgram = IRenderer::CreateShaderProgram();

	pVertexShader->VAddAttribute( "vPosition", Position_VertexData );
	pVertexShader->VCreateFromMemory( vShaderStr, sizeof( vShaderStr ) );
	pPixelShader->VCreateFromMemory( fShaderStr, sizeof( fShaderStr ) );

	pShaderProgram->VSetVertexShader( pVertexShader );
	pShaderProgram->VSetPixelShader( pPixelShader );
	pShaderProgram->VLink();

	pVertexShader->Release();
	pPixelShader->Release();	

	Vector3 vertices[3] =
	{
		Vector3( 0.0f, 0.5f, 0.0f ),
		Vector3( -0.5f, -0.5f, 0.0f ),
		Vector3( 0.5f, -0.5f, 0.0f )
	};

	pVertexBuffer = IRenderer::CreateVertexBuffer();
	pVertexBuffer->VCreate( vertices, sizeof( Vector3 ), 3 );

	pBuffer = IRenderer::CreateBuffer();
	pBuffer->VCreate( 1, sizeof( Matrix ), ConstantBuffer, true );
	pBuffer->VAddProperty( "mWorldViewProjection", BP_MATRIX4 );
    
    pPixelConstantBuffer = IRenderer::CreateBuffer();
	pPixelConstantBuffer->VCreate( 1, sizeof( Vector4 ), ConstantBuffer, true );
	pPixelConstantBuffer->VAddProperty( "u_Color", BP_VECTOR4 );
}

void Update( float fDeltaSeconds )
{
	static ColorF color = ColorF::BLACK;

	static float fColorSpeed = 1.0f;
	color.Red += fDeltaSeconds * fColorSpeed;
	color.Green += fDeltaSeconds * fColorSpeed;
	color.Blue += fDeltaSeconds * fColorSpeed;

	if ( color.Red >= 1.0f || color.Red <= 0.0f )
	{
		fColorSpeed *= -1.0f;
	}	

	BaseApplication::Get()->SetClearColor( color );
}

void Render()
{
	IRenderContext* pRenderContext = IRenderer::Get()->VGetMainContext();
	Matrix mat;
    mat.SetPosition( Vector4( 0.5f, 0.5f, 0.0f, 1.0f ) );
	pBuffer->VSetData( pRenderContext, &mat );
    pPixelConstantBuffer->VSetData( pRenderContext, &ColorF::BLUE );
	pRenderContext->VSetShaderProgram( pShaderProgram );
    pRenderContext->VSetPixelShaderBuffer( pPixelConstantBuffer );
	pRenderContext->VSetVertexShaderBuffer( pBuffer );
	pRenderContext->VSetVertexBuffer( pVertexBuffer );
	pRenderContext->VDraw( 3 );
}

void End()
{
    SAFE_RELEASE( pPixelConstantBuffer );
	SAFE_RELEASE( pBuffer );
	SAFE_RELEASE( pShaderProgram );
	SAFE_RELEASE( pVertexBuffer );
}