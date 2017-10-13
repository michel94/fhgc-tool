package it.unimi.dsi.law.graph;


/*		 
 * Copyright (C) 2010-2015 Paolo Boldi, Massimo Santini and Sebastiano Vigna 
 *
 *  This library is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU Lesser General Public License as published by the Free
 *  Software Foundation; either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This library is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

import it.unimi.dsi.Util;
import it.unimi.dsi.bits.LongArrayBitVector;
import it.unimi.dsi.fastutil.ints.AbstractIntComparator;
import it.unimi.dsi.fastutil.ints.IntArrayList;
import it.unimi.dsi.fastutil.ints.IntArrays;
import it.unimi.dsi.fastutil.ints.IntStack;
import it.unimi.dsi.fastutil.io.BinIO;
import it.unimi.dsi.logging.ProgressLogger;
import it.unimi.dsi.webgraph.BVGraph;
import it.unimi.dsi.webgraph.ImmutableGraph;
import it.unimi.dsi.webgraph.LazyIntIterator;

import java.io.IOException;
import java.util.Collections;
import java.util.Random;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import com.martiansoftware.jsap.FlaggedOption;
import com.martiansoftware.jsap.JSAP;
import com.martiansoftware.jsap.JSAPException;
import com.martiansoftware.jsap.JSAPResult;
import com.martiansoftware.jsap.Parameter;
import com.martiansoftware.jsap.SimpleJSAP;
import com.martiansoftware.jsap.Switch;
import com.martiansoftware.jsap.UnflaggedOption;

/** Computes the visit order with respect to a depth-first visit.
 * 
 * @author Marco Rosa
 */


//RELEASE-STATUS: DIST

public class DFS {
	private final static Logger LOGGER = LoggerFactory.getLogger( DFS.class );

	/** Return the permutation induced by the visit order of a depth-first visit.
	 * 
	 * @param graph a graph.
	 * @param startPerm a permutation that will be used to shuffle successors.
	 * @return  the permutation induced by the visit order of a depth-first visit.
	 */
	public static int[] dfsperm( final ImmutableGraph graph, final int[] startPerm ) {
		final int n = graph.numNodes();

		int[] invStartPerm = Util.invertPermutation( startPerm, new int[ n ] );
		int[] perm = Util.identity( n );
		final IntStack stack = new IntArrayList();
		LongArrayBitVector visited = LongArrayBitVector.ofLength( n );
		final ProgressLogger pl = new ProgressLogger( LOGGER );
		pl.expectedUpdates = n;
		pl.itemsName = "nodes";
		pl.start( "Starting depth-first visit..." );

		int pos = 0;
		for( int j = 0; j < n; j++ ){
			final int start = invStartPerm[ j ];
			if ( visited.getBoolean( start ) ) continue;
			stack.push( start );
			visited.set( start );

			int currentNode;
			IntArrayList successors = new IntArrayList();

			while( ! stack.isEmpty() ) {
				currentNode = stack.popInt();
				perm[ pos++ ] = currentNode;
				int degree = graph.outdegree( currentNode );
				LazyIntIterator iterator = graph.successors( currentNode );	

				successors.clear();
				for( int i = 0; i < degree; i++ ) {
					final int succ = iterator.nextInt();
					if ( ! visited.getBoolean( succ ) ) {
						successors.add( succ );
						visited.set( succ );
					}
				}

				int[] randomSuccessors = successors.elements();
				IntArrays.quickSort( randomSuccessors, 0, successors.size(), new AbstractIntComparator() {
					@Override
					public int compare( int x, int y ) {
						return startPerm[ y ] - startPerm[ x ];
					}
				} );

				for( int i = successors.size(); i-- != 0; ) stack.push( randomSuccessors[ i ] );
				pl.update();
			}	
		}
		

		pl.done();
		return perm;
	}

	public static void main( String[] args ) throws JSAPException, IOException {
		SimpleJSAP jsap = new SimpleJSAP( DFS.class.getName(), "Computes the permutation induced by a depth-first visit.", new Parameter[] {
				new FlaggedOption( "randomSeed", JSAP.LONG_PARSER, "0", JSAP.NOT_REQUIRED, 'r', "random-seed", "The random seed." ),
				new Switch( "random", 'p', "Start from a random permutation." ), 
				new UnflaggedOption( "graph", JSAP.STRING_PARSER, JSAP.REQUIRED, "The basename of the input graph" ),
				new UnflaggedOption( "perm", JSAP.STRING_PARSER, JSAP.REQUIRED, "The name of the output permutation" ), } );


		JSAPResult jsapResult = jsap.parse( args );
		if ( jsap.messagePrinted() ) System.exit( 1 );

		final ImmutableGraph graph = BVGraph.load( jsapResult.getString( "graph" ) );

		final int n = graph.numNodes();
		int[] startPerm = Util.identity( new int[ n ] );
		long seed = jsapResult.getLong( "randomSeed" );
		if ( jsapResult.getBoolean( "random" ) ) Collections.shuffle( IntArrayList.wrap( startPerm ), new Random( seed ) );
		
		BinIO.storeInts( Util.invertPermutationInPlace( dfsperm( graph, startPerm ) ), jsapResult.getString( "perm" ) );
	}
}
