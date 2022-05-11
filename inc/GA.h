#pragma once

#ifdef unix
#include <cstdint>
#include <bitset>
#include <memory>
#include <vector>
#endif

#ifdef _WIN32
#include <SDL.h> // Or whatever ....
#endif

/** @brief This type is the default implementation for the encoding of genes,
 * something like binary string encoding should be implemented here
 */

template <typename Enconding> class GA {
  public:
    GA(GA&) = delete;
    GA& operator=(const GA&) = delete;

    GA() = default;
    virtual ~GA() = default;

    // GA(const Enconding&&);

    /**
     * @brief Calculate the fitness values of every individual. Later used for
     * selecting the individuals
     * @return float The fitnesss value for this phenotype
     */
    virtual float calculateFitness() noexcept = 0;
    virtual void selection() noexcept = 0;
    virtual void crossover() noexcept = 0;

  protected:
    std::vector<Enconding> individuals;

  private:
};