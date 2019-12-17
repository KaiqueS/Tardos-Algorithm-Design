#include "preferencematrix.hpp"

/// Constructors

// Default Constructor. Initializes private members to its default values
Preferencematrix::Preferencematrix( ){

    rowsize = { };
    columnsize = { };

    matrix = { };
}

// Parameterize constructor. Sets a matrix of ROW x COL dimensions
Preferencematrix::Preferencematrix( std::vector<int>::size_type row, std::vector<int>::size_type col ){ // Test this laters

    rowsize = row;
    columnsize = col;

	matrix.resize(row);

	for (std::vector<int>::size_type i = 0; i < matrix.size(); ++i) {

		matrix[ i ] = Profile( col );
	}

    //set_matrix( row, col );
}

// Copy Constructor. Initializes values according to parameters
Preferencematrix::Preferencematrix( const Preferencematrix& copymatrix ){

    rowsize = copymatrix.rowsize;
    columnsize = copymatrix.columnsize;

    matrix = copymatrix.matrix;
}

// Move constructor
Preferencematrix::Preferencematrix( Preferencematrix&& copy ){

	rowsize = std::move( copy.rowsize );
	columnsize = std::move( copy.columnsize );

	matrix = std::move( copy.matrix );

	copy.clear( );
}

// Destructor. Clears the vector MATRIX from memory
Preferencematrix::~Preferencematrix( ){ clear( ); }

/// Setters

// Sets ROWSIZE to row - TODO: modify this
void Preferencematrix::set_rowsz( std::vector<int>::size_type row ){ rowsize = row; }

// Sets COLUMNSIZE to col - TODO: modify this
void Preferencematrix::set_columnsz( std::vector<int>::size_type col ){ columnsize = col; }

// Set a matrix of RowSZ x ColSZ dimensions. Sets a random value to each alternative, where this
// value ranges from 0 to colsz
void Preferencematrix::set_matrix( std::vector<int>::size_type rowsz, std::vector<int>::size_type colsz ){

    std::random_device rd;

    //std::mt19937_64 mt( rd( ) );
    std::mt19937 mt( rd( ) );

    std::uniform_int_distribution<std::vector<int>::size_type> column( 0, ( colsz - 1 ) );

    // Initializes ROWSIZE and COLUMNSIZE to rowsz and colsz
    rowsize = rowsz;
    columnsize = colsz;

    // Generates a profile of colsz size. Every option in this profile is initialized
    // to its default values
    Profile setofalts( colsz );

	// Workaroung below. Begin
	int aux{ 65 };

    // Maps int into chars, store in a stack, assign it to a string
    // Sets alternatives' id's - FIX THIS
	for( std::vector<int>::size_type i = 0; i < setofalts.size( ); ++i ){

        // if the number of options in the profiles is > 26, let numofchars[ setofalts.size / 26 ], where numofchars[ 0 ] = aux, [ 1 ] = aux2... and so on

        // change to is alphabetical
        while( !isalpha( aux ) ){

			++aux;
		}

		char id = static_cast<char>( aux );

		if( isupper( id ) )

			id = static_cast<char>( tolower( id ) );

		std::string name{ id };

		// There was a problem when using setofalts[ i ].set_opt( &id ). Taking the address to a char
		// handles this char as an array, and, thus, the string takes everything from that array, inst
		// ead of taking only one letter
		setofalts[ i ].set_opt( name );

		++aux;
	}
    // Workaround above. End

	// Sets alternatives' values
	for( std::vector<int>::size_type i = 0; i < rowsz; ++i ){

		matrix.push_back( setofalts );

		for( std::vector<int>::size_type j = 0; j < colsz; ++j ){

            //int val = rand( ) % static_cast<int>( colsz );
            int val = static_cast<int>( column( mt ) );

			//matrix[ i ][ j ].set_value( val );
			matrix[ i ].no_indifference( );
		}
	}
}

/// Getters

/// Operators

// Overloaded assignment operator
Preferencematrix& Preferencematrix::operator=( const Preferencematrix& copy ){

	rowsize = copy.rowsize;
	columnsize = copy.columnsize;

	matrix = copy.matrix;

	return *this;
}

