#include "memory"

class Vector3
{
public:
	Vector3() : x(0), y(0), z(0)
	{
	}
	Vector3(float _x, float _y, float _z) : x(_x), y(_y), z(_z)
	{
	}
	Vector3(const Vector3& vector) : x(vector.x), y(vector.y), z(vector.z)
	{
	}
	float x, y, z;

	static Vector3 lerp(const Vector3& start, const Vector3& end, float delta)
	{
		Vector3 res;
		res.x = start.x * (1.0f - delta) + end.x * delta;
		res.y = start.y * (1.0f - delta) + end.y * delta;
		res.z = start.z * (1.0f - delta) + end.z * delta;

		return res;
	}

	~Vector3()
	{
	}
};

class Matrix4x4
{
public:
	Matrix4x4()
	{
	}

	void setIdentity()
	{
		memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setRotationX(float rotRad)
	{
		m_mat[1][1] = cos(rotRad);
		m_mat[1][2] = sin(rotRad);
		m_mat[2][1] = -sin(rotRad);
		m_mat[2][2] = cos(rotRad);
	}

	void setRotationY(float rotRad)
	{
		m_mat[0][0] = cos(rotRad);
		m_mat[2][0] = -sin(rotRad);
		m_mat[0][2] = sin(rotRad);
		m_mat[2][2] = cos(rotRad);
	}

	void setRotationZ(float rotRad)
	{
		m_mat[0][0] = cos(rotRad);
		m_mat[0][1] = -sin(rotRad);
		m_mat[1][0] = sin(rotRad);
		m_mat[1][1] = cos(rotRad);
	}

	void setTranslation(const Vector3& translation)
	{
		setIdentity();

		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}
	
	void setScale(const Vector3& scale)
	{
		setIdentity();

		m_mat[0][0] = scale.x;
		m_mat[1][1] = scale.y;
		m_mat[2][2] = scale.z;
	}

	void setOrthoLH(float width, float height, float nearPlane, float farPlane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (farPlane - nearPlane);
		m_mat[3][2] = -(nearPlane / (farPlane - nearPlane));
	}

	void operator *=(const Matrix4x4& matrix)
	{
		Matrix4x4 out;

		for (int i = 0; i < 4; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				out.m_mat[i][j] =
					m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
					m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
			}
		}

		memcpy(m_mat, out.m_mat, sizeof(float) * 16);
	}

	~Matrix4x4()
	{
	}
private:
	float m_mat[4][4] = {};
};

class Color
{
public:
	Color() : r(0), g(0), b(0)
	{
	}
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b)
	{
	}
	Color(const Color& color) : r(color.r), g(color.g), b(color.b)
	{
	}
	float r, g, b;
	~Color()
	{
	}
};

struct Vertex
{
	Vector3 Position;
	Color Color;
};

__declspec(align(16))
struct Constants
{
	Matrix4x4 World;
	Matrix4x4 View;
	Matrix4x4 Proj;
	unsigned int Time;
};