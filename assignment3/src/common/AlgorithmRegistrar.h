#ifndef ALGO_REGISTRAR__
#define ALGO_REGISTRAR__

#include <string>
#include <memory>
#include <vector>
#include <functional>
#include <iostream>

#include "AbstractAlgorithm.h"

using AlgorithmFactory = std::function<std::unique_ptr<AbstractAlgorithm>()>;

class AlgorithmRegistrar {
    // Private constructor to prevent direct instantiation
    AlgorithmRegistrar() = default;

    class AlgorithmFactoryPair {
        std::string name_;
        AlgorithmFactory algorithmFactory_;
    public:
        AlgorithmFactoryPair(const std::string &name, AlgorithmFactory algorithmFactory)
                : name_(name), algorithmFactory_(std::move(algorithmFactory)) {}

        const std::string &name() const { return name_; }

        std::unique_ptr<AbstractAlgorithm> create() const { return algorithmFactory_(); }
    };

    std::vector<AlgorithmFactoryPair> algorithms;

    // Static pointer to the singleton instance
    static AlgorithmRegistrar* instance;

public:
    // Singleton instance access method
    static AlgorithmRegistrar& getAlgorithmRegistrar() {
        if (!instance) {
            instance = new AlgorithmRegistrar();
        }
        return *instance;
    }

    void registerAlgorithm(const std::string &name, AlgorithmFactory algorithmFactory) {
        algorithms.emplace_back(name, std::move(algorithmFactory));
    }

    auto begin() const { return algorithms.begin(); }

    auto end() const { return algorithms.end(); }

    std::size_t count() const {
        return algorithms.size();
    }

    void clear() { algorithms.clear(); }
};

#endif  // ALGO_REGISTRAR__
