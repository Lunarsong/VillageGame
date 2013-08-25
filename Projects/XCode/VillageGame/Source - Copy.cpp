#include <Engine.h>
#include <Rendering/Types/ColorF.h>

using namespace Engine;

const char vShaderStr[] = 
	"attribute vec4 POSITION; \n"
	"void main() \n"
	"{ \n"
	" gl_Position = POSITION; \n"
	"} \n";

const char fShaderStr[] = 
	"precision mediump float; \n"
	"void main() \n"
	"{ \n"
	" gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0); \n"
	"} \n";

IShaderProgram* pShaderProgram = NULL;
IVertexBuffer* pVertexBuffer = NULL;
void Start()
{
	IVertexShader* pVertexShader = IRenderer::CreateVertexShader();
	IPixelShader* pPixelShader = IRenderer::CreatePixelShader();
	pShaderProgram = IRenderer::CreateShaderProgram();

	VertexLayout layout;
	VertexPosition::CreateLayout( layout );
	pVertexShader->VCreateFromMemory( vShaderStr, sizeof( vShaderStr ), &layout );
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
	pRenderContext->VSetShaderProgram( pShaderProgram );

	pRenderContext->VSetVertexBuffer( pVertexBuffer );
	pRenderContext->VDraw( 3 );
}

void End()
{
	SAFE_RELEASE( pShaderProgram );
	SAFE_RELEASE( pVertexBuffer );
}