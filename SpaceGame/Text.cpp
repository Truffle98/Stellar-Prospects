#include "Text.h"

Text::Text(std::string text, int printSpeed)
{
	_fullText = text;

	if (printSpeed == -1)
	{
		_printSpeed = -1;
		_idx = text.length() - 1;
		_text = _fullText;
		_writing = false;
	}
	else
	{
		_printSpeed = 60.0f / printSpeed;
		_idx = -1;
		_writing = true;
	}
	
	_timer = _printSpeed;
}

void Text::SetPieces(std::vector<TextPiece> pieces)
{
	_pieces = pieces;
}

void Text::ManageText()
{
	if (_pieces.size() > 0)
	{
		int pieceIdx = 0;
		_text = "";

		for (int i = 0; i < _fullText.length(); i++)
		{
			if (_fullText[i] == '~')
			{
				if (pieceIdx == _pieces.size())
				{
					std::cout << "Pieces count too low" << std::endl;
					exit(-1);
				}

				_text += _pieces[pieceIdx].ReturnValue();
				pieceIdx++;
			}
			else
			{
				_text += _fullText[i];
			}
		}

		if (pieceIdx != _pieces.size())
		{
			std::cout << "Pieces count too high" << std::endl;
			exit(-1);
		}

		return;
	}

	if (!_writing)
	{
		return;
	}

	if (_timer > 0)
	{
		_timer--;
		return;
	}

	_idx++;
	_timer = _printSpeed;
	_text = _fullText.substr(0, _idx + 1);

	if (_fullText[_idx] == '.')
	{
		_timer *= 30;
	}

	if (_idx == _fullText.length() - 1)
	{
		_writing = false;
	}

	return;
}

void Text::FinishText()
{
	_idx = _fullText.length() - 1;
	_text = _fullText;
	_writing = false;
}

std::string Text::ReturnText()
{
	return _text;
}

bool Text::ReturnWriting()
{
	return _writing;
}