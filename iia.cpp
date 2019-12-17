#include "iia.hpp"

/// Constructors & Destructor

// Copy constructor
SCT::Irrelevant_Alternatives::Irrelevant_Alternatives( const SCT::Irrelevant_Alternatives& copy ){

	matrix = copy.matrix;

	population = copy.population;
}

// Move constructor
SCT::Irrelevant_Alternatives::Irrelevant_Alternatives( SCT::Irrelevant_Alternatives&& alts ){

	matrix = alts.matrix;

	population = alts.population;
	
	alts.clear( );
}

// Destructor
SCT::Irrelevant_Alternatives::~Irrelevant_Alternatives( ){ clear( ); }

/// Setters

/// Getters

/// Operators

// Copy assignment - leave as it is or change it to default idiom?
SCT::Irrelevant_Alternatives& SCT::Irrelevant_Alternatives::operator=( SCT::Irrelevant_Alternatives copy ){

	swap( *this, copy );

	return *this;
}

// Move assignment
SCT::Irrelevant_Alternatives& SCT::Irrelevant_Alternatives::operator=( SCT::Irrelevant_Alternatives&& alts ){

	matrix = alts.matrix;

	population = alts.population;

	alts.clear( );

	return *this;
}

bool SCT::Irrelevant_Alternatives::operator( )( SCT::Procedure& procedure ){

	// Sorting here will enforce correctness on the matrix comparison conditional, i.e., they will
	// only be equal when its alternatives appear in the same order
	matrix.merge_sort_by_value( );

	Rank original( matrix );
	original.order_ranking( );

	Preferencematrix primematrix = generate_prime_profile( matrix );
	primematrix.merge_sort_by_value( );

	Rank primerank( primematrix );
	primerank.order_ranking( );

	if( matrix != primematrix ){

		if( rank_relations( original, primerank ) == true ){

			Profile winnerset = procedure( original );
			winnerset.sort_by_value( );

			Profile primeset = procedure( primerank );
			primeset.sort_by_value( );

			// Especial attention here. What one wants from this? Not the same values, but same ORDER, i.e.,
			// for i from 0 to set.size, winnerset[ i ] == primeset[ i ]. DEBUG THIS
			if( winnerset == primeset ){

				return true;
			}

			else{

				return false;
			}
		}

		else{

			std::cerr << "Ma boi, you really have a problem here.\n";
		}
	}

	else if( matrix == primematrix ){

		return true;
	}
}

/// Helpers

void SCT::Irrelevant_Alternatives::clear( ){

	matrix.clear( );

	population.clear( );
}

// This is all about relative positioning of alternatives. If, for any two profiles p and p', the rela
// tive positions of its alternatives are the same, then, f( p ) and f( p' ) must keep their relative
// positions equal
// The secret: for a set A with n elements, generate all possible list, without repetitions of options,
// of those elements. Then, for a matrix with k rows, generate all possible matrixes, with repetition
// allowed, using the lists taken from all possible lists


/// Non-member helpers

