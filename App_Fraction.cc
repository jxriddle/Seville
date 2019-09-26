#include "App_Fraction.h"

namespace App
{
   Fraction::Fraction(int numerator, int denominator)
   {
      myNumerator = numerator;
      myDenominator = denominator;
   }

   Fraction::Fraction()
   {
      clear();
   }

}
