#include "error.h"

void setErrorState(ERRORType i)
{
	errState = i;
}
ERRORType getErrorState()
{
	return errState;
}