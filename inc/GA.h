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
 *
 * @todo provide default encoding (bit strings) for GA
 */

template <typename Enconding> class GA {
  public:
    GA(GA&) = delete;
    GA& operator=(const GA&) = delete;

    GA() = default;
    virtual ~GA() = default;

    // GA(const Enconding&&);

    virtual void selection() noexcept = 0;
    virtual std::vector<Enconding> crossover() noexcept = 0;
    virtual void mutation() noexcept = 0;

  protected:
    // "must have" members
    std::vector<Enconding> individuals;
    uint16_t population_size;
    uint16_t generations;

  private:
};