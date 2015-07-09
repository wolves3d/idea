/**
*
*/
class CPlane
{
	public:

	inline CPlane()
	{
	};

	inline CPlane( const vec3 & vNrml, const float fOff )
		: vNormal(vNrml)
		, fOffset(fOff)
		, vOrigin(vec3::vNull)
	{
	}

	inline void Normalize()
	{
		const float fInvMag = 1.f / vNormal.Length();

		vNormal.x	*= fInvMag;
		vNormal.y	*= fInvMag;
		vNormal.z	*= fInvMag;
		fOffset		*= fInvMag;
	}

	bool LineCollision(vec3 * outHitPoint, const Line & line)
	{
		// ��������� ���������� ����� ������� ������� � ���������� ������������.
		float r1 = Dot(vNormal, line.vStart - vOrigin);
		float r2 = Dot(vNormal, line.vEnd - vOrigin);

		// ���� ��� ����� ������� ����� �� ���� ������� �� ���������, �� �������
		// �� ���������� �����������.
		if (f1_sgn(r1) == f1_sgn(r2))
			return false;

		if (0 != outHitPoint)
		{
			// ��������� ����� ����������� ������� � ���������� ������������.
			(*outHitPoint) = (line.vStart + ((line.vEnd - line.vStart) * (-r1 / (r2 - r1))));
		}

		return true;
	}

	vec3	vNormal;
	vec3	vOrigin;
	float	fOffset;
};