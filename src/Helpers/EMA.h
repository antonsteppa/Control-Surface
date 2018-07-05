#pragma once

#include <stdint.h>

/**
 * @brief   A class for single-pole infinite impulse response filters
 *          or exponential moving average filters.
 *
 * Straightforward implementation of the difference equation with a slight
 * optimization by using a factor of two as the pole location (this means
 * that no division or floating point operations are required).
 *
 * Difference equation: @f$ y[n] = \alpha·x[n]+(1-\alpha)·y[n-1] @f$
 * where @f$ \alpha = \left(\frac{1}{2}\right)^{K} @f$, @f$ x @f$ is the
 * input sequence, and @f$ y @f$ is the output sequence.
 *
 * [An in-depth explanation of the EMA filter]
 * (https://tttapa.github.io/Pages/Mathematics/Systems-and-Control-Theory/Digital-filters/Exponential%20Moving%20Average/)
 * 
 * Fixed-point integer arithmetic with a precision of @f$ 2K @f$ is used.
 *
 * @tparam  K
 *          The amount of bits to shift by. This determines the location
 *          of the pole in the EMA transfer function, and therefore the
 *          cut-off frequency.
 * @tparam  int_t
 *          The (signed) integer type to use for the input, intermediate values
 *          and the output.
 *          Should be at least @f$ M+1+2K @f$ bits wide, where @f$ M @f$
 *          is the maximum number of bits of the input.
 *          In case of the Arduino's built-in ADC,
 *          @f$ M = 10 = \log_2(1024) @f$.
 *
 */
template <uint8_t K, class int_t>
class EMA {
  public:
    /**
     * @brief   Filter the input: Given @f$ x[n] @f$, calculate @f$ y[n] @f$.
     *
     * @param   input
     *          The new raw input value.
     * @return  The new filtered output value.
     */
    int_t filter(int_t input);

  private:
    int_t filtered = 0;
    constexpr static int_t fixedPointAHalf = 1 << ((K * 2) - 1);
};

// ------------ Implementation ------------ //

template <uint8_t K, class int_t>
int_t EMA<K, int_t>::filter(int_t input) {
    input = input << (K * 2);
    int_t difference = input - filtered;
    filtered = filtered + (difference >> K);
    return (filtered + fixedPointAHalf) >> (K * 2);
}