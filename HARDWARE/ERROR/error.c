#include "error.h"

static ERRORType errState = 0;

void setErrorState(ERRORType i)
{
	errState = i;
}
ERRORType getErrorState()
{
	return errState;
}
