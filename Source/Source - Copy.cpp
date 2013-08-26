#include "Engine.h"
#include <Rendering/Types/ColorF.h>

using namespace Engine;

const char vShaderStr[] = 
	"\n\
	uniform mat4 mWorldViewProjection;										\n\
																			\n\
	attribute vec4 u_vPosition;												\n\
	attribute vec2 u_vTexCoords;											\n\
																			\n\
	varying vec2 v_vTexCoord;												\n\
	void main()																\n\
	{																		\n\
		v_vTexCoord = u_vTexCoords;											\n\
		gl_Position = mWorldViewProjection * u_vPosition;					\n\
	}																		\n\
	";

const char fShaderStr[] = 
	"																		\n\
	precision mediump float;												\n\
    uniform vec4 u_Color;													\n\
																			\n\
	uniform sampler2D s_Texture01;											\n\
																			\n\
	varying vec2 v_vTexCoord;												\n\
																			\n\
	void main()																\n\
	{																		\n\
		vec4 vTextureColor = texture2D( s_Texture01, v_vTexCoord );			\n\
		gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0) * u_Color * vTextureColor;	\n\
	}																		\n\
	";

IShaderProgram* pShaderProgram = NULL;
IVertexBuffer*	pVertexBuffer = NULL;
IBuffer*		pBuffer = NULL;
IBuffer*		pPixelConstantBuffer = NULL;
ITexture*		pTexture = NULL;

void Start()
{
	ResourceCache::Get()->AddResourceFile( "Working Folder", new DevelopmentResourceZipFile( FileUtils::GetWorkingFolder(), DevelopmentResourceZipFile::Editor ) );
	shared_ptr<BinaryResource> pResource = ResourceCache::Get()->GetResource<BinaryResource>( "test.jpg");

	pTexture = IRenderer::CreateTexture();
	pTexture->VCreate( pResource->Buffer(), pResource->Size() );

	IVertexShader* pVertexShader = IRenderer::CreateVertexShader();
	IPixelShader* pPixelShader = IRenderer::CreatePixelShader();
	pShaderProgram = IRenderer::CreateShaderProgram();

	pVertexShader->VAddAttribute( "u_vPosition", Position_VertexData );
	pVertexShader->VAddAttribute( "u_vTexCoords", UV_VertexData );
	pVertexShader->VCreateFromMemory( vShaderStr, sizeof( vShaderStr ) );
	pPixelShader->VCreateFromMemory( fShaderStr, sizeof( fShaderStr ) );

	pShaderProgram->VSetVertexShader( pVertexShader );
	pShaderProgram->VSetPixelShader( pPixelShader );
	pShaderProgram->VLink();

	pVertexShader->Release();
	pPixelShader->Release();	

	VertexPositionTexture vertices[3] =
	{
		VertexPositionTexture( Vector3( 0.0f, 0.5f, 0.0f ), Vector2( 1.0f, 1.0f ) ),
		VertexPositionTexture( Vector3( -0.5f, -0.5f, 0.0f ), Vector2( 0.0f, 0.0f ) ),
		VertexPositionTexture( Vector3( 0.5f, -0.5f, 0.0f ), Vector2( 1.0f, 0.0f ) )
	};

	pVertexBuffer = IRenderer::CreateVertexBuffer();
	pVertexBuffer->VCreate( vertices, sizeof( VertexPositionTexture ), 3 );

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

	// Set the data buffers
	Matrix mat;
    mat.SetPosition( Vector4( 0.5f, 0.5f, 0.0f, 1.0f ) );
	pBuffer->VSetData( pRenderContext, &mat );
    pPixelConstantBuffer->VSetData( pRenderContext, &ColorF::BLUE );

	// Set the shader and its buffers
	pRenderContext->VSetShaderProgram( pShaderProgram );
    pRenderContext->VSetPixelShaderBuffer( pPixelConstantBuffer );
	pRenderContext->VSetPixelShaderResource( pTexture, 0 );
	pRenderContext->VSetVertexShaderBuffer( pBuffer );

	// Set the vertex buffer and draw
	pRenderContext->VSetVertexBuffer( pVertexBuffer );	
	pRenderContext->VDraw( 3 );
}

void End()
{
	SAFE_RELEASE( pTexture );
    SAFE_RELEASE( pPixelConstantBuffer );
	SAFE_RELEASE( pBuffer );
	SAFE_RELEASE( pShaderProgram );
	SAFE_RELEASE( pVertexBuffer );
}