// Rankings must contain the same alternatives in the same order!
// Still needs more fine tuning - 14x14 new upper bound. ALMOST THERE, THOUGH
Preferencematrix SCT::generate_prime_profile( Preferencematrix& originalmatrix ){

	// Make this sort in descending order
	originalmatrix.merge_sort_by_value( );

	Rank originalrank( originalmatrix );
	originalrank.order_ranking( );

	Preferencematrix primematrix( originalmatrix.get_rowsz( ), originalmatrix.get_columnsz( ) );
	primematrix.merge_sort_by_value( );

	Rank primerank( primematrix );
	primerank.order_ranking( );

	for( std::vector<int>::size_type i = 0; i < originalrank.size( ); ++i ){

		if( ( ( originalrank[ i ].get_xval( ) > originalrank[ i ].get_yval( ) ) == true ) &&
			( ( primerank[ i ].get_xval( ) > primerank[ i ].get_yval( ) ) == false ) ){

			std::vector<int>::size_type distance{ primematrix.begin( )->size( ) };
			std::vector<int>::size_type prof{ 0 };

			std::vector<int>::size_type xindex{ };
			std::vector<int>::size_type yindex{ };

			// Find the profile with the smallest profiles which holds the relation with the least distance between x and y
			for( std::vector<int>::size_type j = 0; j < primematrix.size( ); ++j ){

				std::vector<Options, std::allocator<Options>>::iterator x = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_optx( ) );
				std::vector<Options, std::allocator<Options>>::iterator y = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_opty( ) );

				if( y->get_value( ) > x->get_value( ) ){

					if( static_cast< std::vector<int>::size_type >( std::abs( x - y ) ) < distance ){

						xindex = static_cast< std::vector<int>::size_type >( x - primematrix[ j ].begin( ) );
						yindex = static_cast< std::vector<int>::size_type >( y - primematrix[ j ].begin( ) );

						distance = static_cast< std::vector<int>::size_type >( std::abs( x - y ) );

						prof = j;
					}
				}
			}

			if( distance == 1 ){

				std::string holder{ primematrix[ prof ][ xindex ].get_opt( ) };

				primematrix[ prof ][ xindex ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
				primematrix[ prof ][ yindex ].set_opt( holder );
				// should i merge sort this again? - nah, it sorts by value, swapping position will not alter ranking

				primerank.clear( );
				primerank.generate_ranking( primematrix );
				// should i order ranking here?

				i = -1;
			}

			// Else, if there are k elements between x and y, put y at x and move x and its subsequents elements one place to the right
			else if( distance > 1 ){

				for( std::vector<int>::size_type k = std::min( xindex, yindex ); k <= std::max( xindex, yindex ); ++k ){

					std::string holder{ primematrix[ prof ][ k ].get_opt( ) };

					primematrix[ prof ][ k ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
					primematrix[ prof ][ yindex ].set_opt( holder );
				}

				primerank.clear( );
				primerank.generate_ranking( primematrix );

				i = -1;
			}
		}

		else if( ( ( originalrank[ i ].get_yval( ) > originalrank[ i ].get_xval( ) ) == true ) &&
			( ( primerank[ i ].get_yval( ) > primerank[ i ].get_xval( ) ) == false ) ){

			std::vector<int>::size_type distance{ primematrix.begin( )->size( ) };
			std::vector<int>::size_type prof{ 0 };

			std::vector<int>::size_type xindex{ };
			std::vector<int>::size_type yindex{ };

			// Find the profile with the smallest profiles which holds the relation with the least distance between x and y
			for( std::vector<int>::size_type j = 0; j < primematrix.size( ); ++j ){

				std::vector<Options, std::allocator<Options>>::iterator x = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_optx( ) );
				std::vector<Options, std::allocator<Options>>::iterator y = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_opty( ) );

				if( x->get_value( ) > y->get_value( ) ){

					if( static_cast< std::vector<int>::size_type >( std::abs( x - y ) ) < distance ){

						xindex = static_cast< std::vector<int>::size_type >( x - primematrix[ j ].begin( ) );
						yindex = static_cast< std::vector<int>::size_type >( y - primematrix[ j ].begin( ) );

						distance = static_cast< std::vector<int>::size_type >( std::abs( x - y ) );

						prof = j;
					}
				}
			}

			if( distance == 1 ){

				std::string holder{ primematrix[ prof ][ xindex ].get_opt( ) };

				primematrix[ prof ][ xindex ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
				primematrix[ prof ][ yindex ].set_opt( holder );

				primerank.clear( );
				primerank.generate_ranking( primematrix );

				i = -1;
			}

			// Else, if there are k elements between x and y, put y at x and move x and its subsequents elements one place to the right
			else if( distance > 1 ){

				for( std::vector<int>::size_type k = std::min( xindex, yindex ); k <= std::max( xindex, yindex ); ++k ){

					std::string holder{ primematrix[ prof ][ k ].get_opt( ) };

					primematrix[ prof ][ k ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
					primematrix[ prof ][ yindex ].set_opt( holder );
				}

				primerank.clear( );
				primerank.generate_ranking( primematrix );

				i = -1;
			}
		}

		else if( ( ( originalrank[ i ].get_yval( ) == originalrank[ i ].get_xval( ) ) == true ) &&
			( ( primerank[ i ].get_yval( ) == primerank[ i ].get_xval( ) ) == false ) ){

			// should not this be if xval > yval?
			//if( primerank[ i ].get_optx( ) > primerank[ i ].get_opty( ) )
			if( primerank[ i ].get_xval( ) > primerank[ i ].get_yval( ) ){

				std::vector<int>::size_type distance{ primematrix.begin( )->size( ) };
				std::vector<int>::size_type prof{ 0 };

				std::vector<int>::size_type xindex{ };
				std::vector<int>::size_type yindex{ };

				// Find the profile with the smallest profiles which holds the relation with the least distance between x and y
				for( std::vector<int>::size_type j = 0; j < primematrix.size( ); ++j ){

					std::vector<Options, std::allocator<Options>>::iterator x = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_optx( ) );
					std::vector<Options, std::allocator<Options>>::iterator y = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_opty( ) );

					if( x->get_value( ) > y->get_value( ) ){

						if( static_cast< std::vector<int>::size_type >( std::abs( x - y ) ) < distance ){

							xindex = static_cast< std::vector<int>::size_type >( x - primematrix[ j ].begin( ) );
							yindex = static_cast< std::vector<int>::size_type >( y - primematrix[ j ].begin( ) );

							distance = static_cast< std::vector<int>::size_type >( std::abs( x - y ) );

							prof = j;
						}
					}
				}

				if( distance == 1 ){

					std::string holder{ primematrix[ prof ][ xindex ].get_opt( ) };

					primematrix[ prof ][ xindex ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
					primematrix[ prof ][ yindex ].set_opt( holder );

					primerank.clear( );
					primerank.generate_ranking( primematrix );

					i = -1;
				}

				// Else, if there are k elements between x and y, put y at x and move x and its subsequents elements one place to the right
				else if( distance > 1 ){

					// problem here: xindex can be greater than yindex. This loop will not be executed - DEADLOCK - SOLVED
					// second issue: is the move being done correctly? Is the program correctly reassigning options' names? - NOPE
					for( std::vector<int>::size_type k = std::min( xindex, yindex ); k <= std::max( xindex, yindex ); ++k ){

						std::string holder{ primematrix[ prof ][ k ].get_opt( ) };

						primematrix[ prof ][ k ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
						primematrix[ prof ][ yindex ].set_opt( holder );
					}

					primerank.clear( );
					primerank.generate_ranking( primematrix );

					i = -1;
				}
			}

			//else if( primerank[ i ].get_optx( ) > primerank[ i ].get_opty( ) )
			else if( primerank[ i ].get_yval( ) > primerank[ i ].get_xval( ) ){

				std::vector<int>::size_type distance{ primematrix.begin( )->size( ) };
				std::vector<int>::size_type prof{ 0 };

				std::vector<int>::size_type xindex{ };
				std::vector<int>::size_type yindex{ };

				// Find the profile with the smallest profiles which holds the relation with the least distance between x and y
				for( std::vector<int>::size_type j = 0; j < primematrix.size( ); ++j ){

					std::vector<Options, std::allocator<Options>>::iterator x = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_optx( ) );
					std::vector<Options, std::allocator<Options>>::iterator y = std::find( primematrix[ j ].begin( ), primematrix[ j ].end( ), originalrank[ i ].get_opty( ) );

					if( y->get_value( ) > x->get_value( ) ){

						if( static_cast< std::vector<int>::size_type >( std::abs( x - y ) ) < distance ){

							xindex = static_cast< std::vector<int>::size_type >( x - primematrix[ j ].begin( ) );
							yindex = static_cast< std::vector<int>::size_type >( y - primematrix[ j ].begin( ) );

							distance = static_cast< std::vector<int>::size_type >( std::abs( x - y ) );

							prof = j;
						}
					}
				}

				if( distance == 1 ){

					std::string holder{ primematrix[ prof ][ xindex ].get_opt( ) };

					primematrix[ prof ][ xindex ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
					primematrix[ prof ][ yindex ].set_opt( holder );

					primerank.clear( );
					primerank.generate_ranking( primematrix );

					i = -1;
				}

				// Else, if there are k elements between x and y, put y at x and move x and its subsequents elements one place to the right
				else if( distance > 1 ){

					for( std::vector<int>::size_type k = std::min( xindex, yindex ); k <= std::max( xindex, yindex ); ++k ){

						std::string holder{ primematrix[ prof ][ k ].get_opt( ) };

						primematrix[ prof ][ k ].set_opt( primematrix[ prof ][ yindex ].get_opt( ) );
						primematrix[ prof ][ yindex ].set_opt( holder );
					}

					primerank.clear( );
					primerank.generate_ranking( primematrix );

					i = -1;
				}
			}
		}
	}

	return primematrix;
}

