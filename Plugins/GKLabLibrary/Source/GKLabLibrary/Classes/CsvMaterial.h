#include <iostream>

#pragma once

#ifndef CSVMATERIAL_H
#define CSVMATERIAL_H

struct CsvMaterial
{
	int Num;
	std::wstring Material;
	std::wstring MaterialFullName;
	std::wstring Texture;
	std::wstring ColorRed;
	std::wstring ColorGreen;
	std::wstring ColorBlue;
	std::wstring Comment;
	std::wstring DocumentText;
};

#endif /* CSVMATERIAL_H */
