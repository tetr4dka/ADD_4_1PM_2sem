#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "utils.hpp"

using utils::Vec2d;
using utils::Mat22d;


namespace paint
{
#pragma pack(1)
	struct Pixel
	{
		unsigned char b;
		unsigned char g;
		unsigned char r;
	};

#pragma pack()



	class BMP {
	public:
		BMP(int w, int h) {
			m_w = w;
			m_h = h;
			m_pix = new Pixel *[m_h];
			for (int i = 0; i < m_h; i++)
				m_pix[i] = new Pixel[m_w];

			m_coordinates = new Vec2d *[m_h];
			for (int i = 0; i < m_h; i++)
				m_coordinates[i] = new Vec2d[m_w];


			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
					m_pix[i][j] = { 255, 255, 255 };

			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
				{
					m_coordinates[i][j].set(0, 0, j);
					m_coordinates[i][j].set(1, 0, i);
				}

		}

		/*~BMP()
		{
			for (int i = 0; i < m_h; i++)
				delete[] m_pix[i];
			delete[] m_pix;

			for (int i = 0; i < m_h; i++)
				delete[] m_coordinates[i];
			delete[] m_coordinates;
		}*/


		void Open(std::string name) {
			// ×òåíèå ôàéëà
			std::ifstream in(name, std::ios::binary); 

			BMPHEADER bmpHeader;
			// Ñ÷èòàòü 14 áàéòîâ ïîáàéòîâî è çàïîëíèòü ñòðóêòóðó BMPHEADER
			in.read(reinterpret_cast<char*>(&bmpHeader), sizeof(BMPHEADER));

			BMPINFO bmpInfo;
			in.read(reinterpret_cast<char*>(&bmpInfo), sizeof(BMPINFO));

			for (int i = 0; i < m_h; i++) {
				for (int j = 0; j < m_w; j++)
					in.read(reinterpret_cast<char*>(&m_pix[i][j]), sizeof(Pixel));
				if ((3 * bmpInfo.Width) % 4 != 0)
					for (int j = 0; j < 4 - (3 * bmpInfo.Width) % 4; j++) {
						char c;
						in.read(&c, 1);
					}
			}
		}

		void Fill(unsigned char r, unsigned char g, unsigned char b) {
			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
					m_pix[i][j] = { r, g, b };
		}

		void Save(std::string name) {
			std::ofstream out(name, std::ios::binary);

			int width = m_w;
			int height = m_h;

			// Ôîðìèðîâàíèå çàãîëîâêà
			BMPHEADER bmpHeader_new;
			bmpHeader_new.Type = 0x4D42; // Òèï äàííûõ BMP
			bmpHeader_new.Size = 14 + 40 + (3 * width * height);
			if (width % 4 != 0)
				bmpHeader_new.Size += (4 - (3 * width) % 4) * height;
			bmpHeader_new.OffBits = 54;
			bmpHeader_new.Reserved1 = 0;
			bmpHeader_new.Reserved2 = 0;

			out.write(reinterpret_cast<char*>(&bmpHeader_new), sizeof(BMPHEADER));

			// Ôîðìèðîâàíèå èíôîðìàöèè îá èçîáðàæåíèè
			BMPINFO bmpInfo_new;
			bmpInfo_new.BitCount = 24;
			bmpInfo_new.ClrImportant = 0;
			bmpInfo_new.ClrUsed = 0;
			bmpInfo_new.Compression = 0;
			bmpInfo_new.Height = height;
			bmpInfo_new.Planes = 1;
			bmpInfo_new.Size = 40;
			bmpInfo_new.SizeImage = bmpHeader_new.Size - 54;
			bmpInfo_new.Width = width;
			bmpInfo_new.XPelsPerMeter = 0;
			bmpInfo_new.YPelsPerMeter = 0;

			out.write(reinterpret_cast<char*>(&bmpInfo_new), sizeof(BMPINFO));

			// Çàïèñàòü ïèêñåëè
			for (int i = 0; i < bmpInfo_new.Height; i++)
			{
				for (int j = 0; j < bmpInfo_new.Width; j++)
					out.write(reinterpret_cast<char*>(&m_pix[i][j]), sizeof(Pixel));

				if ((3 * bmpInfo_new.Width) % 4 != 0)
					for (int j = 0; j < 4 - (3 * bmpInfo_new.Width) % 4; j++)
					{
						char c = 0;
						out.write(&c, 1);
					}
			}

			for (int i = 0; i < m_h; i++)
				delete[] m_pix[i];
			delete[] m_pix;
		}

