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

	Vector3 operator *(const float val)
	{
		return Vector3(x * val, y * val, z * val);
	}

	Vector3 operator +(const Vector3& val)
	{
		return Vector3(x + val.x, y + val.y, z + val.z);
	}

	Vector3 operator -(const Vector3& val)
	{
		return Vector3(x - val.x, y - val.y, z - val.z);
	}

	~Vector3()
	{
	}
};

class Vector4
{
public:
	Vector4() : x(0), y(0), z(0), w(0)
	{
	}
	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w)
	{
	}
	Vector4(const Vector4& vector) : x(vector.x), y(vector.y), z(vector.z), w(vector.w)
	{
	}
	float x, y, z, w;

	Vector4 operator *(const float value)
	{
		return Vector4(x * value, y * value, z * value, w * value);
	}

	void cross(Vector4& v1, Vector4& v2, Vector4& v3)
	{
		this->x = v1.y * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.y * v3.w - v3.y * v2.w) + v1.w * (v2.y * v3.z - v2.z * v3.y);
		this->y = -(v1.x * (v2.z * v3.w - v3.z * v2.w) - v1.z * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.z - v3.x * v2.z));
		this->z = v1.x * (v2.y * v3.w - v3.y * v2.w) - v1.y * (v2.x * v3.w - v3.x * v2.w) + v1.w * (v2.x * v3.y - v3.x * v2.y);
		this->w = -(v1.x * (v2.y * v3.z - v3.y * v2.z) - v1.y * (v2.x * v3.z - v3.x * v2.z) + v1.z * (v2.x * v3.y - v3.x * v2.y));
	}

	~Vector4()
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
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;
	}
	
	void setScale(const Vector3& scale)
	{
		m_mat[0][0] = scale.x;
		m_mat[1][1] = scale.y;
		m_mat[2][2] = scale.z;
	}

	void setPerspective(float fov, float aspect, float znear, float zfar)
	{
		float yscale = 1.0f / tan(fov / 2.0f);
		float xscale = yscale / aspect;

		m_mat[0][0] = xscale;
		m_mat[1][1] = yscale;
		m_mat[2][2] = zfar / (zfar - znear);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = (-znear * zfar) / (zfar - znear);
	}

	void setOrthoLH(float width, float height, float nearPlane, float farPlane)
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (farPlane - nearPlane);
		m_mat[3][2] = -(nearPlane / (farPlane - nearPlane));
	}

	void inverse()
	{
		int a, i, j;
		Matrix4x4 out;
		Vector4 v, vec[3];
		float det = 0.0f;

		det = this->getDeterminant();
		if (!det) return;
		for (i = 0; i < 4; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (j != i)
				{
					a = j;
					if (j > i) a = a - 1;
					vec[a].x = (this->m_mat[j][0]);
					vec[a].y = (this->m_mat[j][1]);
					vec[a].z = (this->m_mat[j][2]);
					vec[a].w = (this->m_mat[j][3]);
				}
			}
			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		this->setMatrix(out);
	}

	float getDeterminant()
	{
		Vector4 minor, v1, v2, v3;
		float det;

		v1 = Vector4(this->m_mat[0][0], this->m_mat[1][0], this->m_mat[2][0], this->m_mat[3][0]);
		v2 = Vector4(this->m_mat[0][1], this->m_mat[1][1], this->m_mat[2][1], this->m_mat[3][1]);
		v3 = Vector4(this->m_mat[0][2], this->m_mat[1][2], this->m_mat[2][2], this->m_mat[3][2]);


		minor.cross(v1, v2, v3);
		det = -(this->m_mat[0][3] * minor.x + this->m_mat[1][3] * minor.y + this->m_mat[2][3] * minor.z +
			this->m_mat[3][3] * minor.w);
		return det;
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

	void setMatrix(const Matrix4x4& matrix)
	{
		memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	Vector3 getForward()
	{
		return Vector3(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	Vector3 getRight()
	{
		return Vector3(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}

	Vector3 getTranslation()
	{
		return Vector3(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
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