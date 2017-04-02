#include <iostream>

#pragma once

#ifndef CSVELEMENT_H
#define CSVELEMENT_H

struct CsvElement
{
	int Num;
	std::wstring Level;
	std::wstring Actor;
	std::wstring UniqueID;
	std::wstring ActorFullName;
	std::wstring Mesh;
	int Element;
	std::wstring Material;
	std::wstring Comment;
	std::wstring DocumentPlaceholder;
	std::wstring DocumentText;
};

#endif /* CSVELEMENT_H */
