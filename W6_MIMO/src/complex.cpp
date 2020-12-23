#include "const.h"

// define member function in Complex

Complex Exp(f32 input)
{
    Complex temp(0.0, 0.0);

    temp.set_real(cos(input));
    temp.set_image(sin(input));

    return temp;
}

Complex::Complex(const Complex &c)
{
    this->m_real = c.real();
    this->m_image = c.image();
}

Complex Complex::conj()
{
    Complex temp(0.0, 0.0);
    
    temp.set_real(this->m_real);
    temp.set_image(-this->m_image);

    return temp;
}

Complex & Complex::operator=(const Complex c)
{
    this->m_real = c.real();
    this->m_image = c.image();

    return *this;
}

Complex Complex::operator+(const Complex c)
{
    Complex temp(0.0, 0.0);

    temp.set_real(this->m_real + c.real());
    temp.set_image(this->m_image + c.image());

    return temp;
}

Complex Complex::operator-(const Complex c)
{
    Complex temp(0.0, 0.0);

    temp.set_real(temp.real() - c.real());
    temp.set_image(temp.image() - c.image());

    return temp;
}

Complex & Complex::operator+=(const Complex c)
{
    this->m_real += c.real();
    this->m_image += c.image();

    return *this;
}

Complex & Complex::operator-=(const Complex c)
{
    this->m_real -= c.real();
    this->m_image -= c.image();

    return *this;
}

Complex Complex::operator*(const Complex c)
{
    Complex temp(0.0, 0.0);

    temp.set_real(this->m_real * c.real()
                - this->m_image * c.image());
    temp.set_image(this->m_image * c.real()
                + this->m_real * c.image());

    return temp;
}

Complex Complex::operator*(const u32 num)
{
    Complex temp(0.0, 0.0);

    temp.set_real(this->m_real * num);
    temp.set_image(this->m_image * num);

    return temp;
}

ostream & operator<<(ostream &os, const Complex &c)
{
    os << c.real() << " + " << c.image() << "j";
    return os;
}

Complex operator*(u32 num, const Complex & c)
{
    Complex temp(0.0, 0.0);

    temp.set_real(c.real() * num);
    temp.set_image(c.image() * num);

    return temp;
}

f32 distance(const Complex &c1, const Complex &c2)
{
    f32 D;

    D = sqrt((c1.real() - c2.real()) * (c1.real() - c2.real()) 
        + (c1.image() - c2.image()) * (c1.image() - c2.image()));

    return D;
}

f32 Complex::norm()
{
    f32 norm = 0.0;

    norm = this->m_real * this->m_real + this->m_image * this->m_image;

    return norm;
}

Complex Complex::divide(Complex c)
{
    Complex temp = {0.0, 0.0};

    temp = (*this) * c.conj();
    temp.set_real(temp.real() / c.norm());
    temp.set_image(temp.image() / c.norm());

    return temp;
}

Complex operator*(f32 num, const Complex & c)
{
    Complex temp(0.0, 0.0);

    temp.set_real(c.real() * num);
    temp.set_image(c.image() * num);

    return temp;
}

Complex Complex::operator*(const f32 num)
{
    Complex temp(0.0, 0.0);

    temp.set_real(this->m_real * num);
    temp.set_image(this->m_image * num);

    return temp;
}