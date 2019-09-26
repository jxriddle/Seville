#ifndef APP_FRACTION_H
#define APP_FRACTION_H


namespace App
{
   class Fraction
   {
      private:
         int myNumerator;
         int myDenominator;

         void doClear() { myNumerator = 0; myDenominator = 0; }

      public:
         int numerator() { return myNumerator; }
         void setNumerator(int value) { myNumerator = value; }
         int denominator() { return myDenominator; }
         void setDenominator(int value) { myDenominator = value; }

         int part() { return myNumerator; }
         void setPart(int value) { myNumerator = value; }
         int whole() { return myDenominator; }
         void setWhole(int value) { myDenominator = value; }

         int count() { return myNumerator; }
         void setCount(int value) { myNumerator = value; }
         int total() { return myDenominator; }
         void setTotal(int value) { myDenominator = value; }

         void setFraction(int n, int d) { myNumerator = n; myDenominator = d; }

         void clear() { doClear(); }

         Fraction(int numerator, int denominator);
         Fraction();
   };

}

#endif // APP_FRACTION_H
