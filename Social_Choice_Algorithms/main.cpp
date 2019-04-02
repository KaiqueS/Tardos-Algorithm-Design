#include <iostream>
#include <algorithm>
#include "sctheory.hpp"

// TODO: Exceptions for all classes/functions/methods that deal with vectors/containers -> Check
//       for emptiness
// TODO: Rethink PairWiseRank -> Try to figure out how to, for any two PWR, if any of the two options
//       that it holds are equal, then they ARE the SAME, i.e., have the same address. This way one
//       can use majority rule for PWR withouht appealing to a middle-class
// TODO: Revise Non-Dic
// CONSIDER: .cpp files for graph algorithms and rank algorithms
// BIG QUESTION: what should SCF return? Alternatives or profiles? If the latter, are they composed
// by only the alternatives that satisfies the aggregation procedure, or are they composed by every
// alternative, sorted according to wheter they satisfy or not the procedure? - For Arrow, return profiles, for GS, return an alternative
// TODO: allow choosing between a pure graph, i.e., graph not based on any aggregation procedure and
// a graph based on procedures
// TODO: when allowing for used-defined aggregation procedures, use as example a custom threshold
// procedure, i.e., allow for the user to input a threshold that, when a given option surpasses
// the latter, it becomes the winner or one of the winners. Add that to GUI
// TODO: let the user decide between majority or list based procedures - GUI
// TODO: make it possible for one to enable/disable indifference
// TODO: a method to check for transitivity. Use sctRank, if ( a, b ), ( b, c ) and ( c, a ), then transitivity is false
// TODO: make a container of alternatives. Let the agents push back the alternatives and assign a random value to them,
// this will make things more similar to how SCT works
// TODO: define operator+= for each procedure

bool then( bool argument ){

	if( argument == true )

		return true;

	else

		return false;
}

void test1( ){

	Preferencematrix newmtx{ };

	newmtx.set_matrix( 5, 5 );

	Population listofagents( newmtx.size( ) );

	std::cout << newmtx;

	listofagents.initialize_population( newmtx );

	std::cout << "\n";

	std::cout << listofagents[ 3 ] << "\n";

	std::cout << "\n";

	Rank rank{ };

	rank.generate_ranking( listofagents );

	std::cout << rank << "\n";

	//majority_rule( rank );

	std::cout << "\n";

	Graph graph{ };

	graph.initialize_graph( newmtx );

	graph.make_graph( newmtx, rank );

	//graph.make_graph( newmtx, rank.get_rank( ) );

	//std::cout << graph[ 0 ][ SocialPrefNode::worsethan_index{ 0 } ] -> get_id( ) << '\n';

	print_graph( graph );

	//qualified_majority_rule( graph );

	// Problem here: value > number of opts
	//make_social_order( listofagents, rank );

	std::cout << "\n";

	// Allow for the user to select which procedure does he wants to use

	 //std::cin >> procedure
	 //pass procedure as argument to arrow function

	SCT::Arrow_Impossibility arrow( newmtx, rank, listofagents, graph );

	SCT::Qualified_majority_rule maj;

	maj( listofagents );

	SCT::Simple_majority_rule majority;

	SCT::Proportional proport;

	SCT::Borda_count borda;

	Profile count = borda( listofagents );

	//Profile res = proport( listofagents );

	SCT::Borda_count bordinha;

	bordinha( listofagents );

	/*for( std::vector<int>::size_type i = 0; i < res.size( ); ++i ){

		if( res[ i ].get_status( ) == true )

			std::cout << res[ i ] << " ";
	}*/

//	std::cout << "\nProblem below!\n";

//	bool truth = arrow( majority );

//	std::cout << "Problem above!\n";

//	if( truth )

//		std::cout << "All right.\n\n";

//	else

//		std::cout << "Oh, damn!.\n\n";
}

void test2( ){

	Graph graph{ };

	Preferencematrix mtx{ };

	Rank rank{ };

	mtx.set_matrix( 45, 45 );

	Population population( mtx.get_matrix( ).size( ) );

	population.initialize_population( mtx );

	rank.generate_ranking( population );

	graph.initialize_graph( mtx );

	graph.make_graph( mtx, rank );

	SCT::Qualified_majority_rule maj{ };

	SCT::Borda_count count{ };

	SCT::Simple_majority_rule simple{ };

	Profile social = simple( population );

	//sct::Procedure newproc = { sct::Qualified_majority_rule( ) };

	Profile result = maj( rank );

	Graph newgraph{ };

	Profile newres = count( population );

	std::cout << "The majority winner is: " << result[ 0 ] << "\n\n";

	std::cout << "The borda winner is: " << std::max_element( newres.begin( ), newres.end( ) )->get_opt( ) << "\n\n";

	std::cout << "The simple majority winner is: " << std::max_element( social.begin( ), social.end( ) )->get_opt( ) << "\n\n";

	for( std::vector<int>::size_type i = 0; i < result.size( ); ++i ){

		std::cout << result[ i ] << " ";
	}

	std::cout << "\n\n";

	for( std::vector<int>::size_type i = 0; i < newres.size( ); ++i ){

		std::cout << newres[ i ] << " ";
	}

	//std::string res = sct::qualified_majority( graph );

	std::cout << "\n\n";

	//std::cout << res;

	std::cout << "\n\n";
}

