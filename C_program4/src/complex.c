#include "const.h"

Complex complex_add(Complex c1, Complex c2)
{
    Complex temp = {0.0, 0.0};

    temp.real = c1.real + c2.real;
    temp.image = c1.image + c2.image;

    return temp;
}

Complex complex_multiply(Complex c1, Complex c2)
{
    Complex temp = {0.0, 0.0};

    temp.real = c1.real * c2.real - c1.image * c2.image;
    temp.image = c1.image * c2.real + c1.real * c2.image;

    return temp;
}

Complex conjugate(Complex c)
{
    Complex temp = {0.0, 0.0};

    temp.real = c.real;
    temp.image = -c.image;

    return temp;
}

Complex Exp(double input)
{
    Complex temp = {0.0, 0.0};

    temp.real = cos(input);
    temp.image = sin(input);

    return temp;
}