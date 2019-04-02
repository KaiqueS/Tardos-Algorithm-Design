#include "agent.hpp"

/// Constructors

// Default Constructor
Agent::Agent( ){

    preferences = { };

    id = { };
}

// Copy constructor
Agent::Agent( const Agent& copy ){

    preferences = copy.preferences;

    id = copy.id;
}

Agent::~Agent( ){ clear( ); }

/// Setters

// I do not like this. It would be better if I implemented a set of options, and let agents and matrixes
// take the alternatives, which would be the same, from there.
// Sets agent's preferences. Takes a random row from PrefMatrix and set it to be the agent's preferences.
// Deletes the used row at the end: Avoids repeated preferences for different agents
void Agent::set_preferences( Preferencematrix& prefmatrix ){

    std::random_device rd;

    std::mt19937 mt( rd( ) );

    std::uniform_int_distribution<std::vector<int>::size_type> index( 0, ( prefmatrix.size( ) - 1 ) );

    // std::vector<int>::size_type randindex = static_cast<std::vector<int>::size_type>( rand( ) ) % prefmatrix.size( );
    //preferences = prefmatrix[ randindex ];
    //prefmatrix.erase_row( randindex );

    preferences = prefmatrix[ index( mt ) ];

    prefmatrix.erase_row( index( mt ) );
}

void Agent::set_id( std::string tag ){ id = tag; }

/// Getters

/// Operators

// Overloaded assignment operator.
Agent& Agent::operator=( Agent one ){

	std::swap( *this, one );

    return *this;
}

/// Helpers

void Agent::clear( ){

	preferences.clear( );
}

// Sorts an agent's preferences in decreasing order, according to an option's value
void Agent::sort_preferences( ){

	preferences.value_merge_sort( 0, preferences.size( ) - 1 );
}

std::ostream& operator<<( std::ostream& os, Agent& agt ){

    os << agt.get_id( ) << " preferences: ";

    for( Options opt : agt.get_preferences( ) )

        os << "( " << opt << " )";

    return os;
}
