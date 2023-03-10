#ifndef HELPERS_HPP_
#define HELPERS_HPP_

#define WITH_PROBABILITY_GENERATOR

#include <functional>
#include <random>


extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

using ProbabilityGenerator = std::function<double ()>;
extern ProbabilityGenerator probability_generator;

#endif /* HELPERS_HPP_ */