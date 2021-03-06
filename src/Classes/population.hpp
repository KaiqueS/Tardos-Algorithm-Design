#ifndef POPULATION_HPP
#define POPULATION_HPP

#include "agent.hpp"

class Population{

public:

    // Constructors & Destructor
    Population( );
	Population( std::vector<int>::size_type size );
    Population( std::vector<Agent> people );
    Population( Agent person );
    Population( const Population& copy );
    ~Population( );

    // Setters
    void set_population( std::vector<Agent>& people );
    void set_population( Agent& people );

    // Getters
    std::vector<Agent>& get_population( ){ return  population; }

    // Operators
    Population& operator=( const Population& copy );

    Agent& operator[ ]( const std::vector<int>::size_type index ){ return population[ index ]; }

    // Helpers
    bool empty( );

    std::vector<int>::size_type size( ){ return population.size( ); }

    std::vector<Agent, std::allocator<Agent>>::iterator begin( ){ return population.begin( ); }
    std::vector<Agent, std::allocator<Agent>>::iterator end( ){ return population.end( ); }

	void initialize_population( Preferencematrix mtx );

    void order_preferences( );

    void push_back( Agent& agt ){ population.push_back( agt ); }
    void pop_back( ){ population.pop_back( ); }

	void clear( ){ population.clear( ); }

	void swap( ){ std::vector<Agent>( ).swap( population ); }

	Agent& at( std::vector<int>::size_type index ){ return population.at( index ); }

private:

    std::vector<Agent> population{ };
};

std::ostream& operator<<( std::ostream& os, Population& people );

#endif // POPULATION_HPP
