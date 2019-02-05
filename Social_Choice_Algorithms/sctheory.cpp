#include "sctheory.hpp"

// Remember: function will be a functor, which, in turn, will return a profile of preferences, so
// one has to guarantee that the parameter function will return something that have the below nec
// essary methods, like, e.g., .size( ).

// Checks if the pareto principle is violated or not - Working(?) - TODO: Check for transitivity problems
// TODO: overload pareto principle so that it only deals with graph or rank, but not both
bool SCT::Pareto_Principle::operator( )( sct::Procedure& procedure ){

    Options pareto{ };

    bool validity{ true };

    Profile winner = procedure( rank );

    std::vector<int>::size_type ranksize = winner.size( );

    // Reads PairWiseRank. Since it considers how each pair is being ranked on every individual
    // profile of preferences WITHOUT being a social order, one can proceed as follows:
    for( std::vector<int>::size_type i = 0; i < ranksize; ++i ){

        // Get an option in rank
        // TODO: implement something to make it possible for optx or opty to be selected randomly
        Options opt( rank[ i ].get_optx( ).get_opt( ), false, 0  );

        // Get every option in rank
        for( PairWiseRank pair : rank ){

            // Check if opt is equal to one of the alternatives in pair
            if( opt.get_opt( ) == pair.get_optx( ).get_opt( ) ){

                // If it is, then check if opt beats its adversary
                if( pair.get_xval( ) > pair.get_yval( ) ){

                    // If it beats, then increment the value of opt
                    ++opt;
                }
            }

            else if( opt.get_opt( ) == pair.get_opty( ).get_opt( ) ){

                if( pair.get_xval( ) < pair.get_yval( ) ){

                    ++opt;
                }
            }
        }

        // If it is the case that opt is UNANIMOUSLY select by the agents
        if( opt.get_value( ) == static_cast<int>( ranksize ) - 1 ){

            // Make pareto = opt
            pareto = opt;
        }

        // PROBLEM HERE: Think about some way of using this else to report relevant message
        else{

            //pareto = Options( "NULL", -1 );

            continue;
        }
    }

    // Now, the second part: If there is a pareto option
    if( pareto.get_opt( ) != "NULL" ){

        // Get the first element of socialorder. Remember, this vector ir ordered from the most to
        // the least preferred alternative
        std::vector<Options, std::allocator<Options>>::iterator index = winner.begin( );

        // If it is the case that pareto equals the first element of socialorder
        if( pareto.get_opt( ) == index -> get_opt( ) ){

            // Then the axiom is not being violated
            validity = true;
        }

        // Else, it is being violated
        else if( pareto.get_opt( ) != index -> get_opt( ) ){

            validity = false;
        }
    }

    // Returns final validity
    return validity;
}


/*bool SCT::pareto_principle( Graph& graph , Rank& rank, Profile& procedure ){

    Options pareto{ };

    bool validity{ true };

    std::vector<int>::size_type ranksize = rank.size( );

    // Reads PairWiseRank. Since it considers how each pair is being ranked on every individual
    // profile of preferences WITHOUT being a social order, one can proceed as follows:
    for( std::vector<int>::size_type i = 0; i < ranksize; ++i ){

        // Get an option in rank
        // TODO: implement something to make it possible for optx or opty to be selected randomly
        Options opt( rank[ i ].get_optx( ).get_opt( ), false, 0  );

        // Get every option in rank
        for( PairWiseRank pair : rank ){

            // Check if opt is equal to one of the alternatives in pair
            if( opt.get_opt( ) == pair.get_optx( ).get_opt( ) ){

                // If it is, then check if opt beats its adversary
                if( pair.get_xval( ) > pair.get_yval( ) ){

                    // If it beats, then increment the value of opt
                    ++opt;
                }
            }

            else if( opt.get_opt( ) == pair.get_opty( ).get_opt( ) ){

                if( pair.get_xval( ) < pair.get_yval( ) ){

                    ++opt;
                }
            }
        }

        // If it is the case that opt is UNANIMOUSLY select by the agents
        if( opt.get_value( ) == static_cast<int>( procedure.size( ) ) - 1 ){

            // Make pareto = opt
            pareto = opt;
        }

        // PROBLEM HERE: Think about some way of using this else to report relevant message
        else{

            //pareto = Options( "NULL", -1 );

            continue;
        }
    }

    // Now, the second part: If there is a pareto option
    if( pareto.get_opt( ) != "NULL" ){

        // Get the first element of socialorder. Remember, this vector ir ordered from the most to
        // the least preferred alternative
        std::vector<Options, std::allocator<Options>>::iterator index = procedure.begin( );

        // If it is the case that pareto equals the first element of socialorder
        if( pareto.get_opt( ) == index -> get_opt( ) ){

            // Then the axiom is not being violated
            validity = true;
        }

        // Else, it is being violated
        else if( pareto.get_opt( ) != index -> get_opt( ) ){

            validity = false;
        }
    }

    // Returns final validity
    return validity;
}
*/

