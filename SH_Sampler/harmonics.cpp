#include "util.h"
#include <vector>
#include "harmonics.h"

using namespace std;

Harmonics::Harmonics(int degree)
	:degree_(degree)
{
	int maxfact = degree * degree;
	factorial.resize(maxfact + 1);
	for (int i = 0; i <= maxfact; i++)
	{
		if (i == 0)
			factorial[i] = 1;
		else
			factorial[i] = i * factorial[i - 1];
	}
}

void Harmonics::Evaluate(const std::vector<Vertex>& vertices)
{
	int n = (degree_ + 1) * (degree_ + 1);
	coefs = vector<Vec3>(n, Vec3());
	for (const Vertex& v : vertices)
	{
		vector<float> Y = Basis(v.pos);
		for (int i = 0; i < n; i++)
		{
			coefs[i] = coefs[i] + Y[i] * v.color;
		}
	}
	for (Vec3& coef : coefs)
	{
		coef = 4 * PI * coef / (float)vertices.size();
	}
}

Vec3 Harmonics::Render(const Vec3& pos)
{
	int n = (degree_ + 1) * (degree_ + 1);

	vector<float> Y = Basis(pos);
	Vec3 color;
	for (int i = 0; i < n; i++)
	{
		color = color + Y[i] * coefs[i];
	}
	return color;
}

std::array<cv::Mat, 6> Harmonics::RenderCubemap(int width, int height)
{
	std::array<cv::Mat, 6> imgs;
	for (int k = 0; k < 6; k++)
	{
		imgs[k] = cv::Mat(height, width, CV_32FC3);
		for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				float u = (float)j / (width - 1);
				float v = 1.f - (float)i / (height - 1);
				Vec3 pos = CubeUV2XYZ({ k, u, v });
				Vec3 color = Render(pos);
				imgs[k].at<cv::Vec3f>(i, j) = { color.b, color.g, color.r };
			}
		}
	}
	return imgs;
}

vector<float> Harmonics::Basis(const Vec3& pos)
{
	int n = (degree_ + 1) * (degree_ + 1);
	vector<float> Y(n);
	Vec3 normal = Normalize(pos);
	float x = normal.x;
	float y = normal.y;
	float z = normal.z;

	if (degree_ >= 0)
	{
		Y[0] = 1.f / 2.f * sqrt(1.f / PI);
	}
	if (degree_ >= 1)
	{
		Y[1] = sqrt(3.f / (4.f * PI)) * (-x);
		Y[2] = sqrt(3.f / (4.f * PI)) * z;
		Y[3] = sqrt(3.f / (4.f * PI)) * (-y);
	}
	if (degree_ >= 2)
	{
		Y[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y;
		Y[5] = 1.f / 2.f * sqrt(15.f / PI) * (-y * z);
		Y[6] = 1.f / 4.f * sqrt(5.f / PI) * (3 * z * z - 1);
		Y[7] = 1.f / 2.f * sqrt(15.f / PI) * (-x * z);
		Y[8] = 1.f / 4.f * sqrt(15.f / PI) * (x * x - y * y);
	}
	return Y;
}