// Overloaded move assignment
Preferencematrix& Preferencematrix::operator=( Preferencematrix&& copy ){

	rowsize = std::move( copy.rowsize );
	columnsize = std::move( copy.columnsize );

	matrix = std::move( copy.matrix );

	copy.clear( );

	return *this;
}

/// Helpers

// Deletes an specific row. Used in Agent.h class - DEBUG THIS
void Preferencematrix::erase_row( const std::vector<int>::size_type index ){ matrix.erase( matrix.begin( ) + index ); }

bool Preferencematrix::empty( ){

    if( matrix.empty( ) )

        return true;

    else

        return false;
}

void Preferencematrix::clear( ){

    matrix.clear( );

    std::vector<Profile>( ).swap( matrix );
}

void Preferencematrix::merge_sort_by_value( ){

    for( std::vector<int>::size_type i = 0; i < matrix.size( ); ++i ){

        matrix[ i ].value_merge_sort( 0, matrix.size( ) - 1 );
    }
}

/// Non-member helpers

// Prints PreferenceMatrix
std::ostream& operator<<( std::ostream& os, Preferencematrix& matrix ){

    // Gets the number of profiles
    std::vector<int>::size_type rowsz = matrix.get_matrix( ).size( );

    // Gets the number of options in each profile
    //std::vector<int>::size_type colsz = matrix[ static_cast<std::vector<int>::size_type>( rand( ) ) % rowsz ].size( );
    std::vector<int>::size_type colsz = matrix.begin( ) -> get_alternatives( ).size( );

    os << "Preference Matrix\n\n" << "\t\t\tOptions/Alternatives Columns\n\n";

    for( std::vector<int>::size_type i = 0; i < rowsz; ++i ){

        os << "Row vector id number: " << i << "| ";

        for( std::vector<int>::size_type j = 0; j < colsz; ++j ){

            os << "( " << matrix[ i ][ j ].get_opt( )
               << ", " << matrix[ i ][ j ].get_value( ) << " ) ";
        }

        os << "\n";
    }

    return os;
}

// MODIFIED: used to get two arguments ( Profile& profile, PreferenceMatrix& matrix ) - This method makes NO SENSE AT ALL
/*Profile make_social_order( Preferencematrix& matrix ){

    Profile profile;

    initialize_opts( matrix, profile );

    if( matrix.empty( ) ){

        initialize_opts( matrix, profile );

        make_social_order( matrix );
    }

    else{

        for( std::vector<int>::size_type i = 0; i < profile.size( ); ++i ){

            profile[ i ].set_value( 0 );
        }

        // For every profile in MATRIX
        for( std::vector<int>::size_type i = 0; i < matrix.size( ); ++i ){

            // For every option in every matrix PROFILE
            for( std::vector<int>::size_type j = 0; j < matrix[ i ].size( ); ++j ){

                // Searches, in SOCIALORDER, for every option in every matrix's profile i
                for( std::vector<int>::size_type k = 0; k < profile.size( ); ++k ){

                    // If it is the case that an option in SOCIALORDER is the same as an option
                    // in an matrix's profile
                    if( profile[ k ].get_opt( ) == matrix[ i ][ j ].get_opt( ) ){

                        // Then, increment SOCIALORDER[ k ]'s value
                        // WTF: where did this 4 come from? Should not it be POPULATION.SIZE instead?
                        // TODO: debug this
                        profile[ k ] += ( static_cast<int>( ( profile.size( ) - j ) ) );
                    }
                }
            }
        }
    }

    for( std::vector<int>::size_type i = 0; i < profile.size( ); ++i )

        // THIS IS BULLSHIT. Fix it later
        profile[ i ].set_value( static_cast<int>( static_cast<std::vector<int>::size_type>( profile[ i ].get_value( ) ) / profile.size( ) ) );

    return profile;
}*/

void initialize_opts( Preferencematrix& matrix, Profile& profile ){

	// Hmmmmm.... potential bullshit behavior here
	profile = *matrix.begin( );

    for( std::vector<int>::size_type i = 0; i < profile.size( ); ++i ){

        profile[ i ].set_value( 0 );
    }
}