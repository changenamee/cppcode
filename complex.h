#ifndef __COMPLEX__
#define __COMPLEX__
class Complex;
Complex& __doapl(Complex* ths,const Complex& com);

class Complex{
    public:
        Complex(double r = 0,double i = 0):re(r),im(i){}
        double real() const {return re;}    //返回实部；
        double imagin() const {return im;}  //返回虚部；
        Complex& operator +=(const Complex&);
    private:
        double re,im; //实部虚部
        friend Complex& __doapl(Complex*,const Complex&);
};

inline Complex& 
__doapl(Complex* ths,const Complex& com){
    ths->re += com.re;
    ths->im += com.im;
    return *ths;
}
Complex& Complex::operator +=(const Complex& com){
    return __doapl(this,com);
}

inline double
real(const Complex& c){
    return c.real();
}
inline double
imag(const Complex& c){
    return c.imagin();
}

inline Complex
operator +(const Complex& c1,const Complex& c2){
    return Complex(real(c1)+real(c2),imag(c1)+imag(c2));
}
inline Complex
operator +(const double x,const Complex& y){
    return Complex(x+real(y),imag(y));
}
inline Complex
operator +(const Complex& x,double y){
    return Complex(real(x)+y,imag(x));
}
#endif