		void BW1() {
			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++) {
					//std::cout << (m_pix[i][j].b + m_pix[i][j].g + m_pix[i][j].r) / 3 << std::endl;
					int color = ((int)m_pix[i][j].b + (int)m_pix[i][j].g + (int)m_pix[i][j].r) / 3;

					m_pix[i][j].g = color;
					m_pix[i][j].r = color;
					m_pix[i][j].b = color;

					if ((m_pix[i][j].b != m_pix[i][j].g) || (m_pix[i][j].g != m_pix[i][j].r) || (m_pix[i][j].b != m_pix[i][j].r)) {
						std::cout << (int)m_pix[i][j].b << std::endl;
						std::cout << (int)m_pix[i][j].r << std::endl;
						std::cout << (int)m_pix[i][j].g << std::endl;
					}
				}
		}

		void BW2() {
			for (int i = 1; i < m_h-1; i++)
				for (int j = 1; j < m_w-1; j++) {
					
					m_pix[i][j].b = ((m_pix[i][j].b + m_pix[i][j].g + m_pix[i][j].r) + (m_pix[i][j - 1].b + m_pix[i][j - 1].g + m_pix[i][j - 1].r) + (m_pix[i][j + 1].b + m_pix[i][j + 1].g + m_pix[i][j + 1].r) + (m_pix[i - 1][j].b + m_pix[i - 1][j].g + m_pix[i - 1][j].r) + (m_pix[i - 1][j - 1].b + m_pix[i - 1][j - 1].g + m_pix[i - 1][j - 1].r) + (m_pix[i - 1][j + 1].b + m_pix[i - 1][j + 1].g + m_pix[i - 1][j + 1].r) + (m_pix[i + 1][j].b + m_pix[i + 1][j].g + m_pix[i + 1][j].r) + (m_pix[i + 1][j - 1].b + m_pix[i + 1][j - 1].g + m_pix[i + 1][j - 1].r) + (m_pix[i + 1][j + 1].b + m_pix[i + 1][j + 1].g + m_pix[i + 1][j + 1].r)) / 27;
					
					m_pix[i][j].g = ((m_pix[i][j].b + m_pix[i][j].g + m_pix[i][j].r) + (m_pix[i][j - 1].b + m_pix[i][j - 1].g + m_pix[i][j - 1].r) + (m_pix[i][j + 1].b + m_pix[i][j + 1].g + m_pix[i][j + 1].r) + (m_pix[i - 1][j].b + m_pix[i - 1][j].g + m_pix[i - 1][j].r) + (m_pix[i - 1][j - 1].b + m_pix[i - 1][j - 1].g + m_pix[i - 1][j - 1].r) + (m_pix[i - 1][j + 1].b + m_pix[i - 1][j + 1].g + m_pix[i - 1][j + 1].r) + (m_pix[i + 1][j].b + m_pix[i + 1][j].g + m_pix[i + 1][j].r) + (m_pix[i + 1][j - 1].b + m_pix[i + 1][j - 1].g + m_pix[i + 1][j - 1].r) + (m_pix[i + 1][j + 1].b + m_pix[i + 1][j + 1].g + m_pix[i + 1][j + 1].r)) / 27;
					
					m_pix[i][j].r = ((m_pix[i][j].b + m_pix[i][j].g + m_pix[i][j].r) + (m_pix[i][j - 1].b + m_pix[i][j - 1].g + m_pix[i][j - 1].r) + (m_pix[i][j + 1].b + m_pix[i][j + 1].g + m_pix[i][j + 1].r) + (m_pix[i - 1][j].b + m_pix[i - 1][j].g + m_pix[i - 1][j].r) + (m_pix[i - 1][j - 1].b + m_pix[i - 1][j - 1].g + m_pix[i - 1][j - 1].r) + (m_pix[i - 1][j + 1].b + m_pix[i - 1][j + 1].g + m_pix[i - 1][j + 1].r) + (m_pix[i + 1][j].b + m_pix[i + 1][j].g + m_pix[i + 1][j].r) + (m_pix[i + 1][j - 1].b + m_pix[i + 1][j - 1].g + m_pix[i + 1][j - 1].r) + (m_pix[i + 1][j + 1].b + m_pix[i + 1][j + 1].g + m_pix[i + 1][j + 1].r)) / 27;

				}



		}

