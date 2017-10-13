package it.unimi.dsi.law.util;

/*
 * Copyright (C) 2011-2015 Sebastiano Vigna
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the Free
 *  Software Foundation; either version 3 of the License, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

//RELEASE-STATUS: DIST

/** An {@link Enum} providing different &#x2113; norms. */

public enum Norm {
	/** The {@linkplain #compute(double[]) &#x2113;<sub>1</sub> norm} of a vector is the sum of the absolute
	 * values of its components. We use <a href="http://en.wikipedia.org/wiki/Kahan_summation_algorithm">Kahan's
	 * summation algorithm</a> to contain numerical errors. */
	L_1 {
		public double compute( final double[] v ) {
			double normL1 = 0, c = 0;
			for ( int i = v.length; i-- != 0; ) {
				final double y = Math.abs( v[ i ] ) - c;
				final double t = normL1 + y;
				c = ( t - normL1 ) - y;
				normL1 = t;
			}
			return normL1;
		}

		public double compute( final double[] v, final double[] w ) {
			if ( v.length != w.length ) throw new IllegalArgumentException( "The two vectors have different sizes: " + v.length + " != " + w.length );
			
			double normL1 = 0, c = 0;
			for ( int i = v.length; i-- != 0; ) {
				final double y = Math.abs( v[ i ] - w[ i ] ) - c;
				final double t = normL1 + y;
				c = ( t - normL1 ) - y;
				normL1 = t;
			}
			return normL1;
		}
	},
	/** The {@linkplain #compute(double[]) &#x2113;<sub>2</sub> norm} of a vector is the square root of the sum of the squares
	 * of its components. We use <a href="http://en.wikipedia.org/wiki/Kahan_summation_algorithm">Kahan's
	 * summation algorithm</a> to contain numerical errors.*/
	L_2 {
		public double compute( final double[] v ) {
			double sumOfSquares = 0, c = 0;
			for( int i = v.length ; i-- != 0; ) {
				final double y = ( v[ i ] * v[ i ] ) - c;
				final double t = sumOfSquares + y;
				c = ( t - sumOfSquares ) - y;
				sumOfSquares = t;
			}
			return Math.sqrt( sumOfSquares );
		}
		public double compute( final double[] v, final double[] w ) {
			if ( v.length != w.length ) throw new IllegalArgumentException( "The two vectors have different sizes: " + v.length + " != " + w.length );
			
			double sumOfSquares = 0, c = 0;
			for( int i = v.length; i-- != 0; ) {
				final double y = ( v[ i ] - w[ i ] ) * ( v[ i ] - w[ i ] ) - c;
				final double t = sumOfSquares + y;
				c = ( t - sumOfSquares ) - y;
				sumOfSquares = t;
			}
			return Math.sqrt( sumOfSquares );
		}
	},
	/** The {@linkplain #compute(double[]) &#x2113;<sub>&#x221E;</sub> norm} of a vector is the maximum of the absolute
	 * values of its components. */
	L_INFINITY {
		public double compute( final double[] v ) {
			double norm = 0;
			for ( int i = v.length; i-- != 0; ) norm = Math.max( norm, Math.abs( v[ i ] ) );
			return norm;
		}

		public double compute( final double[] v, final double[] w ) {
			if ( v.length != w.length ) throw new IllegalArgumentException( "The two vectors have different sizes: " + v.length + " != " + w.length );
			
			double norm = 0;
			for ( int i = v.length; i-- != 0; ) norm = Math.max( norm, Math.abs( v[ i ] - w[ i ] ) );
			return norm;
		}
	};

	/** Computes the norm of a vector.
	 *
	 * @param v a vector.
	 * @return the norm of <code>v</code>.
	 */
	public abstract double compute( final double[] v );
	/** Computes the norm of the difference of two vectors.
	 *
	 * @param v the first vector.
	 * @param w the second vector.
	 * @return the norm of <code>v</code>&nbsp;&minus;&nbsp;<code>w</code>.
	 */
	public abstract double compute( final double[] v, final double[] w );

	/** Normalizes a vector to a given norm value.
	 *
	 * @param v the vector to be normalized.
	 * @param norm the new norm value (nonnegative).
	 * @return <code>v</code>.
	 */
	public double[] normalize( final double[] v, final double norm ) {
		if ( norm < 0 ) throw new IllegalArgumentException( "Negative norm: " + norm );
		final double c = norm / compute( v );
		for ( int i = v.length; i-- != 0; ) v[ i ] *= c;
		return v;
	}
}
