#ifndef INPUTMETHOD_H
#define INPUTMETHOD_H

#include <QtGui/QWSInputMethod>

class InputMethod : public QWSInputMethod
{
public:
    InputMethod();

	void sendString(QString str);
};

#endif // INPUTMETHOD_H
