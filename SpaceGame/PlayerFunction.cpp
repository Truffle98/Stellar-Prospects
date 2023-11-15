#include "PlayerFunction.h"

#include "ConsoleBase.h"

PlayerFunction::PlayerFunction(std::string funcName, bool (ConsoleBase::* func)(const std::vector<std::string>&))
{
	_funcName = funcName;
	_func = func;
}

bool PlayerFunction::CallFunc(ConsoleBase* console, const std::vector<std::string>& args)
{
	return (*console.*_func)(args);
}

bool PlayerFunction::CheckArgs(const std::vector <std::string>& args, const std::vector<std::string>& params)
{
	if (params.size() != args.size() - 1)
	{
		return false;
	}

	for (int i = 0; i < params.size(); i++)
	{
		if (params[i] == "int")
		{
			try
			{
				int intValue = std::stoi(args[i + 1]);
			}
			catch (const std::exception& e)
			{
				return false;
			}
		}
	}

	return true;
}

std::string PlayerFunction::ReturnName()
{
	return _funcName;
}