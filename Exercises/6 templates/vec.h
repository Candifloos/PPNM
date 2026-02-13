#pragma once
#include <string>
#include <type_traits>
#include <complex>
#include <utility>
#include<iostream>
#include <typeinfo>

const std::complex<double> imaginary_unit(0.0,1.0); 

template<typename T> struct vec {
    T x, y, z;
    
    // constructors
    vec(T x, T y, T z) : x(x), y(y), z(z) {}
    vec() : vec(T(0), T(0), T(0)) {}                           // default
    vec(const vec&) = default;                                 // copy  a(b) kopierer b til a uden & vil den bare kopiere (kan være rough for store), har derfor & som bare giver en reference - const for at den ikke skal ændre den vektor man kopierer
    vec(vec&&) = default;                                      // move  a(b+c) med && smider b+c til a
    ~vec() = default;                                          // destructor
    
    // assignment
    vec& operator=(const vec&) = default;                     // copy assignment   redefinerer = operator, a = b, vec vil lave en deep copy men vec& vil lave reference
    vec& operator=(vec&&) = default;                          // move assignment   a = b + c, vil generelt gerne give reference & til function da den laver en copy af det argument den får som kan være costly, kan så bruge const for at sikre sig at den ikke ændres
    
    
    // arithmetic
    //if <T> is non complex and <S> is please use explicit vector arithmetic which casts to complex (a + b rather than a+=b)
    template <typename S> vec& operator+=(const vec<S>& o) { x += o.x; y += o.y; z += o.z; return *this; }  //ved ikke om det er mindre safe end explicit at bruge static_cast som længere nede, castes tilbage til <T> though
    template <typename S> vec& operator-=(const vec<S>& o) { x -= o.x; y -= o.y; z -= o.z; return *this; } 
    template<typename S> vec& operator*=(S s){
        x = static_cast<T>(static_cast<S>(x) * s);  y = static_cast<T>(static_cast<S>(y) * s);  z = static_cast<T>(static_cast<S>(z) * s);  //x,y,z bliver cast til whatever s er hvorefter det castes tilbage til hvad x,y,z var
        return *this; 
    };
    // vec& operator/=(T);
    template<typename S> vec& operator/=(S s){
        x = static_cast<T>(x / s);  y = static_cast<T>(y / s);  z = static_cast<T>(z / s);  //x,y,z bliver cast til whatever s er hvorefter det castes tilbage til hvad x,y,z var
        return *this; 
    };

    // utility
    void set(T a, T b, T c) { x = a; y = b; z = c; }

    void print(const std::string& s = "") const;
    double norm() const;

};

template<typename T>
double vec<T>::norm() const {
    auto ax = std::abs(x), ay = std::abs(y), az = std::abs(z);
    return std::sqrt(ax*ax + ay*ay + az*az);
}


template<typename T>
void vec<T>::print(const std::string& s) const {
    std::cout << s << x << " " << y << " " << z << "\n";
}
template<typename T>
std::ostream& operator<<(std::ostream& os, const vec<T>& v) {       //overrides <<
    return os << '(' << v.x << ", " << v.y << ", " << v.z << ')';
}


template<typename> struct is_complex : std::false_type {};                      //"traits take a type and say something about that type" - siger her om noget er komplekst
template<typename T> struct is_complex<std::complex<T>> : std::true_type {};    //https://stackoverflow.com/questions/58694521/what-is-stdfalse-type-or-stdtrue-type

// non-member operators, allows mixed typing and casts to most complex type
template<typename A, typename B>
// auto operator+(vec<A> const& a, vec<B> const& b) {      //ækvivalent med nedenstående i believe
auto operator+(const vec<A>& a, const vec<B>& b) {      //bruger auto da den skal caste til vec<R>
    using R = std::common_type_t<A,B>;
    return vec<R>( static_cast<R>(a.x) + static_cast<R>(b.x),
                   static_cast<R>(a.y) + static_cast<R>(b.y),
                   static_cast<R>(a.z) + static_cast<R>(b.z) );
}
template<typename A, typename B>
auto operator-(const vec<A>& a, const vec<B>& b) {
    using R = std::common_type_t<A,B>;
    return vec<R>( static_cast<R>(a.x) - static_cast<R>(b.x),
                   static_cast<R>(a.y) - static_cast<R>(b.y),
                   static_cast<R>(a.z) - static_cast<R>(b.z) );
}

