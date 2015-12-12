#include "targetver.h"

#include "ValidationResultList.h"

bool isValid(const ValidationResultList & iResults)
{
  for(size_t i=0; i<iResults.size(); i++)
  {
    const VALIDATION_RESULT & result = iResults[i];
    if (!result.isValid)
      return false;
  }
  return true;
}
