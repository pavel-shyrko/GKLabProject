#include <iostream>

#pragma once

#ifndef CSVMATERIAL_H
#define CSVMATERIAL_H

struct CsvMaterial
{
	int Num;
	std::string Material;
	std::string MaterialPath;
	std::string MaterialFullName;
	std::string Texture;
	std::string ColorRed;
	std::string ColorGreen;
	std::string ColorBlue;
	std::string Comment;
	std::string DocumentText;
};

#endif /* CSVMATERIAL_H */
