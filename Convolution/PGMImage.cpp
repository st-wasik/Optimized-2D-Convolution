#include "PGMImage.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

PGMImage::PGMImage()
	: width(0), height(0), maxValue(0), imageArray(nullptr)
{
}


PGMImage::PGMImage(const PGMImage && i)
{
	this->width = i.width;
	this->height = i.height;
	this->maxValue = i.maxValue;

	this->imageArray = std::move(i.imageArray);
}

void PGMImage::swap(PGMImage & i)
{
	std::swap(this->height, i.height);
	std::swap(this->width, i.width);
	std::swap(this->maxValue, i.maxValue);

	std::swap(this->imageArray, i.imageArray);
}

void PGMImage::loadFromFile(std::string path)
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
	std::clog << "Width:" << width << std::endl;
	fin >> height;
	std::clog << "Height:" << height << std::endl;
	fin >> maxValue;
	std::clog << "MaxVal:" << maxValue << std::endl;

	this->~PGMImage();
	imageArray = new float*[height];
	for (int i = 0; i < height; ++i)
	{
		imageArray[i] = new float[width];
	}

	int temp2;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			fin >> temp2;
			imageArray[i][j] = static_cast<float>(temp2);
		}
	}
}

void PGMImage::saveToFile(std::string path)
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
			fout << static_cast<int>(imageArray[i][j]) % maxValue << " ";
		}
		fout << std::endl;
	}
}

int PGMImage::getWidth() const
{
	return this->width;
}

int PGMImage::getHeight() const
{
	return height;
}

int PGMImage::getMaxV() const
{
	return maxValue;
}

void PGMImage::create(int h, int w, int maxv)
{
	this->~PGMImage();

	width = w;
	height = h;
	maxValue = maxv;


	/**/
	imageArray = new float*[height];
	for (int i = 0; i < height; ++i)
	{
		imageArray[i] = new float[width];
		for (int j = 0; j < width; j++)
		{
			imageArray[i][j] = 0;
		}
	}
}

void PGMImage::createChessboard(int h, int w, int maxv)
{
	create(h, w, maxv);

	for(int i=0;i<h;i++)
		for (int j = 0; j < w; j++)
		{
			if (i & 1)
			{
				if(j & 1)
					imageArray[i][j] = maxv;
				else
					imageArray[i][j] = 0;
			}
			else
			{
				if (j & 1)
					imageArray[i][j] = 0;
				else
					imageArray[i][j] = maxv;
			}
		}

}

float* PGMImage::operator[](int h)
{
	return imageArray[h];
}

float PGMImage::at(int h, int w) const
{
	if (w > -1 && w < width && h > -1 && h < height) return imageArray[h][w];
	return 0.0f;
}

PGMImage::~PGMImage()
{
	if (imageArray)
	{
		for (int i = 0; i < height; ++i)
		{
			if (imageArray[i])
				delete[] imageArray[i];
		}
		delete[] imageArray;
	}
}

std::ostream & operator<<(std::ostream & stream, const PGMImage & image)
{
	stream << "< W:" << image.width << ", H:" << image.height << ", MV:" << image.maxValue << " >" << std::endl;
	for (int i = 0; i < image.height; i++)
	{
		for (int j = 0; j < image.width; j++)
		{
			stream << std::setw(3) << image.imageArray[i][j] << " ";
		}
		stream << std::endl;
	}
	return stream;
}
