import Profile
import Preference_Matrix
import Social_Choice_Rank
import SPNode

class Graph:

    # Constructor & Destructor
    def __init__( self ):

        self.nodes: list( SPNode.Social_Preference_Node ) = list( )

    def __del__( self ):

        self.nodes = list()

    # Setters

    # Creates a graph according to a SCRank. For any two nodes a and b,
    # if a's value in SCRank is greater than b's, then, create an edge
    # from a to b where b is inserted in a's .preferred_to list. A simi
    # rule applies for the cases where a is smaller than or equal to b
    def create_graph( self, rank: Social_Choice_Rank.Rank ):

        for i in range( len( rank) ):

            for j in range( len( self.nodes ) ):

                # If x.value > y.value
                if rank[ i ][ 2 ] > rank[ i ][ 3 ]:

                    # If nodes[ j ] == opt_x in rank
                    if self.nodes[ j ].get_id( ) == rank[ i ][ 0 ]:

                        # Then, find opt_y
                        for k in range( len( self.nodes ) ):

                            if self.nodes[ k ].get_id( ) == rank[ i ][ 1 ]: # Can I do that without creating a node, i.e., by using only the nodes in self.node?

                                # Add opt_y to opt_x.preferred_to
                                node: SPNode.Social_Preference_Node = SPNode.Social_Preference_Node()
                                node.set_id( rank[ i ][ 1 ] )

                                self.nodes[ j ].add_preferred( self.nodes[ k ] )

                            else:

                                continue

                    # Else, if nodes[ j ] == opt_y in rank
                    elif self.nodes[ j ].get_id( ) == rank[ i ][ 1 ]:

                        # Then, find opt_x
                        for k in range( len( self.nodes ) ):

                            if self.nodes[ k ].get_id( ) == rank[ i ][ 0 ]:

                                # Add opt_x to opt_y.preferred_to
                                node: SPNode.Social_Preference_Node = SPNode.Social_Preference_Node()
                                node.set_id( rank[ i ][ 0 ] )

                                self.nodes[ j ].add_worse( self.nodes[ k ] ) # Modified here. Removed node, switched with self.nodes[ k ]

                            else:

                                continue

                # If x.value < y.value
                elif rank[ i ][ 2 ] < rank[ i ][ 3 ]:

                    # If nodes[ j ] == opt_x in rank
                    if self.nodes[ j ].get_id( ) == rank[ i ][ 0 ]:

                        # Then, find opt_y
                        for k in range( len( self.nodes ) ):

                            if self.nodes[ k ].get_id( ) == rank[ i ][ 1 ]:

                                # Add opt_y to opt_x.worse_than
                                node: SPNode.Social_Preference_Node = SPNode.Social_Preference_Node()
                                node.set_id( rank[ i ][ 1 ] )

                                self.nodes[ j ].add_worse( self.nodes[ k ] ) # Modified here

                            else:

                                continue

                    # Else, if nodes[ j ] == opt_y in rank
                    elif self.nodes[ j ].get_id( ) == rank[ i ][ 1 ]:

                        # Then, find opt_x
                        for k in range( len( self.nodes ) ):

                            if self.nodes[ k ].get_id( ) == rank[ i ][ 0 ]:

                                # Add opt_x to opt_y.preferred_to
                                node: SPNode.Social_Preference_Node = SPNode.Social_Preference_Node()
                                node.set_id( rank[ i ][ 0 ] )

                                self.nodes[ j ].add_preferred( self.nodes[ k ] ) # Modified here

                            else:

                                continue

                # If x.value == y.value
                elif rank[ i ][ 2 ] == rank[ i ][ 3 ]:

                    # If nodes[ j ] == opt_x
                    if self.nodes[ j ].get_id( ) == rank[ i ][ 0 ]:

                        # Search for opt_y in nodes
                        for k in range( len( self.nodes ) ):

                            # Add opt_y to x.indifferent_to
                            if self.nodes[ k ].get_id( ) == rank[ i ][ 1 ]:

                                self.nodes[ j ].add_indiff( self.nodes[ k ] )

                            else:

                                continue
                    
                    # Else, i nodes[ j ] == opt_y
                    elif self.nodes[ j ].get_id( ) == rank[ i ][ 1 ]:

                        # Search for opt_x in nodes
                        for k in range( len( self.nodes ) ):
    
                            # Add opt_x to y.indifferent_to
                            if self.nodes[ k ].get_id( ) == rank[ i ][ 0 ]:

                                self.nodes[ j ].add_indiff( self.nodes[ k ] )

                            else:

                                continue

    # Getters
    def __getitem__( self, index: int ) -> SPNode.Social_Preference_Node:

        return self.nodes[ index ]

    # Operators

    # Helpers
    def __str__( self ): # Fix this

        string: str = str()

        for i in range( len( self.nodes ) ):

            string += str( self.nodes[ i ] ) + "\n"

        return string

    # Gets alternatives and create nodes to represent them
    def initialize( self, profile: Profile.Profile ):

        self.reserve( len( profile ) )

        for i in range( len( self.nodes ) ):

            node: SPNode.Social_Preference_Node = SPNode.Social_Preference_Node()
            node.set_id( profile[ i ][ 0 ] )

            self.nodes[ i ] = node

    def reserve( self, size: int ):

        self.nodes += [ self.nodes ] * ( size - len( self.nodes ) )

""" Testing Zone """

""" 
#TEST 1
profile: Profile.Profile = [ ( "a", False, 1 ), ( "b", False, 2 ), ( "c", False, 3 ) ]

matrix: Preference_Matrix.Preference_Matrix = Preference_Matrix.Preference_Matrix( )

matrix.append( profile )
matrix.append( profile )
matrix.append( profile )

rank: Social_Choice_Rank.Rank = Social_Choice_Rank.Rank()
rank.generate_ranking( matrix )

graph: Graph = Graph()
graph.initialize( profile )
graph.create_graph( rank )

print( graph )

print( "\n" )
"""

"""
#TEST 2
newmtx: Preference_Matrix.Preference_Matrix = Preference_Matrix.Preference_Matrix( )
newmtx.set_matrix( 4, 4 )

newrank: Social_Choice_Rank.Rank = Social_Choice_Rank.Rank()
newrank.generate_ranking( newmtx ) # not generating rank

newgraph: Graph = Graph()

newprof: Profile.Profile = newmtx[ 0 ]

newgraph.initialize( newprof ) # not initializing
newgraph.create_graph( newrank )

newmtx.print()

print( "\n" )

print( newprof )

print( "\n" )

print( newrank )

print( "\n" )

print( newgraph )

print( "\n" )

print( newgraph[ 0 ] )
"""