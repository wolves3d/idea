#include "Common.h"
#include "ProjectedGrid.h"


CProjectedGrid::CProjectedGrid()
	: m_vertexDecl(NULL)
	, m_VAO(NULL)
	, m_indexBuffer(NULL)
	, m_Material(NULL)
	, m_pCamera(NULL)
	, m_pProjector2(NULL)
	, m_frameBuffer(NULL)
	, m_frameMaterial(NULL)
{
	m_gridWidth = 256;
	m_gridHeight = 256;

	Init();
}


CProjectedGrid::~CProjectedGrid()
{
	Release();
}


void CProjectedGrid::Release()
{
	DEL(m_frameBuffer);
	DEL(m_vertexDecl);

	RELEASE(m_VAO);
	RELEASE(m_indexBuffer);
	RELEASE(m_Material);
	RELEASE(m_frameMaterial);
}


// =============================================================================

struct KnapsackSolver
{
	KnapsackSolver(int packCapacity, int itemCount)
	{
		N = itemCount;
		W = packCapacity;

		pItems = new PackItem[itemCount];
		A = new int[N * W];
	}

	void SetItem(uint id, int weight, int price)
	{
 		pItems[id].weight = weight;
 		pItems[id].price = price;
	}

	void Dump()
	{
		for (int i = 0; i < N; ++i)
		{
			char buff[1024];
			buff[0] = 0;

			for (int j = 0; j < W; ++j)
			{
				sprintf(buff + strlen(buff), "\t%d", GetA(i, j));
			}

			SysWrite(Va("A %s", buff));
		}
	}

	void Solve()
	{
		for (int i = 0; i < W; ++i)
			SetA(0, i, 0);
		
 		for (int i = 0; i < N; ++i)
 			SetA(i, 0, 0);

		for (int k = 1; k < N; ++k)
		{
			for (int s = 1; s < W; ++s)
//			for (int s = 0; s < W; ++s)
			{
				int weight = pItems[k].weight;
				int price = pItems[k].price;
				/*
				if (s < (weight - 1))
				{
					SetA(k, s, GetA(k - 1, s));
				}
				else
				{
					SetA(k, s, max(
						GetA(k - 1, s),
						GetA(k - 1, s - weight) + weight));
				}
				*/
				
				if (s >= weight)
				{
					SetA(k, s, max(
						GetA(k - 1, s),
						GetA(k - 1, s - weight) + price));
				}
				else
				{
					SetA(k, s, GetA(k - 1, s));
				}
				
			}
		}

		Dump();

		result.clear();
		FindAns(N-1, W-1);

		result.clear();
	}

	void FindAns(int k, int s)
	{
		if (0 == GetA(k, s))
			return;

		if (GetA(k, s) == GetA(k - 1, s))
		{
			FindAns(k - 1, s);
		}
		else
		{
			FindAns(k - 1, s - pItems[k].weight);
			result.push_back(k);
		}
	}

	void SetA(int i, int w, int value)
	{
		//int id = (w * N) + i;
		int id = (i * W) + w;
		A[id] = value;
	}

	int GetA(int i, int w)
	{
		//int id = (w * N) + i;
		int id = (i * W) + w;
		return A[id];
	}

	struct PackItem
	{
		int weight;
		int price;
	} * pItems;

	int N;
	int W;
	int * A;
	std::vector <int> result;
};


void BalanceTest()
{
//	KnapsackSolver pack(9+1, 6);
// 	pack.SetItem(0, 5, 5);
// 	pack.SetItem(1, 5, 5);
// 	pack.SetItem(2, 3, 3);
// 	pack.SetItem(3, 2, 2);
// 	pack.SetItem(4, 2, 2);
// 	pack.SetItem(5, 1, 1);

	KnapsackSolver pack(25 + 1, 6);
	pack.SetItem(0, 9, 9);
	pack.SetItem(1, 9, 9);
	pack.SetItem(2, 9, 9);
	pack.SetItem(3, 8, 8);
	pack.SetItem(4, 8, 8);
	pack.SetItem(5, 7, 7);

	pack.Solve();

	int stop = 0;
}

// =============================================================================


