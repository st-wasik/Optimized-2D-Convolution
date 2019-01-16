#pragma once
#include <string>

class PGMImage
{
public:
	PGMImage();
	PGMImage(const PGMImage& i) = delete;
	PGMImage(const PGMImage&& i);

	void swap(PGMImage& i);

	void loadFromFile(std::string path);
	void saveToFile(std::string path);

	int getWidth() const;

	int getHeight() const;

	int getMaxV() const;

	void create(int h, int w, int maxv);
	void createChessboard(int h, int w, int maxv);

	float* operator[](int h);

	float at(int h, int w) const;

	~PGMImage();

private:
	int width;
	int height;
	int maxValue;

	float** imageArray;

	friend std::ostream& operator<<(std::ostream& stream, const PGMImage& image);
};

std::ostream& operator<<(std::ostream& stream, const PGMImage& image);


