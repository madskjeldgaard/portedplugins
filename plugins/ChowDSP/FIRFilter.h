#ifndef FIRFILTER_H_INCLUDED
#define FIRFILTER_H_INCLUDED

#include <stdint.h>
#include <numeric>
#include <vector>

/** FIR filter using a double-buffer and std::inner_product */
class FIRFilter
{
public:
    FIRFilter (int filter_order) : order ((size_t) filter_order)
    {
        h.resize (order);
        z.resize (2 * order);
    }

    void reset()
    {
        zPtr = 0;
        std::fill(z.begin(), z.end(), 0.0f);
    }

    void setCoefs (const float* coefs)
    {
        std::copy(coefs, coefs + order, h.begin());
    }

    inline float process (float x)
    {
        float y = 0.0f;

        // insert input into double-buffered state
        z[zPtr] = x;
        z[zPtr + order] = x;

        y = std::inner_product(z.data() + zPtr, z.data() + zPtr + order, h.data(), 0.0f); // comput inner product

        zPtr = (zPtr == 0 ? order - 1 : zPtr - 1); // iterate state pointer in reverse
        return y;
    }

protected:
    std::vector<float> h;
    const size_t order;

private:
    std::vector<float> z;
    size_t zPtr = 0;
};

#endif //FIRFILTER_H_INCLUDED