		void Rotate(double angle)
		{
			// 1.   
			Vec2d T({ {
				{(double)(m_w / 2)},
				{(double)(m_h / 2)}
			} });

			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
					m_coordinates[i][j] = m_coordinates[i][j] - T;

			// 2. 
			Mat22d R({ {
				{cos(angle), sin(angle)},
				{-sin(angle), cos(angle)}
			} });

			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
				{
					m_coordinates[i][j] = R * m_coordinates[i][j];
					//std::cout << m_coordinates[i][j] << std::endl;
				}

			// 3.      
			int maxX = INT_MIN;
			int minX = INT_MAX;
			int maxY = INT_MIN;
			int minY = INT_MAX;
			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
				{
					if (maxX < m_coordinates[i][j].get(0, 0))
						maxX = m_coordinates[i][j].get(0, 0);
					if (minX > m_coordinates[i][j].get(0, 0))
						minX = m_coordinates[i][j].get(0, 0);
					if (maxY < m_coordinates[i][j].get(1, 0))
						maxY = m_coordinates[i][j].get(1, 0);
					if (minY > m_coordinates[i][j].get(1, 0))
						minY = m_coordinates[i][j].get(1, 0);
				}

			//       -  
			maxX++;
			minX--;
			maxY++;
			minY--;

			int width = maxX - minX;
			int height = maxY - minY;

			//     
			Vec2d shift({ {
				{(double)(width / 2)},
				{(double)(height / 2)}
			} });

			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
					m_coordinates[i][j] = m_coordinates[i][j] + shift;

			//   
			Pixel** new_pixels = new Pixel *[height];
			for (int i = 0; i < height; i++)
				new_pixels[i] = new Pixel[width];

			Vec2d** new_coordinates = new Vec2d *[height];
			for (int i = 0; i < height; i++)
				new_coordinates[i] = new Vec2d[width];

			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
					new_pixels[i][j] = { 0,0,0 };

			for (int i = 0; i < height; i++)
				for (int j = 0; j < width; j++)
				{
					new_coordinates[i][j].set(0, 0, j);
					new_coordinates[i][j].set(0, 0, i);
				}

			//  
			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++)
				{
					int x = (int)(m_coordinates[i][j].get(0, 0));
					int y = (int)(m_coordinates[i][j].get(1, 0));
					new_pixels[y][x] = m_pix[i][j];
				}

			//   
			for (int i = 0; i < m_h; i++)
				delete[] m_pix[i];
			delete[] m_pix;

			for (int i = 0; i < m_h; i++)
				delete[] m_coordinates[i];
			delete[] m_coordinates;

			m_pix = new_pixels;
			m_coordinates = new_coordinates;

			m_w = width;
			m_h = height;

