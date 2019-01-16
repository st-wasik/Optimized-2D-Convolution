#pragma once
#include <string>
#include "PGMImage.hpp"

class EfficientPGMImage
{
public:
	EfficientPGMImage();
	EfficientPGMImage(const PGMImage& i);
	EfficientPGMImage(int h, int w, int maxv);
	EfficientPGMImage(const EfficientPGMImage& i) = delete;

	EfficientPGMImage& operator=(const EfficientPGMImage& i);

	void swap(EfficientPGMImage& i);

	void loadFromFile(std::string path);
	void saveToFile(std::string path);

	const int& getWidth() const;

	const int& getHeight() const;

	const int& getMaxV() const;

	void create(int h, int w, int maxv);
	float& at(int h, int w);

	~EfficientPGMImage();

private:
	int width;
	int height;
	int maxValue;

	float* imageArray;
};


