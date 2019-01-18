#pragma once
#include "EfficientPGMImage.hpp"
#include <atomic>

#define N 201
#define THREADS 8


class conditions
{
public:
	conditions() : conds{0} {}
	std::atomic<bool> conds[THREADS];

	void zero()
	{
		for (int i = 0; i < THREADS; ++i)
		{
			conds[i] = false;
		}
	}

	bool all()
	{
		bool result = true;
		for (int i = 0; i < THREADS; ++i)
		{
			result = result && conds[i];
		}
		return result;
	}
};

class EfficientConvolution
{
public:
	EfficientConvolution(const EfficientPGMImage& i);
	~EfficientConvolution();

	void convolve();

	EfficientPGMImage& getResult();
private:
	static constexpr float C = 0.6f;
	static constexpr float U = 0.1f;
	EfficientPGMImage currentImage;
	EfficientPGMImage nextImage;

	std::atomic<bool> run[N] = { 0 };
	conditions nextImageCond;
};