// Begin with a number of options. Lets say, n.
// Generate n alternatives in a set, the order here will not be important now. This will be the generator set G
// Then, do the following:
	// how many different lists can one get where the first alternative is, lets say, the first element of G
	// repeat the above until every element of G is used
void SCT::generate_all_profiles( Profile preferences ) {

	Profile holder{ preferences };

	Preferencematrix matrix{};

	matrix.push_back( holder );

	// Magic number. For now, use it, but find a way to either justify or make it not magic
	int repetitions{ 1 };

	for( std::vector<int>::size_type i = 2; i < preferences.size( ); ++i ){

		std::vector<int>::size_type transpose_index{ 0 };

		for( std::vector<int>::size_type j = 0; j < std::tgamma( i + 1 ) - repetitions; ++j ) {

			// switch elements between positions holder[ size - 1 ] and holder[ size - i ]
			std::swap( holder[ holder.size( ) - 1 ], holder[ holder.size( ) - i - transpose_index ] ); // removed - j, it would overflow the vector, accessing elements out of scope

			matrix.push_back( holder );

			if( transpose_index < i - 1 ){

				++transpose_index;
			}

			else{

				continue;
			}
		}

		holder = preferences;

		repetitions *= i;
	}
}

void SCT::generate_profiles_2( Profile preferences ){

	Preferencematrix matrix{};

	for( std::vector<int>::size_type i = 0; i < preferences.size( ); ++i ) {

		for( std::vector<int>::size_type j = 0; j < ( ( std::tgamma( preferences.size( ) + 1 ) / preferences.size( ) ) - i ); ++j ) {

			Profile holder( preferences.size( ) );

			holder[ 0 ] = preferences[ i ];

			matrix.push_back( holder );
		}
	}
}