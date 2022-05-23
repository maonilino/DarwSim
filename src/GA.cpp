#include "GA.h"

// template <typename Encoding>
// GA<Encoding>::GA(const Encoding&& phenotype)
//     : individual(std::move(phenotype))
// {
// }

// template <typename Encoding> GA<Encoding>::GA(uint16_t popSize)
//     : population_size(popSize)
// {
// }

template <typename Encoding> float GA<Encoding>::calculateFitness() const noexcept
{
    /**
     * @todo Provide default implementation for binary bit string encoding
     *
     */
    return 0.2f;
}

template class GA<std::bitset<32>>;