#pragma once
#include <string>
#include <vector>

class ConsoleBase;

class PlayerFunction
{

private:
	
	std::string _funcName;
	bool (ConsoleBase::* _func)(const std::vector<std::string>&);

public:

	PlayerFunction(std::string funcName, bool (ConsoleBase::* func)(const std::vector<std::string>&));

	bool CallFunc(ConsoleBase* console, const std::vector<std::string>& args);

	static bool CheckArgs(const std::vector <std::string> &args, const std::vector<std::string>& params);

	std::string ReturnName();

};

