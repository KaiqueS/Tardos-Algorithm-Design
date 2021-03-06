#include <vector>
#include "Classes/agent.h"
#include "Classes/options.h"
#include "Classes/pairsofopts.h"
#include "Classes/socialprefnode.h"
#include "Classes/pairwiserank.h"
#include "helper_functions.cpp"
#include "Classes/cycle.h"
#include "aggregation_rules.cpp"

// Possible optimizations: binary search in rank_generation. Harder, better, faster, stronger.

// Generates, without repetition, all combinations of pairs of alternatives - magic number removed
template<typename Prefs> std::vector<PairsOfOpts<Prefs>> pair_generation( std::vector<Agent<Prefs>>& listofagents ){

	PairsOfOpts<Prefs> compairs;

	std::vector<PairsOfOpts<Prefs>> paircomp{ };

	int randagt = rand( ) % listofagents.size( );

	// gets all possible combination of pairs, including repeated pairs
	for( std::vector<int>::size_type i = 0; i < listofagents[ randagt ].get_preferences( ).size( ); ++i ){

		compairs.xpref = listofagents[ randagt ].get_preferences( )[ i ];

		for( std::vector<int>::size_type j = i + 1; j < listofagents[ randagt ].get_preferences( ).size( ); ++j ){

			compairs.ypref = listofagents[ randagt ].get_preferences( )[ j ];

			paircomp.push_back( compairs );
		}
	}

	// returns all combinations without repetitions
	return paircomp;
}

// Ranks alternatives. The ranking has a form of a vector of quintuples ( x, y, xval, yval, ival ), where
// x and y are the alternatives, and the vals represent how many agents prefer one over the other. ival
// represents indifference. The ranking operates in accord to how agents ranks pairs of alternatives
template<typename Prefs> std::vector<PairWiseRank<Prefs>> rank_generation( std::vector<Agent<Prefs>>& listofagents ){

	// Holder for a pair of options
	PairsOfOpts<Prefs> compairs{ };

	std::vector<PairsOfOpts<Prefs>> ordering = pair_generation( listofagents );

	// quintuples ( x, y, xval, yval, ival ) and then map occurrences into val
	PairWiseRank<Prefs> paircomp{ };

	std::vector<PairWiseRank<Prefs>> ranking{ };

	// Checks how a pair ( x, y ) is ranked for each agent
	for( std::vector<int>::size_type i = 0; i < ordering.size( ); ++i ){

		// Holds both alternatives that will be checked
		compairs.xpref = ordering[ i ].xpref;
		compairs.ypref = ordering[ i ].ypref;

		for( std::vector<int>::size_type j = 0; j < listofagents.size( ); ++j ){

			for( std::vector<int>::size_type k = 0; k < listofagents[ j ].get_preferences( ).size( ); ++ k ){

				// Search for x in agent's preferences
				if( listofagents[ j ].get_preferences( )[ k ].get_alternatives( ) == compairs.xpref.get_alternatives( ) )

					paircomp.set_optx( listofagents[ j ].get_preferences( )[ k ] );

				// Search for y in agent's preferences
				if( listofagents[ j ].get_preferences( )[ k ].get_alternatives( ) == compairs.ypref.get_alternatives( ) )

					paircomp.set_opty( listofagents[ j ].get_preferences( )[ k ] );
			}

			// Compares x and y values. Increments xval, yval, or ival accordingly
			if( paircomp.get_optx( ).get_value( ) > paircomp.get_opty( ).get_value( ) )

				paircomp.incrementx( );

			else if( paircomp.get_optx( ).get_value( ) < paircomp.get_opty( ).get_value( ) )

				paircomp.incrementy( );

			else if( paircomp.get_optx( ).get_value( ) == paircomp.get_opty( ).get_value( ) )

				paircomp.incrementi( );
		}

		// Stores the ranked tuple
		ranking.push_back( paircomp );

		// Resets the tuple for a new ranking
		paircomp = { };
	}

	// returns a vector of quintuples
	return ranking;
}