// Checks if , for any x, y, and z, the relation between x and y is modified by
// the relation between x and z or y and z in the social ranking
// TODO: maybe a bool key? The same as validity above
// TODO: remember to pass a PROCEDURE as argument
bool SCT::Irrelevant_Alternatives::operator( )( sct::Procedure& procedure ){

    // THIS IS GAMBIARRAAAAAAAAAAAAAAAAAAAAA
    Population& people = this -> population;

    bool validity{ true };

    // generate a second profile R' of preferences
    Preferencematrix primeMtx( matrix.get_rowsz( ), matrix.get_columnsz( ) );

    Population population( primeMtx.size( ) );

    population.initialize_population( primeMtx );

    // To optmize: search for the way to falsify the axiom -> check for the case where
    // the condition is true, but the consequence is false

    // Sort agents preferences according to value, will make things here easier
    for( std::vector<int>::size_type i = 0; i < people.size( ) && population.size( ); ++i ){

        people[ i ].sort_preferences( );
        population[ i ].sort_preferences( );
    }

    // if it is the case that, for any two alternatives x and y, and for every person i
    // xRiy iff xR'iy
    for( std::vector<int>::size_type i = 0; i < people.size( ); ++i ){

        for( std::vector<int>::size_type j = 0; j < population.size( ); ++j ){

            if( people[ i ].get_preferences( ) != population[ j ].get_preferences( ) ){

                validity = true;

                return validity;
            }

            // A SHITPOINT, BERG, BE CAREFUL
            else

                validity = false;
        }
    }

    // TODO: workaround below

    Population newpop( people );

    Rank rank{ };
    rank.generate_ranking( newpop );

    Rank primerank{ };
    primerank.generate_ranking( newpop );

    // END of workaround

    Graph graph{ };
    graph.make_graph( matrix, rank );

    Graph primegraph{ };
    primegraph.make_graph( primeMtx, rank );

    // then, generate a social order for each profile and check if x and y are ranked equally in both profiles
    // TODO: this should compare the social orders generated by an ARBITRARY aggregation procedure. REMEMBER to
    // allow for user-specified procedure later
    // NOTE: Q_Majority_rule used for debug purposes only
    if( procedure( rank ) == procedure( primerank ) ){

        // If that is the case, the axiom is not being violated, return true
        validity = true;

        return validity;
    }

    // Else, the axiom is being violated, return false
    // TODO: Print something here, to let one know what was wrong
    else{

        validity = false;

        return validity;
    }
}

