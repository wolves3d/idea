#include "Common.h"
#include "ProjectedGrid.h"


CProjectedGrid::CProjectedGrid()
	: m_vertexDecl(NULL)
	, m_Material(NULL)
	, m_pProjector(NULL)
{
	m_gridWidth = 64;
	m_gridHeight = 64;

	Init();
}


CProjectedGrid::~CProjectedGrid()
{
	Release();
}


void CProjectedGrid::Release()
{
	RELEASE(m_vertexBuffer);
	RELEASE(m_Material);
	DEL(m_vertexDecl);
}


void CProjectedGrid::Init()
{
	m_gridVertexCount = (m_gridWidth * m_gridHeight);

	m_vertexDecl = g_pRenderer->CreateVertexDecl();
	m_vertexDecl->SetStride(sizeof(GridVertex));
	m_vertexDecl->AddAttr("vPos", NULL, TYPE_VEC3, 0);
	m_vertexDecl->AddAttr("vTex", NULL, TYPE_VEC2, sizeof(vec3));

	m_Material = g_pEngine->CreateMaterial();
	m_Material->SetShader(g_pRenderer->GetShader("Land"));
	m_Material->SetTexture(DIFFUSE_MAP, g_pRenderer->GetSysTexture(EST_WHITE));

	m_mvpUniform.AttachToShader("mMVP", m_Material->GetShader());
	m_mvpUniform.Connect(m_Material);

	if (m_vertexBuffer = g_pRenderer->CreateVB())
	{
		uint sizeInBytes = (sizeof(GridVertex)* m_gridVertexCount);
		if (m_vertexBuffer->Alloc(sizeInBytes, 0))
		{
			GridVertex * pVertices = (GridVertex *)(m_vertexBuffer->Lock(0, sizeInBytes));
			if (NULL != pVertices)
			{
				for (uint z = 0; z < m_gridHeight; ++z)
				{
					for (uint x = 0; x < m_gridWidth; ++x)
					{
						uint offset = (z * m_gridHeight) + x;
						GridVertex * vertex = (pVertices + offset);

						vertex->vPos.Set(0.5f * x, 0, 0.5f * z);
						vertex->vTex.Set(((float)x / (m_gridWidth - 1)), ((float)z / (m_gridHeight - 1)));
					}
				}
			}
			m_vertexBuffer->Unlock();
		}
	}


	m_pProjector = g_pEngine->CreateCamera();
	m_pProjector->SetParams(34.516f, 32, 32, 1, 5);
	
	//m_pProjector->SetPos(vec3(0, 0, 0));
}

float wave = 0;

void CProjectedGrid::Update()
{
	uint sizeInBytes = (sizeof(GridVertex)* m_gridVertexCount);
	GridVertex * pVertices = (GridVertex *)(m_vertexBuffer->Lock(0, sizeInBytes));

	if (NULL == pVertices)
		return;
	
	for (uint z = 0; z < m_gridHeight; ++z)
	{
		float step = 10 * ((float)z / (m_gridHeight - 1));
		float h = sinf(wave + step);

		for (uint x = 0; x < m_gridWidth; ++x)
		{
			uint offset = (z * m_gridHeight) + x;
			GridVertex * vertex = (pVertices + offset);

			vertex->vPos.y = h;
			//vertex->vTex.Set(((float)x / (m_gridWidth - 1)), ((float)z / (m_gridHeight - 1)));
		}
	}
	
	wave += 0.001f;

	m_vertexBuffer->Unlock();

	//m_pProjector->SetPos(vec3(5*sinf(wave), 5, 5 * cosf(wave)));
	m_pProjector->SetPos(vec3(0, 5, 5));
	m_pProjector->LookAt(vec3(5 * sinf(wave), 0, 5 * cosf(wave)), vec3::vUp);
	//m_pProjector->LookAt(vec3(1, 0, 0), vec3::vUp);
}


void CProjectedGrid::Render()
{
	Update();

	if (false)
	{
		bbox testBox;
		testBox.vMin = vec3::vNull;
		testBox.vMax.Set(1, 1, 1);
		g_pEngine->PushBBox(testBox, 0xFFFFFFFF);
	}


	m_Material->Assign();

	const mat4 & mVP = g_pGame->m_pCamera->GetViewProjMatrix();
	m_mvpUniform.SetValue(mVP.pRows, 4);

	m_vertexBuffer->Render(m_vertexDecl, 0, m_gridVertexCount, PRIM_POINT);


	m_pProjector->RenderFrustum();
}