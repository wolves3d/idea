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
	m_pProjector->SetParams(25, 32, 32, 1, 100);
	
	//m_pProjector->SetPos(vec3(0, 0, 0));
}

float wave = 0;

float CProjectedGrid::HeightFunc(float x, float z)
{
	return (0.3f * (sinf(4 * x + wave*2)));
}

void CProjectedGrid::Update()
{
	Project();

	mat4 v = m_pProjector->GetViewProjMatrix();
	v.Inverse();
	v = m_invViewProj;

	//v = m_rangeMatrix;
	//v.Mult(m_invViewProj);

	//v = m_projector;

	uint sizeInBytes = (sizeof(GridVertex)* m_gridVertexCount);
	GridVertex * pVertices = (GridVertex *)(m_vertexBuffer->Lock(0, sizeInBytes));

	if (NULL == pVertices)
		return;
	
	CPlane waterPlane(vec3::vUp, 0);

	for (uint z = 0; z < m_gridHeight; ++z)
	{
		float step = 10 * ((float)z / (m_gridHeight - 1));
		float h = 0.1f * sinf(wave + step);

		for (uint x = 0; x < m_gridWidth; ++x)
		{
			uint offset = (z * m_gridHeight) + x;
			GridVertex * vertex = (pVertices + offset);

			//if (false)
			{
				vec4 a(2 * ((float)x / (m_gridWidth - 1)) - 1, 2 * ((float)z / (m_gridHeight - 1)) - 1, -1, 1);
				vec4 b(2 * ((float)x / (m_gridWidth - 1)) - 1, 2 * ((float)z / (m_gridHeight - 1)) - 1, +1, 1);

				a.Multiply(v);
				b.Multiply(v);

				a.w = 1.f / a.w;
				b.w = 1.f / b.w;

				vec3 s((a.x * a.w), (a.y * a.w), (a.z * a.w));
				//s.Multiply(v);

				vec3 e((b.x * b.w), (b.y * b.w), (b.z * b.w));
				//e.Multiply(v);

				//g_pEngine->PushLine(s, e);

				vertex->vPos = vec3::vNull;
				waterPlane.LineCollision(&vertex->vPos, Line(s, e));

				//vertex->vPos.Set(((float)x / (m_gridWidth - 1)), ((float)z / (m_gridHeight - 1)), 0);
				vertex->vPos.y = h;
				vertex->vPos.y = HeightFunc(vertex->vPos.x, vertex->vPos.z);
				
				//vertex->vPos.Multiply(m);
			}
		}
	}
	
	wave += 0.001f;

	m_vertexBuffer->Unlock();

	//m_pProjector->SetPos(vec3(5*sinf(wave), 5, 5 * cosf(wave)));
	m_pProjector->SetPos(vec3(0, 5, 5));
	m_pProjector->LookAt(vec3(5 * sinf(0.5f * wave), 0, 5 * cosf(0.5f * wave)), vec3::vUp);
	//m_pProjector->LookAt(vec3(1, 0, 0), vec3::vUp);
}


void CProjectedGrid::Project()
{
	// -------------------------------------------------------------------------

	ICamera * projector = m_pProjector;
	projector = g_pGame->m_pCamera;

	projector->RenderFrustum();


	CPlane waterPlane(vec3::vUp, 0);

	vec3 hitPoint;
	vec3 linePoints[2];
	vec2 viewport = projector->GetViewport();

	std::vector <vec3> hits;

	projector->Unproject(linePoints, ivec2(0, 0));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
	}

	projector->Unproject(linePoints, ivec2(viewport.x, 0));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
	}

	projector->Unproject(linePoints, ivec2(viewport.x, viewport.y));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
	}

	projector->Unproject(linePoints, ivec2(0, viewport.y));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
	}

	m_invViewProj = (projector->GetViewProjMatrix());
	//m_invViewProj = (projector->GetViewMatrix());
	m_invViewProj.Inverse();
	m_projector = m_invViewProj;

	float minX = 100000;
	float minY = 100000;
	float maxX = -100000;
	float maxY = -100000;

	mat4 tt = m_rangeMatrix;
	tt.Inverse();

	for (uint i = 0; i < hits.size(); ++i)
	{
		vec3 test = hits[i];
		test.Multiply(tt);
		g_pEngine->PushBBox(bbox(test, 0.1f), 0xFF00FF00);

		if (minX > test.x)
			minX = test.x;

		if (minY > test.y)
			minY = test.y;

		if (maxX < test.x)
			maxX = test.x;

		if (maxY < test.y)
			maxY = test.y;
	}

	m_rangeMatrix.pRows[0].Set(maxX - minX, 0, 0, minX);
	m_rangeMatrix.pRows[1].Set(0, maxY - minY, 0, minY);
	m_rangeMatrix.pRows[2].Set(0, 0, 1, 0);
	m_rangeMatrix.pRows[3].Set(0, 0, 0, 0);

	mat4 t = m_rangeMatrix;
	t.Mult(m_projector);
	m_projector = t;
	

	minX = 0;
	minY = 0;
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
}