template<typename T, typename S>
auto operator*(vec<T> const& v, S s) {                  //v * s
    using R = std::common_type_t<T,S>;
    return vec<R>(v.x * s, v.y * s, v.z * s);
}
template<typename S, typename T>                        //s * v
auto operator*(S s, vec<T> const& v) { return v * s; }
template<typename T, typename S>
auto operator/(vec<T> const& v, S s) {
    using R = std::common_type_t<T,S>;
    return vec<R>(v.x / s, v.y / s, v.z / s);
}
template<typename T>
auto operator-(const vec<T>& v){
    T x = -v.x; T y = -v.y; T z = -v.z;
    return vec(x,y,z);
}

template<typename A, typename B>                       
auto dot(const vec<A>& a, const vec<B> b){
    using R = std::common_type_t<A,B>;
    if constexpr (is_complex<R>::value){                //noget med at constexpr gør at det tjekkes ved compile time i stedet for runtime
        R ax = static_cast<R>(a.x);
        R ay = static_cast<R>(a.y);
        R az = static_cast<R>(a.z);
        R bx = static_cast<R>(b.x);
        R by = static_cast<R>(b.y);
        R bz = static_cast<R>(b.z);
        std::complex complexdot =   ax * std::conj(bx) + ay * std::conj(by) + az * std::conj(bz);
        
        //ækvivalent med ovenstående
        // std::complex complexdot =   (ax.real() + ax.imag() * imaginary_unit) * (bx.real() - bx.imag() * imaginary_unit) *
        //                             (ay.real() + ay.imag() * imaginary_unit) * (by.real() - by.imag() * imaginary_unit) *
        //                             (az.real() + az.imag() * imaginary_unit) * (bz.real() - bz.imag() * imaginary_unit);
        return complexdot;
    }
    else {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    };
    
}

template<typename A, typename B>  
auto cross(const vec<A>& a, const vec<B>& b){
    double newx = a.y * b.z - a.z * b.y;
    double newy = a.z * b.x - a.x * b.z;
    double newz = a.x * b.y - a.y * b.x;
    return vec(newx, newy, newz);
}

// approximate equality
template<typename T, typename S>  
bool approx(T a, S b, double acc = 1e-6, double eps = 1e-6){ //should maybe implement it giving a heads-up if compairing int to complex since 2, (2,0) is just true for this
    using R = std::common_type_t<T,S>;
    if constexpr (is_complex<R>::value){
        auto acast = static_cast<R>(a); auto bcast = static_cast<R>(b);
        double realdiff = std::abs(acast.real() - bcast.real()); double imagdiff = std::abs(acast.imag() - bcast.imag());
        if ((realdiff + imagdiff) / 2.0 <= acc) return true;
        double max_ab = std::abs(std::max(std::abs(static_cast<R>(acast)), std::abs(static_cast<R>(bcast))));       //good enough ig
        return (realdiff + imagdiff) / 2 <= eps * max_ab;
    }
    else{
        R diff = std::abs(a-b);
    if (diff <= acc) return true;  // absolute tolerance
    double max_ab = std::max(std::abs(static_cast<R>(a)), std::abs(static_cast<R>(b)));
    return diff <= eps * max_ab;   // relative tolerance
    };
}

template<typename A, typename B>  
bool vec_approx(const vec<A>& a, const vec<B>& b, double acc = 1e-6, double eps = 1e-6){
	if(!approx(a.x,b.x, acc, eps))return false;
	if(!approx(a.y,b.y, acc, eps))return false;
	if(!approx(a.z,b.z, acc, eps))return false;
	return true;
;
}