void test3( ){

	Preferencematrix mtx{ };

	mtx.set_matrix( 4, 4 );

	Rank rank{ };

	rank.generate_ranking( mtx );

	std::cout << rank;

	Population pop{ };

	// Problem here
	pop.initialize_population( mtx );

	Profile prof{ };

	prof = mtx[ 2 ];

	Options opt = prof[ 1 ];

	find_opt( prof, opt );

	prof.sort_by_opt( );

	Graph graph{ };

	graph.initialize_graph( pop );

	graph.make_graph( mtx, rank );

	SCT::Borda_count bordinha;

	SCT::Simple_majority_rule maj;

	SCT::Qualified_majority_rule newmaj;

	SCT::Arrow_Impossibility arrow( mtx, rank, pop, graph );

	//std::cout << arrow( newmaj ) << "\n";
}

void test4( ){

	Profile testprof = { Options( "d", false, 3 ), Options( "b", false, 1 ), Options( "c", false, 2 ), Options( "a", false, 0 ) };
	Profile testprof2 = { Options( "e", false, 4 ), Options( "d", false, 3 ), Options( "b", false, 1 ), Options( "c", false, 2 ), Options( "a", false, 0 ) };

	//merge_sort( testprof, 0, testprof.size( ) - 1 );
	//merge_sort( testprof2, 0, testprof2.size( ) - 1 );
	testprof.value_merge_sort( 0, testprof.size( ) - 1);
	testprof2.value_merge_sort( 0, testprof.size( ) - 1);

	Profile profile = { Options( "d", false, 3 ), Options( "b", false, 1 ), Options( "c", false, 2 ), Options( "a", false, 0 ) };

	profile.value_merge_sort( 0, profile.size( ) - 1 );

	std::cout << testprof << "\n";

	std::cout << testprof2 << "\n";

	std::cout << profile << "\n";
}

void test5( ){

	Profile profile = { Options( "a", false, 3 ) };

	Agent one = { profile, "one" };

	Agent two = one;

	two.set_id( "two" );

	if( one == two )

		std::cout << "true\n";

	else

		std::cout << "false\n";

	std::cout << one << "\n" << two << "\n";

	Options opt{ "opt", false, 5 };
	Options opt2{ "opt2", false, 3 };

	if( opt > opt2 )

		std::cout << "opt is greater than opt2\n";

	if( opt != opt2 )

		std::cout << "opt is not opt2\n";

	std::cout << opt << "\n";

	opt += opt2;

	std::cout << opt;
}

void test6( ){

	Preferencematrix original = { Profile( { Options( "a", false, 5 ), Options( "b", false, 4 ), Options( "c", false, 3 ), Options( "d", false, 2 ), Options( "e", false, 1 ) } ),
								  Profile( { Options( "b", false, 5 ), Options( "c", false, 4 ), Options( "d", false, 3 ), Options( "e", false, 2 ), Options( "a", false, 1 ) } ),
								  Profile( { Options( "c", false, 5 ), Options( "d", false, 4 ), Options( "e", false, 3 ), Options( "a", false, 2 ), Options( "b", false, 1 ) } ),
								  Profile( { Options( "d", false, 5 ), Options( "e", false, 4 ), Options( "a", false, 3 ), Options( "b", false, 2 ), Options( "c", false, 1 ) } ),
								  Profile( { Options( "e", false, 5 ), Options( "a", false, 4 ), Options( "b", false, 3 ), Options( "c", false, 2 ), Options( "d", false, 1 ) } ) };

	Preferencematrix prime =	{ Profile( { Options( "c", false, 5 ), Options( "d", false, 4 ), Options( "e", false, 3 ), Options( "a", false, 2 ), Options( "b", false, 1 ) } ),
								  Profile( { Options( "e", false, 5 ), Options( "d", false, 4 ), Options( "a", false, 3 ), Options( "b", false, 2 ), Options( "c", false, 1 ) } ),
								  Profile( { Options( "c", false, 5 ), Options( "b", false, 4 ), Options( "a", false, 3 ), Options( "d", false, 2 ), Options( "e", false, 1 ) } ),
								  Profile( { Options( "b", false, 5 ), Options( "d", false, 4 ), Options( "a", false, 3 ), Options( "c", false, 2 ), Options( "e", false, 1 ) } ),
								  Profile( { Options( "e", false, 5 ), Options( "a", false, 4 ), Options( "b", false, 3 ), Options( "c", false, 2 ), Options( "d", false, 1 ) } ) };

	Rank left( original );
	Rank right( prime );

	//left.order_ranking( );
	//right.order_ranking( );

	SCT::Simple_majority_rule maj;
	SCT::Borda_count borda;

	Profile first{ borda( original ) };
	Profile second{ borda( prime ) };

	std::cout << original << "\n\n" << prime << "\n";

	std::cout << left << "\n" << right << "\n";

	first.sort_by_opt( );
	second.sort_by_opt( );

	std::cout << first << "\n" << second << "\n";

	if( first == second )

		std::cout << "\ntrue\n\n";

	else

		std::cout << "\nfalse\n\n";
}

int main( ){

	//srand( static_cast<unsigned int>( time( nullptr ) ) );

	test1( );
}
