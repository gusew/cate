/**
 * @brief A collection of classes and factory functions for random functionality
 * @file random.hpp
 */
#pragma once
#include <vector>
#include <random>
#include <stdexcept>
#include <type_traits>
#include <mutex>
#include <libunittest/tuplemap.hpp>
/**
 * @brief Unit testing in C++
 */
namespace unittest {
/**
 * @brief A random object
 */
template<typename T>
class random_object {
public:
    /**
     * @brief Destructor
     */
    virtual
    ~random_object()
    {}
    /**
     * @brief Returns a new random object
     * @returns A random object
     */
    T
    get()
    {
        static std::mutex get_mutex_;
        std::lock_guard<std::mutex> lock(get_mutex_);
        return this->do_get();
    }
    /**
     * @brief Sets a new random seed
     * @param seed The random seed
     */
    void
    seed(int seed)
    {
        static std::mutex seed_mutex_;
        std::lock_guard<std::mutex> lock(seed_mutex_);
        this->do_seed(seed);
    }

protected:
    /**
     * @brief Constructor initializing the Mersenne-Twister generator
     *  with a default random seed of one
     */
    random_object()
        : generator_(1)
    {}
    /**
     * @brief Returns a reference to the random generator
     * @returns A reference to the random generator
     */
    std::mt19937&
    gen()
    {
        return generator_;
    }

private:

    virtual T
    do_get() = 0;

    virtual void
    do_seed(int seed)
    {
        this->gen().seed(seed);
    }

    std::mt19937 generator_;

};

/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief Container of the distribution type used in random_value
 */
template<typename T,
         bool is_integral>
struct distribution;
/**
 * @brief The distribution type container for integral types
 */
template<typename T>
struct distribution<T, true> {
    /**
     * @brief The distribution type for integral types
     */
    typedef typename std::uniform_int_distribution<T> type;
};
/**
 * @brief The distribution type container for non-integral types
 */
template<typename T>
struct distribution<T, false> {
  /**
   * @brief The distribution type for non-integral types
   */
  typedef typename std::uniform_real_distribution<T> type;
};

} // internals

/**
 * @brief A random value. The lower bounds are including. The upper bounds are
 *  including for integral types and excluding for real types
 */
template<typename T>
class random_value : public unittest::random_object<T> {
    /**
     * @brief The distribution type
     */
    typedef typename internals::distribution<T, std::is_integral<T>::value>::type dist_type;

public:
    /**
     * @brief Constructor, range: [0, 1]
     */
    random_value()
        : unittest::random_object<T>()
    {
        typename dist_type::param_type params(0, 1);
        distribution_.param(params);
    }
    /**
     * @brief Constructor, range: [0, maximum]
     * @param maximum The upper bound
     */
    random_value(const T& maximum)
        : unittest::random_object<T>()
    {
        if (!(maximum>0))
            throw std::invalid_argument("maximum must be greater than zero");
        typename dist_type::param_type params(0, maximum);
        distribution_.param(params);
    }
    /**
     * @brief Constructor, range: [minimum, maximum]
     * @param minimum The lower bound
     * @param maximum The upper bound
     */
    random_value(const T& minimum,
                 const T& maximum)
        : unittest::random_object<T>()
    {
        if (!(minimum<maximum))
            throw std::invalid_argument("minimum must be lesser than maximum");
        typename dist_type::param_type params(minimum, maximum);
        distribution_.param(params);
    }

private:

    T
    do_get()
    {
        return distribution_(this->gen());
    }

    dist_type distribution_;

};
/**
 * @brief Factory function for random_value, range: [0, 1]
 * @returns An instance of random_value
 */
template<typename T>
unittest::random_value<T>
make_random_value()
{
    return unittest::random_value<T>();
}
/**
 * @brief Factory function for random_value, range: [0, maximum]
 * @param maximum The upper bound
 * @returns An instance of random_value
 */
template<typename T>
unittest::random_value<T>
make_random_value(const T& maximum)
{
    return unittest::random_value<T>(maximum);
}
/**
 * @brief Factory function for random_value, range: [minimum, maximum]
 * @param minimum The lower bound
 * @param maximum The upper bound
 * @returns An instance of random_value
 */
