#include "inputmethod.h"

InputMethod::InputMethod()
{
}

void InputMethod::sendString(QString str)
{
	sendCommitString(str);
}
