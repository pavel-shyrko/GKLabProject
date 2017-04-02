#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <iterator>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <cctype>

#pragma once

class CSVRow
{
public:
	std::wstring const& operator[](std::size_t index) const
	{
		return m_data[index];
	}

	std::size_t size() const
	{
		return m_data.size();
	}

	void readNextRow(std::wistream& str)
	{
		std::wstring line;
		std::getline(str, line);

		std::wstringstream  lineStream(line);
		std::wstring cell;

		m_data.clear();
		while (std::getline(lineStream, cell, L','))
		{
			m_data.push_back(cell);
		}
		// This checks for a trailing comma with no data after it.
		if (!lineStream && cell.empty())
		{
			// If there was a trailing comma then add an empty element.
			m_data.push_back(L"");
		}
	}

private:
	std::vector<std::wstring> m_data;
};

