#ifndef PROGRAM_LOGIC_H
#define PROGRAM_LOGIC_H
#include <iostream>
#include <vector>
#include "Classes/preferencematrix.h"
#include "Classes/agent.h"
#include "SCT_Algorithms/SCTheory.cpp"
#include "Classes/pairwiserank.h"
#include "Classes/socialprefnode.h"

class Program_Logic
{
public:
	Program_Logic();


	static void run_project(int row, int column){
			//	Run project


		srand( time( NULL ) );

		Preferencematrix<char> newmtx{ };

		newmtx.set_matrix( row, column );

		std::vector<Agent<char>> listofagents( newmtx.get_matrix( ).size( ) );

		newmtx.print_mtx( );

		for( std::vector<int>::size_type i = 0; i < listofagents.size( ); ++i ){

			listofagents[ i ].set_id( std::to_string( i ) );
			listofagents[ i ].set_preferences( newmtx );
		}

		std::cout << "\n\n";

		std::vector<PairWiseRank<char>> rank = rank_generation( listofagents );

		std::vector<SocialPrefNode<char>> graph( listofagents[ 0 ].get_preferences( ).size( ) );

		condorcet_paradox( listofagents, rank, graph );

		//listofagents[ 2 ].get_indifference( );

		std::cout << "\n\n___________________DEBUG_PREFS______________________\n\n";

		for( std::vector<int>::size_type i = 0; i < listofagents.size( ); ++i ){

			std::cout << "Agent " << listofagents[ i ].get_id() << " pref. : ";

			for( std::vector<int>::size_type j = 0; j < listofagents[ i ].get_preferences().size(); ++j ){

				std::cout << "( " << listofagents[ i ].get_preferences()[ j ].get_alternatives() << " , ";
				std::cout << listofagents[ i ].get_preferences()[ j ].get_value() << " ) ";
			}

			std::cout << "\n";
		}

		std::cout << "\n";

		for( std::vector<int>::size_type i = 0; i < listofagents.size( ); ++i ){

			listofagents[i].print_rank( );

			std::cout << "\n";
		}

		std::cout << "____________________________________________________";

		std::cout << "\n\n" << std::flush;

	}
};

#endif // PROGRAM_LOGIC_H