template<typename T>
unittest::random_value<T>
make_random_value(const T& minimum,
                  const T& maximum)
{
    return unittest::random_value<T>(minimum, maximum);
}
/**
 * @brief A random bool (true, false)
 */
template<>
class random_value<bool> : public unittest::random_object<bool> {
public:
    /**
     * @brief Constructor
     */
    random_value();

private:

    bool
    do_get();

    std::uniform_int_distribution<int> distribution_;

};
/**
 * @brief Factory function for random_value<bool> (true, false)
 * @returns An instance of random_value<bool>
 */
unittest::random_value<bool>
make_random_bool();
/**
 * @brief A random choice from a given container
 */
template<typename Container>
class random_choice : public unittest::random_object<typename Container::value_type> {
    /**
     * @brief The type of the container elements
     */
    typedef typename Container::value_type element_type;
    /**
     * @brief The distribution type
     */
    typedef std::uniform_int_distribution<size_t> dist_type;

public:
    /**
     * @brief Constructor
     * @param container The container to choose from
     */
    random_choice(const Container& container)
        : unittest::random_object<element_type>(),
          container_(container)
    {
        if (container_.size()==0)
            throw std::invalid_argument("container is empty");
        if (container_.size()>=2) {
            typename dist_type::param_type params(0, container_.size() - 1);
            distribution_.param(params);
        }
    }

private:

    element_type
    do_get()
    {
        element_type result(*std::begin(container_));
        if (container_.size()==1)
            return result;
        const size_t index = distribution_(this->gen());
        size_t count = 0;
        for (auto& value : container_) {
            if (count==index) {
                result = value;
                break;
            }
            ++count;
        }
        return result;
    }

    Container container_;
    dist_type distribution_;

};
/**
 * @brief Factory function for random_choice
 * @param container A container
 * @returns An instance of random_choice
 */
template<typename Container>
unittest::random_choice<Container>
make_random_choice(const Container& container)
{
    return unittest::random_choice<Container>(container);
}
/**
 * @brief A random container
 */
template<typename Container>
class random_container : public unittest::random_object<Container> {
    /**
     * @brief The type of the container elements
     */
    typedef typename Container::value_type element_type;
    /**
     * @brief The distribution type
     */
    typedef std::uniform_int_distribution<size_t> dist_type;

public:
    /**
     * @brief Constructor
     * @param rand The random object used to fill the container
     * @param size The container size
     */
    random_container(unittest::random_object<element_type>& rand,
                     size_t size)
        : unittest::random_object<Container>(),
          rand_(&rand),
          fixed_size_(true),
          size_(size)
    {}
    /**
     * @brief Constructor
     * @param rand The random object used to fill the container
     * @param min_size The minimum container size (including)
     * @param max_size The maximum container size (including)
     */
    random_container(unittest::random_object<element_type>& rand,
                     size_t min_size,
                     size_t max_size)
        : unittest::random_object<Container>(),
          rand_(&rand),
          fixed_size_(false)
    {
        if (!(min_size<max_size))
            throw std::invalid_argument("min_size must be lesser than max_size");
        typename dist_type::param_type params(min_size, max_size);
        distribution_.param(params);
    }

private:

    Container
    do_get()
    {
        size_t size(size_);
        if (!fixed_size_)
            size = distribution_(this->gen());
        std::vector<element_type> result(size);
        for (size_t i=0; i<size; ++i)
            result[i] = rand_->get();
        Container container(result.begin(), result.end());
        return std::move(container);
    }

    void
    do_seed(int seed)
    {
        rand_->seed(seed);
        this->gen().seed(seed);
    }

    unittest::random_object<element_type>* rand_;
    bool fixed_size_;
    size_t size_;
    dist_type distribution_;

};
/**
 * @brief Factory function for random_container
 * @param rand The random object used to fill the container
 * @param size The container size
 * @returns An instance of random_container
 */
template<typename Container>
unittest::random_container<Container>
make_random_container(unittest::random_object<typename Container::value_type>& rand,
                      size_t size)
{
    return unittest::random_container<Container>(rand, size);
}
/**
 * @brief Factory function for random_container
 * @param rand The random object used to fill the container
 * @param min_size The minimum container size (including)
 * @param max_size The maximum container size (including)
 * @returns An instance of random_container
 */
