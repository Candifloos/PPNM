#pragma once
#include <string>

struct vec {
    double x, y, z;

    // constructors
    vec(double x, double y, double z) : x(x), y(y), z(z) {}  // parameterized   kan referere til dem som self (eller this i c++)
    vec() : vec(0, 0, 0) {}                                    // default
    vec(const vec&) = default;                                 // copy  a(b) kopierer b til a uden & vil den bare kopiere (kan være rough for store), har derfor & som bare giver en reference - const for at den ikke skal ændre den vektor man kopierer
    vec(vec&&) = default;                                      // move  a(b+c) med && smider b+c til a
    ~vec() = default;                                          // destructor

    // assignment
    vec& operator=(const vec&) = default;                     // copy assignment   redefinerer = operator, a = b, vec vil lave en deep copy men vec& vil lave reference
    vec& operator=(vec&&) = default;                          // move assignment   a = b + c, vil generelt gerne give reference & til function da den laver en copy af det argument den får som kan være costly, kan så bruge const for at sikre sig at den ikke ændres

    // arithmetic
    vec& operator+=(const vec&);                            //overrider + operator (equivalent med at definere en __plus__ i python), havde man bare vec vil den lave en kopi men vil have den ændret (da +=)
    vec& operator-=(const vec&);
    vec& operator*=(double);
    vec& operator/=(double);

    // utility
    void set(double a, double b, double c) { x = a; y = b; z = c; }
    void print(const std::string& s = "") const;              // for debugging
    double norm();



    // stream output
    friend std::ostream& operator<<(std::ostream&, const vec&);

    // void print(const std::string& s) const;
};

// non-member operators
vec operator-(const vec&);
vec operator-(const vec&, const vec&);  //laver en ny vec så ingen &, vil dog ikke ændre bc (a=b+c) så giver dem references (for ikke at lave kopi) og constant så de ikke ændres
vec operator+(const vec&, const vec&);
vec operator*(const vec&, double);
vec operator*(double, const vec&);
vec operator/(const vec&, double);


double dot(const vec&, const vec&);
vec cross(const vec&, const vec&);

// approximate equality
bool approx(double, double, double acc = 1e-6, double eps = 1e-6);
bool vec_approx(const vec& a, const vec& b, double acc = 1e-6, double eps = 1e-6);

std::ostream& operator<<(std::ostream&, const vec&);