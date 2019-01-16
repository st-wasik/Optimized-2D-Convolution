#include <iostream>

#include "PGMImage.hpp"
#include "Convolution.hpp"
#include "EfficientConvolution.h"
#include <chrono>
#include "half.hpp"
#include <vector>
#include <numeric>
#include <algorithm>

int main()
{
	EfficientPGMImage fstE;
	fstE.loadFromFile("1.pgm");
	std::vector< long long> vect;
	for (int i = 0; i < 25; i++)
	{
		EfficientConvolution c(fstE);
		auto start = std::chrono::steady_clock::now();
		c.convolve();
		auto end = std::chrono::steady_clock::now();
		std::cout << "elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
		vect.push_back(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
		if (i == 24)
			c.getResult().saveToFile("out.pgm");
	}

	std::sort(vect.begin(), vect.end());
	for (auto& x : vect)
	{
		std::cout << x << " ";
	}
	std::cout << std::endl;


	std::cout << "Done" << std::endl;
	std::cin.get();
	return EXIT_SUCCESS;
}