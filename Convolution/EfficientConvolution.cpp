#include "EfficientConvolution.h"
#include <iostream>
#include <algorithm>
#include <condition_variable>
#include <xmmintrin.h>

EfficientConvolution::EfficientConvolution(const EfficientPGMImage & i)
{
	currentImage = i;
	nextImage.create(i.getHeight(), i.getWidth(), i.getMaxV());
}

EfficientConvolution::~EfficientConvolution()
{
}

void waitFor(std::atomic<bool>& v)
{
	if (!v)
	{
		//std::clog;
		while (!v);
	}
}

void EfficientConvolution::convolve()
{
	const int& nx = currentImage.getWidth();
	const int& ny = currentImage.getHeight();

	const int ny1 = ny - 1;
	const int ny2 = ny - 2;
	const int nx1 = nx - 1;
	const int nx2 = nx - 2;

	run[0] = true;

	auto inside_thread = [&](const int& thread_no, const int& i)
	{
		const int i1 = i + 1;
		int start = (thread_no)*ny / THREADS + 1,
			end = (thread_no + 1 == THREADS) ? (thread_no + 1)*ny / THREADS - 1 : (thread_no + 1)*ny / THREADS + 1;

		_mm_prefetch((char*)&currentImage.at(start, 0), 3);
		_mm_prefetch((char*)&nextImage.at(start, 0), 3);

		waitFor(run[i]);

		if (thread_no == 0)
		{
			//left top
			nextImage.at(0, 0) = C * currentImage.at(0, 0) + U * (currentImage.at(0, 1) + currentImage.at(1, 0));

			// top
			for (int j = 1; j < nx2; j++)
			{
				nextImage.at(0, j) = U* currentImage.at(0, j - 1) + C * currentImage.at(0, j) + U * ( + currentImage.at(0, j + 1) + currentImage.at(1, j));
			}

			//right top
			nextImage.at(0, nx1) = U* currentImage.at(0, nx2) +  C * currentImage.at(0, nx1) + U * (currentImage.at(1, nx1));
		}

		for (int y = start; y < end; ++y)
		{
			nextImage.at(y, 0) = C * currentImage.at(y, 0) + U * (currentImage.at(y, 1) + currentImage.at(y - 1, 0) + currentImage.at(y + 1, 0));
			for (int x = 1; x < nx2; ++x)
			{
				nextImage.at(y, x) = U * (currentImage.at(y, x - 1)) + C * currentImage.at(y, x) + U * (currentImage.at(y, x + 1) + currentImage.at(y - 1, x) + currentImage.at(y + 1, x));
			}
			nextImage.at(y, nx1) = U * currentImage.at(y, nx2) + C * currentImage.at(y, nx1) + U * (currentImage.at(y - 1, nx1) + currentImage.at(y + 1, nx1));
		}

		if (thread_no == N - 1)
		{
			//left bottom
			nextImage.at(ny1, 0) = U*currentImage.at(ny2, 0) +  C * currentImage.at(ny1, 0)  + U * (currentImage.at(ny1, 1));

			// bottom
			for (int j = 1; j < nx2; j++)
			{
				nextImage.at(ny1, j) =U * currentImage.at(ny1, j - 1) + C * currentImage.at(ny1, j) + U * ( + currentImage.at(ny1, j + 1) + currentImage.at(ny2, j));
			}

			//right bottom
			nextImage.at(ny1, nx1) = U* currentImage.at(ny1, nx2) + C * currentImage.at(ny1, nx1) + U * (currentImage.at(ny2, nx1));

		}
		_mm_prefetch((char*)&nextImage.at(start, 0), 3);
		nextImageCond.conds[thread_no] = true;
		if (nextImageCond.all())
		{
			currentImage.swap(nextImage);
			nextImageCond.zero();
			run[i1] = true;
		}
	};

	auto thread_convolve = [&](const int& thread_no)
	{
		for (int i = 0; i < N - 1; i++)
		{
			inside_thread(thread_no, i);
		}
	};

	std::thread* threads[THREADS];
	for (int i = 1; i < THREADS; i++)
	{
		threads[i] = new std::thread(thread_convolve, i);
	}

	constexpr int thread_no = 0;
	for (int i = 0; i < N - 1; i++)
	{
		const int i1 = i + 1;
		inside_thread(thread_no, i);

		//	for (int i = 0; i < THREADS && i != thread_no; i++)
		//	{
		//		result = result && InsideReady[i1][i];
		//	}
		//	if (result)
		//	{
		//		currentImage.swap(nextImage);
		//	}

		/*
		if (!all(InsideReady[i], THREADS))
		{
			//std::cout << "Outer waiting\n" << std::flush;
			while (!all(InsideReady[i], THREADS));
		}*/
		//auto start = std::chrono::steady_clock::now();
		/*{
			//left top
			nextImage.at(0, 0) = C * currentImage.at(0, 0) + U * (currentImage.at(0, 1) + currentImage.at(1, 0));

			//left bottom
			nextImage.at(ny1, 0) = C * currentImage.at(ny1, 0) + U * (currentImage.at(ny2, 0) + currentImage.at(ny1, 1));

			//right top
			nextImage.at(0, nx1) = C * currentImage.at(0, nx1) + U * (currentImage.at(0, nx2) + currentImage.at(1, nx1));

			//right bottom
			nextImage.at(ny1, nx1) = C * currentImage.at(ny1, nx1) + U * (currentImage.at(ny2, nx1) + currentImage.at(ny1, nx2));

			// top
			for (int j = 1; j < nx2; j++)
			{
				nextImage.at(0, j) = C * currentImage.at(0, j) + U * (currentImage.at(0, j - 1) + currentImage.at(0, j + 1) + currentImage.at(1, j));
			}

			// bottom
			for (int j = 1; j < nx2; j++)
			{
				nextImage.at(ny1, j) = C * currentImage.at(ny1, j) + U * (currentImage.at(ny1, j - 1) + currentImage.at(ny1, j + 1) + currentImage.at(ny2, j));
			}

			//left
			for (int j = 1; j < ny2; j++)
			{
				nextImage.at(j, 0) = C * currentImage.at(j, 0) + U * (currentImage.at(j - 1, 0) + currentImage.at(j + 1, 0) + currentImage.at(j, 1));
			}

			//right
			for (int j = 1; j < ny2; j++)
			{
				nextImage.at(j, nx1) = C * currentImage.at(j, nx1) + U * (currentImage.at(j - 1, nx1) + currentImage.at(j + 1, nx1) + currentImage.at(j, nx2));
			}

			OuterRingReady[i1] = true;
			//auto end = std::chrono::steady_clock::now();
			//std::cout << "OUTER elapsed: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + "ms\n" << std::flush;
		}*/
		//OuterRingReady[i1] = true;
		//inside
		/*{
		//	if (!OuterRingReady[i])
		//	{
		//		//c.lock();
		//		//std::cout << std::to_string(thread_no) + " waiting, it: "+std::to_string(i) + "\n" << std::flush;
		//		//c.unlock();
		//		while (!OuterRingReady[i]);
		//	}
		//	//start = std::chrono::steady_clock::now();

		//	for (int x = thread_no + 1; x < ny2; x += THREADS)
		//	{
		//		for (int j = 1; j < nx2; j++)
		//		{
		//			nextImage.at(x, j) = C * currentImage.at(x, j) + U * (currentImage.at(x - 1, j) + currentImage.at(x + 1, j) + currentImage.at(x, j - 1) + currentImage.at(x, j + 1));
		//		}
		//	}

		//	bool result = OuterRingReady[i1];
		//	for (int i = 0; i < THREADS && i != thread_no; i++)
		//	{
		//		result = result && InsideReady[i1][i];
		//	}
		//	if (result)
		//	{
		//		currentImage.swap(nextImage);
		//	}

		//	InsideReady[i1][thread_no] = true;
		//	// end = std::chrono::steady_clock::now();
		//	//std::cout << "INNER2 elapsed: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + "ms\n" << std::flush;
		/*} */
	}

	//join
	for (int i = 1; i < THREADS; i++)
	{
		if (threads[i])
		{
			//auto start = std::chrono::steady_clock::now();
			threads[i]->join();
			//std::cout << std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count()) + '\n' << std::flush;
			delete threads[i];
			threads[i] = nullptr;
		}
	}
}

EfficientPGMImage & EfficientConvolution::getResult()
{
	return currentImage;
}