bool SCT::irrelevant_alternatives( Preferencematrix& mtx, Population& people ){

    bool validity{ true };

    // generate a second profile R' of preferences
    Preferencematrix primeMtx( mtx.get_rowsz( ), mtx.get_columnsz( ) );

    Population population( primeMtx.get_matrix( ).size( ) );

    population.initialize_population( primeMtx );

    // To optmize: search for the way to falsify the axiom -> check for the case where
    // the condition is true, but the consequence is false

    // Sort agents preferences according to value, will make things here easier
    for( std::vector<int>::size_type i = 0; i < people.size( ) && population.size( ); ++i ){

        people[ i ].sort_preferences( );
        population[ i ].sort_preferences( );
    }

    // if it is the case that, for any two alternatives x and y, and for every person i
    // xRiy iff xR'iy
    for( std::vector<int>::size_type i = 0; i < people.size( ); ++i ){

        for( std::vector<int>::size_type j = 0; j < population.size( ); ++j ){

            if( people[ i ].get_preferences( ) != population[ j ].get_preferences( ) ){

                validity = true;

                return validity;
            }

            // A SHITPOINT, BERG, BE CAREFUL
            else

                validity = false;
        }
    }

	// TODO: fiz workaround below

	Population newpop( people );

    Rank rank{ };
	rank.generate_ranking( newpop );

    Rank primerank{ };
	primerank.generate_ranking( newpop );

	// END of workaround

    Graph graph{ };
    graph.make_graph( mtx, rank );

    Graph primegraph{ };
    primegraph.make_graph( primeMtx, rank );

    // then, generate a social order for each profile and check if x and y are ranked equally in both profiles
    // TODO: this should compare the social orders generated by an ARBITRARY aggregation procedure. REMEMBER to
    // allow for user-specified procedure later
    // NOTE: Q_Majority_rule used for debug purposes only
    if( qualified_majority_rule( graph ) == qualified_majority_rule( primegraph ) ){

        // If that is the case, the axiom is not being violated, return true
        validity = true;

        return validity;
    }

    // Else, the axiom is being violated, return false
    // TODO: Print something here, to let one know what was wrong
    else{

        validity = false;

        return validity;
    }
}

// Find an agent that, whenever ONLY HER prefers x to y, the social order becomes xPy
// If is that the case that another person also prefers x to y, then the clause is not
// valid
// TODO: revise this
bool SCT::non_dictatorship( Population& people, Rank& rank, Graph& graph ){

    for( std::vector<int>::size_type i = 0; i < people.size( ); ++i )

        people[ i ].sort_preferences( );

    Profile dictatedopts{ };

    Population dictators{ };

    // TODO: revise this. Maybe there is a case where x == y == 1, since there is repe
    // tition in pairwiserank
    for( std::vector<int>::size_type i = 0; i < rank.size( ); ++i ){

        // if only one person prefers x to y
        if( rank[ i ].get_xval( ) == 1 ){

            dictatedopts.push_back( rank[ i ].get_optx( ) );
        }

        else if( rank[ i ].get_yval( ) == 1 ){

            dictatedopts.push_back( rank[ i ].get_opty( ) );
        }
    }

    for( std::vector<int>::size_type i = 0; i < people.size( ); ++i ){

        for( std::vector<int>::size_type j = 0; j < people[ i ].get_preferences( ).size( ); ++j ){

            for( std::vector<int>::size_type k = 0; k < dictatedopts.size( ); ++k ){

                if( people[ i ][ j ].get_opt( ) == dictatedopts[ k ].get_opt( ) )

                    dictators.push_back( people[ i ] );
            }
        }
    }

    for( std::vector<int>::size_type i = 0; i < dictators.size( ); ++i ){

        for( std::vector<int>::size_type j = 0; j < dictators.size( ); ++j )

            if( j != i && dictators[ i ].get_id( ) == dictators[ j ].get_id( ) )

                dictators.pop_back( );
    }

    if( dictators.size( ) > 1 ){

        //std::cout << "There is more than one dictator. Axiom not violated, return true.\n";

        return true;
    }

    else if( dictators.size( ) == 1 ){

        std::cout << "There is a dictator: " << dictators.begin( ) -> get_id( ) << ". Axiom violated, return false.\n";

        return false;
    }

    else if( dictators.empty( ) ){

        return true;
    }
        // If it is the case that, for one agent i, only i have xPiy
            // Check the social order
                // If it is the case that xPy in the social order
                    // Return false and the agent i is a dictator
                // Else, return true
    else

        return true;
}

