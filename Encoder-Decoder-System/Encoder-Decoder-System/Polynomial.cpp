#include "Polynomial.h"
#include "Logger.h"

namespace wd_codec
{

    namespace galois
    {
        inline Polynomial& Polynomial::operator <<= (const unsigned int& n) {
            //to shift  right the polynomial n places, nX0 in begin
            size_t  init_size= poly_.size();
            if (init_size > 0) {
                poly_.resize(init_size + n,Field_Element(field_,0));
                for (size_t i = init_size - 1; static_cast<int>(i) >= 0; --i)//move 
                {
                    poly_[i + n] = poly_[i];
                }
                for (unsigned int i = 0; i < n; ++i)//fill 0
                {
                    poly_[i] = 0;
                }
            }
            return *this;
        

        }
        inline Polynomial& Polynomial::operator >>= (const unsigned int& n) {
            // to shift  left  the polynomial n  places, make it smaller size-n
            if (n <= poly_.size())
            {
                for (unsigned int i = 0; i <= deg() - n; ++i)
                {
                    poly_[i] = poly_[i + n];
                }

                poly_.resize(poly_.size() - n, Field_Element(field_, 0));
            }
            else if (static_cast<int>(n) >= (deg() + 1))
            {
                poly_.resize(0, Field_Element(field_, 0));
        inline Polynomial::Polynomial(const Field& gfield)
            : field_(const_cast<Field&>(gfield))
        {
            wd_codec::Logger::log(wd_codec::INFO, " Start Generate Polynomial.");
            poly_.clear();
            poly_.reserve(256);//set the capacity to 256
            wd_codec::Logger::log(wd_codec::INFO, " Finish Generate Polynomial.");

        }
        inline Polynomial::Polynomial(const Field& gfield, const unsigned int& degree)
            : field_(const_cast<Field&>(gfield))
        {
            wd_codec::Logger::log(wd_codec::INFO, " Start Generate Polynomial.");
            poly_.reserve(256);//set the capacity to 256
            //resize the poly_ to degree+1 size, and peds the poly_ with  Field_Element(field_, 0)
            poly_.resize(degree + 1, Field_Element(field_, 0));
            wd_codec::Logger::log(wd_codec::INFO, " Finish Generate Polynomial.");
        }

        inline Polynomial::Polynomial(const Field& gfield, const unsigned int& degree, const Field_Element element[])
            : field_(const_cast<Field&>(gfield))
        {
            wd_codec::Logger::log(wd_codec::INFO, " Start Generate Polynomial.");
            poly_.reserve(256);

            if (element != NULL)
            {
                /*
                   It is assumed that element is an array of field elements
                   with size/element count of degree + 1.
                */
                for (unsigned int i = 0; i <= degree; ++i)
                {
                    poly_.push_back(element[i]);
                }
            }
            else
                poly_.resize(degree + 1, Field_Element(field_, 0));
            wd_codec::Logger::log(wd_codec::INFO, " Finish Generate Polynomial.");
        }

        inline Polynomial::Polynomial(const Polynomial& polynomial)
            : field_(const_cast<Field&>(polynomial.field_)),
            poly_(polynomial.poly_)
        {}

        inline Polynomial::Polynomial(const Field_Element& element)
            : field_(const_cast<Field&>(element.galois_field()))
        {
            wd_codec::Logger::log(wd_codec::INFO, " Start Generate Polynomial.");
            poly_.resize(1, element);
            wd_codec::Logger::log(wd_codec::INFO, " Finish Generate Polynomial.");
        }

        Field_Element& Polynomial::operator[] (const std::size_t& term) {
            assert(term < poly_.size());
            return poly_[term];
        }
        const Field_Element& Polynomial::operator[](const std::size_t& term) const {
            assert(term < poly_.size());
            return poly_[term];
        }
        //function that return the placeing of value as X in the polynomial

        inline const Field_Element Polynomial::operator () (const Field_Element& value) const
        {
            //Note :  not like schifras imlementation
            return (*this)(value.poly());
        }
        //function that return the placeing of value as X in the polynomial
        inline Field_Element Polynomial::operator() (field_symbol    value)
        {
            if (!poly_.empty())
            {
                int i = 0;
                field_symbol total_sum = 0;
                //loop that go throught the poly_ vector
                for (const_poly_iter it = poly_.begin(); it != poly_.end(); ++it, ++i)
                {
                    //total_sum =+ (value ^ i)* coeffitient[i]
                    total_sum ^= field_.mul(field_.exp(value, i), (*it).poly());
                }
                //total_sum = poly_[0] + poly_[1]*value + poly_[2]*value^2 + ... + poly_[i]*value^i
                return Field_Element(field_, total_sum);
            }
            return Field_Element(field_, 0);
        }
        //function that return the placeing of value as X in the polynomial
        inline Field_Element Polynomial::operator()(const Field_Element& value)  {
            //Note :  not like schifras imlementation
            return (*this)(value.poly());
        }
        //function that return the placeing of value as X in the polynomial
        inline const Field_Element  Polynomial::operator()(field_symbol         value) const {
            if (!poly_.empty())
            {
                int i = 0;
                field_symbol total_sum = 0;
                //loop that go throught the poly_ vector
                for (const_poly_iter it = poly_.begin(); it != poly_.end(); ++it, ++i)
                {
                    //total_sum =+ (value ^ i)* coeffitient[i]
                    total_sum ^= field_.mul(field_.exp(value, i), (*it).poly());
                }
                //total_sum = poly_[0] + poly_[1]*value + poly_[2]*value^2 + ... + poly_[i]*value^i
                return Field_Element(field_, total_sum);
            }
            return Field_Element(field_, 0);
        }

        inline Polynomial& Polynomial::operator  =  (const Polynomial& polynomial) {
            if ((this != &polynomial) && (&field_ == &(polynomial.field_)))
            {
                poly_ = polynomial.poly_;
            }
            return (*this);
        }
        inline Polynomial& Polynomial::operator  =  (const Field_Element& element) {
            if (&field_ == &(element.galois_field()))
            {
                poly_.resize(1, element);
            }
            return *this;
        }
        inline Polynomial& Polynomial::operator +=  (const Polynomial& element) {
            const_poly_iter it1 = element.poly_.begin();
            for (poly_iter it0 = poly_.begin(); it0 != poly_.end() && it1 != element.poly_.end(); ++it0, ++it1) {
                 (*it0) += (*it1);
            }
            while (it1 != element.poly_.end()) {
                 poly_.push_back(*it1);
                 ++it1;
            }
            simplify(*this);

            return (*this);
        }

        inline Polynomial& Polynomial::operator +=  (const Field_Element& element) {
            poly_[0] += element;
            return *this;
        }
        inline Polynomial& Polynomial ::operator -=  (const Polynomial& element) {
            return (*this += element);
        }
        inline Polynomial& Polynomial::operator -=  (const Field_Element& element) {
            poly_[0] -= element;
            return *this;
        }
        //function that check if two polynomials are equal
        inline bool Polynomial::operator==(const Polynomial& polynomial) const {
            if (field_ == polynomial.field_)
            {
                if (poly_.size() != polynomial.poly_.size())
                    return false;
                const_poly_iter it0 = poly_.begin();
                const_poly_iter it1 = polynomial.poly_.begin();
                while (it0 != poly_.end() && (*it0) == (*it1)) {
                    it0++;
                    it1++;
                }
                return (it0 == poly_.end());
            }
            else
                return false;
        }
        inline bool Polynomial::operator!=(const Polynomial& polynomial) const {
            return !((*this) == polynomial);
        }

        inline Polynomial& Polynomial::operator ^= (const unsigned int& n)
        {
            Polynomial result = *this;

            for (std::size_t i = 0; i < n; ++i)
            {
                result *= *this;
            }

            *this = result;

            return *this;
        }

        inline Polynomial& Polynomial::operator %= (const unsigned int& power)
        {
            if (poly_.size() >= power)
            {
                poly_.resize(power, Field_Element(field_, 0));
                simplify(*this);

            }

            return *this;
        }


        inline Polynomial gcd(const Polynomial& a, const Polynomial& b)
        {//calculates the greatest common divisor of two polynomials ,  ensuring both polynomials same Galois Field.
            if (&a.galois_field() == &b.galois_field())
            {
                if ((!a.valid()) && (!b.valid()))//not same 
                {
                    Polynomial error_polynomial(a.galois_field());
                    return error_polynomial;
                }

                if (!a.valid()) return b;
                if (!b.valid()) return a;

                Polynomial x = a % b;
                Polynomial y = b;
                Polynomial z = x;

                while ((z = (y % x)).valid())
                {
                    y = x;
                    x = z;
                }
                return x;
            }
            else
            {
                Polynomial error_polynomial(a.galois_field());
                return error_polynomial;
            }
        }



        inline Polynomial generate_X(const Field& gfield) {//create base polynomail
            const Field_Element xgfe[2] = {
                                        Field_Element(gfield, 0),
                                        Field_Element(gfield, 1)
            };
            Polynomial X_(gfield, 1, xgfe);//todo fix this creation
            return X_;
        }
        inline Polynomial operator + (const Polynomial& a, const Polynomial& b) {
            Polynomial result = a;
            result += b;
            return result;
        }
        inline Polynomial operator + (const Polynomial& a, const Field_Element& b) {
            Polynomial result = a;
            result += b;
            return result;
        }
        inline Polynomial operator + (const Field_Element& a, const Polynomial& b) {
            Polynomial result = b;
            result += a;
            return result;
        }
        inline Polynomial operator + (const Polynomial& a, const field_symbol& b) {
            return a + Field_Element(a.galois_field(), b);
        }
        inline Polynomial operator + (const field_symbol& a, const Polynomial& b) {
            return b + Field_Element(b.galois_field(), a);
        }
        inline Polynomial operator - (const Polynomial& a, const Polynomial& b) {
            return a - b;
        }
        inline Polynomial operator - (const Polynomial& a, const Field_Element& b) {
            Polynomial result = a;
            result -= b;
            return result;
        }
        inline Polynomial operator - (const Field_Element& a, const Polynomial& b) {
            Polynomial result = b;
            result -= a;
            return result;
        }
        inline Polynomial operator - (const Polynomial& a, const field_symbol& b) {
            return a - Field_Element(a.galois_field(), b);
        }
        inline Polynomial operator - (const field_symbol& a, const Polynomial& b) {
            return b - Field_Element(b.galois_field(), a);
        }
        inline Polynomial operator * (const Polynomial& a, const Polynomial& b) {
            Polynomial result = a;
            result *= b;
            return result;
        }
        inline Polynomial operator * (const Field_Element& a, const Polynomial& b) {
            Polynomial result = a;
            result *= b;
            return result;
        }
        inline Polynomial operator * (const Polynomial& a, const Field_Element& b) {
            Polynomial result = a;
            result *= b;
            return result;
        }
        inline Polynomial operator / (const Polynomial& a, const Polynomial& b) {
            Polynomial result = a;
            result /= b;
            return result;
        }
        inline Polynomial operator / (const Polynomial& a, const Field_Element& b) {
            Polynomial result = a;
            result /= b;
            return result;
        }
       inline  Polynomial operator % (const Polynomial& a, const Polynomial& b) {
           return a % b;
        }
       inline Polynomial operator % (const Polynomial& a, const unsigned int& power) {
           Polynomial result = a;
           result %= power;
           return result;

        }
       inline Polynomial operator ^ (const Polynomial& a, const int& n) {
           Polynomial result = a;
           result ^= n;
           return result;
        }
       inline Polynomial operator <<(const Polynomial& a, const unsigned int& n) {
           Polynomial result = a;
           result <<= n;
           return result;
        }
       inline Polynomial operator >>(const Polynomial& a, const unsigned int& n) {
           Polynomial result = a;
           result >>= n;
           return result;
        }
    }
}