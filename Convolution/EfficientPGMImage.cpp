#include "EfficientPGMImage.hpp"
#include <iostream>
#include <fstream>
EfficientPGMImage::EfficientPGMImage() 
	: width(0), height(0), maxValue(0), imageArray(nullptr)
{
}

EfficientPGMImage::EfficientPGMImage(const PGMImage & image)
{
	width = image.getHeight();
	height = image.getWidth();
	maxValue = image.getMaxV();

	imageArray = new float[height*width];

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			this->at(i, j) = image.at(i,j);
		}
	}
}

EfficientPGMImage::EfficientPGMImage(int h, int w, int maxv)
	: width(w), height(h), maxValue(maxv), imageArray(new float[h*w])
{
}

EfficientPGMImage & EfficientPGMImage::operator=(const EfficientPGMImage & i)
{
	create(i.getHeight(), i.getWidth(), i.getMaxV());

	for (int x = 0; x < width*height; x++)
	{
		this->imageArray[x] = i.imageArray[x];
	}

	return *this;
}

void EfficientPGMImage::swap(EfficientPGMImage & i)
{
	std::swap(this->width, i.width);
	std::swap(this->height, i.height);
	std::swap(this->maxValue, maxValue);

	std::swap(this->imageArray, i.imageArray);
}

void EfficientPGMImage::loadFromFile(std::string path)
{

	std::ifstream fin(path);

	if (!fin)
	{
		std::clog << "Cannot find specified file: " + path << std::endl;
		return;
	}

	std::string temp;
	fin >> temp;
	if (temp != "P2")
	{
		std::clog << "Not a PGM file: " + path << std::endl;
		return;
	}

	fin >> width;
	fin >> height;
	fin >> maxValue;

	create(height, width, maxValue);

	int temp2;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			fin >> temp2;
			at(i,j) = static_cast<float>(temp2);
		}
	}

}

void EfficientPGMImage::saveToFile(std::string path)
{
	std::ofstream fout(path);
	if (!fout)
	{
		std::clog << "Cannot find specified file: " + path << std::endl;
		return;
	}

	fout << "P2" << std::endl;
	fout << width << " " << height << std::endl << maxValue << std::endl;

	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			float x = at(i, j);
			fout << (x>(float)maxValue ? maxValue:(int)x) << " ";
		}
		fout << std::endl;
	}
}

const int& EfficientPGMImage::getWidth() const
{
	return width;
}

const int& EfficientPGMImage::getHeight() const
{
	return height;
}

const int& EfficientPGMImage::getMaxV() const
{
	return maxValue;
}

void EfficientPGMImage::create(int h, int w, int maxv)
{
	width = w;
	height = h; 
	maxValue = maxv;
	if (imageArray != nullptr) delete[] imageArray;
	imageArray = new float[h*w];
}

float & EfficientPGMImage::at(int h, int w)
{
	return imageArray[h*height + w];
}

EfficientPGMImage::~EfficientPGMImage()
{
	if (imageArray != nullptr) delete[] imageArray;
	imageArray = nullptr;
}