// TODO: use this only for the case where there are only three alternatives,
// so -> check rank and graph sizes, then check for condorcerity
bool SCT::condorcet_paradox( Rank& rank, Graph& graph ){

    int selection{ };

    Graph result{ };

    std::cout << "Which data structure do you wish to use? 1- Rank 2- Graph: ";

    std::cin >> selection;

    if( selection != 1 && selection != 2 ){

        std::cerr << "Invalid options.\n";

        return condorcet_paradox( rank, graph );
    }

    // THIS IS SHITTY AF. Use recursion instead, for god's sake.
    else{

        if( selection == 1 ){

            if( ( ( ( rank.size( ) ^ 2 ) - rank.size( ) ) / 2 ) > 3 ){

                std::cerr << "Invalid number of options. Condorcert only applies to cases where the number of options is 3. Use another rank.\n";

                return false;
            }

            else{

                Graph visited{ };

                for( std::vector<int>::size_type i = 0; i < graph.size( ); ++i ){

                    SocialPrefNode actualnode( graph[ i ] );

                    visited.push_back( actualnode );

                    actualnode.set_onstack( true );

                    for( long j = 0; j < static_cast<long>( actualnode.get_preferences( ).size( ) ); ++j ){

                        if( actualnode[ SocialPrefNode::preferences_index{ j } ] -> get_onstack( ) == false ){

                            actualnode[ SocialPrefNode::preferences_index{ j } ] -> set_onstack( true );

                            visited.push_back( *actualnode[ SocialPrefNode::preferences_index{ j } ] );

                            // FULL GAMBIARRA MODE ON
                            actualnode = *actualnode[ SocialPrefNode::preferences_index{ j } ];

                            j = 0;
                        }

                        else

                            continue;
                    }
                }
            }
        }

        else if( selection == 2 ){

            if( static_cast<int>( graph.size( ) ) != 3 ){

                std::cerr << "Invalid number of options. Condorcert only applies to cases where the number of options is 3. Use another rank.\n";

                return false;
            }

            else{

                Graph visited{ };

                for( std::vector<int>::size_type i = 0; i < graph.size( ); ++i ){

                    SocialPrefNode actualnode( graph[ i ] );

                    actualnode.set_onstack( true );

                    visited.push_back( actualnode );

                    for( long j = 0; j <static_cast<long>( actualnode.get_preferences( ).size( ) ); ++j ){

                        if( actualnode[ SocialPrefNode::preferences_index{ j } ] -> get_onstack( ) == false ){

                            actualnode[ SocialPrefNode::preferences_index{ j } ] -> set_onstack( true );

                            visited.push_back( *actualnode[ SocialPrefNode::preferences_index{ j } ] );

                            // FULL GAMBIARRA MODE ON
                            actualnode = *actualnode[ SocialPrefNode::preferences_index{ j } ];

                            --j;
                        }

                        else

                            continue;
                    }
                }

                result = visited;
            }
        }
    }

    for( std::vector<int>::size_type i = 0; i < result.size( ); ++i ){

        for( std::vector<int>::size_type j = 0; j < result.size( ); ++j ){

            if( j != i ){

                if( result[ j ].get_id( ) == result[ i ].get_id( ) ){

                    std::cout << "There is a cycle!\n";

                    return false;
                }
            }
        }
    }

    std::cout << "There are no cycles!\n";

    return true;
}

bool SCT::is_single_peaked( Rank& rank, Graph& graph ){


}

// If not true: analyze the profiles of preferences, both individual and social, then search for some kind of structure,
// i.e., single peakedness degree, individual impact on social profile, etc.
// If true, search for single peakedness or known structures/feats that causes the truthness
bool SCT::arrow_impossibility( Population& pop, Preferencematrix& mtx , Rank& rank, Graph& graph, sct::Procedure& procedure ){

    bool validity{ true };

    sct::Borda_count borda{ };

    Profile newprof = borda( pop );
    // If it is the case that every conditions is satisfied, then, check the structure of the preferences for single-
    // peakedness or anything that might have made it possible for the result to hold

    if( pareto_principle( graph, rank, newprof ) == false ){

        validity = false;

        std::cout << "Pareto principle violated.\n";
    }

    if( irrelevant_alternatives( mtx, pop ) == false ){

        validity = false;

        std::cout << "Irrelevant alternatives violated.\n";
    }

    if( non_dictatorship( pop, rank, graph ) == false ){

        validity = false;

        std::cout << "Nondictatorship violated.\n";
    }

    if( validity == true ){

        std::cout << "Everything all right.\n";

        return validity;
    }

    else{

        std::cout << "Shit!.\n";

        return validity;
    }
}
