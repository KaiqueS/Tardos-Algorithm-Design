#pragma once

#ifndef PREFERENCEMATRIX_H
#define PREFERENCEMATRIX_H

#include<iostream>
#include<vector>
#include<random>
#include<algorithm>
#include "options.h"

template<typename PrefCol> class Preferencematrix{

public:

	// Constructors & Destructor
	Preferencematrix( );

	Preferencematrix( std::vector<std::vector<Options<PrefCol>>> copymatrix );

	~Preferencematrix( ){ std::cout << "Matrix destructed.\n\n"; }

	// Setters
	void set_matrix( int rowsz, int colsz );

	// Getters
	std::vector<std::vector<Options<PrefCol>>> get_matrix( ){ return matrix; }

	void print_mtx( );

	// Operators
	std::vector<Options<PrefCol>>& operator[ ]( int position ){ return matrix[ position ]; }

	Preferencematrix<PrefCol>& operator=( Preferencematrix copy );

	//Helpers
	void delete_row( int rowindex );

private:

	std::vector<std::vector<Options<PrefCol>>> matrix{ };
};

/* Constructors */

// Default Constructor. Initializes private members to its default values
template<typename PrefCol> Preferencematrix<PrefCol>::Preferencematrix( ){

	matrix = { };
}

// Parameterized Constructor. Initializes values according to parameters
template<typename PrefCol> Preferencematrix<PrefCol>::Preferencematrix( std::vector<std::vector<Options<PrefCol>>> copymatrix ){

	matrix = copymatrix;
}

/* Setters*/

// Set a matrix of RowSZ x ColSZ dimension. Sets a random value to each alternative
template<typename PrefCol> void Preferencematrix<PrefCol>::set_matrix( int rowsz, int colsz ){

	std::vector<Options<PrefCol>> setofalts( colsz );

	// Sets alternatives' id's
	for( int i = 0; i < setofalts.size( ); ++i ){

		int randaltid = rand( ) % colsz;

		std::string subs = std::to_string( i );

		char const* pchar = subs.c_str( );

		setofalts[ i ].set_alternatives( *pchar );
	}

	// Sets alternatives' values
	for( int i = 0; i < rowsz; ++i ){

		auto randvalind = std::default_random_engine{ };

		std::shuffle( setofalts.begin( ), setofalts.end( ), randvalind );

		matrix.push_back( setofalts );

		for( int j = 0; j < colsz; ++j ){

			int val = rand( ) % colsz;

			matrix[ i ][ j ].set_value( val );
		}
	}

	/* Deprecated
	
	for( int i = 0; i < rowsz; ++i ){

		std::vector<Options<PrefCol>> altern{ };

		matrix.push_back( altern );

		for( int j = 0; j < colsz; ++j ){

			Options<PrefCol> newopt{ };

			int val = rand( ) % colsz;

			newopt.set_value( val );
			
			matrix[ i ].push_back( newopt );
		}
	}*/
}
 
/* Getters */

// Prints matrix
template<typename PrefCol> void Preferencematrix<PrefCol>::print_mtx( ){

	std::cout << "Preference Matrix\n\n" << "\t\t\tOptions/Alternatives Columns\n\n";

	for( int i = 0; i < matrix.size( ); ++i ){

		std::cout << "Row vector id number: " << i << "| ";

		for( int j = 0; j < matrix[ i ].size( ); ++j ){

			std::cout << "( " << matrix[ i ][ j ].get_alternatives( ) << ", " <<
				matrix[ i ][ j ].get_value( ) << " ) ";
		}

		std::cout << "\n";
	}
}

/* Operators */

// Overloaded assignment operator
template<typename PrefCol> Preferencematrix<PrefCol>& Preferencematrix<PrefCol>::operator=( Preferencematrix copy ){

	matrix = copy.get_matrix( );

	return *this;
}

template<typename PrefCol> bool operator==( Preferencematrix<PrefCol>& one, Preferencematrix<PrefCol>& two ){

	if( one.get_matrix( ) == two.get_matrix( ) )

		return true;

	else

		return false;
}
template<typename PrefCol> bool operator!=( Preferencematrix<PrefCol>& one, Preferencematrix<PrefCol>& two ){

	if( one.get_matrix( ) != two.get_matrix( ) )

		return true;

	else

		return false;
}

/* Helpers */

// Delete a specified row. Used in Agent.h class
template<typename PrefCol> void Preferencematrix<PrefCol>::delete_row( int rowindex ){

	matrix.erase( matrix.begin( ) + rowindex );
}

#endif // PREFERENCEMATRIX_H