template<typename Container>
unittest::random_container<Container>
make_random_container(unittest::random_object<typename Container::value_type>& rand,
                      size_t min_size,
                      size_t max_size)
{
    return unittest::random_container<Container>(rand, min_size, max_size);
}
/**
 * @brief Factory function for random_container<vector>
 * @param rand The random object used to fill the vector
 * @param size The vector size
 * @returns An instance of random_container<vector>
 */
template<typename T>
unittest::random_container<std::vector<T>>
make_random_vector(unittest::random_object<T>& rand,
                   size_t size)
{
    return unittest::random_container<std::vector<T>>(rand, size);
}
/**
 * @brief Factory function for random_container<vector>
 * @param rand The random object used to fill the vector
 * @param min_size The minimum vector size (including)
 * @param max_size The maximum vector size (including)
 * @returns An instance of random_container<vector>
 */
template<typename T>
unittest::random_container<std::vector<T>>
make_random_vector(unittest::random_object<T>& rand,
                   size_t min_size,
                   size_t max_size)
{
    return unittest::random_container<std::vector<T>>(rand, min_size, max_size);
}
/**
 * @brief A random tuple build from random objects
 */
template<typename ...Args>
class random_tuple : public unittest::random_object<std::tuple<Args...>> {
public:
    /**
     * @brief Constructor
     * @param rands Random objects used to fill the tuple
     */
    random_tuple(unittest::random_object<Args>&... rands)
        : unittest::random_object<std::tuple<Args...>>(),
          rand_tuple_(&rands...)
    {}

private:

    std::tuple<Args...>
    do_get()
    {
        std::tuple<Args...> result;
        unittest::internals::tuple_transform(get_func(), rand_tuple_, result);
        return std::move(result);
    }

    void
    do_seed(int seed)
    {
        unittest::internals::tuple_for_each(seed_func(), rand_tuple_, seed);
    }

    /**
     * @brief Helper class to set the random seed
     */
    struct seed_func {
        /**
         * @brief Sets a new random seed
         * @param rand The random object
         * @param seed The random seed
         */
        template<typename T>
        void operator()(unittest::random_object<T>* rand, int seed) const
        {
            rand->seed(seed);
        }
    };
    /**
     * @brief Helper class to retrieve a random object
     */
    struct get_func {
        /**
         * @brief Returns a random object
         * @param rand The random object
         * @returns A random object
         */
        template<typename T>
        T operator()(unittest::random_object<T>* rand) const
        {
            return rand->get();
        }
    };

    std::tuple<unittest::random_object<Args>*...> rand_tuple_;

};
/**
 * @brief Factory function for random_tuple
 * @param rands The random objects used to fill the tuple
 * @returns An instance of random_tuple
 */
template<typename ...Args>
unittest::random_tuple<Args...>
make_random_tuple(unittest::random_object<Args>&... rands)
{
    return unittest::random_tuple<Args...>(rands...);
}
/**
 * @brief A random pair build from two random objects
 */
template<typename F,
         typename S>
class random_pair : public unittest::random_object<std::pair<F,S>> {
public:
    /**
     * @brief Constructor
     * @param rand_fst Random object used to fill the first pair element
     * @param rand_snd Random object used to fill the second pair element
     */
    random_pair(unittest::random_object<F>& rand_fst,
                unittest::random_object<S>& rand_snd)
        : unittest::random_object<std::pair<F,S>>(),
          rand_fst_(&rand_fst),
          rand_snd_(&rand_snd)
    {}

private:

    std::pair<F,S>
    do_get()
    {
        return std::make_pair(rand_fst_->get(), rand_snd_->get());
    }

    void
    do_seed(int seed)
    {
        rand_fst_->seed(seed);
        rand_snd_->seed(seed);
    }

    unittest::random_object<F>* rand_fst_;
    unittest::random_object<S>* rand_snd_;
};
/**
 * @brief Factory function for random_pair
 * @param rand_fst The random object used to fill the first pair element
 * @param rand_snd The random object used to fill the second pair element
 * @returns An instance of random_pair
 */