			for (int i =0; i<m_h;i++)
				for (int j = 0; j < m_w; j++) {
					if ((m_pix[i][j].b + m_pix[i][j].g + m_pix[i][j].r == 0) && (m_pix[i][j + 1].b + m_pix[i][j + 1].g + m_pix[i][j + 1].r == 0)) {
						m_pix[i][j] = { 255, 255, 255 };
					}				
				}
		}

		void Interpole() {
			for (int i = 1; i < m_h - 1; i++)
				for (int j = 1; j < m_w - 1; j++) {
					if (m_pix[i][j].b + m_pix[i][j].g + m_pix[i][j].r == 0)
					{
						int count = 0;
						int sumR = 0;
						int sumG = 0;
						int sumB = 0;
						for (int x = -1; x < 2; x++)
							for (int y = -1; y < 2; y++) {
								if (m_pix[i + x][j + y].b + m_pix[i + x][j + y].g + m_pix[i + x][j + y].r != 0){
									count += 1;
									sumR += m_pix[i + x][j + y].r;
									sumG += m_pix[i + x][j + y].g;
									sumB += m_pix[i + x][j + y].b;
								}
							}
						if (count != 0) {
							m_pix[i][j].r += sumR / count;
							m_pix[i][j].g += sumG / count;
							m_pix[i][j].b += sumB / count;
						}
					}
				}
		}

		void Incrypt(char digit) {//функция шифрования текстовой информации в изображении BMP
			//int space = 0;
			//for (int i = 0; i < m_h; i++)
				//for (int j = 0; j < m_w; j++) {
			if (digit == '0') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[4][m_space + 2].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[3][m_space + 3].g += 1;
				m_pix[2][m_space].g += 1;
				m_pix[2][m_space + 3].g += 1;
				m_pix[1][m_space].g += 1;
				m_pix[1][m_space + 3].g += 1;
				m_pix[0][m_space + 1].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}
			if (digit == '1') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[3][m_space + 1].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[1][m_space + 1].g += 1;
				m_pix[0][m_space + 1].g += 1;
			}
			if (digit == '2') {
				m_pix[4][m_space].g += 1;
				m_pix[4][m_space + 1].g += 1;
				m_pix[3][m_space + 2].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[1][m_space].g += 1;
				m_pix[0][m_space].g += 1;
				m_pix[0][m_space + 1].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}
			if (digit == '3') {
				m_pix[4][m_space].g += 1;
				m_pix[4][m_space + 1].g += 1;
				m_pix[3][m_space + 2].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[1][m_space + 2].g += 1;
				m_pix[0][m_space].g += 1;
				m_pix[0][m_space + 1].g += 1;
			}
			if (digit == '4') {
				m_pix[4][m_space].g += 1;
				m_pix[4][m_space + 2].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[3][m_space + 2].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[2][m_space + 3].g += 1;
				m_pix[1][m_space + 2].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}
			if (digit == '5') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[4][m_space + 2].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[2][m_space].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[1][m_space + 2].g += 1;
				m_pix[0][m_space].g += 1;
				m_pix[0][m_space + 1].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}
			if (digit == '6') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[4][m_space + 2].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[2][m_space].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[1][m_space].g += 1;
				m_pix[1][m_space + 3].g += 1;
				m_pix[0][m_space + 1].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}
			if (digit == '7') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[4][m_space + 2].g += 1;
				//m_pix[3][m_space].g += 1;
				m_pix[3][m_space + 3].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[1][m_space + 1].g += 1;
				m_pix[0][m_space].g += 1;
			}
			if (digit == '8') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[4][m_space + 2].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[3][m_space + 3].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[1][m_space].g += 1;
				m_pix[1][m_space + 3].g += 1;
				m_pix[0][m_space + 1].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}
			if (digit == '9') {
				m_pix[4][m_space + 1].g += 1;
				m_pix[4][m_space + 2].g += 1;
				m_pix[3][m_space].g += 1;
				m_pix[3][m_space + 3].g += 1;
				m_pix[2][m_space + 1].g += 1;
				m_pix[2][m_space + 2].g += 1;
				m_pix[2][m_space + 3].g += 1;
				m_pix[1][m_space + 3].g += 1;
				m_pix[0][m_space + 1].g += 1;
				m_pix[0][m_space + 2].g += 1;
			}

			m_space += 6;
		}

		void Decrypt() {// Функция дешифрования текстовой информации на основе изменения цвета
			for (int i = 0; i < m_h; i++)
				for (int j = 0; j < m_w; j++) {
					m_pix[i][j].b *= 200;
					m_pix[i][j].g *= 200;
					m_pix[i][j].r *= 200;
				}
		}




	private:
		int m_w, m_h, m_space = 0;
		Pixel** m_pix;
		Vec2d** m_coordinates = nullptr;
#pragma pack(1) 
		struct BMPHEADER
		{
			unsigned short    Type;
			unsigned int      Size;
			unsigned short    Reserved1;
			unsigned short    Reserved2;
			unsigned int      OffBits;
		};
#pragma pack()

#pragma pack(1)
		struct BMPINFO
		{
			unsigned int    Size;
			int             Width;
			int             Height;
			unsigned short  Planes;
			unsigned short  BitCount;
			unsigned int    Compression;
			unsigned int    SizeImage;
			int             XPelsPerMeter;
			int             YPelsPerMeter;
			unsigned int    ClrUsed;
			unsigned int    ClrImportant;
		};
#pragma pack()
	};

}