// Creates a graph GRAPH composed by nodes of alternatives. Relates those nodes according to how the alt-
// ernatives are related to each other, i.e., for three alternatives x, y, and z, if x > y, then, one has
// that y is in x.preferred, and x is in y.worsethan. If x == z, then x is in z.indifference and z is in
template<typename Prefs> std::vector<SocialPrefNode<Prefs>> make_graph( std::vector<Agent<Prefs>>& listofagents, std::vector<PairWiseRank<Prefs>>& rank, std::vector<SocialPrefNode<Prefs>>& graph ){

	int randagt = rand( ) % listofagents.size( );
	//int randindx = rand( ) % listofagents[ randagt ].get_preferences( ).size( );

	// Initializes nodes' ids. Take as argument the ids from an agent
	for( std::vector<int>::size_type i = 0; i < graph.size( ); ++i ){

		graph[ i ].set_id( listofagents[ randagt ].get_preferences( )[ i ].get_alternatives( ) );
	}

	// Checks how alternatives are related. Links them accordingly to their relation
	for( std::vector<int>::size_type i = 0; i < rank.size( ); ++i ){

		for( std::vector<int>::size_type j = 0; j < graph.size( ); ++j ){

			// if x > y
			if( rank[ i ].get_xval( ) > rank[ i ].get_yval( ) ){

				// If graph[ j ] == x, set preferredto = y, i.e., x is preferred to y
				if( graph[ j ].get_id( ) == rank[ i ].get_optx( ).get_alternatives( ) ){

					for( std::vector<int>::size_type k = 0; k < graph.size( ); ++k ){

						if( graph[ k ].get_id( ) == rank[ i ].get_opty( ).get_alternatives( ) )

							graph[ j ].set_pref( graph[ k ] );
					}
				}

				// Else if graph[ j ] == y, set worse = x, i.e., y is worse than x
				else if( graph[ j ].get_id( ) == rank[ i ].get_opty( ).get_alternatives( ) ){

					for( std::vector<int>::size_type k = 0; k < graph.size( ); ++k ){

						if( graph[ k ].get_id( ) == rank[ i ].get_optx( ).get_alternatives( ) )

							graph[ j ].set_worse( graph[ k ] );
					}
				}
			}

			// if y > x
			else if( rank[ i ].get_xval( ) < rank[ i ].get_yval( ) ){

				// If graph[ j ] == x, set worsethan = y
				if( graph[ j ].get_id( ) == rank[ i ].get_optx( ).get_alternatives( ) ){

					for( std::vector<int>::size_type k = 0; k < graph.size( ); ++k ){

						if( graph[ k ].get_id( ) == rank[ i ].get_opty( ).get_alternatives( ) )

							graph[ j ].set_worse( graph[ k ] );
					}
				}

				// Else if scgraph[ j ] == y, set preferences = y
				else if( graph[ j ].get_id( ) == rank[ i ].get_opty( ).get_alternatives( ) ){

					for( std::vector<int>::size_type k = 0; k < graph.size( ); ++k ){

						if( graph[ k ].get_id( ) == rank[ i ].get_optx( ).get_alternatives( ) )

							graph[ j ].set_pref( graph[ k ] );
					}
				}
			}

			// if x == y
			else if( rank[ i ].get_xval( ) == rank[ i ].get_yval( ) ){

				// If scgraph[ j ] == x, set indiff = y
				if( graph[ j ].get_id( ) == rank[ i ].get_optx( ).get_alternatives( ) ){

					for( std::vector<int>::size_type k = 0; k < graph.size( ); ++k ){

						if( graph[ k ].get_id( ) == rank[ i ].get_opty( ).get_alternatives( ) ){

							graph[ k ].set_indiff( graph[ j ] );
						}
					}
				}

				// Else if scgraph[ j ] == y, set indiff = x
				else if( graph[ j ].get_id( ) == rank[ i ].get_opty( ).get_alternatives( ) ){

					for( std::vector<int>::size_type k = 0; k < graph.size( ); ++k ){

						if( graph[ k ].get_id( ) == rank[ i ].get_optx( ).get_alternatives( ) ){

							graph[ k ].set_indiff( graph[ j ] );
						}
					}
				}
			}
		}
	}

	// Returns the graph
	return graph;
}

// Hamiltonian Paths for each node in GRAPH
template<typename Prefs> Cycle<Prefs> make_paths( std::vector<SocialPrefNode<Prefs>> graph ){ // returns a vector of cycles

	Cycle<Prefs> pathway{ };

	while( graph.size( ) != 0 ){

		auto comparison = [ ]( SocialPrefNode<Prefs>& left, SocialPrefNode<Prefs>& right ){

			return left.get_preferences( ).size( ) < right.get_preferences( ).size( );
		};

		auto max = std::max_element( graph.begin( ), graph.end( ), comparison );

		pathway.set_path( max -> get_id( ) );

		// Remove max from graph to get a new, and different, max value
		for( std::vector<int>::size_type j = 0; j < graph.size( ); ++j ){

			if( max -> get_id( ) == graph[ j ].get_id( ) ){

				graph.erase( graph.begin( ) + j );
			}
		}
	}

	// Gets all paths
	return pathway;
}

template<typename Prefs> void condorcet_paradox( std::vector<Agent<Prefs>>& listofagents, std::vector<PairWiseRank<Prefs>>& rank, std::vector<SocialPrefNode<Prefs>>& graph ){

	// Prints social ranking of alternatives - Used for debugging only
	for( std::vector<int>::size_type i = 0; i < rank.size( ); ++i )

		std::cout << rank[ i ] << "\n";

	std::cout << "\n";

	// Creates a graph. This graph will be checked for cycles
	make_graph( listofagents, rank, graph );

	print_graph( graph );

	Cycle<Prefs> hamiltonian_path = make_paths( graph );

	Options<Prefs> winner = majority_rule( graph );

	// Check for cycles
	// std::vector<Cycle<Prefs>> paths = check_cycle( graph );
		// If there are no cycles, Use outdegree as a mean to determine if a node is the Condorcet winner
		// Else, use another method

	std::cout << "The Hamiltonian path is: ";

	for( std::vector<int>::size_type i = 0; i < hamiltonian_path.get_path( ).size( ); ++i )

		std::cout << hamiltonian_path.get_path( )[ i ] << " ";

	std::cout << "\n" << winner.get_alternatives( ) << " is the winner.\n";
}

template<typename Prefs> void arrow_impossibility( std::vector<Agent<Prefs>>& listofagents, std::vector<PairWiseRank<Prefs>>& rank, std::vector<SocialPrefNode<Prefs>>& graph ){

	// Checks if all the below conditions are satisfied simultaneously
		// Universal Domain
		// Pareto Principle
		// Independence of Irrelevant Alternatives
		// Non-dictatorship
}
