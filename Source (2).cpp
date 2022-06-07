#include <iostream>
#include "BMP.hpp"

int main()
{
	try
	{
		paint::BMP bmp(100, 100);
		//original_image.Fill({ 255,255,255 });
		//test_bmp.Open("city.bmp");
		bmp.Fill(0, 0, 0);
		//original_image.Save("original.bmp");
		int n;
		char c;
		std::cout << "Type count of digits: " << std::endl;
		std::cin >> n;
		for (int i = 0; i < n; i++) {
			std::cout << "Type " << i << " digit: " << std::endl;
			std::cin >> c;
			bmp.Incrypt(c);
		}
		bmp.Save("do.bmp");


		
		paint::BMP bmp1(100, 100);
		bmp1.Open("do.bmp");
		
		
		
		bmp1.Decrypt();
		//original_image.BW1();
		//original_image.Save("A.bmp");
		//test_bmp.Fill(0, 255, 0);
		//test_bmp.Rotate(acos(-1) / 4);
		//test_bmp.Interpole();
		bmp1.Save("posle.bmp");


		

	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return 0;
}