template<typename F,
         typename S>
unittest::random_pair<F,S>
make_random_pair(unittest::random_object<F>& rand_fst,
                 unittest::random_object<S>& rand_snd)
{
    return unittest::random_pair<F,S>(rand_fst, rand_snd);
}
/**
 * @brief A random shuffle of a given container
 */
template<typename Container>
class random_shuffle : public unittest::random_object<Container> {
public:
    /**
     * @brief Constructor
     * @param container The container
     */
    random_shuffle(const Container& container)
        : unittest::random_object<Container>(),
          vector_(std::begin(container), std::end(container)),
          size_(container.size())
    {}
    /**
     * @brief Constructor
     * @param container The container
     * @param size The size of the shuffled container
     */
    random_shuffle(const Container& container,
                   size_t size)
        : unittest::random_object<Container>(),
          vector_(std::begin(container), std::end(container)),
          size_(size)
    {
        if (size<1 || size>container.size())
            throw std::invalid_argument("size out of range");
    }

private:

    Container
    do_get()
    {
        auto first = vector_.begin();
        shuffle(first, vector_.end(), this->gen());
        Container container(first, first + size_);
        return std::move(container);
    }

    std::vector<typename Container::value_type> vector_;
    size_t size_;

};
/**
 * @brief Factory function for random_shuffle
 * @param container A container
 * @returns An instance of random_shuffle
 */
template<typename Container>
unittest::random_shuffle<Container>
make_random_shuffle(const Container& container)
{
    return unittest::random_shuffle<Container>(container);
}
/**
 * @brief Factory function for random_shuffle
 * @param container A container
 * @param size The size of the shuffled container
 * @returns An instance of random_shuffle
 */
template<typename Container>
unittest::random_shuffle<Container>
make_random_shuffle(const Container& container,
                    size_t size)
{
    return unittest::random_shuffle<Container>(container, size);
}

/**
 * @brief Internal functionality, not relevant for most users
 */
namespace internals {
/**
 * @brief Container for the combination type used in random_combination
 */
template<typename Container1,
         typename Container2>
struct combination {
    /**
     * @brief The type of the random combination
     */
    typedef
    std::vector<
        std::pair<
                  typename Container1::value_type,
                  typename Container2::value_type
                 >
               >
    type;
};

} // internals

/**
 * @brief A random combination of two containers
 */
template<typename Container1,
         typename Container2>
class random_combination : public unittest::random_object<typename unittest::internals::combination<Container1, Container2>::type> {
    /**
     * @brief The type of the random combination
     */
    typedef typename unittest::internals::combination<Container1, Container2>::type combination_type;

public:
    /**
     * @brief Constructor
     * @param container1 A container
     * @param container2 Another container
     * @param size The number of combinations
     */
    random_combination(const Container1& container1,
                       const Container2& container2,
                       size_t size)
        : unittest::random_object<combination_type>(),
          container1_(container1),
          container2_(container2),
          granter_(container1.size() * container2.size(), false),
          size_(size)
    {
        if (size<1 || size>granter_.size())
            throw std::invalid_argument("size out of range");
        for (size_t i=0; i<size_; ++i)
            granter_[i] = true;
    }

private:

    combination_type
    do_get()
    {
        size_t index = 0;
        combination_type combination;
        combination.reserve(size_);
        shuffle(granter_.begin(), granter_.end(), this->gen());
        for (auto& value1 : container1_) {
            for (auto& value2 : container2_) {
                if (granter_[index])
                    combination.push_back(std::make_pair(value1, value2));
                ++index;
            }
        }
        return std::move(combination);
    }

    Container1 container1_;
    Container2 container2_;
    std::vector<bool> granter_;
    size_t size_;

};
/**
 * @brief Factory function for random_combination
 * @param container1 A container
 * @param container2 Another container
 * @param size The number of combinations
 * @returns An instance of random_combination
 */
template<typename Container1,
         typename Container2>
unittest::random_combination<Container1, Container2>
make_random_combination(const Container1& container1,
                        const Container2& container2,
                        size_t size)
{
    return unittest::random_combination<Container1, Container2>(container1, container2, size);
}

} // unittest
