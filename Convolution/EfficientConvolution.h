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
/*
class Conditions
{
public:
	Conditions() : v{ 0 }, size(THREADS)
	{
	}

	void wait(const int& n)
	{
		std::unique_lock<std::mutex> lock(m[n]);
		cv[n].wait(lock, [&]() {return v[n]; });
	}

	void notify(const int& n)
	{
		//std::cout << "notifying for " + std::to_string(n) + "\n" << std::flush;
		std::unique_lock<std::mutex> lock(m[n]);
		v[n] = 1;
	}

	char all()
	{
		char result = v[0];
		for (size_t i = 1; i < size; ++i)
		{
			result = result & v[i];
		}
		return result;
	}

	void reset(const int& n)
	{
		std::unique_lock<std::mutex> lock(m[n]);
		v[n] = 0;
	}

	void resetAll()
	{
		for (size_t i = 0; i < size; i++)
		{
			std::unique_lock<std::mutex> lock(m[i]);
			v[i] = 0;
		}
	}

private:
	const size_t size;
	std::mutex m[THREADS];
	std::condition_variable cv[THREADS];
	char v[THREADS];
};
*/

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


	//std::atomic<bool> InsideReady[N][THREADS] = { 0 };

};


