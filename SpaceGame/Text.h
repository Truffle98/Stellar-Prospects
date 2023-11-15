#pragma once
#include <string>
#include <vector>
#include <iostream>

#include "TextPiece.h"

class Text
{

private:

	std::string _text;
	std::string _fullText;
	std::vector<TextPiece> _pieces;

	int _idx;
	int _printSpeed;
	int _timer;
	bool _writing;

public:

	Text(std::string text, int printSpeed);

	void SetPieces(std::vector<TextPiece> pieces);

	void ManageText();

	void FinishText();

	std::string ReturnText();

	bool ReturnWriting();
};