void CProjectedGrid::Init()
{
	int a = sizeof(int);
	int b = sizeof(long);
	int c = sizeof(void *);

	BalanceTest();

	m_gridVertexCount = (m_gridWidth * m_gridHeight);

	m_vertexDecl = g_pRenderer->CreateVertexDecl();
	m_vertexDecl->SetStride(sizeof(GridVertex));

	uint offset = 0;
	m_vertexDecl->AddAttr("vPos", NULL, TYPE_VEC3, offset);
	offset += sizeof(vec3);
	/*
	m_vertexDecl->AddAttr("vTex0", NULL, TYPE_VEC2, offset);
	offset += sizeof(vec2);
	*/
	m_vertexDecl->AddAttr("vTangent", NULL, TYPE_VEC3, offset);
	offset += sizeof(vec3);
	/*
	m_vertexDecl->AddAttr("vBitangent", NULL, TYPE_VEC3, offset);
	offset += sizeof(vec3);
	*/
	m_vertexDecl->AddAttr("vNormal", NULL, TYPE_VEC3, offset);
	
	m_Material = g_pEngine->CreateMaterial();
	m_Material->SetShader(g_pRenderer->GetShader("Water"));
	m_Material->SetTexture(DIFFUSE_MAP, g_pRenderer->GetSysTexture(EST_WHITE));

	PTexture pTex;
	//g_pEngine->LoadTexture(pTex, "texture.tga");
	g_pEngine->LoadTexture(pTex, "normals.tga");
	m_Material->SetTexture(DIFFUSE_MAP, pTex);

	IShader * materialShader = m_Material->GetShader();
	m_mvpUniform.AttachToShader("mMVP", materialShader);
	m_lightPos.AttachToShader("vLightPos", materialShader);
	m_shift.AttachToShader("waveParams", materialShader);
	m_eyePos.AttachToShader("eyePos", materialShader);
	m_uniformModelViewMatrix.AttachToShader("modelView", materialShader);

	m_mvpUniform.Connect(m_Material);
	m_lightPos.Connect(m_Material);
	m_shift.Connect(m_Material);
	m_eyePos.Connect(m_Material);
	m_uniformModelViewMatrix.Connect(m_Material);

	if (m_VAO = g_pRenderer->CreateVB())
	{
		m_VAO->AllocMulti(3);

		IVertexBuffer * positions = m_VAO->GetSubBuffer(0);
		positions->Alloc(m_gridVertexCount * sizeof(vec3), 0);

		IVertexBuffer * normals = m_VAO->GetSubBuffer(1);
		normals->Alloc(m_gridVertexCount * sizeof(vec3), 0);

		IVertexBuffer * tangents = m_VAO->GetSubBuffer(2);
		tangents->Alloc(m_gridVertexCount * sizeof(vec3), 0);

		/*
		uint sizeInBytes = (sizeof(GridVertex)* m_gridVertexCount);
		if (m_vertexBuffer->Alloc(sizeInBytes, 0))
		{
		}
		*/
	}


	/////////////////////////////////////////////////////////////////////////

	const uint nIndexCount = ((m_gridWidth-1) * (m_gridHeight-1) * 6);
	m_indexBuffer = g_pRenderer->CreateIB();
	m_indexBuffer->SetAttrs(nIndexCount, TYPE_SHORT);

	const uint bufferSize = (sizeof(short) * nIndexCount);
	m_indexBuffer->Alloc(bufferSize, 0);

	short * m_pIndices = (short *)m_indexBuffer->Lock(0, bufferSize);

	for (uint y = 0; y < (m_gridHeight-1); ++y)
	{
		const uint nRawStart = ((m_gridWidth-1) * y);

		for (uint x = 0; x < (m_gridWidth-1); ++x)
		{
			uint nOffset = (nRawStart + x) * 6;
			int nVertexOffset = (m_gridWidth * y) + x;

			if (nOffset >= nIndexCount)
			{
				int a = 0;
			}

			// Первый треугльник
			// v1 --- v2 
			// |     /
			// |   /
			// | /
			// v3
			m_pIndices[nOffset] = nVertexOffset;					// v1
			m_pIndices[nOffset + 1] = nVertexOffset + 1;			// v2
			m_pIndices[nOffset + 2] = nVertexOffset + m_gridWidth;	// v3

			// Воторой треугльник
			//        v1 
			//       / |
			//     /   |
			//   /     |
			// v3 --- v2
			m_pIndices[nOffset + 3] = nVertexOffset + 1;				// v1
			m_pIndices[nOffset + 4] = nVertexOffset + m_gridWidth + 1;	// v2
			m_pIndices[nOffset + 5] = nVertexOffset + m_gridWidth;		// v3
		}
	}

	m_indexBuffer->Unlock();

	m_pCamera = g_pEngine->CreateCamera();
	m_pProjector2 = g_pEngine->CreateCamera();
	m_pCamera->SetPos(vec3(0, 2, 0));
	m_pCamera->SetParams(45, 1, 1, 1, 20);
	m_pProjector2->SetParams(45, 1, 1, 1, 20);

	//m_pCamera = g_pGame->m_pCamera;

	//==========================================================================

	m_frameBuffer = g_pRenderer->CreateRenderTarget();
	m_frameMaterial = g_pEngine->CreateMaterial();

	m_frameBuffer->Init(m_gridWidth, m_gridHeight, 3, false);

	IShader * waterQuadShader = g_pRenderer->GetShader("WaterQuad");
	m_frameMaterial->SetShader(waterQuadShader);
	m_frameMaterial->EnableDepthTest(false);
	m_frameMaterial->EnableDepthWrite(false);

	PTexture pHeightTex;
	g_pEngine->LoadTexture(pHeightTex, "texture.tga");
	m_frameMaterial->SetTexture(DIFFUSE_MAP, pHeightTex);

	m_uniformIVP.AttachToShader("invViewProj", waterQuadShader);
	m_uniformIVP.Connect(m_frameMaterial);

	m_waveSetup.AttachToShader("setup", waterQuadShader);
	m_waveParams.AttachToShader("waveParams", waterQuadShader);
	m_waveDirs.AttachToShader("waveDirs", waterQuadShader);

	m_waveSetup.Connect(m_frameMaterial);
	m_waveDirs.Connect(m_frameMaterial);
	m_waveParams.Connect(m_frameMaterial);
}


