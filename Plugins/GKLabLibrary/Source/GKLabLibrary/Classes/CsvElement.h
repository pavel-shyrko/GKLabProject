#include <iostream>

#pragma once

#ifndef CSVELEMENT_H
#define CSVELEMENT_H

struct CsvElement
{
	int Num;
	std::string Level;
	std::string Actor;
	std::string ActorFName;
	std::string UniqueID;
	std::string ActorFullName;
	std::string Mesh;
	int Element;
	std::string Comment;
	std::string DocumentPlaceholder;
	std::string DocumentText;
};

#endif /* CSVELEMENT_H */