PTexture CProjectedGrid::GetFrameTexture(uint nColor)
{
	return m_frameBuffer->GetTexture(nColor);
}


float wave = 0;

float CProjectedGrid::HeightFunc(float x, float z)
{
	//return (0.2f * (sinf(4 * x + wave * 2)));
	return (0.08f * (sinf(4 * x + wave*2)));
}


void CProjectedGrid::UpdateNormals(GridVertex * vertexArray)
{
	/*
	const uint lastX = (m_gridWidth - 1);
	const uint lastZ = (m_gridHeight - 1);
	vec3 vU, vV, vZ;

	for (uint z = 0; z < m_gridHeight; ++z)
	{
		for (uint x = 0; x < m_gridWidth; ++x)
		{
			vU = (lastX != x)
				? vertexArray[1].vPos - vertexArray[0].vPos
				: vertexArray[0].vPos - (vertexArray - 1)->vPos;

			vV = (lastZ != z)
				? vertexArray[m_gridWidth].vPos - vertexArray[0].vPos
				: vertexArray[0].vPos - (vertexArray - m_gridWidth)->vPos;

			vZ = Cross(vU, vV);

			//vU.NormalizeTo(0.5f);
			//vV.NormalizeTo(0.5f);
			//vZ.NormalizeTo(0.5f);

			//g_pEngine->PushLine(vertexArray[0].vPos, vertexArray[0].vPos + vU, 0xFFFF0000);
			//g_pEngine->PushLine(vertexArray[0].vPos, vertexArray[0].vPos + vV, 0xFF00FF00);
			//g_pEngine->PushLine(vertexArray[0].vPos, vertexArray[0].vPos + vZ, 0xFF0000FF);

			vertexArray->vTangent = vU.Normalize();
			vertexArray->vBitangent = vV.Normalize();
			vertexArray->vNormal = vZ.Normalize();

			++vertexArray;
		}
	}
	*/
}


void CProjectedGrid::Update()
{
	Project();

	g_pEngine->DrawFullframeQuad(m_frameBuffer, m_frameMaterial);
	m_frameBuffer->CopyToVertexBuffer(m_VAO->GetSubBuffer(0), 0);
	m_frameBuffer->CopyToVertexBuffer(m_VAO->GetSubBuffer(1), 1);
	m_frameBuffer->CopyToVertexBuffer(m_VAO->GetSubBuffer(2), 2);
	/*
	//mat4 v = m_pProjector->GetViewProjMatrix();
	//v.Inverse();
	mat4 v = m_invViewProj;

	//v = m_rangeMatrix;
	//v.Mult(m_invViewProj);

	uint sizeInBytes = (sizeof(GridVertex)* m_gridVertexCount);
	GridVertex * pVertices = (GridVertex *)(m_vertexBuffer->Lock(0, sizeInBytes));

	if (NULL == pVertices)
		return;
	
	CPlane waterPlane(vec3::vUp, 0);

	const float xStep = 2.0f * (1.0f / (m_gridWidth - 1));
	const float yStep = 2.0f * (1.0f / (m_gridHeight - 1));
	float yCoord = -1.0f;
	GridVertex * curVertex = pVertices;

	for (uint z = 0; z < m_gridHeight; ++z)
	{
		float xCoord = -1.0f;

		for (uint x = 0; x < m_gridWidth; ++x)
		{
			vec4 a(xCoord, yCoord, -1, 1);
			vec4 b(xCoord, yCoord, +1, 1);
			a.Multiply(m_invViewProj);
			b.Multiply(m_invViewProj);
			a.w = 1.f / a.w;
			b.w = 1.f / b.w;

			vec3 s((a.x * a.w), (a.y * a.w), (a.z * a.w));
			vec3 e((b.x * b.w), (b.y * b.w), (b.z * b.w));
				
			waterPlane.LineCollision(&curVertex->vPos, s, e);
			curVertex->vPos.y = HeightFunc(curVertex->vPos.x, curVertex->vPos.z);

			curVertex->vTex.Set(curVertex->vPos.x * 0.1f, curVertex->vPos.z * 0.1f);
			xCoord += xStep;
			++curVertex;
		}

		yCoord += yStep;
	}

	UpdateNormals(pVertices);

	m_vertexBuffer->Unlock();
	*/

	wave += 0.001f;

	m_lookPoint.Set(5 * sinf(0.5f * wave), 0, 5 * cosf(0.5f * wave));
	m_lookPoint.Set(5, 0, 0);
	m_pCamera->LookAt(m_lookPoint, vec3::vUp);

	m_pProjector2->SetPos(m_pCamera->GetPos() + vec3(-30, 20, 0));
	m_pProjector2->LookAt(m_lookPoint, vec3::vUp);
}


void CProjectedGrid::Project()
{
	// -------------------------------------------------------------------------

	ICamera * projector = m_pProjector2;
	//projector = g_pGame->m_pCamera;

	m_pCamera->RenderFrustum(0xFF000000);
	m_pProjector2->RenderFrustum(0xFFFF0000);

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
		m_bottomLeftHit = hitPoint;
	}

	projector->Unproject(linePoints, ivec2(viewport.x, 0));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
		m_bottomRightHit = hitPoint;
	}

	projector->Unproject(linePoints, ivec2(viewport.x, viewport.y));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
		m_topRightHit = hitPoint;
	}

	projector->Unproject(linePoints, ivec2(0, viewport.y));
	if (true == waterPlane.LineCollision(&hitPoint, Line(linePoints)))
	{
		g_pEngine->PushBBox(bbox(hitPoint, 0.1f), 0xFFFF0000);
		hits.push_back(hitPoint);
		m_topLeftHit = hitPoint;
	}

	m_invViewProj = (projector->GetViewProjMatrix());
//	m_invViewProj = (projector->GetProjMatrix());
	m_invViewProj.Inverse();

	float minX = 100000;
	float minY = 100000;
	float maxX = -100000;
	float maxY = -100000;

	mat4 mz = projector->GetViewProjMatrix();
	mz.Inverse();

	for (uint i = 0; i < hits.size(); ++i)
	{
		vec3 test = hits[i];
		//test.Multiply(mz);
		g_pEngine->PushBBox(bbox(test, 0.1f), 0xFF00FF00);

		if (minX > test.x)
			minX = test.x;

		if (minY > test.z)
			minY = test.z;

		if (maxX < test.x)
			maxX = test.x;

		if (maxY < test.z)
			maxY = test.z;
	}

	m_rangeMatrix.pRows[0].Set(maxX - minX, 0, 0, minX);
	m_rangeMatrix.pRows[1].Set(0, maxY - minY, 0, minY);
	m_rangeMatrix.pRows[2].Set(0, 0, 1, 0);
	m_rangeMatrix.pRows[3].Set(0, 0, 0, 0);


      	//mat4 t = m_rangeMatrix;
      	//t.Mult(m_invViewProj);
     	//m_projector = t;

 	//mat4 t = m_invViewProj;
	//t.Mult(m_rangeMatrix);
	//m_projector = t;
	m_projector = projector->GetViewMatrix();
	//m_projector.Inverse();
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

	const mat4 & modelViewMatrix = g_pGame->m_pCamera->GetViewMatrix();
	m_uniformModelViewMatrix.SetValue(modelViewMatrix.pRows, 4);

	//m_lookPoint.Set(5 * sinf(0.5f * wave), 0, 5 * cosf(0.5f * wave));

	

	vec4 light(8  + 3.0f * cosf(2 * wave), 4 + 1.5f * sinf(2 * wave), 0, 1);
	//vec4 light(8, 2, 0, 1);
	//const vec3 & dirLightPos = m_pProjector2->GetPos();
	//vec4 light(dirLightPos.x, dirLightPos.y, dirLightPos.z, 1);

	vec4 shift(0.2 * wave, 0, 0, 0);
	m_shift.SetValue(&shift, 1);

	m_lightPos.SetValue(&light, 1);
	g_pEngine->PushBBox(bbox(vec3(light.x, light.y, light.z), 0.1f), 0xFFFFFFF);


	// =================================================================================================

	struct WaveDesc
	{
		WaveDesc(vec2 waveDir, float waveLen, float waveAmp, float waveQ, float waveTime)
			: dir(waveDir)
			, len(waveLen)
			, amp(waveAmp)
			, Q(waveQ)
			, time(waveTime)
		{
		}

		vec2 dir;
		float len;
		float amp;
		float Q;
		float time;
	};

	WaveDesc waves[] = 
	{
		WaveDesc(vec2(1.0f, 0.0f), 25.0f, 0.15f, 0.0f, wave * 0.09f),
		WaveDesc(vec2(1.0f, 0.5f), 15.0f, 0.2f, 0.9f, wave * 3),
		//WaveDesc(vec2(0.7071f, 0.7071f), 2.0f, 0.1f, 0.1f, 20 * wave),
//		WaveDesc(vec2(0.0f, 1.0f), 10.0f, 0.0f, 0.3f, wave * 0.09f),
	};

	uint waveCount = (sizeof(waves) / sizeof(WaveDesc));
	float time = (wave * 1.09f);

	vec4 waveDirs[4];
	vec4 waveParams[4];
	vec4 setup(waveCount, 0, 0, 0);

	for (uint i = 0; i < waveCount; ++i)
	{
		WaveDesc & w = waves[i];
		waveDirs[i].Set(w.dir.x, w.dir.y, 0, 0);
		waveParams[i].Set(w.len, w.amp, w.Q, w.time);
	}


	m_waveSetup.SetValue(&setup, 1);
	m_waveDirs.SetValue(waveDirs, waveCount);
	m_waveParams.SetValue(waveParams, waveCount);

	m_uniformIVP.SetValue(m_invViewProj.pRows, 4);

	// =================================================================================================

	const vec3 & camPos = g_pGame->m_pCamera->GetPos();
	vec4 eye(camPos.x, camPos.y, camPos.z, 1);
	m_eyePos.SetValue(&eye, 1);

	m_VAO->RenderIndexed(m_vertexDecl, m_indexBuffer, PRIM_TRIANGLE);
	//m_VAO->Render(m_vertexDecl, 0, m_gridVertexCount, PRIM_POINT);
}